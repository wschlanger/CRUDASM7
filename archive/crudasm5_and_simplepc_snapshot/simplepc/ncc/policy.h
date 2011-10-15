// policy.h
// Copyright (C) 2009 Willow Schlanger

#ifndef l_policy_h__ncc_included
#define	l_policy_h__ncc_included

#include "token_enums.h"

struct CompilerPolicy
{
	static bool isCharSigned() { return false; }
	static bool arePointersSigned() { return false; }
	static U4 getShortSize() { return 2; }
	static U4 getIntSize() { return 4; }
	static U4 getLongSize() { return 4; }
	static U4 getLongLongSize() { return 8; }	// 0 if not supported
	static U4 getPointerSize() { return 2; }	// far pointers--add 2
	static U4 getFloatSize() { return sizeof(float); }
	static U4 getDoubleSize() { return sizeof(double); }
	static U4 getLongDoubleSize() { if(sizeof(LongDouble) > sizeof(double)) return sizeof(LongDouble); else return 0; }
};

#endif	// l_policy_h__ncc_included
