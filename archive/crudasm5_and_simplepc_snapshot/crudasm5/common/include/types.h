// types.h
// Copyright (C) 2010 Willow Schlanger

#ifndef l_types_h__ceres_included
#define l_types_h__ceres_included

// Note; TULL(x) and TSLL(x) are literals that are always 64bits in size.
// When porting: if sizeof(int) == 8 then define TULL(x) and TSLL(x) as x.
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

// sizeof(UINT) == sizeof(void *)
// sizeof(SINT) == sizeof(void *)
// These are not used anymore--use size_t instead.
#ifndef NO_INT_TYPES
typedef unsigned long long UINT;
typedef signed long long SINT;
#endif

#ifndef PACKED
#ifdef _MSC_VER
#define PACKED(x) x
#else
#define PACKED(x) __attribute__((packed)) x
#endif
#endif

#endif	// l_types_h__ceres_included
