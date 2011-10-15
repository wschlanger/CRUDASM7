// expr.h
// Copyright (C) 2009 Willow Schlanger

#ifndef l_expr_h__ncc_included
#define	l_expr_h__ncc_included

#include "policy.h"

struct BasicType
{
	union
	{
		struct
		{
			U4 size : 5;			// in bytes (0 to 16)
			U4 modifier : 2;		// 0=signed,1=unsigned,2=floating-point,3=other
			// other: if size is 0 then it's void
			// if size is 1 then it's boolean (we implement bool's as a byte).
			
			// note: if indirection != 0 then size and modifier apply to the base type
			// only!
			U4 indirection : 8;		// number of *'s (up to 255)

			U4 is_const : 1;		// const modifier was specified
			U4 is_volatile : 1;
			U4 is_static : 1;		// \ do these two even
			U4 is_register : 1;		// / belong here at all?
		}	s;
		U4 x;
	}	u;
	BasicType()
	{
		zero();
	}
	void zero() { u.x = 0; }
	bool operator==(const BasicType &src) const
	{
		return !(*this != src);
	}
	bool operator!=(const BasicType &src) const
	{
		return *this < src || src < *this;
	}
	bool operator<(const BasicType &src) const
	{
		return u.x < src.u.x;
	}
};

// All Expression's have a ConstantValue.
struct ConstantValue
{
	union
	{
		U8 qvalue;
		float fvalue;
		double dvalue;
		LongDouble lvalue;
	}	u;
	U4 offset;			// name of a symbol to add to this value
						// 0 for literal values
	U2 size : 5;		// in bytes; 0 if not constant
	U2 type : 2;		// 0=signed,1=unsigned,2=floating-point
						// literal values are always 0 or 1
	U2 offsetIsCode : 1;	// not applicable if offet == 0
	U2 indirection : 8;
	
	// We default to (int)(0).
	ConstantValue()
	{
		u.qvalue = 0;
		offset = 0;
		size = CompilerPolicy::getIntSize();
		type = 0;
		offsetIsCode = 0;	// not applicable
		indirection = 0;	// is not a pointer
	}
	
	bool isLiteral() const
	{
		return offset == 0 && isConstant();
	}
	
	bool isConstant() const
	{
		return size != 0 && type != 2;
	}
};

// When you create a new Expression, do these things:
// 1. update cv. It defaults to (int)(0), which is probably not what you want.
// 2. Walk thru the children of the 'expression' node. Convert nodes into Expression
//    nodes.
// 3. Delete children from the expression node and set is userPtr.
//    Our Compiler will need an 'std::list<Expression>' variable to track all
//    Expressions.
struct Expression
{
	ConstantValue cv;
};

#endif	// l_expr_h__ncc_included
