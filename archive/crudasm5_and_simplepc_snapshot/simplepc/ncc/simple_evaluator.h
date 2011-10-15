// simple_evaluator.h
// Copyright (C) 2009 Willow Schlanger

// This is included by: emulator.h
// Do not include it directly, but include emulator.h instead when it is needed.

#include <iostream>

class SimpleEvaluator
{
	U8 &rflags;
	public:
	SimpleEvaluator(U8 &rflagsT) :
		rflags(rflagsT)
	{
	}
	
	static Operand comp_cmul(Operand a, Operand b)
	{
		Operand c(a.low_size);
		if(a.low_size == 1)
			c.u.s.base.cvalue[0] = (S1)(a.u.s.base.cvalue[0]) * (S1)(b.u.s.base.cvalue[0]);
		else
		if(a.low_size == 2)
			c.u.s.base.svalue[0] = (S2)(a.u.s.base.svalue[0]) * (S2)(b.u.s.base.svalue[0]);
		else
		if(a.low_size == 4)
			c.u.s.base.ivalue[0] = (S4)(a.u.s.base.ivalue[0]) * (S4)(b.u.s.base.ivalue[0]);
		else
		{
			assert(a.low_size == 8);
			c.u.s.base.value = (S8)(a.u.s.base.value) * (S8)(b.u.s.base.value);
		}
		return c;
	}
	static Operand comp_add(Operand a, Operand b)
	{
		Operand c(a.low_size);
assert(a.low_size == b.low_size);
		if(a.low_size == 1)
			c.u.s.base.cvalue[0] = a.u.s.base.cvalue[0] + b.u.s.base.cvalue[0];
		else
		if(a.low_size == 2)
			c.u.s.base.svalue[0] = a.u.s.base.svalue[0] + b.u.s.base.svalue[0];
		else
		if(a.low_size == 4)
			c.u.s.base.ivalue[0] = a.u.s.base.ivalue[0] + b.u.s.base.ivalue[0];
		else
		{
			assert(a.low_size == 8);
			c.u.s.base.value = a.u.s.base.value + b.u.s.base.value;
		}
		return c;
	}
	static Operand comp_sub(Operand a, Operand b)
	{
		Operand c(a.low_size);
		if(a.low_size == 1)
			c.u.s.base.cvalue[0] = a.u.s.base.cvalue[0] - b.u.s.base.cvalue[0];
		else
		if(a.low_size == 2)
			c.u.s.base.svalue[0] = a.u.s.base.svalue[0] - b.u.s.base.svalue[0];
		else
		if(a.low_size == 4)
			c.u.s.base.ivalue[0] = a.u.s.base.ivalue[0] - b.u.s.base.ivalue[0];
		else
		{
			assert(a.low_size == 8);
			c.u.s.base.value = a.u.s.base.value - b.u.s.base.value;
		}
		return c;
	}
	static Operand comp_and(Operand a, Operand b)
	{
		Operand c(a.low_size);
		if(a.low_size == 1)
			c.u.s.base.cvalue[0] = a.u.s.base.cvalue[0] & b.u.s.base.cvalue[0];
		else
		if(a.low_size == 2)
			c.u.s.base.svalue[0] = a.u.s.base.svalue[0] & b.u.s.base.svalue[0];
		else
		if(a.low_size == 4)
			c.u.s.base.ivalue[0] = a.u.s.base.ivalue[0] & b.u.s.base.ivalue[0];
		else
		{
			assert(a.low_size == 8);
			c.u.s.base.value = a.u.s.base.value & b.u.s.base.value;
		}
		return c;
	}
	static Operand comp_xor(Operand a, Operand b)
	{
		Operand c(a.low_size);
		if(a.low_size == 1)
			c.u.s.base.cvalue[0] = a.u.s.base.cvalue[0] ^ b.u.s.base.cvalue[0];
		else
		if(a.low_size == 2)
			c.u.s.base.svalue[0] = a.u.s.base.svalue[0] ^ b.u.s.base.svalue[0];
		else
		if(a.low_size == 4)
			c.u.s.base.ivalue[0] = a.u.s.base.ivalue[0] ^ b.u.s.base.ivalue[0];
		else
		{
			assert(a.low_size == 8);
			c.u.s.base.value = a.u.s.base.value ^ b.u.s.base.value;
		}
		return c;
	}
	static Operand comp_or(Operand a, Operand b)
	{
		Operand c(a.low_size);
		if(a.low_size == 1)
			c.u.s.base.cvalue[0] = a.u.s.base.cvalue[0] | b.u.s.base.cvalue[0];
		else
		if(a.low_size == 2)
			c.u.s.base.svalue[0] = a.u.s.base.svalue[0] | b.u.s.base.svalue[0];
		else
		if(a.low_size == 4)
			c.u.s.base.ivalue[0] = a.u.s.base.ivalue[0] | b.u.s.base.ivalue[0];
		else
		{
			assert(a.low_size == 8);
			c.u.s.base.value = a.u.s.base.value | b.u.s.base.value;
		}
		return c;
	}
	static Operand get_literal(int size, U8 value)
	{
		Operand op(size);
		op.u.s.base.value = value;
		return op;
	}
	static Operand comp_zero(Operand src)
	{
		for(int i = 0; i < src.low_size; ++i)
			if(src.u.s.base.cvalue[i] != 0)
				return get_literal(1, 0);
		return get_literal(1, 1);
	}
	static Operand comp_nz(Operand src)
	{
		for(int i = 0; i < src.low_size; ++i)
			if(src.u.s.base.cvalue[i] != 0)
				return get_literal(1, 1);
		return get_literal(1, 0);
	}
	static Operand comp_parity(Operand src)
	{
		Operand c(1);
		U1 d = src.u.s.base.cvalue[0];
		U1 pf = 1;
		for(UINT i = 0; i < 8; ++i)
			pf ^= (d >> 1) & 1;
		c.u.s.base.cvalue[0] = pf;
		return c;
	}
	static Operand comp_sign(Operand src)
	{
		if(src.low_size == 1)
			return get_literal(1, (src.u.s.base.cvalue[0] >> 7) & 1);
		else
		if(src.low_size == 2)
			return get_literal(1, (src.u.s.base.svalue[0] >> 15) & 1);
		else
		if(src.low_size == 4)
			return get_literal(1, (src.u.s.base.ivalue[0] >> 31) & 1);
		else
		if(src.low_size == 8)
			return get_literal(1, (src.u.s.base.value >> 63) & 1);
		else
			assert(0);
	}
	static Operand comp_sign2(Operand src)
	{
		if(src.low_size == 1)
			return get_literal(1, (src.u.s.base.cvalue[0] >> 6) & 1);
		else
		if(src.low_size == 2)
			return get_literal(1, (src.u.s.base.svalue[0] >> 14) & 1);
		else
		if(src.low_size == 4)
			return get_literal(1, (src.u.s.base.ivalue[0] >> 30) & 1);
		else
		if(src.low_size == 8)
			return get_literal(1, (src.u.s.base.value >> 62) & 1);
		else
			assert(0);
	}
	static Operand get_zx(int new_size, Operand src)
	{
		U1 fill = 0;
		Operand c(new_size);
		int i;
		for(i = 0; i < src.low_size; ++i)
			c.u.s.base.cvalue[i] = src.u.s.base.cvalue[i];
		for(; i < new_size; ++i)
			c.u.s.base.cvalue[i] = fill;
		return c;
	}
	static Operand get_sx(int new_size, Operand src)
	{
		U1 fill = (comp_sign(src).u.s.base.cvalue[0] != 0) ? 0xff : 0x00;
		Operand c(new_size);
		int i;
		for(i = 0; i < src.low_size; ++i)
			c.u.s.base.cvalue[i] = src.u.s.base.cvalue[i];
		for(; i < new_size; ++i)
			c.u.s.base.cvalue[i] = fill;
		return c;
	}
	static Operand get_tr(int new_size, Operand src)
	{
		Operand c = src;
		c.low_size = new_size;
		return c;
	}
	static Operand comp_quest(Operand q, Operand a, Operand b)
	{
		Operand c(a.low_size);
		if(q.u.s.base.cvalue[0] != 0)
			c = a;
		else
			c = b;
		return c;
	}
	static Operand comp_bitquest(Operand q, Operand a, Operand b)
	{
		return comp_quest(q, a, b);
	}
	static Operand comp_ofs(Operand src)
	{
		Operand c(src.low_size);
		stdwrite(c, src);
		return c;
	}
	static Operand comp_seg(Operand src)
	{
		Operand c(2);
		c.u.s.base.svalue[0] = src.u.s.aux;
///std::cerr << "(seg=" << std::hex << src.u.s.aux << std::dec << ")";
		return c;
	}
	static Operand comp_neg(Operand src)
	{
		return comp_sub(get_literal(src.low_size, 0), src);
	}
	static Operand comp_not(Operand src)
	{
		return comp_xor(get_literal(src.low_size, ~0LL), src);
	}
	static Operand comp_ltu(Operand x, Operand y)
	{
		bool result;
		if(x.low_size == 1)
			result = (U1)(x.u.s.base.cvalue[0]) < (U1)(y.u.s.base.cvalue[0]);
		else
		if(x.low_size == 2)
			result = (U2)(x.u.s.base.svalue[0]) < (U2)(y.u.s.base.svalue[0]);
		else
		if(x.low_size == 4)
			result = (U4)(x.u.s.base.ivalue[0]) < (U4)(y.u.s.base.ivalue[0]);
		else
		if(x.low_size == 8)
			result = (U8)(x.u.s.base.value) < (U8)(y.u.s.base.value);
		else
		{
			assert(0);
			throw -1;
		}
		return (result) ? get_literal(1, 1) : get_literal(1, 0);
	}
	static Operand comp_leu(Operand x, Operand y)
	{
		bool result;
		if(x.low_size == 1)
			result = (U1)(x.u.s.base.cvalue[0]) <= (U1)(y.u.s.base.cvalue[0]);
		else
		if(x.low_size == 2)
			result = (U2)(x.u.s.base.svalue[0]) <= (U2)(y.u.s.base.svalue[0]);
		else
		if(x.low_size == 4)
			result = (U4)(x.u.s.base.ivalue[0]) <= (U4)(y.u.s.base.ivalue[0]);
		else
		if(x.low_size == 8)
			result = (U8)(x.u.s.base.value) <= (U8)(y.u.s.base.value);
		else
		{
			assert(0);
			throw -1;
		}
		return (result) ? get_literal(1, 1) : get_literal(1, 0);
	}
	static Operand comp_geu(Operand x, Operand y)
	{
		bool result;
		if(x.low_size == 1)
			result = (U1)(x.u.s.base.cvalue[0]) >= (U1)(y.u.s.base.cvalue[0]);
		else
		if(x.low_size == 2)
			result = (U2)(x.u.s.base.svalue[0]) >= (U2)(y.u.s.base.svalue[0]);
		else
		if(x.low_size == 4)
			result = (U4)(x.u.s.base.ivalue[0]) >= (U4)(y.u.s.base.ivalue[0]);
		else
		if(x.low_size == 8)
			result = (U8)(x.u.s.base.value) >= (U8)(y.u.s.base.value);
		else
		{
			assert(0);
			throw -1;
		}
		return (result) ? get_literal(1, 1) : get_literal(1, 0);
	}
	static Operand comp_gtu(Operand x, Operand y)
	{
		bool result;
		if(x.low_size == 1)
			result = (U1)(x.u.s.base.cvalue[0]) > (U1)(y.u.s.base.cvalue[0]);
		else
		if(x.low_size == 2)
			result = (U2)(x.u.s.base.svalue[0]) > (U2)(y.u.s.base.svalue[0]);
		else
		if(x.low_size == 4)
			result = (U4)(x.u.s.base.ivalue[0]) > (U4)(y.u.s.base.ivalue[0]);
		else
		if(x.low_size == 8)
			result = (U8)(x.u.s.base.value) > (U8)(y.u.s.base.value);
		else
		{
			assert(0);
			throw -1;
		}
		return (result) ? get_literal(1, 1) : get_literal(1, 0);
	}
	static Operand comp_lts(Operand x, Operand y)
	{
		bool result;
		if(x.low_size == 1)
			result = (S1)(x.u.s.base.cvalue[0]) < (S1)(y.u.s.base.cvalue[0]);
		else
		if(x.low_size == 2)
			result = (S2)(x.u.s.base.svalue[0]) < (S2)(y.u.s.base.svalue[0]);
		else
		if(x.low_size == 4)
			result = (S4)(x.u.s.base.ivalue[0]) < (S4)(y.u.s.base.ivalue[0]);
		else
		if(x.low_size == 8)
			result = (S8)(x.u.s.base.value) < (S8)(y.u.s.base.value);
		else
		{
			assert(0);
			throw -1;
		}
		return (result) ? get_literal(1, 1) : get_literal(1, 0);
	}
	static Operand comp_les(Operand x, Operand y)
	{
		bool result;
		if(x.low_size == 1)
			result = (S1)(x.u.s.base.cvalue[0]) <= (S1)(y.u.s.base.cvalue[0]);
		else
		if(x.low_size == 2)
			result = (S2)(x.u.s.base.svalue[0]) <= (S2)(y.u.s.base.svalue[0]);
		else
		if(x.low_size == 4)
			result = (S4)(x.u.s.base.ivalue[0]) <= (S4)(y.u.s.base.ivalue[0]);
		else
		if(x.low_size == 8)
			result = (S8)(x.u.s.base.value) <= (S8)(y.u.s.base.value);
		else
		{
			assert(0);
			throw -1;
		}
		return (result) ? get_literal(1, 1) : get_literal(1, 0);
	}
	static Operand comp_ges(Operand x, Operand y)
	{
		bool result;
		if(x.low_size == 1)
			result = (S1)(x.u.s.base.cvalue[0]) >= (S1)(y.u.s.base.cvalue[0]);
		else
		if(x.low_size == 2)
			result = (S2)(x.u.s.base.svalue[0]) >= (S2)(y.u.s.base.svalue[0]);
		else
		if(x.low_size == 4)
			result = (S4)(x.u.s.base.ivalue[0]) >= (S4)(y.u.s.base.ivalue[0]);
		else
		if(x.low_size == 8)
			result = (S8)(x.u.s.base.value) >= (S8)(y.u.s.base.value);
		else
		{
			assert(0);
			throw -1;
		}
		return (result) ? get_literal(1, 1) : get_literal(1, 0);
	}
	static Operand comp_gts(Operand x, Operand y)
	{
		bool result;
		if(x.low_size == 1)
			result = (S1)(x.u.s.base.cvalue[0]) > (S1)(y.u.s.base.cvalue[0]);
		else
		if(x.low_size == 2)
			result = (S2)(x.u.s.base.svalue[0]) > (S2)(y.u.s.base.svalue[0]);
		else
		if(x.low_size == 4)
			result = (S4)(x.u.s.base.ivalue[0]) > (S4)(y.u.s.base.ivalue[0]);
		else
		if(x.low_size == 8)
			result = (S8)(x.u.s.base.value) > (S8)(y.u.s.base.value);
		else
		{
			assert(0);
			throw -1;
		}
		return (result) ? get_literal(1, 1) : get_literal(1, 0);
	}
	static Operand comp_eq(Operand x, Operand y)
	{
		bool result;
		if(x.low_size == 1)
			result = (S1)(x.u.s.base.cvalue[0]) == (S1)(y.u.s.base.cvalue[0]);
		else
		if(x.low_size == 2)
			result = (S2)(x.u.s.base.svalue[0]) == (S2)(y.u.s.base.svalue[0]);
		else
		if(x.low_size == 4)
			result = (S4)(x.u.s.base.ivalue[0]) == (S4)(y.u.s.base.ivalue[0]);
		else
		if(x.low_size == 8)
			result = (S8)(x.u.s.base.value) == (S8)(y.u.s.base.value);
		else
		{
			assert(0);
			throw -1;
		}
		return (result) ? get_literal(1, 1) : get_literal(1, 0);
	}
	static Operand comp_ne(Operand x, Operand y)
	{
		bool result;
		if(x.low_size == 1)
			result = (S1)(x.u.s.base.cvalue[0]) != (S1)(y.u.s.base.cvalue[0]);
		else
		if(x.low_size == 2)
			result = (S2)(x.u.s.base.svalue[0]) != (S2)(y.u.s.base.svalue[0]);
		else
		if(x.low_size == 4)
			result = (S4)(x.u.s.base.ivalue[0]) != (S4)(y.u.s.base.ivalue[0]);
		else
		if(x.low_size == 8)
			result = (S8)(x.u.s.base.value) != (S8)(y.u.s.base.value);
		else
		{
			assert(0);
			throw -1;
		}
		return (result) ? get_literal(1, 1) : get_literal(1, 0);
	}
	
	static Operand comp_adc_af(Operand a, Operand b, Operand c)
	{
		Operand result(1);
		U1 aa = a.u.s.base.cvalue[0] & 0x0f;
		U1 bb = b.u.s.base.cvalue[0] & 0x0f;
		U1 cc = c.u.s.base.cvalue[0];	// always 0 or 1
		U1 d = aa + bb + cc;			// was U2 (makes no difference)
		result.u.s.base.cvalue[0] = (d > 0x0f) ? 1 : 0;
		return result;
	}
	
	static Operand comp_adc_cf(Operand a, Operand b, Operand c)
	{
		Operand result(1);
		result.u.s.base.cvalue[0] = comp_add_cf(a, b).u.s.base.cvalue[0];
		Operand one = get_literal(a.low_size, c.u.s.base.cvalue[0]);
		if(comp_add_cf(comp_add(a, b), one).u.s.base.cvalue[0] != 0)
			result.u.s.base.cvalue[0] = 1;
		return result;
	}
	
	static Operand comp_adc_of(Operand a, Operand b, Operand c)
	{
		Operand result(1);
		result.u.s.base.cvalue[0] = comp_add_of(a, b).u.s.base.cvalue[0];
		Operand one = get_literal(a.low_size, c.u.s.base.cvalue[0]);
		if(comp_add_of(comp_add(a, b), one).u.s.base.cvalue[0] != 0)
			result.u.s.base.cvalue[0] = 1;
		return result;
	}
	
	static Operand comp_sbb_af(Operand a, Operand b, Operand c)
	{
		Operand result(1);
		U1 aa = a.u.s.base.cvalue[0];
		U1 bb = b.u.s.base.cvalue[0];
		result.u.s.base.cvalue[0] = (aa & 0x0f) < (bb & 0x0f);
		if((aa & 0x0f) == (bb & 0x0f))
		{
			if(c.u.s.base.cvalue[0] == 1)
				result.u.s.base.cvalue[0] = 1;
		}
		return result;
	}
	
	static Operand comp_sbb_cf(Operand a, Operand b, Operand c)
	{
		Operand result(1);
		result.u.s.base.cvalue[0] = comp_sub(a, b).u.s.base.cvalue[0];
		Operand one = get_literal(a.low_size, c.u.s.base.cvalue[0]);
		if(comp_sub_cf(comp_sub(a, b), one).u.s.base.cvalue[0] != 0)
			result.u.s.base.cvalue[0] = 1;
		return result;
	}
	
	static Operand comp_sbb_of(Operand a, Operand b, Operand c)
	{
		Operand result(1);
		result.u.s.base.cvalue[0] = comp_sub(a, b).u.s.base.cvalue[0];
		Operand one = get_literal(a.low_size, c.u.s.base.cvalue[0]);
		if(comp_add_of(comp_sub(a, b), one).u.s.base.cvalue[0] != 0)
			result.u.s.base.cvalue[0] = 1;
		return result;
	}
	
	static Operand comp_add_af(Operand a, Operand b)
	{
		Operand result(1);
		U1 aa = a.u.s.base.cvalue[0] & 0x0f;
		U1 bb = b.u.s.base.cvalue[0] & 0x0f;
		U1 c = aa + bb;
		result.u.s.base.cvalue[0] = c > 0x0f;
		return result;
	}
	
	static Operand comp_add_cf(Operand a, Operand b)
	{
		Operand result(1);
		
		if(a.low_size == 1)
		{
			U1 aa = a.u.s.base.cvalue[0];
			U1 bb = b.u.s.base.cvalue[0];
			U1 cc = aa + bb;
			result.u.s.base.cvalue[0] = cc < aa;
		}
		else
		if(a.low_size == 2)
		{
			U2 aa = a.u.s.base.svalue[0];
			U2 bb = b.u.s.base.svalue[0];
			U2 cc = aa + bb;
			result.u.s.base.cvalue[0] = cc < aa;
		}
		else
		if(a.low_size == 4)
		{
			U4 aa = a.u.s.base.ivalue[0];
			U4 bb = b.u.s.base.ivalue[0];
			U4 cc = aa + bb;
			result.u.s.base.cvalue[0] = cc < aa;
		}
		else
		if(a.low_size == 8)
		{
			U8 aa = a.u.s.base.value;
			U8 bb = b.u.s.base.value;
			U8 cc = aa + bb;
			result.u.s.base.cvalue[0] = cc < aa;
		}
		else
		{
			assert(0);
			throw -1;
		}
		
		return result;
	}
	
	static Operand comp_add_of(Operand a, Operand b)
	{
		Operand result(1);
		
		if(a.low_size == 1 && b.u.s.base.cvalue[0] == 0x80)
			result.u.s.base.cvalue[0] = 1;
		else
		if(a.low_size == 2 && b.u.s.base.svalue[0] == 0x8000)
			result.u.s.base.cvalue[0] = 1;
		else
		if(a.low_size == 4 && b.u.s.base.ivalue[0] == 0x80000000)
			result.u.s.base.cvalue[0] = 1;
		else
		if(a.low_size == 8 && b.u.s.base.value == 0x8000000000000000ull)
			result.u.s.base.cvalue[0] = 1;
		else
			result.u.s.base.cvalue[0] = (comp_sub_of(a, comp_neg(b))).u.s.base.cvalue[0];
		
		return result;
	}
	
	static Operand comp_sub_af(Operand a, Operand b)
	{
		Operand result(1);
		U1 aa = a.u.s.base.cvalue[0];
		U1 bb = b.u.s.base.cvalue[0];
		result.u.s.base.cvalue[0] = (aa & 0x0f) < (bb & 0x0f);
		return result;
	}
	
	static Operand comp_sub_cf(Operand a, Operand b)
	{
		Operand result(1);
		
		assert(a.low_size == b.low_size);
		
		if(a.low_size == 1)
		{
			U1 aa = a.u.s.base.cvalue[0];
			U1 bb = b.u.s.base.cvalue[0];
			result.u.s.base.cvalue[0] = aa < bb;
		}
		else
		if(a.low_size == 2)
		{
			U2 aa = a.u.s.base.svalue[0];
			U2 bb = b.u.s.base.svalue[0];
			result.u.s.base.cvalue[0] = aa < bb;
		}
		else
		if(a.low_size == 4)
		{
			U4 aa = a.u.s.base.ivalue[0];
			U4 bb = b.u.s.base.ivalue[0];
			result.u.s.base.cvalue[0] = aa < bb;
		}
		else
		if(a.low_size == 8)
		{
			U8 aa = a.u.s.base.value;
			U8 bb = b.u.s.base.value;
			result.u.s.base.cvalue[0] = aa < bb;
		}
		else
		{
			assert(0);
			throw -1;
		}
		
		return result;
	}
	
	static Operand comp_sub_of(Operand a, Operand b)
	{
		Operand result(1);
		
		if(a.low_size == 1)
		{
			S1 aa = a.u.s.base.cvalue[0];
			S1 bb = b.u.s.base.cvalue[0];
			U1 less = aa < bb;
			S1 cc = aa - bb;
			U1 sign = cc < 0;
			result.u.s.base.cvalue[0] = ((sign ^ less) != 0) ? 1 : 0;
		}
		else
		if(a.low_size == 2)
		{
			S2 aa = a.u.s.base.svalue[0];
			S2 bb = b.u.s.base.svalue[0];
			U1 less = aa < bb;
			S2 cc = aa - bb;
			U1 sign = cc < 0;
			result.u.s.base.cvalue[0] = ((sign ^ less) != 0) ? 1 : 0;
		}
		else
		if(a.low_size == 4)
		{
			S4 aa = a.u.s.base.ivalue[0];
			S4 bb = b.u.s.base.ivalue[0];
			U1 less = aa < bb;
			S4 cc = aa - bb;
			U1 sign = cc < 0;
			result.u.s.base.cvalue[0] = ((sign ^ less) != 0) ? 1 : 0;
		}
		else
		if(a.low_size == 8)
		{
			S8 aa = a.u.s.base.value;
			S8 bb = b.u.s.base.value;
			U1 less = aa < bb;
			S8 cc = aa - bb;
			U1 sign = cc < 0;
			result.u.s.base.cvalue[0] = ((sign ^ less) != 0) ? 1 : 0;
		}
		else
		{
			assert(0);
			throw -1;
		}
		
		return result;
	}
	
	static Operand comp_rcl_cf(Operand dest, Operand tmpcount, Operand incf)
	{
		Operand result(dest.low_size);
		Operand outcf(1);
		do_rcl(dest, tmpcount, incf, result, outcf);
		return outcf;
	}
	
	static Operand comp_rcr_cf(Operand dest, Operand tmpcount, Operand incf)
	{
		Operand result(dest.low_size);
		Operand outcf(1);
		do_rcr(dest, tmpcount, incf, result, outcf);
		return outcf;
	}
	
	static Operand comp_land(Operand bit_x, Operand bit_y)
	{
		if(bit_x.u.s.base.cvalue[0] != 0 && bit_y.u.s.base.cvalue[0] != 0)
			return get_literal(1, 1);
		return get_literal(1, 0);
	}
	
	static Operand comp_lor(Operand bit_x, Operand bit_y)
	{
		if(bit_x.u.s.base.cvalue[0] != 0 || bit_y.u.s.base.cvalue[0] != 0)
			return get_literal(1, 1);
		return get_literal(1, 0);
	}

	static Operand comp_lnot(Operand bit_x)
	{
		if(bit_x.u.s.base.cvalue[0] == 0)
			return get_literal(1, 1);
		return get_literal(1, 0);
	}
	
	static Operand comp_rcl(Operand dest, Operand tmpcount, Operand incf)
	{
		Operand result(dest.low_size);
		Operand outcf(1);
		do_rcl(dest, tmpcount, incf, result, outcf);
		return result;
	}
	
	static Operand comp_rcr(Operand dest, Operand tmpcount, Operand incf)
	{
		Operand result(dest.low_size);
		Operand outcf(1);
		do_rcr(dest, tmpcount, incf, result, outcf);
		return result;
	}
	
	private:
	static void do_rcr(Operand dest, Operand intmpcount, Operand incf, Operand &result, Operand &outcf)
	{
		outcf.u.s.base.cvalue[0] = incf.u.s.base.cvalue[0];
		result = dest;

		Operand tmpcf(1);
		Operand adder = get_literal(dest.low_size, 0);
		Operand tmpcount = intmpcount;
		Operand one = get_literal(dest.low_size, 1);
		
		U8 bignum;
		if(dest.low_size == 1)
			bignum = 0x80;
		else
		if(dest.low_size == 2)
			bignum = 0x8000;
		else
		if(dest.low_size == 4)
			bignum = 0x80000000;
		else
		if(dest.low_size == 8)
			bignum = 0x8000000000000000ull;
		
		while(tmpcount.u.s.base.cvalue[0] != 0)
		{
			tmpcf.u.s.base.cvalue[0] = comp_lsb(result).u.s.base.cvalue[0];
			
			adder.u.s.base.value = 0;
			if(outcf.u.s.base.cvalue[0] != 0)
				adder.u.s.base.value = bignum;
			
			result = comp_add(comp_shru(result, one), adder);
			outcf.u.s.base.cvalue[0] = tmpcf.u.s.base.cvalue[0];
			--tmpcount.u.s.base.cvalue[0];
		}
	}
	
	static void do_rcl(Operand dest, Operand intmpcount, Operand incf, Operand &result, Operand &outcf)
	{
		outcf.u.s.base.cvalue[0] = incf.u.s.base.cvalue[0];
		result = dest;
		
		Operand tmpcf(1);
		Operand adder = get_literal(dest.low_size, 0);
		Operand tmpcount = intmpcount;
		Operand one = get_literal(dest.low_size, 1);
		
		while(tmpcount.u.s.base.cvalue[0] != 0)
		{
			tmpcf.u.s.base.cvalue[0] = comp_sign(result).u.s.base.cvalue[0];
			
			adder.u.s.base.value = 0;
			if(outcf.u.s.base.cvalue[0] != 0)
				adder.u.s.base.value = 1;
			
			result = comp_add(comp_shl(result, one), adder);
			outcf.u.s.base.cvalue[0] = tmpcf.u.s.base.cvalue[0];
			--tmpcount.u.s.base.cvalue[0];
		}
	}
	public:
	
	static Operand comp_rol(Operand a, Operand b)
	{
		Operand c = a;
		
		U1 tmpcount = b.u.s.base.cvalue[0];
		Operand tmpcf = get_literal(a.low_size, 0);
		Operand one = get_literal(1, 1);
		while(tmpcount != 0)
		{
			tmpcf.u.s.base.cvalue[0] = comp_sign(c).u.s.base.cvalue[0];
			c = comp_add(comp_shl(c, one), tmpcf);
			--tmpcount;
		}
		
		return c;
	}
	
	static Operand comp_ror(Operand a, Operand b)
	{
		Operand c = a;
		U1 tmpcount = b.u.s.base.cvalue[0];
		Operand one = get_literal(1, 1);
		U8 adder, aa;
		
		if(a.low_size == 1)
			adder = 0x80;
		else
		if(a.low_size == 2)
			adder = 0x8000;
		else
		if(a.low_size == 4)
			adder = 0x80000000;
		else
		if(a.low_size == 8)
			adder = 0x8000000000000000ull;
		else
		{
			assert(0);
			throw -1;
		}
		
		while(tmpcount != 0)
		{
			if((c.u.s.base.cvalue[0] & 1) != 0)
				aa = adder;
			else
				aa = 0;
			c = comp_shru(c, one);
			c.u.s.base.value += aa;
			
			--tmpcount;
		}
		
		return c;
	}
	
	static Operand comp_sdiv(Operand x, Operand y)
	{
		if(x.low_size == 1)
			return get_literal(x.low_size, (S1)(x.u.s.base.cvalue[0]) / (S1)(y.u.s.base.cvalue[0]));
		if(x.low_size == 2)
			return get_literal(x.low_size, (S2)(x.u.s.base.svalue[0]) / (S2)(y.u.s.base.svalue[0]));
		if(x.low_size == 4)
			return get_literal(x.low_size, (S4)(x.u.s.base.ivalue[0]) / (S4)(y.u.s.base.ivalue[0]));
		if(x.low_size == 8)
			return get_literal(x.low_size, (S8)(x.u.s.base.value) / (S8)(y.u.s.base.value));
		assert(0);
		throw -1;
		return Operand(0);
	}
	
	static Operand comp_smod(Operand x, Operand y)
	{
		if(x.low_size == 1)
			return get_literal(x.low_size, (S1)(x.u.s.base.cvalue[0]) % (S1)(y.u.s.base.cvalue[0]));
		if(x.low_size == 2)
			return get_literal(x.low_size, (S2)(x.u.s.base.svalue[0]) % (S2)(y.u.s.base.svalue[0]));
		if(x.low_size == 4)
			return get_literal(x.low_size, (S4)(x.u.s.base.ivalue[0]) % (S4)(y.u.s.base.ivalue[0]));
		if(x.low_size == 8)
			return get_literal(x.low_size, (S8)(x.u.s.base.value) % (S8)(y.u.s.base.value));
		assert(0);
		throw -1;
		return Operand(0);
	}
	
	static Operand comp_udiv(Operand x, Operand y)
	{
		if(x.low_size == 1)
			return get_literal(x.low_size, (U1)(x.u.s.base.cvalue[0]) / (U1)(y.u.s.base.cvalue[0]));
		if(x.low_size == 2)
			return get_literal(x.low_size, (U2)(x.u.s.base.svalue[0]) / (U2)(y.u.s.base.svalue[0]));
		if(x.low_size == 4)
			return get_literal(x.low_size, (U4)(x.u.s.base.ivalue[0]) / (U4)(y.u.s.base.ivalue[0]));
		if(x.low_size == 8)
			return get_literal(x.low_size, (U8)(x.u.s.base.value) / (U8)(y.u.s.base.value));
		assert(0);
		throw -1;
		return Operand(0);
	}
	
	static Operand comp_umod(Operand x, Operand y)
	{
		if(x.low_size == 1)
			return get_literal(x.low_size, (U1)(x.u.s.base.cvalue[0]) % (U1)(y.u.s.base.cvalue[0]));
		if(x.low_size == 2)
			return get_literal(x.low_size, (U2)(x.u.s.base.svalue[0]) % (U2)(y.u.s.base.svalue[0]));
		if(x.low_size == 4)
			return get_literal(x.low_size, (U4)(x.u.s.base.ivalue[0]) % (U4)(y.u.s.base.ivalue[0]));
		if(x.low_size == 8)
			return get_literal(x.low_size, (U8)(x.u.s.base.value) % (U8)(y.u.s.base.value));
		assert(0);
		throw -1;
		return Operand(0);
	}
	
	static Operand comp_shl(Operand x, Operand  y)
	{
		if(x.low_size == 1)
			return get_literal(x.low_size, x.u.s.base.cvalue[0] << y.u.s.base.cvalue[0]);
		if(x.low_size == 2)
			return get_literal(x.low_size, x.u.s.base.svalue[0] << (U2)(y.u.s.base.cvalue[0]));
		if(x.low_size == 4)
			return get_literal(x.low_size, x.u.s.base.ivalue[0] << (U4)(y.u.s.base.cvalue[0]));
		if(x.low_size == 8)
			return get_literal(x.low_size, x.u.s.base.value << (U8)(y.u.s.base.cvalue[0]));
		assert(0);
		throw -1;
		return Operand(0);
	}
	
	static Operand comp_shrs(Operand x, Operand y)
	{
		if(x.low_size == 1)
			return get_literal(x.low_size, (S1)(x.u.s.base.cvalue[0]) >> (S1)(y.u.s.base.cvalue[0]));
		if(x.low_size == 2)
			return get_literal(x.low_size, (S2)(x.u.s.base.svalue[0]) >> (S2)(y.u.s.base.cvalue[0]));
		if(x.low_size == 4)
			return get_literal(x.low_size, (S4)(x.u.s.base.ivalue[0]) >> (S4)(y.u.s.base.cvalue[0]));
		if(x.low_size == 8)
			return get_literal(x.low_size, (S8)(x.u.s.base.value) >> (S8)(y.u.s.base.cvalue[0]));
		assert(0);
		throw -1;
		return Operand(0);
	}
	
	static Operand comp_shru(Operand x, Operand y)
	{
		if(x.low_size == 1)
			return get_literal(x.low_size, x.u.s.base.cvalue[0] >> y.u.s.base.cvalue[0]);
		if(x.low_size == 2)
			return get_literal(x.low_size, x.u.s.base.svalue[0] >> (U2)(y.u.s.base.cvalue[0]));
		if(x.low_size == 4)
			return get_literal(x.low_size, x.u.s.base.ivalue[0] >> (U4)(y.u.s.base.cvalue[0]));
		if(x.low_size == 8)
			return get_literal(x.low_size, x.u.s.base.value >> (U8)(y.u.s.base.cvalue[0]));
		assert(0);
		throw -1;
		return Operand(0);
	}
	
	static Operand comp_smul_overflow(Operand a, Operand b)
	{
		Operand  result(1);
		if(a.low_size == 1)
		{
			S2 c = (S2)(S1)(a.u.s.base.cvalue[0]) * (S2)(S1)(b.u.s.base.cvalue[0]);
			return get_literal(1, (c != (S2)(S1)(c)) ? 1 : 0);
		}
		if(a.low_size == 2)
		{
			S4 c = (S4)(S2)(a.u.s.base.svalue[0]) * (S4)(S2)(b.u.s.base.svalue[0]);
			return get_literal(1, (c != (S4)(S2)(c)) ? 1 : 0);
		}
		if(a.low_size == 4)
		{
			S8 c = (S8)(S4)(a.u.s.base.ivalue[0]) * (S8)(S4)(b.u.s.base.ivalue[0]);
			return get_literal(1, (c != (S8)(S4)(c)) ? 1 : 0);
		}
		
		assert(0);//unimplemented for 64bit operands
		throw -1;
	}
	
	// FIXME: This won't work when src1, src2 are 64bit operands.
	static Operand comp_umul_overflow(Operand a, Operand b)
	{
		Operand  result(1);
		if(a.low_size == 1)
		{
			U2 c = (U2)(U1)(a.u.s.base.cvalue[0]) * (U2)(U1)(b.u.s.base.cvalue[0]);
			return get_literal(1, (c != (U2)(U1)(c)) ? 1 : 0);
		}
		if(a.low_size == 2)
		{
			U4 c = (U4)(U2)(a.u.s.base.svalue[0]) * (U4)(U2)(b.u.s.base.svalue[0]);
			return get_literal(1, (c != (U4)(U2)(c)) ? 1 : 0);
		}
		if(a.low_size == 4)
		{
			U8 c = (U8)(U4)(a.u.s.base.ivalue[0]) * (U8)(U4)(b.u.s.base.ivalue[0]);
			return get_literal(1, (c != (U8)(U4)(c)) ? 1 : 0);
		}
		
		assert(0);//unimplemented for 64bit operands
		throw -1;
	}
	
	static Operand comp_shl_cf(Operand tmpdest, Operand tmpcount)
	{
		Operand result(1);
		Operand count(1);
		count.u.s.base.cvalue[0] = tmpcount.u.s.base.cvalue[0];
		
		// Return value is not used if tmpcount == 0 on input. So,
		// we just return 0 in that case.
		if(tmpcount.u.s.base.cvalue[0] == 0)
		{
			result.u.s.base.cvalue[0] = 0;
			return result;
		}
		
		Operand dest = tmpdest;
		Operand one = get_literal(1, 1);
		while(count.u.s.base.cvalue[0] != 0)
		{
			result = comp_sign(dest);
			dest = comp_shl(dest, one);
			--count.u.s.base.cvalue[0];
		}
		return result;
	}
	
	static Operand comp_shr_cf(Operand tmpdest, Operand tmpcount)
	{
		Operand result(1);
		Operand count(1);
		count.u.s.base.cvalue[0] = tmpcount.u.s.base.cvalue[0];
		
		// Return value is not used if tmpcount == 0 on input. So,
		// we just return 0 in that case.
		if(tmpcount.u.s.base.cvalue[0] == 0)
		{
			result.u.s.base.cvalue[0] = 0;
			return result;
		}
		
		Operand dest = tmpdest;
		Operand one = get_literal(1, 1);
		while(count.u.s.base.cvalue[0] != 0)
		{
			result = comp_lsb(dest);
			dest = comp_shru(dest, one);
			--count.u.s.base.cvalue[0];
		}
		return result;
	}
	
	static Operand comp_sar_cf(Operand tmpdest, Operand tmpcount)
	{
		Operand result(1);
		Operand count(1);
		count.u.s.base.cvalue[0] = tmpcount.u.s.base.cvalue[0];
		
		// Return value is not used if tmpcount == 0 on input. So,
		// we just return 0 in that case.
		if(tmpcount.u.s.base.cvalue[0] == 0)
		{
			result.u.s.base.cvalue[0] = 0;
			return result;
		}
		
		Operand dest = tmpdest;
		Operand one = get_literal(1, 1);
		while(count.u.s.base.cvalue[0] != 0)
		{
			result = comp_lsb(dest);
			dest = comp_shrs(dest, one);
			--count.u.s.base.cvalue[0];
		}
		return result;
	}
	
	// not static because it needs access to rflags.
	Operand comp_xcond(Operand cc)
	{
		enum
		{
			o_of = 11,
			o_cf = 0,
			o_zf = 6,
			o_sf = 7,
			o_pf = 2
		};
		U1 result;
		switch(cc.u.s.base.cvalue[0] & 0xe)
		{
			case 0:
				result = (rflags >> o_of) & 1;
				break;
			case 2:
				result = (rflags >> o_cf) & 1;
				break;
			case 4:
				result = (rflags >> o_zf) & 1;
				break;
			case 6:
				result = ((rflags >> o_cf) | (rflags >> o_zf)) & 1;
				break;
			case 8:
				result = (rflags >> o_sf) & 1;
				break;
			case 0xa:
				result = (rflags >> o_pf) & 1;
				break;
			case 0xc:
				result = ((rflags >> o_sf) ^ (rflags >> o_of)) & 1;
				break;
			case 0xe:
				result = ((rflags >> o_zf) | ((rflags >> o_sf) ^ (rflags >> o_of))) & 1;
				break;
			default:
				assert(0);
				throw -1;
				break;
		}
		if((cc.u.s.base.cvalue[0] & 1) != 0)
			result ^= 1;
		return get_literal(1, result);
	}
	
	static Operand comp_lxor(Operand bit_x, Operand bit_y)
	{
		return get_literal(1, bit_x.u.s.base.cvalue[0] ^ bit_y.u.s.base.cvalue[0]);
	}
	
	static Operand comp_bitzx(Operand src)	// output is a byte
	{
		return src;		// input bit can be only 0 or 1
	}
	
	static Operand comp_lsb(Operand src)	// output is a bit
	{
		return get_literal(1, src.u.s.base.cvalue[0] & 1);
	}
	
	private:
	// This should be used to write when dest size is B1/B2/B4/B8 and we want the low part of src.
	static void stdwrite(Operand &dest, Operand &src)
	{
		for(int i = 0; i < dest.low_size; ++i)
			dest.u.s.base.cvalue[i] = src.u.s.base.cvalue[i];
	}
};
