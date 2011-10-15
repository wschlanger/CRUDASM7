// simple_pc.cpp
// Copyright (C) 2009 Willow Schlanger

#include "simple_pc.h"

static int stop_state = 0;	// change to -1 to avoid halting on \> prompt

//---begin required template code---
template <class Executor, class Evaluator>
bool GeneratedEmulator<Executor, Evaluator>::execute()
{
#include "out_emulator.h"
}
//---end required template code---

#include "imetadata.h"
#include <iostream>

class SimpleMemory :
	public LinearMemory
{
	public:
	SimpleMemory(U1 *ramT, U8 ramsizeT) :
		LinearMemory(ramT, ramsizeT)
	{
	}
	virtual void read_physical(U8 linear, U1 *data, U2 dsize, char type)
	{
		LinearMemory::read_physical(linear, data, dsize, type);
	}
	virtual void write_physical(U8 linear, U1 *data, U2 dsize, char type)
	{
		LinearMemory::write_physical(linear, data, dsize, type);
	}
	virtual ~SimpleMemory()
	{
	}
	virtual void read_linear(U8 linear, U1 *data, U2 dsize, char type)
	{
		LinearMemory::read_linear(linear, data, dsize, type);
	}
	virtual void write_linear(U8 linear, U1 *data, U2 dsize, char type)
	{
		LinearMemory::write_linear(linear, data, dsize, type);
	}
};

class SimpleIoHardware :
	public IoHardware
{
	U1 port70value;
	public:
	SimpleIoHardware()
	{
		port70value = 0;
	}
	virtual ~SimpleIoHardware()
	{
	}
	virtual Operand do_inport(int size_bytes, Operand port16)
	{
		Operand tmp(size_bytes);
		tmp.u.s.base.value = 0xffffffffffffffffULL;
		if(port16.u.s.base.svalue[0] == 0x71)
		{
			if((port70value & 0x7f) == 0x0f)
				tmp.u.s.base.cvalue[0] = 0x00;
			else
			if((port70value & 0x7f) == 0x0b)
				tmp.u.s.base.cvalue[0] = 0x02;
		}
		return tmp;
	}
	virtual void do_outport(Operand port, Operand value)
	{
		if(port.u.s.base.svalue[0] == 0x0070)
			port70value = value.u.s.base.cvalue[0];
	}
};

SimplePC::SimplePC(U8 ram_size_t, U1 *diskT, U8 disk_sector_count_t)
{
	disk = diskT;
	disk_sector_count = disk_sector_count_t;
	ram_size = ram_size_t;
	ram = new U1 [ram_size];
	linmem = new SimpleMemory(ram, ram_size);
	for(UINT i = 0; i < ram_size; ++i)
	{
		ram[i] = 0;
	}
	for(UINT i = 0xa0000; i < 0x100000; ++i)
	{
		ram[i] = 0xff;
	}
	// begin bios memory initialization
	U1 vecs[0x400];
	for(UINT i = 0; i < 0x100; ++i)
	{
		((U2 *)(vecs + 4 * i))[0] = i;
		((U2 *)(vecs + 4 * i))[1] = 0xf000;
	}
	((U2 *)(vecs + 0x1e * 4))[0] = 0xefc7;
	((U2 *)(vecs + 0x1e * 4))[1] = 0xf000;
	linmem->write_linear(0, vecs, 0x400, 'n');

	U1 vec1e[11] = {0xaf, 0x02, 0x25, 0x02, 0x12, 0x1b, 0xff, 0x6c, 0xf6, 0x0f, 0x08};
	/*for(UINT i = 0; i < 11; ++i)
	{
		ram[0xfefc7 + i] = vec1e[i];
	}*/
	linmem->write_linear(0xfefc7, vec1e, 11, 'n');
	
	U1 himem[] = {'0', '7', '/', '0', '5', '/', '0', '7', '\0', 0xfc, 0x24};
	linmem->write_linear(0xffff5, himem, 11, 'n');

#if 0	
	ram[0xffff5] = '0';				// supply a valid BIOS date
	ram[0xffff6] = '7';
	ram[0xffff7] = '/';
	ram[0xffff8] = '0';
	ram[0xffff9] = '5';
	ram[0xffffa] = '/';
	ram[0xffffb] = '0';
	ram[0xffffc] = '7';
	ram[0xffffd] = 0x00;
	*(U2 *)(&ram[0xffffe]) = 0x24fc;	// model byte and what else?
#endif

	U1 raw[10] = {0x08, 0x00, 0xfc, 0x00, 0x01, 0x74, 0x40, 0x00, 0x00, 0x00};
	linmem->write_linear(0xfe6f5, raw, 10, 'n');
#if 0
	ram[0xfe6f5] = 0x08;			// what do these mean?
	ram[0xfe6f6] = 0x00;
	ram[0xfe6f7+0] = 0xfc;
	ram[0xfe6f7+1] = 0x00;
	ram[0xfe6f7+2] = 0x01;
	ram[0xfe6f7+3] = 0x74;
	ram[0xfe6f7+4] = 0x40;
	ram[0xfe6f7+5] = 0x00;
	ram[0xfe6f7+6] = 0x00;
	ram[0xfe6f7+7] = 0x00;
#endif

	U2 tmps;
	tmps = 640;							// 640kb conventional ram
	linmem->write_linear(0x413, (U1 *)(&tmps), 2, 'n');
	tmps = 0x4225;						// int 0x11 value
	linmem->write_linear(0x410, (U1 *)(&tmps), 2, 'n');

	////*(U2 *)(&ram[0x413]) = 640;		// 640kb conventional ram
	///*(U2 *)(&ram[0x410]) = 0x4225;	// int 0x11 value
	// end bios memory initialization
	
	iohw = new SimpleIoHardware();
	exec = new SimpleExecutor(linmem, iohw);
	eval = new SimpleEvaluator(exec->rflags);
	emulator = new GeneratedEmulator<SimpleExecutor, SimpleEvaluator>(exec, eval);
}

SimplePC::~SimplePC()
{
	delete emulator;
	delete eval;
	delete exec;
	delete iohw;
	delete linmem;
	delete [] ram;
}

void SimplePC::reset()
{
	exec->init();						// make all regs 0 but rflags should be 2
	exec->rip = 0x7c00;
	exec->rflags |= 0x200;				// set IF
	exec->regs[rSP].u.value = 0x4000;	// set SP
	exec->regs[rDX].u.value = 0;		// dl=0 (boot drive number)
	exec->osz = 0;
	exec->asz = 0;
	exec->ssz = 0;
	linmem->write_linear(0x7c00, disk, 512, 'n');
}

void SimplePC::pre_exec()
{
	decode.icode = &icode;
	decode.dsz = x86s::argsize_16;		// fixme, check for 32/64bit default code

	U8 offset = exec->rip + exec->get_seg_base(rCS);
	decode.insn = ram + offset;
	decode.end = decode.insn + 15;		// fixme, check for segment limit violation during fetch
	int x = x86s::decode(decode);
	if(x != 0 || decode.encoding == 0xffffff || decode.icode->insn >= x86s::insn__count)
	{
		std::cerr << "decode error" << std::endl;
		throw 1;
	}
	
	exec->size = decode.size;
	exec->osz_mask = 0xffffULL;			// fixme, cehck for 32/64bit code
	exec->rip_next = (exec->rip + exec->size) & exec->osz_mask;
	exec->set_insn(decode.icode->insn);
	exec->rint = 0xffff;				// added
	
	setup_args();
}

U8 SimplePC::get_mem_arg_offset(int i)
{
	using namespace x86s;
	// Fixme, this is not 64bit ready!
	U1 ath = get_argtype_hi(decode.icode->argtype[i]);
	assert(ath == argtypehi_mem_mem || ath == argtypehi_mem_rm);
	U8 offset = (U8)(U4)(decode.icode->disp);
	UINT base = decode.icode->ea.base;
	UINT index = decode.icode->ea.index;
	if(exec->asz == 0)
	{
		if(decode.icode->ea.disp8)
			offset = (U2)(S2)(S1)(offset);
		if(base != 31)
			offset += (U2)(exec->regs[base].u.svalue[0]);
		if(index != 31)
			offset += (((U2)(exec->regs[index].u.svalue[0])) << decode.icode->ea.index_scale);
		offset &= 0xffffUL;
	}
	else
	if(exec->asz == 1)
	{
		if(decode.icode->ea.disp8)
			offset = (U4)(S4)(S4)(offset);
		if(base != 31)
			offset += (U4)(exec->regs[base].u.ivalue[0]);
		if(index != 31)
			offset += (((U4)(exec->regs[index].u.ivalue[0])) << decode.icode->ea.index_scale);
		offset &= 0xffffffffUL;
	}
	else
	if(exec->asz == 2)
	{
		if(decode.icode->ea.disp8)
			offset = (U8)(S8)(S1)(offset);
		if(base != 31)
			offset += (U8)(exec->regs[base].u.value);
		if(index != 31)
			offset += (((U8)(exec->regs[index].u.svalue)) << decode.icode->ea.index_scale);
	}
	else
	{
		assert(0);
	}
///std::cerr << "<" << std::hex << offset << std::dec << ">";
	return offset;
}

void SimplePC::setup_args()
{
	bool immyet = false;
	const char *arginfo = get_arg_info(exec->get_insn(), 16 << exec->osz, 16 << exec->asz);
	
	for(int i = 0; i < MAX_ARGS; ++i)
	{
		U1 atl = get_argtype_lo(decode.icode->argtype[i]);
		if(atl == argtype_void)
		{
			break;
		}

		///assert(arginfo[i] != 4);	// make sure our arg info is good
		if(arginfo[i] == 4)
		{
			///testing code. implict arguments appear at the end for e.g. ret, etc.
			///best to do a semantics fix rather than rely on this hack.
			break;
			///std::cerr << "Internal error - arginfo is bad." << std::endl;
			///throw -1;
		}
						
		if(atl == argtype_imm)
		{
			// immediate arguments are always input.
			U1 ath = get_argtype_hi(decode.icode->argtype[i]);
			if(ath == argtypehi_imm_implict8 || ath == argtypehi_imm_cc)
			{
				exec->write_arg(i, exec->make_literal(1, decode.icode->argvalue[i]));
			}
			else
			{
				int size;
				U1 true_size = specialize_arg_size(decode.icode->argsize[i], decode.dsz, exec->osz, exec->asz);
				int szlo = get_argsize_lo(true_size);
				// immediates will not have a "high" part.
				if(szlo == argsize_8)
					size = 1;
				else
				if(szlo <= argsize_64)
					size = 2 << szlo;
				else
				{
					assert(0);
					throw -1;
				}
				Operand operand(size);
///std::cerr << "[literalSize=" << size << "]";
				U4 *src = (!immyet) ? &decode.icode->imm : &decode.icode->disp;
				immyet = true;
				if(size < 8)
					operand.u.s.base.value = (U8)(U4)(*src);
				else
					operand.u.s.base.value = (U8)(U4)(decode.icode->imm) | (((U8)(U4)(decode.icode->disp)) << 32);
///std::cout << "[" << std::hex << operand.u.s.base.value << "]";
				exec->write_arg(i, operand);
			}
		}
		else
		if(atl == argtype_reg)
		{
			int size;
		
			U1 true_size = specialize_arg_size(decode.icode->argsize[i], decode.dsz, exec->osz, exec->asz);
			int szlo = get_argsize_lo(true_size);
			// regs will not have a "high" part.
			if(szlo == argsize_8)
				size = 1;
			else
			if(szlo <= argsize_64)
				size = 2 << szlo;
			else
			{
				assert(0);
				throw -1;
			}
			if((arginfo[i] & 1) != 0)
			{
				// Reads a register argument.
				U1 ath = get_argtype_hi(decode.icode->argtype[i]);
				UINT av = (UINT)(U1)(decode.icode->argvalue[i]);
				if(ath == argtypehi_reg_r || ath == argtypehi_reg_basecode)
				{
					Operand operand(size);
///std::cerr << "{" << size << "}";
					// FIXME--not 64bit ready!
					if(get_argsize_lo(true_size) == argsize_8)
					{
						operand.u.s.base.cvalue[0] = exec->regs[av & 3].u.cvalue[(av >> 2) & 1];
					}
					else
					{
						for(SINT j = 0; j < size; ++j)
							operand.u.s.base.cvalue[j] = exec->regs[av].u.cvalue[j];
					}
					exec->write_arg(i, operand);
				}
				else
				if(ath == argtypehi_reg_sr)
				{
					assert(av < 6);
					Operand operand(2);
					operand.u.s.base.svalue[0] = exec->sregs[av].value;
					exec->write_arg(i, operand);	// oops, left this out!
///std::cerr << std::hex << "[sreg " << av << " = " << operand.u.s.base.svalue[0] << std::dec << "]";
				}
				else
					assert(0);
			}
		}
		else
		if(atl == argtype_mem)
		{
			///U1 atl = get_argtype_lo(decode.icode->argtype[i]);
			U1 ath = get_argtype_hi(decode.icode->argtype[i]);
			///assert(atl == argtype_mem);
			if(decode.icode->insn == insn_lea)
			{
				// EA itself is used.
				// Remember to test LEA BX,[ECX]. zx<> and sx<> should truncate if
				// the destination size is smaller than the source size.
				U8 offset = get_mem_arg_offset(i);
				Operand operand(2 << exec->asz);
				operand.u.s.base.value = offset;
				exec->write_arg(i, operand);
			}
			else
			if((arginfo[i] & 3) != 0)
			{
				// this memory operand is either read, or written.
				int sreg;
				if(ath == argtypehi_mem_strd)
					sreg = rES;
				else
					sreg = decode.icode->ea.sreg;
				if(sreg == 6)
					sreg = 2;
				if(sreg == 7)
					sreg = 3;
///std::cerr << "(sreg=" << sreg << ")";
				// sreg is now valid.
				U8 offset;
				U8 *xoffset = &mem_offset;
				if(ath == argtypehi_mem_mem || ath == argtypehi_mem_rm)
					offset = get_mem_arg_offset(i);
				else
				if(ath == argtypehi_mem_disp)
				{
					if(exec->asz < 2)
						offset = decode.icode->disp;
					else
						offset = ((U8)(decode.icode->disp)) | (((U8)(decode.icode->imm) << 32));
				}
				else
				if(ath == argtypehi_mem_strs)
				{
					xoffset = &strs_offset;
					if(exec->asz == 0)
						offset = exec->regs[rSI].u.svalue[0];
					else
					if(exec->asz == 1)
						offset = exec->regs[rSI].u.ivalue[0];
					else
						offset = exec->regs[rSI].u.value;
				}
				else
				if(ath == argtypehi_mem_strd)
				{
					xoffset = &strd_offset;
					if(exec->asz == 0)
						offset = exec->regs[rDI].u.svalue[0];
					else
					if(exec->asz == 1)
						offset = exec->regs[rDI].u.ivalue[0];
					else
						offset = exec->regs[rDI].u.value;
				}
				else
				if(ath == argtypehi_mem_xs)
				{
					if(exec->asz == 0)
						offset = (U2)(exec->regs[rBX].u.svalue[0] + (U2)(U1)(exec->regs[rAX].u.cvalue[0]));
					else
					if(exec->asz == 1)
						offset = (U4)(exec->regs[rBX].u.svalue[0] + (U4)(U1)(exec->regs[rAX].u.cvalue[0]));
					else
						offset = (U8)(exec->regs[rBX].u.svalue[0] + (U8)(U1)(exec->regs[rAX].u.cvalue[0]));
				}
				else
				{
					assert(0);
					throw -1;
				}

				// FIXME--verify the memory can be read/written here, before we try to execute the
				// instruction. (seg, offset) is valid here.
				
				// Add segment base to offset.
				offset += exec->get_seg_base(sreg);
///std::cerr << "[" << i << "-" << sreg << "-" << std::hex << offset << "-" << exec->get_seg_base(sreg) << "]";
				// Fixme--do limit checking here.
								
				*xoffset = offset;		// save linear offset for later use
				
				if((arginfo[i] & 1) != 0)
				{
					// Reads a memory argument.
					U1 true_size = specialize_arg_size(decode.icode->argsize[i], decode.dsz, exec->osz, exec->asz);
					U1 size_lo = get_argsize_lo(true_size);
					U1 size_hi = get_argsize_hi(true_size);
					
					Operand operand(0);
					// Low part of structure may have a size of 1, 2, 4, or 8 bytes only.
					if(size_lo == argsize_8)
					{
						operand.low_size = 1;
						linmem->read_linear(offset, operand.u.s.base.cvalue, operand.low_size, 'p');
					}
					else
					if(size_lo <= argsize_64)
					{
						operand.low_size = 2 << size_lo;
						linmem->read_linear(offset, operand.u.s.base.cvalue, operand.low_size, 'p');
					}
					else
					{
						// note: 24 bits should not happen for low part.
						assert(0);
					}
					
					// We have two structure cases: (offset,seg) and (limit,base).
					
					if(size_hi != argsize__end)
					{
						if(size_hi == argsize_16)
						{
							operand.high_size = 2;
							linmem->read_linear(offset + operand.low_size, (U1 *)(&operand.u.s.aux), operand.high_size, 'p');
							operand.type = 1;
///std::cerr << std::hex << "[" << operand.u.s.aux << ":" << operand.u.s.base.svalue[0] << "]" << std::dec;
						}
						else
						{
							assert(size_hi == argsize_24 || size_hi == argsize_32 || size_hi == argsize_64);
							assert(size_lo == argsize_16);
							
							operand.u.s.aux = operand.u.s.base.svalue[0];
							operand.high_size = 2;
							linmem->read_linear(offset + operand.low_size, operand.u.s.base.cvalue, 2, 'p');
							
							operand.type = (size_hi == argsize_24) ? 3 : 2;
							if(operand.type == 3)
								operand.u.s.base.value &= 0xffffffUL;	// only read the low 24 bits
						}
					}
					
					exec->write_arg(i, operand);
				}
			}
		}
		else
		{
			std::cerr << "Internal error - don't know what kind of argument instruction has!" << std::endl;
			throw -1;
		}
	}
}

static bool is_rep(UINT insn)
{
	switch(insn)
	{
		case insn__movs:
		case insn__stos:
		case insn__lods:
		case insn__ins:
		case insn__outs:
			return true;
		default:
			break;
	}
	return false;
}

static bool is_repcond(UINT insn)
{
	switch(insn)
	{
		case insn__cmps:
		case insn__scas:
			return true;
		default:
			break;
	}
	return false;
}

bool SimplePC::execute()
{
	if((decode.icode->lockrep & 2) == 2)
	{
///std::cerr << "***REP'd***" << std::endl;
		bool rcx_zero;
		//begin fixme--make this use ECX/RCX instead here!
		rcx_zero = (exec->regs[rCX].u.svalue[0] == 0);
		//end fixme
		if(rcx_zero)
		{
			if(is_rep(decode.icode->insn) || is_repcond(decode.icode->insn))
			{
				// fixme, make this work with 32 and 64 bit code!
				exec->rip = (exec->rip + exec->size) & 0xffffULL;	// next instruction
				return false;										// done!
			}
		}
	}
	
	// begin bugfix
	exec->osz = decode.icode->osz;
	exec->asz = decode.icode->asz;
	// end bugfix
	
	emulator->execute();
	
	if((decode.icode->lockrep & 2) == 2)
	{
		if(is_rep(decode.icode->insn) || is_repcond(decode.icode->insn))
		{
			// fixme, make this work for 32/64bit code!
			--exec->regs[rCX].u.svalue[0];
			if(exec->regs[rCX].u.svalue[0] != 0)
			{
				if(is_rep(decode.icode->insn))
					return true;		// run us again
				// it's a repcond insn.
				if(decode.icode->lockrep == 2)
				{
					// repnz
					if((exec->rflags & (1 << 6)) == 0)
						return true;
				}
				else
				{
					// repz
					if((exec->rflags & (1 << 6)) != 0)
						return true;
				}
			}
		}
	}
	
	// go to next instruction.
	exec->rip = exec->rip_next;
	
	return false;
}

// Go thru arguments.
void SimplePC::post_exec()
{
	const char *arginfo = get_arg_info(exec->get_insn(), 16 << exec->osz, 16 << exec->asz);
	
	for(int i = 0; i < MAX_ARGS; ++i)
	{
		U1 atl = get_argtype_lo(decode.icode->argtype[i]);
		if(atl == argtype_void)
		{
			break;
		}
		if((arginfo[i] & 2) == 0)
		{
			// this argument is not an output.
			continue;
		}
		
		Operand src = exec->read_arg(i);
		
		if(atl == argtype_reg)
		{
			UINT av = decode.icode->argvalue[i];
			
			U1 ath = get_argtype_hi(decode.icode->argtype[i]);
			if(ath == argtypehi_reg_r || ath == argtypehi_reg_basecode)
			{
				U1 true_size = specialize_arg_size(decode.icode->argsize[i], decode.dsz, exec->osz, exec->asz);
///std::cerr << "(" << std::hex << (int)true_size << ")";
				int szlo = get_argsize_lo(true_size);
///std::cerr << "(" << (int)szlo << std::dec << ")";
				if(szlo == argsize_8)
					exec->regs[av & 3].u.cvalue[(av >> 2) & 1] = src.u.s.base.cvalue[0];
				else
				{
					assert(szlo <= argsize_64);
					int sizebytes = (2 << szlo);
///std::cerr << "[" << sizebytes << "]";
					for(int i = 0; i < sizebytes; ++i)
						exec->regs[av].u.cvalue[i] = src.u.s.base.cvalue[i];
				}
			}
			else
			if(ath == argtypehi_reg_sr)
			{
				exec->sregs[av].value = src.u.s.base.svalue[0];
			}
			else
			{
				assert(0);
				throw -1;
			}
		}
		else
		if(atl == argtype_mem)
		{
			// Write back memory operands here!
			U1 ath = get_argtype_hi(decode.icode->argtype[i]);
			U8 *offset = &mem_offset;
			if(ath == argtypehi_mem_strs)
				offset = &strs_offset;
			else
			if(ath == argtypehi_mem_strd)
				offset = &strd_offset;
			
			if(src.type >= 2)
			{
				linmem->write_linear(*offset, (U1 *)(&src.u.s.aux), 2, 'p');
				U8 base = src.u.s.base.value;
				if(src.type == 3)
					base &= 0xffffffUL;		// only write low 24 bits
				linmem->write_linear(*offset + 2, (U1 *)(&base), src.high_size, 'p');
			}
			else
			{
				linmem->write_linear(*offset, src.u.s.base.cvalue, src.low_size, 'p');
				if(src.type == 1)
					linmem->write_linear(*offset + (U8)(src.low_size), (U1 *)(&src.u.s.aux), 2, 'p');
			}
		}
	}
	
	if(exec->rint != 0xffff)
	{
		// fixme, simulate an interrupt here!
		assert(exec->rint < 0x100);
		U2 ofs, seg;
		linmem->read_linear(exec->rint * 4, (U1 *)(&ofs), 2, 'i');
		linmem->read_linear(exec->rint * 4 + 2, (U1 *)(&seg), 2, 'i');
		
		exec->do_push(exec->make_literal(2, exec->rflags));				// push flags
		exec->do_push(exec->make_literal(2, exec->sregs[rCS].value));	// push cs
		exec->do_push(exec->make_literal(2, (exec->rip /*+ exec->size*/) & exec->osz_mask));	// push ip
		
		exec->rflags &= ~0x200ULL;	// clear IF
		
		exec->rip = ofs;
		exec->sregs[rCS].value = seg;
		
		exec->rint = 0xffff;
	}
}

static void invalid_intr(U1 vector, SimpleExecutor *exec)
{
	std::cerr << "Unsupported ROMBIOS interrupt - 0x" << std::hex << (UINT)(vector) << std::dec << std::endl;
	std::cerr << "RAX=" << std::hex << exec->regs[rAX].u.value << std::dec << std::endl;
	throw -1;
}

// ** Note, make this use linmem->read_linear() and write_linear() instead of directly accessing ram.
// ** Use a type code of 'b' for bios.
// ** Now if an interrupt is handled, we want to compare sregs and gregs and dump the new/old values
// ** into any logs we may be making. Also make note of BIOS reads/writes/old value for memory writes.
// ** Note that in real mode, a seg reg base always is 16 times its value.
bool SimplePC::handle_intr()
{
	if(exec->sregs[rCS].value == 0xf000 && exec->rip < 0x100)
	{
		// BIOS simulator.
		U1 vector = exec->rip;
		U8 fland = ~0ULL;
		U8 flor = 0ULL;
		
		switch(vector)
		{
			case 0x13:
			{
				switch(exec->regs[rAX].u.cvalue[1])
				{
					case 0x00:
						exec->regs[0].u.cvalue[1] = 0;	// AH=00 (success)
						fland = ~1ULL;
						break;
					case 0x08:
///std::cout << "get-disk-params(dl=" << std::hex << (int)(exec->regs[2].u.cvalue[0]) << std::dec << ")" << std::endl;
						// get drive parameters
						if(exec->regs[2].u.cvalue[0] != 0)
						{
							// we leave BH alone; BOCHS clears it to 0.
							if(exec->regs[2].u.cvalue[0] >= 0x80)
							{
								exec->regs[0].u.svalue[0] = 0;
								exec->regs[1].u.svalue[0] = 0;
								exec->regs[2].u.svalue[0] = 0;
								exec->regs[3].u.cvalue[0] = 0;
///flor = 1;
							}
							else
							{
								exec->regs[7].u.svalue[0] = 0xefc7;
								exec->sregs[0].value = 0xf000;
								exec->regs[0].u.svalue[0] = 0;
								exec->regs[1].u.svalue[0] = 0;
								exec->regs[2].u.svalue[0] = 1;	// 1 drive in system
								exec->regs[3].u.cvalue[0] = 0;
							}
							fland = ~1ULL;
						}
						else
						{
							fland = ~1ULL;
							exec->regs[7].u.svalue[0] = 0xefc7;
							exec->sregs[0].value = 0xf000;
							exec->regs[0].u.svalue[0] = 0;
							exec->regs[3].u.cvalue[0] = 4;
							exec->regs[1].u.svalue[0] = 0x4f12;
							exec->regs[2].u.cvalue[0] = 0x0101;
						}
						break;
					case 0x15:
						// get disk type.
						fland = ~1ULL;	// success
						if(exec->regs[2].u.cvalue[0] == 0)
							exec->regs[0].u.cvalue[1] = 1+1;	// floppy with changeline support
						else
							exec->regs[0].u.cvalue[1] = 0;		// no such drive
						break;
					case 0x16:	// report disk not changed
						fland = ~1ULL;
						exec->regs[0].u.cvalue[1] = 0;
						break;
					case 0x02:
						if(read_disk())
						{
							exec->regs[0].u.cvalue[1] = 0;	// success
							fland = ~1ULL;
						}
						else
						{
///std::cout << "read failed - drive=(" << std::hex << (int)(exec->regs[2].u.cvalue[0]) << std::dec << ")" << std::endl;
							exec->regs[0].u.cvalue[1] = 1;	// invalid ???
							exec->regs[0].u.cvalue[0] = 0;	// no sectors transferred
							flor = 1ULL;					// simulate disk error
						}
						break;
					default:
						invalid_intr(vector, exec);
						break;
				}
				break;
			}
			case 0x12:
			{
				U2 x;
				linmem->read_linear(0x413, (U1 *)(&x), 2, 'b');
				exec->regs[0].u.svalue[0] = x;
				//exec->regs[0].u.svalue[0] = *((U2 *)(ram + 0x413));
				break;
			}
			case 0x11:
			{
				U2 x;
				linmem->read_linear(0x410, (U1 *)(&x), 2, 'b');
				exec->regs[0].u.svalue[0] = x;//*((U2 *)(ram + 0x410));
				break;
			}
			case 0x10:
			{
				switch(exec->regs[rAX].u.cvalue[1])
				{
					case 0x02:	// set cursor position?
						std::cerr << (char)(8);
						break;
					case 0x06:	// scroll window up, used to clear screen
						std::cerr << std::endl << "---" << std::endl << std::endl;
						break;
					case 0x01:	// set text mode cursor shape
						break;
					case 0x0f:
						exec->regs[0].u.cvalue[1] = 80;
						exec->regs[0].u.cvalue[0] = 3;
						exec->regs[3].u.cvalue[1] = 0;
						break;
					case 0x0e:
						std::cerr << (char)(exec->regs[0].u.cvalue[0]);
						if(stop_state == 0 && exec->regs[0].u.cvalue[0] == '\\')
							stop_state = 1;
						else
						if(stop_state == 1 && exec->regs[0].u.cvalue[0] == '>')
						{
							std::cerr << std::endl;
							std::cerr << std::endl;
							std::cerr << "---prompt detected---" << std::endl;
							throw -1;
						}
						else
						if(stop_state != -1)
							stop_state = 0;
						break;
					default:
						invalid_intr(vector, exec);
						break;
				}
				break;
			}
			case 0x16:
			{
				switch(exec->regs[rAX].u.cvalue[1])
				{
					case 0x00:
						std::cerr << "--- user input requested ---" << std::endl;
						throw -1;
						break;
					case 0x01:
///std::cout << "[KeyCheck]";
					{
						U4 x;
						linmem->read_linear(0x46c, (U1 *)(&x), 4, 'b');
						x += 1000;
						linmem->write_linear(0x46c, (U1 *)(&x), 4, 'b');
						///*(U4 *)(ram + 0x46c) += 1000;
						flor = (1ULL << 6);	// no keystroke available (set ZF)
						break;
					}
					case 0x02:	// ???
					{
						///*(U4 *)(ram + 0x46c) += 1000;
						U4 x;
						linmem->read_linear(0x46c, (U1 *)(&x), 4, 'b');
						x += 1000;
						linmem->write_linear(0x46c, (U1 *)(&x), 4, 'b');
						exec->regs[0].u.cvalue[0] = 0x20;	// dunno about this one
						break;
					}
					default:
						invalid_intr(vector, exec);
						break;
				}
				break;
			}
			case 0x15:
			{
				switch(exec->regs[rAX].u.cvalue[1])
				{
					case 0xc0:	// get config
						fland = ~1ULL;
						exec->regs[0].u.cvalue[1] = 0;	// success
						exec->regs[3].u.svalue[0] = 0xe6f5;
						exec->sregs[0].value = 0xf000;
						break;
					case 0x41:
						exec->regs[0].u.cvalue[1] = 0x86;	// ala bochs (???)
						flor = 1ULL;	// we don't support 0x41 (wait on external event)
						break;
					case 0x88:	// get extended memory size
						exec->regs[0].u.svalue[0] = (ram_size - 0x100000) / 1024;
						fland = ~1ULL;
						break;
					case 0x87:	// copy extended memory
						// note--we fake this since we always do a cold boot and there is
						// no need to erase a VIDSK header in this case.
						fland = ~1ULL;
						exec->regs[0].u.cvalue[1] = 0;
						break;
					default:
						invalid_intr(vector, exec);
						break;
				}
				break;
			}
			case 0x14:
			{
				switch(exec->regs[rAX].u.cvalue[1])
				{
					case 0x00:
						// initialize serial port
						break;
					default:
						invalid_intr(vector, exec);
						break;
				}
				break;
			}
			case 0x17:
			{
				switch(exec->regs[rAX].u.cvalue[1])
				{
					case 0x01:
						// initialize printer port
						break;
					default:
						invalid_intr(vector, exec);
						break;
				}
				break;
			}
			case 0x1a:
			{
				switch(exec->regs[rAX].u.cvalue[1])
				{
					case 0x00:
					{
						// get system time
						U2 x, y;
						linmem->read_linear(0x46c + 2, (U1 *)(&x), 2, 'b');
						linmem->read_linear(0x46c, (U1 *)(&y), 2, 'b');
						exec->regs[1].u.svalue[0] = x;///*(U2 *)(ram + 0x46c + 2);
						exec->regs[2].u.svalue[0] = y;///*(U2 *)(ram + 0x46c + 0);
						exec->regs[0].u.svalue[0] = 0;		// no midnight passed
						fland = ~1ULL;			// clear CF
						break;
					}
					case 0x02:
						// get rtc time
						exec->regs[1].u.svalue[0] = 0x0631;
						exec->regs[2].u.svalue[0] = 0x2100;
						fland = ~1ULL;			// clear CF
						break;
					case 0x04:
						// get rtc date
						exec->regs[1].u.svalue[0] = 0x1995;
						exec->regs[2].u.svalue[0] = 0x0101;
						fland = ~1ULL;			// clear CF
						break;
					case 0x01:	// for FreeDOS (???)
					{
						U2 x;
						x = exec->regs[1].u.svalue[0];
						linmem->write_linear(0x46c + 2, (U1 *)(&x), 2, 'b');
						///*(U2 *)(ram + 0x46c + 2) = exec->regs[1].u.svalue[0];
						x = exec->regs[2].u.svalue[0];
						linmem->write_linear(0x46c + 0, (U1 *)(&x), 2, 'b');
						///*(U2 *)(ram + 0x46c + 0) = exec->regs[2].u.svalue[0];
						break;
					}
					case 0x03:	// set RTC time (FreeDOS)
					case 0x05:	// set RTC date (FreeDOS)
						break;
					default:
						invalid_intr(vector, exec);
						break;
				}
				break;
			}
			default:
			{
				invalid_intr(vector, exec);
				break;
			}
		}
		
		//FIXME--this ugly hack is good for real mode only.
		///exec->set_insn(insn__iret);
		///emulator->execute();
		///exec->rip = exec->rip_next;
		
		Operand tmp(0);
		tmp = exec->do_pop();
		exec->rip = tmp.u.s.base.svalue[0];
		tmp = exec->do_pop();
		exec->sregs[rCS].value = tmp.u.s.base.svalue[0];
		tmp = exec->do_pop();
		exec->rflags = tmp.u.s.base.svalue[0];
		
		// *** check iret
		
		exec->rflags &= fland;
		exec->rflags |= flor;
		
		return true;
	}
	return false;
}

bool SimplePC::read_disk()
{
	U1 al = exec->regs[0].u.cvalue[0];
	assert(al > 0 && al < 0x7f);
	U1 ch = exec->regs[1].u.cvalue[1];
	U1 cl = exec->regs[1].u.cvalue[0];
	U1 dh = exec->regs[2].u.cvalue[1];
	U1 dl = exec->regs[2].u.cvalue[0];
	if(dl != 0)
		return false;

	U4 sector = (cl & 63) - 1;
	U4 head = dh;
	U4 cyl = (U4)(ch) + (((U4)(cl & 0xc0)) << 2);
	U4 lba = 0x12 * 2 * cyl + 0x12 * head + sector;
	assert(((lba + (U4)(al)) - 1) < disk_sector_count);
	
	U1 *src = disk + lba * 0x200;
	U8 es_base = ((U8)(U2)(exec->sregs[0].value)) << 4;
	U1 *dest = ram + (es_base + (U8)(exec->regs[3].u.svalue[0]));
	
	/*for(U4 i = 0; i < al; ++i)
	{
		for(U4 j = 0; j < 512; ++j)
		{
			*(dest++) = *(src++);
		}
	}*/
	linmem->write_linear(dest - ram, src, (U4)(al) * 512, 'b');

///std::cout << "read lba=" << lba << ", count=" << (UINT)(al) << " to " << std::hex << exec->sregs[0].value << ":" << exec->regs[3].u.svalue[0] << std::dec << std::endl;
	return true;
}

//----------------------------------------------------------------------------------------------------------------------

// test program
#include <stdio.h>
#include "../crudasm/asmwriter.h"

int main()
{
	U8 disk_sector_count = 2880;
	U1 *disk = new U1 [disk_sector_count * 512];
	FILE *f = fopen("boot.img", "rb");
	if(f == NULL)
	{
		delete [] disk;
		std::cerr << "Error: can't open boot.img!" << std::endl;
		return 1;
	}
	if(fread(disk, disk_sector_count * 0x200, 1, f) != 1)
	{
		fclose(f);
		delete [] disk;
		std::cerr << "Error: can't read boot.img!" << std::endl;
		return 1;
	}
	fclose(f);
	SimplePC pc(0x100000*2 /*2mb ram*/, disk, disk_sector_count);

	pc.reset();
	NasmWriter disasm;
	char dest[1024];
	U8 tmpofs;
	U2 tmpseg;
	
U8 old_rip = ~0ULL;
U8 sequence = 0;

	try
	{
		bool again;
		for(;;)
		{
			++sequence;
if(old_rip == pc.exec->rip)
{
	std::cout << "infinite loop detected" << std::endl;
	throw -1;
}
old_rip = pc.exec->rip;

			if(pc.handle_intr())
			{
				// handled a BIOS invokation.
			}
tmpseg = pc.exec->sregs[rCS].value;
tmpofs = pc.exec->rip;
			do
			{
				pc.pre_exec();
				again = pc.execute();
				pc.post_exec();
			}	while(again);
		}
	}
	catch(int)
	{
		std::cerr << "STOP condition detected - emulation halted!" << std::endl;
	}
	
	std::cerr << "Executed " << sequence << " instructions." << std::endl;

	std::cerr << std::hex << "RAX=" << pc.exec->regs[rAX].u.value << std::dec << std::endl;
	std::cerr << std::hex << "RBX=" << pc.exec->regs[rBX].u.value << std::dec << std::endl;
	std::cerr << std::hex << "RCX=" << pc.exec->regs[rCX].u.value << std::dec << std::endl;
	std::cerr << std::hex << "RDX=" << pc.exec->regs[rDX].u.value << std::dec << std::endl;
	std::cerr << std::hex << "ES=" << pc.exec->sregs[0].value << std::dec << std::endl;
std::cerr << "---  " << std::flush;	
dest[0] = '\0';
disasm.disasm(dest, &pc.decode, NULL, NULL, tmpofs + pc.exec->size);
std::cerr << dest << std::endl;

	delete [] disk;
	return 0;
}
