// types.h
// Copyright (C) 2010 Willow Schlanger

#ifndef l_types_h__ceres_included
#define l_types_h__ceres_included

#ifndef BITS32

// Note: TULL(x) and TSLL(x) are literals that are always 64bits in size.
// When porting: if sizeof(int) == 8 then define TULL(x) and TSLL(x) as x.
// If sizeof(long) == 8, we need to add a UL or L suffix.
// If sizeof(long) == 4 but sizeof(long long) == 8, change UL -> ULL, etc.
#define TULL(x) (x##UL)
#define TSLL(x) (x##L)

typedef unsigned char U1;
typedef unsigned short U2;
typedef unsigned int U4;
typedef unsigned long U8;

typedef signed char S1;
typedef signed short S2;
typedef signed int S4;
typedef signed long S8;

#else

#define TULL(x) (x##ULL)
#define TSLL(x) (x##LL)

typedef unsigned char U1;
typedef unsigned short U2;
typedef unsigned int U4;
typedef unsigned long long U8;

typedef signed char S1;
typedef signed short S2;
typedef signed int S4;
typedef signed long long S8;

#endif

#ifndef PACKED
#ifdef _MSC_VER
#define PACKED(x) x
#else
#define PACKED(x) __attribute__((packed)) x
#endif
#endif

#endif	// l_types_h__ceres_included
