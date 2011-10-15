// walker.cpp
// Copyright (C) 2008 Willow Schlanger

// fixme--improve 64-bit support

#include "../x86s/types.h"
#include "../x86s/x86s_common.h"
#include "../x86s/x86s_decode.h"
#include "asmwriter.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <set>

#ifdef _MSC_VER
namespace std
{
}
#endif

using namespace std;
using namespace x86s;

struct mem_t
{
	U1 length : 4;		// 0 means never encountered
	U1 entry : 1;		// entrypoint? - return target is not an entrypoint
	U1 invalid : 1;		// not used
	U1 notused1 : 1;
	U1 notused2 : 1;
	
	void clear()
	{
		length = 0;
		entry = 0;
	}
};

struct Walker
{
	U1 *data;
	U4 bufsize;
	int bits;
	U8 loadofs;
	mem_t *mem;
	std::set<U4> targets;
	icode_t icode;
	decode_state_t s;
	NasmWriter disasm;
	
	bool add_target(U4 x)
	{
		if(x > bufsize)
			throw __LINE__;
		if(mem[x].length != 0)
			return false;
		targets.insert(x);
		return true;
	}

	Walker(U1 *data_t, U4 bufsize_t, int bits_t, U8 loadofs_t)
	{
		data = data_t;
		bufsize = bufsize_t;
		bits = bits_t;
		loadofs = loadofs_t;

		mem = new mem_t [bufsize];
		for(U4 i = 0; i < bufsize; ++i)
			mem[i].clear();
		
		add_target(0);		// entrypoint
		s.icode = &icode;
		if(bits == 16)
			s.dsz = argsize_16;
		else
		if(bits == 32)
			s.dsz = argsize_32;
		else
			s.dsz = argsize_64;
	}
	~Walker()
	{
		delete [] mem;
	}
	void walk()
	{
		if(targets.empty())
			return;
		std::set<U4>::iterator i = targets.begin();
		U4 offset = *i;
		targets.erase(i);
		printf("; Now disassembling %08x\n", loadofs + offset);
		
		// disassemble instructions, until:
		// - we encounter an instruction we've already encountered before.
		// - an invalid opcode is encountered.
		// - a return is encountered.
		// - follow unconditional jumps
		// - for calls and branch targets, add the target.
		// - FLAG the insn in the comments if it's an indirect call
		// - For indirect jumps, print a message and return.
		
		s.insn = data + offset;

		UINT curofs;
		UINT bytesleft;

		// ---
		int x;
		for(;;)
		{
			curofs = s.insn - data;
			if(curofs == 0x10000 && bits == 16)
			{
				printf("; Wrap around deteted - NOT SUPPORTED!\n");
				return;
			}
			
			if(mem[curofs].length != 0)
			{
				printf("; Already been here - %08x\n", (U4)(loadofs + curofs));
				return ;
			}
			
			bytesleft = bufsize - curofs;
			s.end = (bytesleft <= 15) ? (s.insn + bytesleft) : (s.insn + 15);
			x = decode(s);
			if(x == 1)
			{
				printf("; Reached end of code segment.\n");
				return;
			}
			else
			if(x != 0)
			{
				printf("; Invalid opcode detected.\n");
				return;
			}
	
			if(s.encoding == 0xffffff || s.icode->insn >= insn__count)
			{
				printf("; ---");
				return;
			}
			
			printf("%08x  ", (U4)(loadofs + curofs));
			char dest[1024];
			
			dest[0] = '\0';
			disasm.disasm(dest, &s, NULL, NULL, (U8)(loadofs) + (U8)(curofs) + (U8)(s.size));
			mem[curofs].length = s.size;
	
			printf("%s\n", dest);
			
			switch(encodings[s.encoding].insn)
			{
				case insn__ret:
				case insn__retf:
				case insn__retfnum:
				case insn__retnum:
				case insn__iret:
					printf("; Return detected.\n");
					return;
					break;
				case insn_jmp:
					offset = s.icode->imm + (U4)curofs + (U4)s.size + (U4)loadofs;
					if(bits == 16)
						offset &= 0xffff;
					printf("; Jump detected to %04x\n", offset);
					if((offset - loadofs) > bufsize)
						throw __LINE__;
					if(mem[offset - loadofs].length != 0)
					{
						printf("; Already been there.\n");
						return;
					}					
					s.insn = data + offset - loadofs;
					continue;
				case insn_call:
				case insn__jcc:
				case insn__jrcxz:
				case insn__loopnz:
				case insn__loopz:
				case insn__loop:
					offset = s.icode->imm + (U4)curofs + (U4)s.size + (U4)loadofs;
					if(bits == 16)
						offset &= 0xffff;
					printf("; Branch detected to %04x\n", offset);
					if((offset - loadofs) > bufsize)
						throw __LINE__;
					if(mem[offset - loadofs].length != 0)
					{
						printf("; Already been there.\n");
					}
					else
					{
						printf("; Added to list.\n");
						add_target(offset - loadofs);
					}
					break;
				case insn__calli:
				case insn__callfd:
				case insn__callfi:
					printf("; call not followed\n");
					break;
				case insn__jmpi:
				case insn__jmpfd:
				case insn__jmpfi:
					printf("; jmp not handled - returnning!\n");
					return;
				default:
					break;
			}
			
			s.insn += s.size;
			if(s.insn == (bufsize + data))
				break;
		}
		// ---
	}
};

// This decodes a buffer of data - each instruction - and prints out
// the result to stdout.
static void do_decode(U1 *data, U4 bufsize, int bits, U8 loadofs)
{
	Walker w(data, bufsize, bits, loadofs);

	while(!w.targets.empty())
	{
		printf("\n");
		w.walk();
	}

#if 0
	icode_t icode;
	decode_state_t s;
	UINT curofs;
	UINT bytesleft;
	
	s.icode = &icode;
	s.insn = data;
	if(bits == 16)
		s.dsz = argsize_16;
	else
	if(bits == 32)
		s.dsz = argsize_32;
	else
		s.dsz = argsize_64;
	
	char dest[1024];

	NasmWriter disasm;

	int x;
	for(;;)
	{
		curofs = s.insn - data;
		bytesleft = bufsize - curofs;
		s.end = (bytesleft <= 15) ? (s.insn + bytesleft) : (s.insn + 15);
		x = decode(s);
		if(x == 1)
		{
			// bugfix from release 0.05.
			//printf("abort: cs segment limit exceeded (or internal error)\n");
			for(int i = 0; i < bytesleft; ++i)
				printf("%08x  db 0x%02x\n", (U4)(loadofs + curofs + i), data[curofs + i]);
			//end bugfix
			return;
		}
		else
		if(x != 0)
		{
			//printf("abort: invalid opcode detected\n");
			//return;
			printf("%08x  db 0x%02x\n", (U4)(loadofs + curofs), data[curofs]);

			++s.insn;
			if(s.insn == (bufsize + data))
				break;
			continue;
		}

		if(s.encoding == 0xffffff || s.icode->insn >= insn__count)
		{
			printf("---");
			return;
		}
		
		printf("%08x  ", (U4)(loadofs + curofs));

		dest[0] = '\0';
		disasm.disasm(dest, &s, NULL, NULL, (U8)(loadofs) + (U8)(curofs) + (U8)(s.size));

		printf("%s\n", dest);
		
		s.insn += s.size;
		if(s.insn == (bufsize + data))
			break;
	}
#endif
}

void help()
{
	printf("crudasm1  Copyright (C) 2008 Willow Schlanger\n");
	printf("This program disassembles an x86 binary image file\n");
	printf("\n");
	printf("usage: crudasm1 file bits origin [bytes to skip]\n");
	printf("example: crudasm myprogram.com 16 0x100\n");
}

static U1 *read_file(const char *filename, U4 *size)
{
	U1 *data;
	FILE *f = fopen(filename, "rb");
	if(f == NULL)
		return NULL;
	if(feof(f))
	{
		fclose(f);
		return NULL;	// no data
	}
	fseek(f, -1, SEEK_END);
	*size = 1 + ftell(f);
	data = (U1 *)malloc(*size);
	rewind(f);
	if(fread(data, *size, 1, f) != 1)
	{
		free(data);
		fclose(f);
		return NULL;	// unable to read
	}
	fclose(f);
	return data;
}

int main()
{
	const char *filename = "a.com";
	int bits = 16;
	U4 origin = 0x100;
	
	//printf("%s %d %08x %08x\n", filename, bits, origin, skip);
	U4 size;
	U1 *dat = read_file(filename, &size);
	if(dat == NULL)
	{
		printf("Error: unable to open file \'%s\' or file has a size of 0 bytes\n", filename);
		return 1;
	}
	U1 *bin = dat;
	
	try
	{	
		do_decode(bin, size, bits, origin);
	}
	catch(int x)
	{
		printf("Internal error.\n");
	}
	
	free(dat);
	
	return 0;
}
