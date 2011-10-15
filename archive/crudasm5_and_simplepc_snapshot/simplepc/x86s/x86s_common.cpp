// x86s_common.cpp
// Copyright (C) 2008 Willow Schlanger

#include "types.h"
#include "x86s_common.h"

namespace x86s
{

U1 get_argtype_lo(U1 x)
{
	return x & ARGTYPE_LO_MASK;
}

U1 get_argtype_hi(U1 x)
{
	return x >> ARGTYPE_HI_SHIFT;
}

U1 get_argsize_lo(U1 x)
{
	return x & ARGSIZE_LO_MASK;
}

UINT get_argsize_bytes(U1 x)
{
	U1 lo = get_argsize_lo(x);
	U1 hi = get_argsize_hi(x);
	UINT sz = 0;
	
	switch(lo)
	{
		case argsize_16:
			sz = 2;
			break;
		case argsize_32:
			sz = 4;
			break;
		case argsize_64:
			sz = 8;
			break;
		case argsize_8:
			sz = 1;
			break;
		case argsize_128:
			sz = 16;
			break;
		case argsize_24:
			sz = 4;
			break;
		case argsize_512b:
			sz = 512;
			break;
		default:
			return 0;	// unknown
	}
	
	switch(hi)
	{
		case argsize__end:	// no hi size
			break;
		case argsize_16:
			sz += 2;
			break;
		case argsize_32:
			sz += 4;
			break;
		case argsize_64:
			sz += 8;
			break;
		case argsize_8:		// not used
			sz += 1;
			break;
		case argsize_128:	// not used
			sz += 16;
			break;
		case argsize_24:
			sz += 4;
			break;
		case argsize_512b:	// not used
			sz += 512;
			break;
		default:
			return 0;
	}
	
	return sz;
}

// This returns argsize__end if the size is unrecognized or if there is no hi-size.
U1 get_argsize_hi(U1 x)
{
	switch(x >> ARGSIZE_HI_SHIFT)
	{
		case argsize_hi_none:
			return argsize__end;
		case argsize_hi_16:
			return argsize_16;
		case argsize_hi_32:
			return argsize_32;
		case argsize_hi_64:
			return argsize_64;
		case argsize_hi_24:
			return argsize_24;
		case argsize_hi_osz:
			return argsize_osz;
		case argsize_hi_osz_24:
			return argsize_osz_24;
		case argsize_hi_osz_old:
			return argsize_osz_old;
		default:
			break;
	}
	return argsize__end;
}

U1 specialize_arg_size(U1 argsize, U4 dsz, U4 osz, U4 asz)
{
	U1 lo = get_argsize_lo(argsize);
	U1 hi = argsize >> ARGSIZE_HI_SHIFT;
	
	switch(lo)
	{
		case argsize_osz_new:		// 64 bits, or 128 if rex.w is used (used by _cmpxchgxb)
			lo = (osz == argsize_64) ? argsize_128 : argsize_64;
			break;
		case argsize_osz_32:		// size is osz unless osz = 16, then use 32 bits (movsx, movzx)
			lo = (osz == argsize_16) ? argsize_32 : osz;
			break;
		case argsize_osz_ptr:		// 64 bits in long mode (default osz = 64), else 32 bits (vmread, vmwrite)
			lo = (dsz == argsize_64) ? argsize_64 : argsize_32;
			break;
		case argsize_osz:
			lo = osz;
			break;
		case argsize_osz_old:		// osz, but never 64 bits (16 or 32 bits)
			lo = (osz == argsize_64) ? argsize_32 : osz;
			break;
		case argsize_osz_24:		// 64 bits if in long mode. else: ->24 if o16, ->32 if o32
			if(dsz == argsize_64)
				lo = argsize_64;
			else
				lo = (osz == argsize_16) ? argsize_hi_24 : argsize_hi_32;
			break;
		case argsize_asz:			// used for loop,loopz,loopnz,jrcxz
			lo = asz;
			break;
		default:
			break;
	}
	
	switch(hi)
	{
		case argsize_hi_osz:
			hi = osz;
			break;
		case argsize_hi_osz_24:
			if(dsz == argsize_64)
				hi = argsize_64;
			else
				hi = (osz == argsize_16) ? argsize_hi_24 : argsize_hi_32;
			break;
		case argsize_hi_osz_old:
			hi = (osz == argsize_64) ? argsize_32 : osz;
			break;
		default:
			break;
	}
	
	return ARGSIZE(lo, hi);
}

bool arg_is_osz_sensitive(U1 argsize)
{
	U1 lo = get_argsize_lo(argsize);
	U1 hi = argsize >> ARGSIZE_HI_SHIFT;
	
	switch(lo)
	{
		case argsize_osz_new:		// 64 bits, or 128 if rex.w is used (used by _cmpxchgxb)
		case argsize_osz_32:		// size is osz unless osz = 16, then use 32 bits (movsx, movzx)
		case argsize_osz_ptr:		// 64 bits in long mode (default osz = 64), else 32 bits (vmread, vmwrite)
		case argsize_osz:
		case argsize_osz_old:		// osz, but never 64 bits (16 or 32 bits)
		case argsize_osz_24:		// 64 bits if in long mode. else: ->24 if o16, ->32 if o32
		case argsize_asz:			// used for loop,loopz,loopnz,jrcxz
			return true;
		default:
			break;
	}
	switch(hi)
	{
		case argsize_hi_osz:
		case argsize_hi_osz_24:
		case argsize_hi_osz_old:
			return true;
		default:
			break;
	}
	return false;
}

}	// namespace x86s
