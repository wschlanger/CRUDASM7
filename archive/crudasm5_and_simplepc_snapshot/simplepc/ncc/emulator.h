// emulator.h
// Copyright (C) 2009 Willow Schlanger

#ifndef l_emulator_h__included
#define l_emulator_h__included

#include "get_sizes.h"
#include "get_insn_enums.h"
#include "get_x86s.h"
#include <cassert>

template <class Executor, class Evaluator>
class GeneratedEmulator
{
	Executor *exec;
	Evaluator *eval;

	public:
	GeneratedEmulator(Executor *execT, Evaluator *evalT) :
		exec(execT),
		eval(evalT)
	{ }
	bool execute();
};

// Begin concrete emulator stuff.

struct Operand
{
	union
	{
		U8 big[2];
		struct
		{
			union
			{
				U8 value;
				U1 cvalue[8];
				U2 svalue[4];
				U4 ivalue[2];
				FLOAT fvalue;
				DOUBLE dvalue;
				LDOUBLE ldvalue;
			}	base;
			U2 aux;
		}	s;
	} u;
	U2 low_size : 5;
	U2 high_size : 5;
	U2 type : 2;		// 0 : high_size is 0
						// 1 : (offset, seg). aux holds seg.
						// 2,3 : (limit, base). aux holds limit.
						//       if 3, then low_size is 4 but
						//       only the low 3 bytes are used.
	U2 : 4;
	
	Operand(int size_bytes)
	{
		low_size = size_bytes;
		high_size = 0;
		type = 0;
	}
	
	void write(const Operand &src)
	{
		*this = src;	// is this good style ?
	}
	
	int get_simple_size()
	{
		if(type == 0)	// has no high size
			return low_size;
		return -1;
	}
};

struct OperandU :
	public Operand
{
	OperandU() :
		Operand(0)
	{
	}
	OperandU(const Operand &src) :
		Operand(src)
	{
	}
};

#include "simple_evaluator.h"

enum { rAX = 0, rCX, rDX, rBX, rSP, rBP, rSI, rDI };
enum { rES = 0, rCS, rSS, rDS, rFS, rGS };

struct GenReg
{
	union
	{
		U8 value;
		U4 ivalue[2];
		U2 svalue[4];
		U1 cvalue[8];
	} u;
};

struct SegReg
{
	U2 value;
};

class SimpleExecutorBase
{
	public:
	U8 rip;			// true current insn pointer
	U8 size;		// size of current instruction
	U8 osz_mask;	// 0xffffULL (o16), 0xffffffffULL (o32), or ~0ULL (o64)
	U8 rip_next;	// default is (rip + size) & osz_mask.
	U2 rint;		// default is 0xffff (no interrupt).
					// else, 0..0xff means, raise that interrupt
	U8 rflags;
	GenReg regs[16];
	SegReg sregs[6];
	U1 osz, asz;	// 0=16,1=32,2=64; for currently decoded instruction
	U1 ssz;			// stack address size. 0=16,1=32,2=64
	
	SimpleExecutorBase()
	{
		init();
	}
	
	void init()
	{
		rip = 0;
		size = 0;
		osz_mask = 0xffffULL;
		rip_next = 0;
		rint = 0xffff;
		do_write_rflags(0);
		for(int i = 0; i < 16; ++i)
			regs[i].u.value = 0;
		for(int i = 0; i < 6; ++i)
		{
			sregs[i].value = 0;
		}
		osz = 0;
		asz = 0;
		ssz = 0;
		// also initialize segment info (besides value) here when p-mode implemented.
	}
	
	private:
	void do_write_rflags(U8 value)
	{
		// fixme, surely some of these bits are hardwired, which ones??
		rflags = (value & 0xffffffffffff7ed7ULL) | 2;
	}
	public:
	
	void write_extern_RIP(Operand src)
	{
		rip_next = src.u.s.base.value;
		if(osz == 0)
			rip_next &= 0xffffULL;
		else
		if(osz == 1)
			rip_next &= 0xffffffffULL;
	}
	
	// can't write to RIP1.
	
	void write_extern_RIPC(Operand src)
	{
		rip_next = src.u.s.base.value;
		if(osz == 0)
			rip_next &= 0xffffULL;
		else
		if(osz == 1)
			rip_next &= 0xffffffffULL;
		rip_next = (rip + size + rip_next) & osz_mask;
		if(osz == 0)
			rip_next &= 0xffffULL;
		else
		if(osz == 1)
			rip_next &= 0xffffffffULL;
	}
	
	void write_extern_RINT(Operand src)
	{
		rint = (U2)(src.u.s.base.cvalue[0]);
	}
	
	// 'undefined' is a bit - that reads as zero.
	Operand read_extern_undefined() { return make_literal(1, 0); }
	
	//---
	Operand read_extern_RIP() { return make_literal(8, rip); }
	Operand read_extern_RIP1() { return make_literal(8, (rip + size) & osz_mask); }
	// can't read RIPC or RINT
	Operand read_extern_RFLAGS() { return make_literal(8, rflags); }

	Operand read_extern_OF() { return make_literal(1, (rflags >> 11) & 1); }
	Operand read_extern_SF() { return make_literal(1, (rflags >> 7) & 1); }
	Operand read_extern_ZF() { return make_literal(1, (rflags >> 6) & 1); }
	Operand read_extern_AF() { return make_literal(1, (rflags >> 4) & 1); }
	Operand read_extern_PF() { return make_literal(1, (rflags >> 2) & 1); }
	Operand read_extern_CF() { return make_literal(1, (rflags >> 0) & 1); }
	Operand read_extern_DF() { return make_literal(1, (rflags >> 10) & 1); }
	Operand read_extern_IF() { return make_literal(1, (rflags >> 9) & 1); }

	Operand read_extern_AL() { return make_literal(1, regs[0].u.cvalue[0]); }
	Operand read_extern_CL() { return make_literal(1, regs[1].u.cvalue[0]); }
	Operand read_extern_DL() { return make_literal(1, regs[2].u.cvalue[0]); }
	Operand read_extern_BL() { return make_literal(1, regs[3].u.cvalue[0]); }
	Operand read_extern_AH() { return make_literal(1, regs[0].u.cvalue[1]); }
	Operand read_extern_CH() { return make_literal(1, regs[1].u.cvalue[1]); }
	Operand read_extern_DH() { return make_literal(1, regs[2].u.cvalue[1]); }
	Operand read_extern_BH() { return make_literal(1, regs[3].u.cvalue[1]); }
	
	Operand read_extern_AX() { return make_literal(2, regs[0].u.svalue[0]); }
	Operand read_extern_CX() { return make_literal(2, regs[1].u.svalue[0]); }
	Operand read_extern_DX() { return make_literal(2, regs[2].u.svalue[0]); }
	Operand read_extern_BX() { return make_literal(2, regs[3].u.svalue[0]); }
	Operand read_extern_SP() { return make_literal(2, regs[4].u.svalue[0]); }
	Operand read_extern_BP() { return make_literal(2, regs[5].u.svalue[0]); }
	Operand read_extern_SI() { return make_literal(2, regs[6].u.svalue[0]); }
	Operand read_extern_DI() { return make_literal(2, regs[7].u.svalue[0]); }
	
	Operand read_extern_EAX() { return make_literal(4, regs[0].u.ivalue[0]); }
	Operand read_extern_ECX() { return make_literal(4, regs[1].u.ivalue[0]); }
	Operand read_extern_EDX() { return make_literal(4, regs[2].u.ivalue[0]); }
	Operand read_extern_EBX() { return make_literal(4, regs[3].u.ivalue[0]); }
	Operand read_extern_ESP() { return make_literal(4, regs[4].u.ivalue[0]); }
	Operand read_extern_EBP() { return make_literal(4, regs[5].u.ivalue[0]); }
	Operand read_extern_ESI() { return make_literal(4, regs[6].u.ivalue[0]); }
	Operand read_extern_EDI() { return make_literal(4, regs[7].u.ivalue[0]); }
	
	Operand read_extern_RAX() { return make_literal(8, regs[0].u.value); }
	Operand read_extern_RCX() { return make_literal(8, regs[1].u.value); }
	Operand read_extern_RDX() { return make_literal(8, regs[2].u.value); }
	Operand read_extern_RBX() { return make_literal(8, regs[3].u.value); }
	Operand read_extern_RSP() { return make_literal(8, regs[4].u.value); }
	Operand read_extern_RBP() { return make_literal(8, regs[5].u.value); }
	Operand read_extern_RSI() { return make_literal(8, regs[6].u.value); }
	Operand read_extern_RDI() { return make_literal(8, regs[7].u.value); }
	
	Operand read_extern_ES() { return make_literal(2, sregs[0].value); }
	Operand read_extern_CS() { return make_literal(2, sregs[1].value); }
	Operand read_extern_SS() { return make_literal(2, sregs[2].value); }
	Operand read_extern_DS() { return make_literal(2, sregs[3].value); }
	Operand read_extern_FS() { return make_literal(2, sregs[4].value); }
	Operand read_extern_GS() { return make_literal(2, sregs[5].value); }
	
	// can't read RBPX, RSPX (use RBP, RSP instead).
	//---
	
	void write_extern_AL(Operand src) { regs[0].u.cvalue[0] = src.u.s.base.cvalue[0]; }
	void write_extern_CL(Operand src) { regs[1].u.cvalue[0] = src.u.s.base.cvalue[0]; }
	void write_extern_DL(Operand src) { regs[2].u.cvalue[0] = src.u.s.base.cvalue[0]; }
	void write_extern_BL(Operand src) { regs[3].u.cvalue[0] = src.u.s.base.cvalue[0]; }
	void write_extern_AH(Operand src) { regs[0].u.cvalue[1] = src.u.s.base.cvalue[0]; }
	void write_extern_CH(Operand src) { regs[1].u.cvalue[1] = src.u.s.base.cvalue[0]; }
	void write_extern_DH(Operand src) { regs[2].u.cvalue[1] = src.u.s.base.cvalue[0]; }
	void write_extern_BH(Operand src) { regs[3].u.cvalue[1] = src.u.s.base.cvalue[0]; }
	
	void write_extern_AX(Operand src) { regs[0].u.svalue[0] = src.u.s.base.svalue[0]; }
	void write_extern_CX(Operand src) { regs[1].u.svalue[0] = src.u.s.base.svalue[0]; }
	void write_extern_DX(Operand src) { regs[2].u.svalue[0] = src.u.s.base.svalue[0]; }
	void write_extern_BX(Operand src) { regs[3].u.svalue[0] = src.u.s.base.svalue[0]; }
	void write_extern_SP(Operand src) { regs[4].u.svalue[0] = src.u.s.base.svalue[0]; }
	void write_extern_BP(Operand src) { regs[5].u.svalue[0] = src.u.s.base.svalue[0]; }
	void write_extern_SI(Operand src) { regs[6].u.svalue[0] = src.u.s.base.svalue[0]; }
	void write_extern_DI(Operand src) { regs[7].u.svalue[0] = src.u.s.base.svalue[0]; }

	void write_extern_EAX(Operand src) { regs[0].u.ivalue[0] = src.u.s.base.ivalue[0]; }
	void write_extern_ECX(Operand src) { regs[1].u.ivalue[0] = src.u.s.base.ivalue[0]; }
	void write_extern_EDX(Operand src) { regs[2].u.ivalue[0] = src.u.s.base.ivalue[0]; }
	void write_extern_EBX(Operand src) { regs[3].u.ivalue[0] = src.u.s.base.ivalue[0]; }
	void write_extern_ESP(Operand src) { regs[4].u.ivalue[0] = src.u.s.base.ivalue[0]; }
	void write_extern_EBP(Operand src) { regs[5].u.ivalue[0] = src.u.s.base.ivalue[0]; }
	void write_extern_ESI(Operand src) { regs[6].u.ivalue[0] = src.u.s.base.ivalue[0]; }
	void write_extern_EDI(Operand src) { regs[7].u.ivalue[0] = src.u.s.base.ivalue[0]; }
	
	void write_extern_OF(Operand src) { UINT bit = 11; rflags &= ~(1LL << bit); if(src.u.s.base.cvalue[0] != 0)  rflags |= (1LL << bit); }
	void write_extern_SF(Operand src) { UINT bit = 7; rflags &= ~(1LL << bit); if(src.u.s.base.cvalue[0] != 0)  rflags |= (1LL << bit); }
	void write_extern_ZF(Operand src) { UINT bit = 6; rflags &= ~(1LL << bit); if(src.u.s.base.cvalue[0] != 0)  rflags |= (1LL << bit); }
	void write_extern_AF(Operand src) { UINT bit = 4; rflags &= ~(1LL << bit); if(src.u.s.base.cvalue[0] != 0)  rflags |= (1LL << bit); }
	void write_extern_PF(Operand src) { UINT bit = 2; rflags &= ~(1LL << bit); if(src.u.s.base.cvalue[0] != 0)  rflags |= (1LL << bit); }
	void write_extern_CF(Operand src) { UINT bit = 0; rflags &= ~(1LL << bit); if(src.u.s.base.cvalue[0] != 0)  rflags |= (1LL << bit); }
	void write_extern_DF(Operand src) { UINT bit = 10; rflags &= ~(1LL << bit); if(src.u.s.base.cvalue[0] != 0)  rflags |= (1LL << bit); }
	void write_extern_IF(Operand src) { UINT bit = 9; rflags &= ~(1LL << bit); if(src.u.s.base.cvalue[0] != 0)  rflags |= (1LL << bit); }
	
	// Note: for POP seg_reg, we only read the low 2 bytes of the value from the stack.
	void write_extern_ES(Operand src) { write_seg_reg(0, src); }
	void write_extern_CS(Operand src) { write_seg_reg(1, src); }
	void write_extern_SS(Operand src) { write_seg_reg(2, src); }
	void write_extern_DS(Operand src) { write_seg_reg(3, src); }
	void write_extern_FS(Operand src) { write_seg_reg(4, src); }
	void write_extern_GS(Operand src) { write_seg_reg(5, src); }
	
	void write_extern_RBPX(Operand src)
	{
		if(ssz == 0)
			write_extern_BP(src);
		else
		if(ssz == 1)
			write_extern_EBP(src);
		else
			regs[rBP].u.value = src.u.s.base.value;
	}
	
	void write_extern_RSPX(Operand src)
	{
		if(ssz == 0)
			write_extern_SP(src);
		else
		if(ssz == 1)
			write_extern_ESP(src);
		else
			regs[rSP].u.value = src.u.s.base.value;
	}
	
	void write_extern_RFLAGS(Operand src)
	{
		// 'src' size is 8 bytes. only use low (16 << osz) bits.
		U8 keep;
		if(osz == 0)
			keep = 0xffffffffffff0000ULL;
		else
		if(osz == 1)
			keep = 0xffffffff00000000ULL;
		else
			keep = 0ULL;
		
		do_write_rflags((rflags & keep) | (src.u.s.base.value & ~keep));
	}
	
	private:
	void write_seg_reg(int sreg, Operand &src)
	{
		// Note: should do checks here if in p-mode, and throw an exception if fails.
		// catch() handler will then raise a CPU exception.
		// For now we emulate a real-mode processor only.
		sregs[sreg].value = src.u.s.base.svalue[0];
	}
	
	///protected:
	public:
	
	U8 get_seg_base(int sreg)
	{
		return (U8)(sregs[sreg].value) << 4;
	}
	
	U8 get_seg_limit(int sreg)
	{
		return 0xffff;	// 64kb segment for now
	}
	
	public:
	Operand make_literal(int size, U8 value) { Operand op(size); op.u.s.base.value = value; return op; }
};

//---

class LinearMemory
{
	public:
	U1 *ram;
	U8 size;
	
	LinearMemory(U1 *ramT, U8 sizeT) :
		ram(ramT),
		size(sizeT)
	{
	}
	
	// type codes for read_linear and write_linear:
	// 'u'=push from insn
	// 'o'=pop from insn
	// 'p'=pre (read) or post (write) exec, i.e. argumet memory access.
	// 'i'=int insn pseudodescriptor fetch (read only)
	// 'n'=initialization of memory (begins all 0's, except a0000..<100000=all ff's)
	//     writes to 0x7c00..0x7dfe inclusive are from sector 0 of the boot disk
	// 'b'=bios access
	
	// In the future, this might throw a page fault exception.
	// It is used by SimpleExecutor::read/write_virtual().
	
	virtual ~LinearMemory() { }

	virtual void read_linear(U8 linear, U1 *data, U4 dsize, char type)
	{
		read_physical(linear, data, dsize, type);
	}
	
	virtual void write_linear(U8 linear, U1 *data, U4 dsize, char type)
	{
		write_physical(linear, data, dsize, type);
	}
	
	virtual void read_physical(U8 linear, U1 *data, U4 dsize, char type)
	{
///std::cout << "[read from " << std::hex << linear << "]" << std::dec;
		if((linear + dsize - 1ull) > size)
		{
			for(UINT i = 0; i < dsize; ++i)
			{
				if((linear + i) < size)
					data[i] = ram[linear + i];
				else
					data[i] = 0xff;
			}
			return;
		}
		for(UINT i = 0; i < dsize; ++i)
			data[i] = ram[linear + i];
	}
	
	virtual void write_physical(U8 linear, U1 *data, U4 dsize, char type)
	{
///std::cout << "[write to " << std::hex << linear << "]" << std::dec;
		U8 a;
		for(UINT i = 0; i < dsize; ++i)
		{
			a = linear + i;
			if(a < size)
			{
				if(a >= 0xa0000 && a < 0x100000)
				{
					// ignore writes to adaptor space - unless type is
					// memory iNitialization sequence
					if(type == 'n')
						ram[a] = data[i];
				}
				else
					ram[a] = data[i];
			}
			else
				;	// ignore writes beyond physical memory
		}
	}
};

class IoHardware
{
	public:
	virtual ~IoHardware() { }
	virtual Operand do_inport(int size_bytes, Operand port16) = 0;
	virtual void do_outport(Operand port, Operand value) = 0;
};

#include <cassert>
#include <iostream>
class SimpleExecutor :
	public SimpleExecutorBase
{
	OperandU args[MAX_ARGS];
	LinearMemory *linmem;
	IoHardware *iohw;
	public:
	
	SimpleExecutor(LinearMemory *linmemT, IoHardware *iohwT) :
		linmem(linmemT),
		iohw(iohwT)
	{
	}
	
	int get_osz_bits() { return 16 << osz; }
	int get_asz_bits() { return 16 << asz; }
	int get_arg_size(int argnum) { assert(args[argnum].high_size == 0); return args[argnum].low_size; }
	void write_arg(int argnum, Operand src)
	{
		// actually since we're only WRITING to an argument, the argument need not have its size already
		// specified.
		/*
		if(args[argnum].low_size != src.low_size)
		{
			std::cerr << "Size mismatch - " << argnum << " - " << args[argnum].low_size << " vs. " << src.low_size << std::endl;
		}
		assert(args[argnum].low_size == src.low_size);
		*/
		args[argnum] = src;
	}
	Operand read_arg(int argnum) { return args[argnum]; }
	
	void do_assert(Operand src)
	{
		if(src.u.s.base.cvalue[0] == 0)
		{
			// an emulator assertion has failed - search notes.c for assert() calls to find cause.
			///assert(0);
			std::cerr << "Semantic ssertion failed!" << std::endl;
			throw -1;
		}
	}

	void do_reserve(Operand src)
	{
		assert(src.low_size == 8);
		xreserve(src.u.s.base.value);
	}
	
	void do_restore(Operand src)
	{
		assert(src.low_size == 8);
		xrestore(src.u.s.base.value);
	}

	void do_discard(Operand src)
	{
		assert(src.low_size == 8);
		xdiscard(src.u.s.base.value);
	}
	
	void do_commit(Operand src)
	{
		assert(src.low_size == 8);
		xcommit(src.u.s.base.value);
	}

	//--begin push/pop--
	
	// Note: if a segment register is PUSHed, it's zero-extended.
	void do_push(Operand src)
	{
		xreserve(2 << osz);	// make sure push will succeed
		xcommit(2 << osz);
		write_to_stack(src);
	}
	
	// Note: if a segment register is POPed, we want only the low 16 bits to be used.
	Operand do_pop()
	{
		xrestore(2 << osz);	// make sure pop will succeed
		Operand tmp = read_from_stack();
		xdiscard(2 << osz);
		return tmp;
	}
	
	//--end push/pop--
	
	private:
	void xrestore(U8 count)
	{
		// fixme--check for stack overflow here.
	}
	void xreserve(U8 count)
	{
		// fixme--check for stack overflow here.
	}
	void xcommit(U8 count)
	{
		xreserve(count);
		// fixme, make this work for ESP,RSP too.
		regs[rSP].u.svalue[0] -= (count);
	}
	void xdiscard(U8 count)
	{
		xrestore(count);
		// fixme, make this work for ESP,RSP too.
		regs[rSP].u.svalue[0] += (count);
	}
	
	// src operand must have a size of at least osz.
	void write_to_stack(Operand src)
	{
		U2 opsize = 2 << osz;
		
		// If a segment register is PUSHed in 32bit or 64bit mode,
		// we have to fill the operand with zero's.
		for(int i = src.low_size; i < opsize; ++i)
			src.u.s.base.cvalue[i] = 0;
		
		U8 offset;
		
		// fixme, make this work for ESP,RSP too.
		offset = regs[rSP].u.svalue[0];
		
		write_virtual(rSS, offset, src.u.s.base.cvalue, opsize, 'u' /* pUsh */);
	}
	
	// Returned operand has size of (2 << osz) bytes.
	Operand read_from_stack()
	{
		U2 opsize = 2 << osz;
		U8 offset;
		
		// fixme, make this work for ESP,RSP too.
		offset = regs[rSP].u.svalue[0];
		
		Operand dest(opsize);
		read_virtual(rSS, offset, dest.u.s.base.cvalue, opsize, 'o' /* pOp */);
		return dest;
	}
	
	// in the future, read_virtual() and write_virtual() might want to return an error code and should do
	// some privlige checks, etc. See LinearMemory for information on the 'type' argument.
	
	void read_virtual(UINT seg, U8 offset, U1 *data, U4 opsize, char type)
	{
		assert((offset + (U8)(opsize) - 1ULL) <= 0xffffULL);	// else exception
		offset += ((U8)(get_seg_base(seg)));
		///offset &= 0xffffULL;
		linmem->read_linear(offset, data, opsize, type);
	}
	
	void write_virtual(UINT seg, U8 offset, U1 *data, U4 opsize, char type)
	{
		assert((offset + (U8)(opsize) - 1ULL) <= 0xffffULL);	// else exception
		offset += ((U8)(get_seg_base(seg)));
		///offset &= 0xffffULL;
		linmem->write_linear(offset, data, opsize, type);
	}
	
	int insn_num;
	
	public:
	
	void set_insn(int insn_num_t) { insn_num = insn_num_t; }
	int get_insn() { return insn_num; }
	
	Operand do_inport(int size_bytes, Operand port16) { return iohw->do_inport(size_bytes, port16); }
	void do_outport(Operand port, Operand value) { iohw->do_outport(port, value); }
};

#endif	// l_emulator_h__included
