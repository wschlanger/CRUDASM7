// hasher.h
// Copyright (C) 2009 Willow Schlanger

#ifndef l_hasher_h__raid__included
#define l_hasher_h__raid__included

//---
// lookup8.c, by Bob Jenkins, January 4 1997, Public Domain.
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
typedef  unsigned long  long ub8;   /* unsigned 8-byte quantities */
typedef  unsigned long  int  ub4;   /* unsigned 4-byte quantities */
typedef  unsigned       char ub1;

#define hashsize(n) ((ub8)1<<(n))
#define hashmask(n) (hashsize(n)-1)
#define mix64(a,b,c) \
{ \
  a -= b; a -= c; a ^= (c>>43); \
  b -= c; b -= a; b ^= (a<<9); \
  c -= a; c -= b; c ^= (b>>8); \
  a -= b; a -= c; a ^= (c>>38); \
  b -= c; b -= a; b ^= (a<<23); \
  c -= a; c -= b; c ^= (b>>5); \
  a -= b; a -= c; a ^= (c>>35); \
  b -= c; b -= a; b ^= (a<<49); \
  c -= a; c -= b; c ^= (b>>11); \
  a -= b; a -= c; a ^= (c>>12); \
  b -= c; b -= a; b ^= (a<<18); \
  c -= a; c -= b; c ^= (b>>22); \
}
ub8 hash2( ub8 *k, ub8 length, ub8 level)
{
  register ub8 a,b,c,len;

  /* Set up the internal state */
  len = length;
  a = b = level;                         /* the previous hash value */
  c = 0x9e3779b97f4a7c13LL; /* the golden ratio; an arbitrary value */

  /*---------------------------------------- handle most of the key */
  while (len >= 3)
  {
    a += k[0];
    b += k[1];
    c += k[2];
    mix64(a,b,c);
    k += 3; len -= 3;
  }

  /*-------------------------------------- handle the last 2 ub8's */
  c += (length<<3);
  switch(len)              /* all the case statements fall through */
  {
    /* c is reserved for the length */
  case  2: b+=k[1];
  case  1: a+=k[0];
    /* case 0: nothing left to add */
  }
  mix64(a,b,c);
  /*-------------------------------------------- report the result */
  return c;
}
//---

class Hasher
{
	U8 hash;
	public:
	Hasher() { hash = 0; }
	void add(U4 value)
	{
		U8 value2 = value;
		hash = hash2((ub8 *)(&value2), 1, hash);
	}
	U8 get() const
	{
		return hash;
	}
};

#endif	// l_hasher_h__raid__included
