// types.h
// Copyright (C) 2008 Willow Schlanger

// FIXME -- move this someplace else.
// Change 'l_x86s__types_h__included' to 'l_types_h__included' so we can reuse it elsewhere.

#ifndef l_x86s__types_h__included
#define l_x86s__types_h__included

#define PACKED(x) __attribute__((packed)) x

// The following needs to be edited for your platform.
// If LITTLE_ENDIAN, assume unaligned memory accesses are OK.
// --- Note: this program REQUIRES a little endian architecture that can access unaligned memory.
//#define LITTLE_ENDIAN 1

typedef unsigned char U1;
typedef signed char S1;
typedef unsigned short U2;
typedef signed short S2;
typedef unsigned U4;
typedef signed S4;

// The following needs to be edited for your platform.
#ifdef _MSC_VER
typedef unsigned _int64 U8;
typedef signed _int64 S8;
#else
typedef unsigned long U8;
typedef signed long S8;
#endif

// These must have the same size as void *.
typedef unsigned long UINT;
typedef signed long SINT;

//#if LITTLE_ENDIAN
#define GET_U2(x) (*(const U2 *)(x))
#define GET_U4(x) (*(const U4 *)(x))
#define GET_U8(x) (*(const U8 *)(x))
/*#else
#define GET_U2(x) (((U2)(((const U1 *)(x))[0])) | (((U2)(((const U1 *)(x))[1])) << 8))
#define GET_U4(x) (GET_U2((x)) | GET_U2((const U1 *)((x)) + 2))
#define GET_U9(x) (GET_U4((x)) | GET_U4((const U1 *)((x)) + 4))
#endif*/

#endif	// l_x86s__types_h__included
