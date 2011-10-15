// crudasm5.cpp
// Copyright (C) 2011 Willow Schlanger. All rights reserved.

#include "crudasm5.h"
#include <iostream>
#include <cstdio>
#include <string>
#include <string.h>
#include <stddef.h>

// This is initalized by clearing it to all zero's.
struct intel_meta_t
{
	U2 insn_size : 5;			// 31 for invalid opcodes
	U2 in_wanted : 1;
	U2 procedure : 1;
	///U2 impure_proc : 1;			// contains invalid code
	U2 local_visited : 2;		// 0=not visited, 1=scheduled, 2=visited
	U2 indirect : 2;			// 0=not indirect jmp/call, 1=indirect call, 2=indirect jmp reg, 3=indirect jmp mem
	U2 bb_type : 2;				// 0=not start of BB (unless procedure is set)
								// 1=after a CALL insn, may or may not start a BB, depending on policy
								// 2=after a Jcc/Jrcxz/LOOP/LOOPZ/LOOPNZ insn, always starts BB
								// 3=in label here, always starts BB
	U2 : 3;
};

enum
{
	ICODE_BUFFER_SIZE = 16		// must be a power of 2
};

struct icode_buffer_t
{
	U8 size;
	U8 index;
	struct
	{
		U8 address;
		ix_icode_t code;
		U1 insn_len;
	}	buf[ICODE_BUFFER_SIZE];

	icode_buffer_t()
	{
		reset();
	}

	void reset()
	{
		size = 0;
		index = 0;
	}
};

struct switch_table_t
{
	U8 default_target;
	std::map<U8, U8> out_edges;		// case (begins with 0) => image offset
};

class IntelCodeScanner
{
	IntelMCDecoder *dec;
	U1 *image;
	U8 image_size;
	U8 origin;

	U1 dsz;
	U1 ssz;
	intel_meta_t *meta;

	U4 in__iret;
	U4 in__retnum;
	U4 in__ret;
	U4 in__retfnum;
	U4 in__retf;
	U4 in__loopnz;
	U4 in__loopz;
	U4 in__loop;
	U4 in__jrcxz;
	U4 in_jmp;
	U4 in__jo;
	U4 in__jno;
	U4 in__jc;
	U4 in__jnc;
	U4 in__jz;
	U4 in__jnz;
	U4 in__jbe;
	U4 in__ja;
	U4 in__js;
	U4 in__jns;
	U4 in__jp;
	U4 in__jnp;
	U4 in__jl;
	U4 in__jge;
	U4 in__jle;
	U4 in__jg;
	U4 in_call;
	U4 in__callfd;
	U4 in__jmpfd;
	U4 in__calli;
	U4 in__callfi;
	U4 in__jmpi;
	U4 in__jmpfi;
	U4 in_cmp;
	U4 in_movzx;
	U4 in_sysenter;
	U4 in_sysexit;
	U4 in_syscall;
	U4 in_sysret;

	std::map<U8, std::set<U8> > xref;
	std::set<U8> procs;
	std::map<U8, switch_table_t> swtable;

public:
	switch_table_t *getSwitch(U8 address)
	{
		if(swtable.find(address) == swtable.end())
			return NULL;
		return &swtable[address];
	}

	const std::set<U8> *getXref(U8 x)
	{
		if(xref.find(x) == xref.end())
			return NULL;
		return &xref[x];
	}
	const intel_meta_t *getMeta() const { return meta; }

	IntelCodeScanner(IntelMCDecoder *decT, U1 *imageT, U8 image_sizeT, U8 originT) :
		dec(decT),
		image(imageT),
		image_size(image_sizeT),
		origin(originT)
	{
		meta = new intel_meta_t [image_size];
		memset(meta, 0, sizeof(intel_meta_t) * image_size);

		in__iret = dec->getDecoder()->getSyntax()->getSymbol("_iret");
		in__retnum = dec->getDecoder()->getSyntax()->getSymbol("_retnum");
		in__ret = dec->getDecoder()->getSyntax()->getSymbol("_ret");
		in__retfnum = dec->getDecoder()->getSyntax()->getSymbol("_retfnum");
		in__retf = dec->getDecoder()->getSyntax()->getSymbol("_retf");
		in__loopnz = dec->getDecoder()->getSyntax()->getSymbol("_loopnz");
		in__loopz = dec->getDecoder()->getSyntax()->getSymbol("_loopz");
		in__loop = dec->getDecoder()->getSyntax()->getSymbol("_loop");
		in__jrcxz = dec->getDecoder()->getSyntax()->getSymbol("_jrcxz");
		in_jmp = dec->getDecoder()->getSyntax()->getSymbol("jmp");
		in__jo = dec->getDecoder()->getSyntax()->getSymbol("_jo");
		in__jno = dec->getDecoder()->getSyntax()->getSymbol("_jno");
		in__jc = dec->getDecoder()->getSyntax()->getSymbol("_jc");
		in__jnc = dec->getDecoder()->getSyntax()->getSymbol("_jnc");
		in__jz = dec->getDecoder()->getSyntax()->getSymbol("_jz");
		in__jnz = dec->getDecoder()->getSyntax()->getSymbol("_jnz");
		in__jbe = dec->getDecoder()->getSyntax()->getSymbol("_jbe");
		in__ja = dec->getDecoder()->getSyntax()->getSymbol("_ja");
		in__js = dec->getDecoder()->getSyntax()->getSymbol("_js");
		in__jns = dec->getDecoder()->getSyntax()->getSymbol("_jns");
		in__jp = dec->getDecoder()->getSyntax()->getSymbol("_jp");
		in__jnp = dec->getDecoder()->getSyntax()->getSymbol("_jnp");
		in__jl = dec->getDecoder()->getSyntax()->getSymbol("_jl");
		in__jge = dec->getDecoder()->getSyntax()->getSymbol("_jge");
		in__jle = dec->getDecoder()->getSyntax()->getSymbol("_jle");
		in__jg = dec->getDecoder()->getSyntax()->getSymbol("_jg");
		in_call = dec->getDecoder()->getSyntax()->getSymbol("call");
		in__callfd = dec->getDecoder()->getSyntax()->getSymbol("_callfd");
		in__jmpfd = dec->getDecoder()->getSyntax()->getSymbol("_jmpfd");
		in__calli = dec->getDecoder()->getSyntax()->getSymbol("_calli");
		in__callfi = dec->getDecoder()->getSyntax()->getSymbol("_callfi");
		in__jmpi = dec->getDecoder()->getSyntax()->getSymbol("_jmpi");
		in__jmpfi = dec->getDecoder()->getSyntax()->getSymbol("_jmpfi");
		in_cmp = dec->getDecoder()->getSyntax()->getSymbol("cmp");
		in_movzx = dec->getDecoder()->getSyntax()->getSymbol("movzx");
		in_sysenter = dec->getDecoder()->getSyntax()->getSymbol("sysenter");
		in_sysexit = dec->getDecoder()->getSyntax()->getSymbol("sysexit");
		in_syscall = dec->getDecoder()->getSyntax()->getSymbol("syscall");
		in_sysret = dec->getDecoder()->getSyntax()->getSymbol("sysret");
	}

	~IntelCodeScanner()
	{
		delete [] meta;
	}

	void scan(std::set<U8> &targets, U1 dszT, U1 sszT, bool do_xref = false)
	{
		dsz = dszT;
		ssz = sszT;
		bool redo2 = false;
		std::set<U8> new_procs;
		scan1(targets, redo2, new_procs, do_xref);
		if(redo2)
		{
			scan2(procs, true);
		}
		else
		{
			scan2(new_procs, false);
		}
	}

private:
	//    Pass 2 scan of machine code.
	void scan2(std::set<U8> &new_procs, bool start_over)
	{
	}

	// Pass 1 scan of machine code. Main objective: determine where the procedures are.
	// * Need to add SWITCH/CASE recognition! No-Returns?
	void scan1(std::set<U8> &targets, bool &redo2, std::set<U8> &new_procs, bool do_xref)
	{
		redo2 = false;

		// These are targets we "want" to visit.
		// The insn size (1..31) is set after it's
		// been visited. Invalid opcodes get a size
		// of 31; never-before visited bytes have a
		// size of 0.
		std::list<U8> wanted;
		for(std::set<U8>::iterator i = targets.begin(); i != targets.end(); ++i)
		{
			if(*i < image_size)
			{
				wanted.push_back(*i);
				meta[*i].in_wanted = 1;
			}
		}

		icode_buffer_t icodes;

		U8 rip_mask = TULL(0xffff);
		if(dsz == 1)
			rip_mask = TULL(0xffffffff);
		else
		if(dsz == 2)
			rip_mask = TULL(0xffffffffffffffff);

		while(!wanted.empty())
		{
			U8 proc_address = wanted.front();
			wanted.pop_front();

			if(meta[proc_address].procedure != 0)
			{
				continue;
			}
			procs.insert(proc_address);
			new_procs.insert(proc_address);
			if(meta[proc_address].insn_size != 0)
			{
				redo2 = true;
			}
			meta[proc_address].procedure = 1;

			for(size_t i = 0; i < image_size; ++i)
			{
				meta[i].local_visited = 0;
			}
			
			std::list<U8> items;
			items.push_front(proc_address);

			while(!items.empty())
			{
				U8 address = items.front();
				items.pop_front();

				icodes.reset();

				for(;;)
				{
					if(meta[address].local_visited == 2)
						break;
					meta[address].local_visited = 2;

					ix_icode_t &icode = icodes.buf[icodes.index].code;
					if(!dec->decode(&icode, image_size - address, dsz, image + address))
					{
						meta[address].insn_size = 31;		// invalid opcode
						////meta[proc_address].impure_proc = 1;
						break;
					}

					U8 insn_size = dec->getState()->insn_size;
					if(meta[address].insn_size == 0)
					{
						meta[address].insn_size = insn_size;
					}

					icodes.buf[icodes.index].address = address;
					icodes.buf[icodes.index].insn_len = insn_size;
					if(icodes.size != ICODE_BUFFER_SIZE)
						++icodes.size;
					icodes.index = (icodes.index + 1) & (ICODE_BUFFER_SIZE - 1);

					bool tryNextInsn = true;
					U8 nextAddress = ((address + origin + insn_size) & rip_mask) - origin;

					U4 insn = icode.insn;
					IntelSyntaxInsn *insnT = dec->getDecoder()->getSyntax()->getInsn(insn);
					U4 insn_name = insnT->proto.name;

					if(insn_name == in__iret ||
						insn_name == in__retnum ||
						insn_name == in__ret ||
						insn_name == in__retfnum ||
						insn_name == in__retf ||
						insn_name == in_sysenter ||
						insn_name == in_sysexit ||
						insn_name == in_syscall ||
						insn_name == in_sysret
					)
					{
						tryNextInsn = false;
					}
					else
					if(insn_name == in__loopnz ||
						insn_name == in__loopz ||
						insn_name == in__loop ||
						insn_name == in__jrcxz ||
						insn_name == in__jo ||
						insn_name == in__jno ||
						insn_name == in__jc ||
						insn_name == in__jnc ||
						insn_name == in__jz ||
						insn_name == in__jnz ||
						insn_name == in__jbe ||
						insn_name == in__ja ||
						insn_name == in__js ||
						insn_name == in__jns ||
						insn_name == in__jp ||
						insn_name == in__jnp ||
						insn_name == in__jl ||
						insn_name == in__jge ||
						insn_name == in__jle ||
						insn_name == in__jg
					)
					{
						U8 dest = address + origin + insn_size + (U8)(S8)(S4)(icode.imm);
						dest = (dest & rip_mask) - origin;	// rip_mask is determined by dsz, should really be determined by icode.osz
						if(dest >= image_size)
						{
							////meta[proc_address].impure_proc = 1;
						}
						else
						{
							meta[dest].bb_type = 3;
							if(do_xref)
								xref[dest].insert(address);
							if(meta[dest].local_visited == 0)
							{
								meta[dest].local_visited = 1;	// scheduled
								items.push_back(dest);
							}

							if(nextAddress < image_size)
							{
								if(meta[nextAddress].bb_type == 0)
									meta[nextAddress].bb_type = 2;
							}
						}
					}
					else
					if(insn_name == in_jmp)
					{
						U8 dest = address + origin + insn_size + (U8)(S8)(S4)(icode.imm);
						dest = (dest & rip_mask) - origin;	// rip_mask is determined by dsz, should really be determined by icode.osz
						if(dest >= image_size)
						{
							////meta[proc_address].impure_proc = 1;
							tryNextInsn = false;
						}
						else
						{
							meta[dest].bb_type = 3;
							if(do_xref)
								xref[dest].insert(address);
							if(meta[dest].local_visited == 0)
							{
								nextAddress = dest;
							}
							else
							{
								tryNextInsn = false;
							}
						}
					}
					else
					if(insn_name == in_call)
					{
						U8 dest = address + origin + insn_size + (U8)(S8)(S4)(icode.imm);
						dest = (dest & rip_mask) - origin;	// rip_mask is determined by dsz, should really be determined by icode.osz
						if(dest >= image_size)
						{
							////meta[proc_address].impure_proc = 1;
						}
						else
						{
							if(meta[dest].in_wanted == 0)
							{
								meta[dest].in_wanted = 1;
								if(meta[dest].procedure == 0)
								{
									wanted.push_back(dest);
								}
							}
							if(nextAddress < image_size)
							{
								if(meta[nextAddress].bb_type == 0)
									meta[nextAddress].bb_type = 1;
							}
						}
					}
					else
					if(insn_name == in__callfd)
					{
						////meta[proc_address].impure_proc = 1;
					}
					else
					if(insn_name == in__jmpfd)
					{
						////meta[proc_address].impure_proc = 1;
						tryNextInsn = false;
					}
					else
					if(insn_name == in__calli)
					{
						meta[address].indirect = 1;
						if(nextAddress < image_size)
						{
							if(meta[nextAddress].bb_type == 0)
								meta[nextAddress].bb_type = 1;
						}
						// This could be a call via an import, a call via a register (which again may be an import),
						// or a callback call.
					}
					else
					if(insn_name == in__callfi)
					{
						////meta[proc_address].impure_proc = 1;
					}
					else
					if(insn_name == in__jmpi)
					{
						// default behavior:
						tryNextInsn = false;
						meta[address].indirect = 2;
						if(IntelArgTypes::is_mem(icode.argtype[0]))
							meta[address].indirect = 3;
						bool ok = handleSwitchCase1(icodes);	// only do this in pass 1
						//if(!ok)  ok = handleSwitchCase2(icodes);
						if(ok)
						{
							if(swtable.find(address) != swtable.end())
							{
								switch_table_t &swt = swtable[address];
								for(std::map<U8, U8>::iterator swi = swt.out_edges.begin(); swi != swt.out_edges.end(); ++swi)
								{
									U8 dest = swi->second;
									meta[dest].bb_type = 3;
									if(do_xref)
										xref[dest].insert(address);
									if(meta[dest].local_visited == 0)
									{
										meta[dest].local_visited = 1;	// scheduled
										items.push_back(dest);
									}
								}
							}
						}
					}
					else
					if(insn_name == in__jmpfi)
					{
						////meta[proc_address].impure_proc = 1;
						tryNextInsn = false;
					}

					if(!tryNextInsn)
						break;
					address = nextAddress;
					if(address >= image_size)
					{
						////meta[proc_address].impure_proc = 1;
						break;
					}
					if(meta[address].local_visited != 0)
						break;	// it's scheduled [or already been visited], so don't revisit it
				}
			}
		}
	}

	// Returns true if we found and recognized a switch (type 1), false otherwise.
	// Type 1:
	//   cmp reg1,imm
	//   ..may be anything here, as long as it doesn't write to CF (jae) or CF/ZF (ja), or reg1
	//   ja/jae defaultLabel
	//   ..may be anything here, as long as it doesn't write to reg1
	//     as a special exception, we may do this exactly once:
	//     movzx reg1,byte [reg1+disp]  ; may be rip-relative
	//   jmp [reg1*OperandSize+disp]	; may be rip-relative
	// This is traditionally used for 32-bit code.
	// There are other styles of 32-bit switches.
	bool handleSwitchCase1(icode_buffer_t &icodes)
	{
		U8 insn_size = icodes.buf[(icodes.index - 1) & (ICODE_BUFFER_SIZE - 1)].insn_len;
		U8 address = icodes.buf[(icodes.index - 1) & (ICODE_BUFFER_SIZE - 1)].address;
		ix_icode_t &icode0 = icodes.buf[(icodes.index - 1) & (ICODE_BUFFER_SIZE - 1)].code;
		U4 insn = icode0.insn;
		IntelSyntaxInsn *insnT = dec->getDecoder()->getSyntax()->getInsn(insn);
		U4 insn_name = insnT->proto.name;
		if(insn_name != in__jmpi)
			return false;
		if(!IntelArgTypes::is_mem(icode0.argtype[0]))
			return false;
		if(icode0.ea.base != 31)
			return false;		// base register not allowed
		if(icode0.ea.index == 31)
			return false;		// index register is required
		if(icode0.asz != dsz)
			return false;		// address size must match default size
		if(icode0.ea.index_scale != (dsz + 1))
			return false;		// we must be scaling by default size
		if(dsz == 2)
		{
			// 64 bit mode.
			if(icode0.osz == 0)
				return false;	// db 0x66 was used
		}
		else
		if(dsz == 1)
		{
			// 32 bit mode.
			if(icode0.osz != 1)
				return false;	// db 0x66 was used
		}
		else
			return false;		// switch tables not recognized in 16bit mode

		U1 reg1 = icode0.ea.index;	// this is not 31

		if(!icode0.has_disp || icode0.disp == 0)
			return 0;
		U8 disp = icode0.disp;	// zero-extend from 32 bits
		if(icode0.rip_relative != 0)
		{
			disp = origin + address + insn_size;
			if((U8)(icode0.disp) < TULL(0x80000000))
				disp += (U8)(icode0.disp);
			else
				disp -= (U8)(U4)(TULL(0x100000000) - (U8)(U4)(icode0.disp));
		}

		if(disp < origin)
			return false;
		disp -= origin;
		if(disp >= image_size)
			return false;

		// 'disp' is now the offset within the image, of where the switch table is.

		U8 idx = (icodes.index - 1) & (ICODE_BUFFER_SIZE - 1);
		U8 startIdx = idx;
		U8 endIdx = (icodes.index - 1 - icodes.size) & (ICODE_BUFFER_SIZE - 1);
		U8 cmp_value = 0;
		bool found_cmp = false;
		for(;;)
		{
			idx = (idx - 1) & (ICODE_BUFFER_SIZE - 1);
			if(idx == endIdx)
				break;
			// Check for CMP reg1,imm
			ix_icode_t &icode = icodes.buf[idx & (ICODE_BUFFER_SIZE - 1)].code;
			U4 insn = icode.insn;
			IntelSyntaxInsn *insnT = dec->getDecoder()->getSyntax()->getInsn(insn);
			U4 insn_name = insnT->proto.name;
			if(insn_name == in_cmp)
			{
				if(icode.argtype[0] == IntelArgTypes::Treg_gr &&
					icode.argsize[0] == (2 << dsz) &&
					icode.argvalue[0] == reg1 &&
					icode.argtype[1] == IntelArgTypes::Timm_1st
				)
				{
					if(dsz == 2)
						cmp_value = (U8)(S8)(S4)(icode.imm);
					else
						cmp_value = (U8)(icode.imm);
					found_cmp = true;
					break;
				}
			}
		}

		if(!found_cmp)
			return false;

		// Now scan down from idx to startIdx, inclusive.
		// idx is actually the CMP reg1,imm thing.

		// mode 0 : waiting for ja/jae <default_label>.
		// mode 1 : got ja/jae, waiting for movzx/jmp
		// mode 2 : got jmp, from mode 1 [accept state]
		// mode 3 : got movzx, from mode 1
		// mode 4 : got jmp, from mode 3 [accept state]

		// NOTE: Ideally we would process instruction SEMANTICS, to see what
		// is written to between modes. This would detect an instruction that
		// "ruins" our pattern despite it seeming to match at a glance.
		int mode = 0;
		U8 default_label = 0;
		U8 movzx_offset = 0;
		bool is_jae = false;

		idx = (idx + 1) & (ICODE_BUFFER_SIZE - 1);	// skip CMP
		if(idx == startIdx)
			return false;
		for(;;)
		{
			ix_icode_t &icode = icodes.buf[idx & (ICODE_BUFFER_SIZE - 1)].code;
			U4 insn = icode.insn;
			IntelSyntaxInsn *insnT = dec->getDecoder()->getSyntax()->getInsn(insn);
			U4 insn_name = insnT->proto.name;

			switch(mode)
			{
			case 0:
				{
					if(insn_name == in__ja || insn_name == in__jnc)
					{
						is_jae = (insn_name == in__jnc);
						mode = 1;
						U8 dest = icodes.buf[idx & (ICODE_BUFFER_SIZE - 1)].address + origin + (U8)(icodes.buf[idx & (ICODE_BUFFER_SIZE - 1)].insn_len) + (U8)(S8)(S4)(icode.imm);
						U8 rip_mask = TULL(0xffff);
						if(dsz == 1)
							rip_mask = TULL(0xffffffff);
						else
						if(dsz == 2)
							rip_mask = TULL(0xffffffffffffffff);
						dest = (dest & rip_mask) - origin;	// rip_mask is determined by dsz, should really be determined by icode.osz
						if(dest >= image_size)
							return false;
						default_label = dest;
					}
					break;
				}
			case 1:
				{
					if(insn_name == in_movzx)
					{
						if(icode.argtype[0] == IntelArgTypes::Treg_gr &&
							icode.argsize[0] == (2 << dsz) &&
							icode.argvalue[0] == reg1 &&
							icode.argtype[1] == IntelArgTypes::Tmem_ea &&
							icode.argsize[1] == 1 &&
							icode.has_disp &&
							icode.disp != 0
						)
						{
							if(icode.ea.base == 31 && icode.ea.index == reg1 && icode.ea.index_scale == 0)
								;
							else
							if(icode.ea.index == 31 && icode.ea.base == reg1)
								;
							else
								break;		// from switch

							U8 disp = icode.disp;	// zero-extend from 32 bits
							if(icode.rip_relative != 0)
							{
								U8 addr2 = icodes.buf[idx & (ICODE_BUFFER_SIZE - 1)].address;
								disp = origin + addr2 + insn_size;
								if((U8)(icode.disp) < TULL(0x80000000))
									disp += (U8)(icode0.disp);
								else
									disp -= (U8)(U4)(TULL(0x100000000) - (U8)(U4)(icode.disp));
							}

							if(disp >= origin)
							{
								disp -= origin;
								if(disp < image_size)
								{
									movzx_offset = disp;
									mode = 3;
								}
							}
						}
					}
					else
					if(insn_name == in__jmpi && idx == startIdx)
					{
						mode = 2;
					}
					break;
				}
			case 3:
				{
					if(insn_name == in__jmpi && idx == startIdx)
					{
						mode = 4;
					}
					break;
				}
			default:
				break;
			}

			if(mode == 2 || mode == 4)
				break;
			if(idx == startIdx)
				return false;
			idx = (idx + 1) & (ICODE_BUFFER_SIZE - 1);
		}

		bool got_movzx = (mode == 4);

		/*if(disp == 0)
			return false;*/

		if(got_movzx)
		{
			/*if(movzx_offset == 0)
				return false;*/
			if(movzx_offset >= image_size)
				return false;
			if(cmp_value == 0)
				return false;
			U8 cmp_max = cmp_value;
			if(!is_jae)
				++cmp_max;
			swtable[address].default_target = default_label;
			for(U8 cmp_i = 0; cmp_i < cmp_max; ++cmp_i)
			{
				if(movzx_offset + cmp_i >= image_size)
					return false;
				U1 c = image[movzx_offset + cmp_i];
				U8 ofs = disp + (U8)(c) * 4;
				if(ofs + 3 >= image_size)
					return false;
				U4 target = *(U4 *)(image + ofs);
				if(target < origin)
					return false;
				target -= origin;
				if(target >= image_size)
					return false;
				swtable[address].out_edges[cmp_i] = target;
			}
			return true;
		}
		else
		{
			if(cmp_value == 0)
				return false;
			U8 cmp_max = cmp_value;
			if(!is_jae)
				++cmp_max;
			swtable[address].default_target = default_label;
			for(U8 cmp_i = 0; cmp_i < cmp_max; ++cmp_i)
			{
				U8 ofs = disp + cmp_i * 4;
				if(ofs + 3 >= image_size)
					return false;
				U4 target = *(U4 *)(image + ofs);
				if(target < origin)
					return false;
				target -= origin;
				if(target >= image_size)
					return false;
				swtable[address].out_edges[cmp_i] = target;
			}
			return true;
		}

		return false;
	}
};

void disassemble(IntelDecoder &decoder, U1 *fd, long fs, std::string fn, std::ostream &os)
{
	bool is64on32;

	if(fs == 0)
		return;		// nothing to do!

	ix_decoder_state s;
	IntelMCDecoder dec(&s, &decoder);
	IntelDisassembler disassembler(&decoder);
	ix_icode_t icode;

	ImageLoader ldr(fd, fs);
	U1 bits = ldr.getBits();

	if(bits == 3)
	{
		std::cerr << "Image is 64-bits, but this program was compiled in 32-bit mode!" << std::endl;
		return;
	}

	fd = ldr.getLoadedData();
	fs = ldr.getLoadedSize();

	U8 offset = ldr.getEntrypoint();
	U8 startOffset = ldr.getStartOffset();

	if(ldr.isLibrary())
		std::cerr << "Image type: DLL" << std::endl;
	else
	if(bits != 0)
		std::cerr << "Image type: EXE" << std::endl;
	else
		std::cerr << "Image type: COM" << std::endl;

	U8 origin = 0;

	std::set<U8> targets;
	targets.insert(ldr.getEntrypoint());

	if(bits == 0)
	{
		origin = 0x100;
		startOffset = 0;
	}
	else
	{
		LoadContext context(fs, fd);
		bool is64on32 = false;
		bool ok = context.load(&is64on32);
		if(is64on32)
		{
			std::cerr << "Image is 64-bits, but this program was compiled in 32-bit mode!" << std::endl;
			return;
		}
		if(!ok)
		{
			std::cerr << "Unable to load image!" << std::endl;
			return;
		}

		// OK to "move" DLL here, i.e. change origin (crudasm5 doesn't do that though).

		origin = context.get_origin();

		if(context.load_log_begin() != context.load_log_end())
		{
			std::cerr << "The following is the error log from loading the file:" << std::endl;
			std::list<std::string>::const_iterator i = context.load_log_begin();
			for(; i != context.load_log_end(); ++i)
			{
				std::cerr << "> " << *i << std::endl;
			}
		}

		for(std::map<U8, const std::string *>::const_iterator i = context.exports_begin(); i != context.exports_end(); ++i)
		{
			// This may get the entrypoint again, possibly.
			targets.insert(i->first);
		}
	}

	std::cerr << "Scanning " << targets.size() << " entrypoints..." << std::endl;
	IntelCodeScanner scanner(&dec, fd, fs, origin);
	scanner.scan(targets, bits /* dsz */, bits /* ssz */, true /* do_xref */);

	std::cerr << "Disassembling..." << std::endl;
	os << std::endl;

	// Disassemble, from beginning to end.
	for(offset = /*0*/startOffset; offset < fs;)
	{
		if(dec.decode(&icode, fs - offset, bits, fd + offset))
		{
			U8 len = s.insn_size;

			if(scanner.getMeta()[offset].insn_size == 0 /*|| scanner.getMeta()[offset].insn_size == 31*/)
			{
				U8 h = 1;
				for(; h < len; ++h)
				{
					if(scanner.getMeta()[offset + h].insn_size != 0 && scanner.getMeta()[offset + h].insn_size != 31)
						break;
				}
				if(h < len)
				{
					offset += h;
					continue;
				}
			}

			if(scanner.getMeta()[offset].procedure != 0)
			{
				os << "\n; Procedure entrypoint";
				/*if(scanner.getMeta()[offset].impure_proc != 0)
					os << " (impure)";*/
				os << "\n";
			}
			else
			if(scanner.getMeta()[offset].bb_type == 3)
			{
				const std::set<U8> *xref = scanner.getXref(offset);
				if(xref != NULL)
				{
					os << "\nxref:";
					for(std::set<U8>::const_iterator i = xref->begin(); i != xref->end(); ++i)
					{
						os << " " << std::hex << (origin + *i) << std::dec;
					}
				}
				os << "\n";
			}

			char prefix[1024];
			char dest[1024];
			dest[0] = '\0';
			disassembler.disasm(dest, NULL, NULL, offset + origin, &s, &icode, bits);
			std::sprintf(prefix, "%08x%08x%c ", (U4)(((offset + origin) >> 16) >> 16), (U4)(offset + origin), (scanner.getMeta()[offset].insn_size != 0) ? ' ' : '-');
			char suffix[1024];
			suffix[0] = '\0';
			bool sw = false;
			if(scanner.getMeta()[offset].indirect != 0)
			{
				char *tmp = suffix + sprintf(suffix, "  ; [indirect=%d]", scanner.getMeta()[offset].indirect);
				if(icode.ea.index_scale != 0)
				{
					sprintf(tmp, "  ; [sw]");
					sw = true;
				}
			}
			os << prefix << dest << suffix << "\n";

			if(sw && scanner.getSwitch(offset) != NULL)
			{
				switch_table_t *swt = scanner.getSwitch(offset);
				os << "; default => 0x" << std::hex << (origin + swt->default_target) << std::dec << "\n";
				for(std::map<U8, U8>::iterator swi = swt->out_edges.begin(); swi != swt->out_edges.end(); ++swi)
				{
					os << std::hex << "; case 0x" << swi->first << " => 0x" << (origin + swi->second) << std::dec << "\n";
				}
			}

			offset += (size_t)(len);
		}
		else
		{
			///std::cerr << ">> " << dec.explain_error() << std::endl;
			++offset;
		}
	}

	if(true)
	{
		std::cerr << "Writing invalid opcodes..." << std::endl;
		using namespace std;
		FILE *fo = fopen("invalid.bin", "wb");
		for(offset = 0; offset < fs; ++offset)
		{
			if(scanner.getMeta()[offset].insn_size == 31)
			{
				fputc(0xb0, fo);
				fputc(0xcc, fo);
				for(U8 i = 0; i < 30; ++i)
				{
					if(i + offset >= fs)
						break;
					fputc(fd[i + offset], fo);
				}
				for(U8 i = 0; i < 15; ++i)
					fputc(0x90, fo);
			}
		}
		fclose(fo);
	}

	os << std::flush;
}

int main(int argc, char **argv)
{
	QCoreApplication app(argc, argv);

	std::cerr << "CRUDASM5  EXE/DLL Disassembler  Copyright (C) 2008-2011 Willow Schlanger" << std::endl;
	if(argc != 2)
	{
		std::cerr << "usage: crudasm5 infile" << std::endl;
		return 1;
	}

	using namespace std;
	FILE *f;
	std::string intel_xml = "intel.xml";
	
	QFile file(intel_xml.c_str());
	if(!file.open(QIODevice::ReadOnly))
	{
		std::cerr << "Error: unable to open file: " << intel_xml << std::endl;
		return 1;
	}
	QDomDocument doc("intel");
	if(!doc.setContent(&file))
	{
		file.close();
		std::cerr << "Error: parse error reading file: " << intel_xml << std::endl;
		return 1;
	}
	file.close();

	QDomElement root = doc.documentElement();
	if(root.tagName() != "intel")
	{
		std::cerr << "Error: invalid file: " << intel_xml << std::endl;
		std::cerr << "root tag name is: " << qPrintable(root.tagName()) << std::endl;
		return 1;
	}

	IntelSyntax syntax;
	IntelDecoder decoder(&syntax);
	std::cerr << "Loading XML..." << std::endl;
	QString result = syntax.load(root);
	QString result2;
	if(result == "")
		result2 = decoder.init();
	if(result != "")
	{
		std::cerr << "Error parsing Intel XML for syntax: " << qPrintable(result) << std::endl;
		return 1;
	}
	if(result2 != "")
	{
		std::cerr << "Error initializing Intel decoder table: " << qPrintable(result2) << std::endl;
		return 1;
	}

	std::cerr << "Reading binary..." << std::endl;
	f = fopen(argv[1], "rb");
	if(f == NULL)
	{
		std::cerr << "Error: file not found: " << argv[1] << std::endl;
		return 1;
	}
	long fs = 0;
	if(!feof(f))
	{
		fseek(f, -1, SEEK_END);
		fs = 1 + ftell(f);
		rewind(f);
	}
	if(fs == 0)
	{
		std::cerr << "Error: file has 0 size: " << argv[1] << std::endl;
		fclose(f);
		return 1;
	}
	U1 *fd = new U1 [fs];
	if(fread(fd, fs, 1, f) != 1)
	{
		std::cerr << "Error: error reading file: " << argv[1] << std::endl;
		fclose(f);
		return 1;
	}
	fclose(f);

	std::string fn = argv[1];

	// Disassemble here.
	disassemble(decoder, fd, fs, fn, std::cout);

	delete [] fd;
	return 0;
}
