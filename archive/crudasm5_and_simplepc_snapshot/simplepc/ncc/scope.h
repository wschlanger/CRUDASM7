// scope.h
// Copyright (C) 2009 Willow Schlanger

#ifndef l_scope_h__included
#define l_scope_h__included

#include <iostream>

#include <map>
#include <vector>
#include <list>
#include <set>
#include <string>
#include <cstddef>
#include <cstdio>
#include <stdexcept>
#include <cstring>

#include "token_enums.h"

// Special Assembler Symbol.
#define SASYM "@"

class StringTable
{
	public:
	std::map<std::string, U4> m;
	std::vector<const std::string *> v;

	StringTable()
	{
		getString("");	// entry 0 must be ""
		getString("file0" SASYM);	// entry 1 must be unique to the current file
	}
	U4 getString(const char *s)
	{
		std::map<std::string, U4>::iterator i = m.find(s);
		if(i == m.end())
		{
			U4 x = v.size();
			i = m.insert(std::pair<const std::string, U4>(s, x)).first;
			v.push_back(&(i->first));
			return x;
		}
		return i->second;
	}
	const std::string &getString(U4 num)
	{
		static const std::string sEmpty = "";
		if(num > v.size())
			return sEmpty;
		return *v[num];
	}
};

class ObjectCodeWriter
{
	U1 *buf;
	U4 offset;
	public:
	ObjectCodeWriter()
	{
		buf = new U1 [65536];
		offset = 0;
	}

	void finish()
	{
		if(offset != 0)
			writeBuffer(buf, offset);
		buf = NULL;
	}
	
	virtual ~ObjectCodeWriter()
	{
		if(buf != NULL)
			throw std::runtime_error("forgot to call ObjectCodeWriter::finish()");
	}

	virtual void writeBuffer(U1 *buf, U4 size) = 0;
	
	void addB(U4 b)
	{
		addBytes(&b, 1);
	}
	
	void addW(U4 b)
	{
		addBytes(&b, 2);
	}
	
	void addD(U4 b)
	{
		addBytes(&b, 4);
	}
	
	void addQ(U8 b)
	{
		addBytes(&b, 8);
	}

	void addP(U4 classT, U4 typeT, U8 sizeT)
	{
		U4 x = (classT << 24) + typeT;
		bool size64 = (sizeT > 0xffffffffull);
		if(size64)
			x += 0x80000000;
		addD(x);
		if(size64)
			addQ(sizeT);
		else
			addD((U4)(sizeT));
	}

	private:
	
	// adds up to 16 bytes to the image.
	void addBytes(void *bytesT, U4 size)
	{
		U1 *bytes = (U1 *)(bytesT);
		if(offset + 16 > 65536)
		{
			writeBuffer(buf, offset);
			offset = 0;
		}
		for(U4 i = 0; i < size; ++i)
			buf[offset + i] = bytes[i];
		offset += size;
	}
};

class FileObjectCode :
	public ObjectCodeWriter
{
	FILE *f;
	U4 fileOffset;
	U4 fileSizeOffset;
	U4 startObject;
	U4 objectSizeOffset;
	public:
	
	FileObjectCode(const char *outName, const char *fname)
	{
		f = std::fopen(outName, "wb");
		fileOffset = 0;
		
		writePrefix(0, 1, 20 + 8);
		writeByte('N');
		writeByte('O');
		writeByte('F');
		writeByte('F');
		writeByte('1');
		writeByte('.');
		writeByte('0');
		writeByte('\0');
		fileSizeOffset = fileOffset;
		writeQword(0);
		writeWord(0x0100);
		writeWord(0x0100);
		
		startObject = fileOffset;
		writePrefix(0, 2, 1 + std::strlen(fname) + 16 + 8);
		writeByte('N');
		writeByte('O');
		writeByte('B');
		writeByte('J');
		writeByte('E');
		writeByte('C');
		writeByte('T');
		writeByte('\0');
		objectSizeOffset = fileOffset;
		writeQword(0);
		for(const char *i = fname; *i != '\0'; ++i)
			writeByte(*i);
		writeByte('\0');
	}
	
	virtual ~FileObjectCode()
	{
		U4 fSize = fileOffset;
		U4 oSize = fileOffset - startObject;
		writePrefix(0, 3, 8);
		writePrefix(0, 0, 8);
		fseek(f, objectSizeOffset, SEEK_SET);
		fileOffset = objectSizeOffset;
		writeQword(oSize);
		fseek(f, fileSizeOffset, SEEK_SET);
		fileOffset = fileSizeOffset;
		writeQword(fSize);
	
		std::fclose(f);
	}

	void writeBuffer(U1 *buf, U4 size)
	{
		fwrite(buf, size, 1, f);
		fileOffset += size;
	}
	
	private:
	void writePrefix(U4 classT, U4 typeT, U8 sizeT)
	{
		U4 x = (classT << 24) + typeT;
		bool size64 = (sizeT > 0xffffffffull);
		if(size64)
			x += 0x80000000;
		writeDword(x);
		if(size64)
			writeQword(sizeT);
		else
			writeDword((U4)(sizeT));
	}
	
	U4 writeByte(U4 b)
	{
		U4 x = fileOffset;
		fwrite(&b, 1, 1, f);
		fileOffset += 1;
		return x;
	}
	U4 writeWord(U4 b)
	{
		U4 x = fileOffset;
		fwrite(&b, 2, 1, f);
		fileOffset += 2;
		return x;
	}
	U4 writeDword(U4 b)
	{
		U4 x = fileOffset;
		fwrite(&b, 4, 1, f);
		fileOffset += 4;
		return x;
	}
	U4 writeQword(U8 b)
	{
		U4 x = fileOffset;
		fwrite(&b, 8, 1, f);
		fileOffset += 8;
		return x;
	}
};

class Declaration
{
	public:
	Declaration()
	{
	}
	virtual ~Declaration()
	{
		for(std::list<struct Declaration *>::iterator i = decs.begin();
			i != decs.end(); ++i
		)
			delete *i;
	}
	std::list<struct Declaration *> decs;	// in order
	std::map<U4, struct Declaration *> decsNamed;	// may contain aliases too
	
	struct Declaration *searchExactName(U4 name)
	{
		std::map<U4, struct Declaration *>::iterator i = decsNamed.find(name);
		if(i != decsNamed.end())
			return i->second;
		return NULL;
	}
	
	void add(Declaration *d, U4 name = 0)
	{
		decs.push_back(d);
		if(name != 0)
			decsNamed[name] = d;
	}
	
	virtual void serializeOut(ObjectCodeWriter &wr) = 0;
};

struct RootDeclaration :
	public Declaration
{
	virtual ~RootDeclaration() { }
	
	virtual void serializeOut(ObjectCodeWriter &wr)
	{
		for(std::list<struct Declaration *>::iterator i = decs.begin();
			i != decs.end(); ++i
		)
		{
			(*i)->serializeOut(wr);
		}
	}
};

struct NamedDeclaration :
	public Declaration
{
	U4 prefix;		// 0 means no prefix
	U4 name;		// a name of 0 ("") means it's an anonymous scope

	NamedDeclaration(U4 prefixT, U4 nameT) :
		prefix(prefixT),
		name(nameT)
	{
	}
	
	virtual ~NamedDeclaration() { }
};

struct Enum :
	public NamedDeclaration
{
	// For C we don't need to put anything in here.
	Enum(U4 prefixT, U4 nameT) :
		NamedDeclaration(prefixT, nameT)
	{
	}

	virtual ~Enum() { }

	virtual void serializeOut(ObjectCodeWriter &wr)
	{
		wr.addP(1, 3, 16);
		wr.addD(prefix);
		wr.addD(name);
	}
};

class Typedef :
	public NamedDeclaration
{
	class Type *type;
	
	public:

	// For C we don't need to put anything in here.
	Typedef(U4 prefixT, U4 nameT, class Type *typeT) :
		NamedDeclaration(prefixT, nameT),
		type(typeT)
	{
	}

	virtual ~Typedef() { }

	virtual void serializeOut(ObjectCodeWriter &wr)
	{
		wr.addP(1, 11, 8 + 16);
		wr.addD(prefix);
		wr.addD(name);
		wr.addQ((U8)(UINT)(type));
	}
};

struct EnumValue :
	public NamedDeclaration
{
	S8 value;

	// For C we don't need to put anything in here.
	EnumValue(U4 prefixT, U4 nameT, S8 valueT) :
		NamedDeclaration(prefixT, nameT),
		value(valueT)
	{
	}

	virtual ~EnumValue() { }

	virtual void serializeOut(ObjectCodeWriter &wr)
	{
		wr.addP(1, 4, 8 + 8 + 16);
		wr.addQ((U8)(value));
		wr.addQ(0);
		wr.addD(prefix);
		wr.addD(name);
		for(std::list<struct Declaration *>::iterator i = decs.begin();
			i != decs.end(); ++i
		)
		{
			(*i)->serializeOut(wr);
		}
	}
};

struct Scope :
	public NamedDeclaration
{
	Scope *parent;	// may be NULL
	
	Scope(Scope *parentT, U4 prefixT, U4 nameT) :
		parent(parentT),
		NamedDeclaration(prefixT, nameT)
	{
	}
	
	virtual ~Scope() { }

	// This searches the current scope for 'name'.
	// If not found, it will go up to the parent scope.
	// Note--this searches DOWN from the current scope. For
	// example: struct S1 { struct S2 { S1 *x; } y; } z;
	// Here, when "S1 *x" is found, we need to getDeclaration()
	// for "S1" at the S2 scope. This begins by searching the
	// current scope; if not found, it goes up a level.
	// Ex.: struct S { struct S *S; } S;
	// This is valid C code.
	Declaration *getDeclaration(U4 name)
	{
		Scope *s = this;
		Declaration *d;
		while(s != NULL)
		{
			d = s->searchExactName(name);
			if(d != NULL)
				return d;
			s = s->parent;
		}
	}
};

struct ObjectCode
{
	std::string idPrefix;	// e.g. "_"
	StringTable strTab;
	RootDeclaration root;
	bool cppMode;
	Declaration *fcnScope;
	
	ObjectCode()
	{
		cppMode = false;
		idPrefix = "_";
		fcnScope = &root;	// if in a function { } body, this ->'s to that fcn; else it ->'s to root.
	}
	
	// Note--ident must already have idPrefix prepended.
	// This gets the internal name for a given identifier that
	// is a structure.
	std::string getStructName(std::string ident, char cType = 'S')
	{
		if(cppMode)
			return ident;
		if(cType == 'u')
			return std::string("CU" SASYM) + ident;	// CU = C Union
		return std::string("CS" SASYM) + ident;	// CS = C Struct
	}
	
	std::string getEnumName(std::string ident)
	{
		if(cppMode)
			return ident;
		return std::string("CE" SASYM) + ident;	// CE = C Enum
	}
	
	U4 getFilePrefix()
	{
		return 1;	// return 0 when pragma export(enabled)--must be at global scope when enabled/disabled.
	}
	
	void writeObject(ObjectCodeWriter &wr)
	{
		writeStringTable(wr);
		root.serializeOut(wr);
	}
	
	private:
	void writeStringTable(ObjectCodeWriter &wr)
	{
		std::vector<char> table;
		for(std::vector<const std::string *>::iterator i = strTab.v.begin(); i != strTab.v.end(); ++i)
		{
			if((*i)->empty())
				table.push_back('\1');
			else
			{
				for(std::string::const_iterator j = (*i)->begin(); j != (*i)->end(); ++j)
					table.push_back(*j);
			}
			table.push_back('\0');
		}
		table.push_back('\0');
		wr.addP(1, 0, 8 + table.size());
		for(U4 i = 0; i < table.size(); ++i)
			wr.addB(table[i]);
	}
};

// This is a scope; in particular it represents class/struct/union's.
class ClassScope :
	public Scope
{
	U8 size;
	U4 alignment;
	U1 isUnion;
	U1 hasPadding;
	U1 isEmpty;
	
	public:
	
	// This is called whenever a basic (e.g. built-in type) is added to the
	// union or struct. It updates various internal fields. sizeT may be at
	// most 16 and must be at least 1.
	void updateAddBasic(U8 sizeT, bool packed)
	{
		if(isEmpty)
		{
			hasPadding = 0;
			isEmpty = 0;
			size = 0;				// was 1
		}
		U8 sizeMask = 15;
		if(sizeT < 2)
			sizeMask = 1;
		else
		if(sizeT < 4)
			sizeMask = 3;
		else
		if(sizeT < 8)
			sizeMask = 7;
		U8 padding = 0;
		if((size & sizeMask) != 0 && !packed)
			padding = (sizeMask + 1) - (size & sizeMask);
		if(padding != 0)
			hasPadding = 1;
		U8 realSize = size + padding;
		if(realSize > alignment)
			alignment = realSize;
		if(isUnion && realSize > size)
			size = realSize;
		else
			size += realSize;
	}
	
	// This is called whenever a composite (e.g. struct or union) data member
	// is added inside THIS sturct or union. Ex.: struct S1 { struct S2 s; };
	void updateAddComposite(ClassScope *item, bool packed)
	{
		if(isEmpty)
		{
			hasPadding = 0;
			isEmpty = 0;
			size = 0;				// was 1
		}
		U8 sizeT = item->alignment;
		U8 sizeMask = 15;
		if(sizeT < 2)
			sizeMask = 1;
		else
		if(sizeT < 4)
			sizeMask = 3;
		else
		if(sizeT < 8)
			sizeMask = 7;
		U8 padding = 0;
		if((sizeT & sizeMask) != 0 && !packed)
			padding = (sizeMask + 1) - (sizeT & sizeMask);
		if(padding != 0)
			hasPadding = 1;
		U8 realSize = item->size + padding;
		if(item->alignment > alignment)
			alignment = item->alignment;
		if(isUnion && realSize > size)
			size = realSize;
		else
			size += realSize;
	}

	ClassScope(Scope *parentT, U4 prefixT, U4 nameT, bool isUnionT = false) :
		Scope(parentT, prefixT, nameT),
		isUnion(isUnionT)
	{
		hasPadding = true;
		isEmpty = true;
		size = 1;		// by default classes have a size of 1
		alignment = 1;	// and prefer to be aligned on a byte boundary
		// FIXME--need to automatically update these somehow.
	}
	
	virtual ~ClassScope() { }

	virtual void serializeOut(ObjectCodeWriter &wr)
	{
		wr.addP(1, 1, 16);
		wr.addD(prefix);
		wr.addD(name);
		
		wr.addP(1, 5, 8 + 4 * 5);
		wr.addD(0xffffffffu);
		wr.addD(isUnion + hasPadding * 2 + isEmpty * 4);
		wr.addD(alignment);
		wr.addQ(size);
		
		for(std::list<struct Declaration *>::iterator i = decs.begin();
			i != decs.end(); ++i
		)
		{
			(*i)->serializeOut(wr);
		}

		wr.addP(1, 6, 8);
		wr.addP(1, 2, 8);
	}
};

#endif	// l_scope_h__included
