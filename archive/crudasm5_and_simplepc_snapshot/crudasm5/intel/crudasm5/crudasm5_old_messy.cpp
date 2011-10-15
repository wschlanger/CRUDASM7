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
	bool switch_valid;

	switch_table_t()
	{
		switch_valid = false;
	}
};

enum
{
	bbt_unvisited,						// we haven't visited this basic block yet (scheduled)
	bbt_invalid_opcode,					// basic block is followed by an invalid opcode
										// note: the invalid opcode's size in meta[] is 31,
										// however as far as bb_t::end_offset is concerned
										// the invalid opcode is only one byte in length!
	bbt_return,							// ends with one of {ret, ret n, retf, retf n}
	bbt_exit_other,						// ends with {sysenter, syscall, sysexit, sysret}
	bbt_pass,							// pass control to out_edges[0], num_out_edges is 1.
	// "invoke" means we jump to the entrypoint of a procedure.
	bbt_invoke_internal,				// JMP to another procedure's entrypoint, within same module
										// edges.target points to the procedure; num_out_edges is 0!
	bbt_invoke_external,				// out_edge_arg contains the offset of an import descriptor
										// note: after pass 2, a decompiler would convert, via the
										// PEB method, any of these that go back to our module,
										// into bbt_invoke_internal's.
	bbt_call_internal,					// see bbt_invoke_internal (CALL was used instead of JMP)
										// edges.target[1] is the call target;
										// edges.out_edges[0] is the insn following the CALL
	bbt_call_external,					// see bbt_invoke_external (CALL was used instead of JMP)
										// note that some procedures could do nothing other than
										// jump to an import (after possibly following some jmp's
										// and xchg reg1,reg1 or mov reg1,reg1's). pass2 will
										// simply treat calls to these procedures as internal call's.
										// the call-graph fixup step done before pass 3 is done by a
										// true decompiler, will be responsible for converting these
										// to bbt_call_external's (depending on the true target of
										// the import as determined by the PEB method).
										// Here, edges.out_edges[0] is the insn following the CALL
	bbt_switch,							// a switch we recognized, with at least 2 targets in it
										// out_edges[0] is the first case target (for switch_case_values[0]).
										// etc. out_edge_arg is the default case target, normally covered by
										// a ja/jae in an immediately preceding basic block.
	bbt_cond_jump,						// {Jcc, JrCXZ, LOOP, LOOPZ, LOOPNZ}
										// edges.out_edges[0] is the insn following the branch
										// edges.out_edges[1] is the branch target
	// For indirect calls/jumps that seemingly don't go to an import, use these. These could be callbacks.
	// It's also possible for pass 3+ to realize the call target, is a known constant. I.e.
	//   mov/lea rcx,<target>
	//   call rcx
	//   ...
	//   call rcx
	// Here, <target> could be an import (which might go back to us) or the offset of a local procedure.
	// We will not be able to visit "other" call/jump indirect targets before visiting the caller, so the
	// signature of the target will be unknown at this time. Finally these could be true indirect calls/invokations
	// or even an unhandled switch statement (latter case, using JMP, leads to impure code that can't be decompiled).
	bbt_other_call_indirect,
	bbt_other_jump_indirect,

	// PASS (fall thru) to an internal procedure. similar to bb_invoke_internal.
	bbt_pass_proc_internal,

	// NOTE: For the following:
	// 1. bbt_other_call_indirect
	// 2. bbt_cond_jump
	// 3. bbt_switch
	// 4. bbt_call_external
	// 5. bbt_call_internal
	// For the above, edges.out_edges_many[0] is the insn following the instruction
	// (or is the default label for switch). If this is NULL, then out_edge_arg is
	// the address of a local procedure we are to fall thru to for that edge, i.e.
	// a synthetic invokation.

	bbt__end_enum
};

struct bb_t
{
	union
	{
		bb_t *out_edges;				// if there's only 1
		bb_t **out_edges_many;			// if more than 1
		struct procedure_t *target;
		U8 *switch_case_values;
	}	edges;
	U4 offset;							// offset within image of this basic block
	U4 end_offset;						// offset of last insn + length of that insn
	U4 num_out_edges;					// may be 0
	U4 out_edge_arg;					// extra info determined by 'bb_type'
	U1 bb_type;							// if bbt_unvisited, only offset & bb_type are valid

	bb_t()
	{
		bb_type = bbt_unvisited;
		num_out_edges = 0;
	}

	~bb_t()
	{
		if(bb_type == bbt_unvisited)
			return;
		if(bb_type == bbt_switch && edges.switch_case_values != NULL)
		{
			delete [] edges.switch_case_values;
		}
		else
		if(num_out_edges > 1 && edges.out_edges_many != NULL)
		{
			delete [] edges.out_edges_many;
		}
	}
};

struct procedure_t
{
	U8 offset;							// offset within image of entrypoint
	std::map<U8, bb_t> basic_blocks;
	bool valid2;						// this procedure_t is valid, from pass 2
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
	U4 in_mov;
	U4 in_add;
	U4 in_lea;
	U4 in__sxacc;
	U4 in_movsxd;

	std::map<U8, std::set<U8> > xref;
	std::set<U8> procs;							// set of procedures known due to pass 1
	std::map<U8, switch_table_t> swtable;

	bool redo2;
	std::set<U8> new_procs;
	std::set<U8> *pass2_procs;
	size_t pass2_orig_size;
	std::set<U8>::iterator pass2_iter;

	std::map<U8, procedure_t> procs2;			// map of procedures, from pass 2

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
		in_add = dec->getDecoder()->getSyntax()->getSymbol("add");
		in_mov = dec->getDecoder()->getSyntax()->getSymbol("mov");
		in_lea = dec->getDecoder()->getSyntax()->getSymbol("lea");
		in__sxacc = dec->getDecoder()->getSyntax()->getSymbol("_sxacc");
		in_movsxd = dec->getDecoder()->getSyntax()->getSymbol("movsxd");

		scan2_clear();
	}

	~IntelCodeScanner()
	{
		delete [] meta;
	}

	void prescan(std::set<U8> &targets, U1 dszT, U1 sszT, bool do_xref = false)
	{
		dsz = dszT;
		ssz = sszT;
		redo2 = false;
		new_procs.clear();
		scan1(targets, redo2, new_procs, do_xref);
		pass2_procs = &new_procs;
		if(redo2)
		{
			*pass2_procs = procs;
		}
		pass2_orig_size = pass2_procs->size();
		pass2_iter = pass2_procs->begin();

		// Reset local_visited flags.
		for(size_t i = 0; i < image_size; ++i)
		{
			meta[i].local_visited = 0;
		}
	}

	// This returns true if done, false if we need to call it again.
	bool scan()
	{
		if(redo2)
		{
			scan2_clear();
			redo2 = false;
		}

		if(pass2_procs->empty())
			return true;

		size_t count = (pass2_orig_size + 99) / 100;
		if(pass2_procs->size() < count)
			count = pass2_procs->size();

		for(size_t i = 0; i < count; ++i)
		{
			if(pass2_iter == pass2_procs->end())
				return true;
			scan2(*pass2_iter);
			++pass2_iter;
		}

		return false;	// not done yet
	}

private:
	
	// Clear previous pass-2 information here (delete any "procedures").
	// Also called by the constructor.
	void scan2_clear()
	{
		procs2.clear();
	}

	procedure_t *lookUpProc(U8 address)
	{
		std::map<U8, procedure_t>::iterator p_iter = procs2.find(address);
		if(p_iter != procs2.end())
			return &p_iter->second;
		procedure_t &proc = procs2[address];
		proc.offset = address;
		proc.valid2 = false;
		return &proc;
	}

	bb_t *lookUpBb(procedure_t &proc, U8 address)
	{
		std::map<U8, bb_t>::iterator i = proc.basic_blocks.find(address);
		if(i != proc.basic_blocks.end())
			return &i->second;
		bb_t &newBb = proc.basic_blocks[address];
		newBb.bb_type = bbt_unvisited;
		newBb.edges.out_edges = NULL;
		newBb.end_offset = address;
		newBb.num_out_edges = 0;
		newBb.offset = address;
		newBb.out_edge_arg = 0;
		return &newBb;
	}

	// Pass 2 scan of machine code.
	// NOTE: we need to know about IMPORTS. This information is needed to build a proper
	// call graph. All we need is module-specific info, i.e. which offsets are import
	// addresses? This will help recognize jmp [<import>]'s; we can then mark the
	// procedure in question a STUB procedure for that import. We do not know import
	// names yet. Pass 3 will have knowledge of imports: is it noreturn? What is its
	// return depth / signature? Where is it REALLY? Pass 3 is not done by CRUDASM5 however.
	// CRUDASM5 will, for out1.asm, generate a list of callees; it gets the advertized name
	// of imports (not the real target) and lists that.
	void scan2(U8 address)
	{
		// Remove this procedure in the event that it already exists.
		std::map<U8, procedure_t>::iterator p_iter = procs2.find(address);
		if(p_iter != procs2.end())
		{
			procs2.erase(p_iter);
		}

		// Create procedure, intialize its entrypoint address.
		procedure_t &proc = procs2[address];
		proc.offset = address;
		proc.valid2 = false;

		std::list<bb_t *> targets;

		bb_t &entry_bb = proc.basic_blocks[address] = bb_t();
		entry_bb.offset = address;
		entry_bb.bb_type = bbt_unvisited;
		targets.push_back(&entry_bb);

std::cerr << std::hex << "\n--- begin proc " << address << " ---" << std::dec << std::endl;

		ix_icode_t icode;
		U8 rip_mask = TULL(0xffff);
		if(dsz == 1)
			rip_mask = TULL(0xffffffff);
		else
		if(dsz == 2)
			rip_mask = TULL(0xffffffffffffffff);

		while(!targets.empty())
		{
std::cerr << std::endl;
			bb_t *bb = targets.front();
			U8 address = bb->offset;
			targets.erase(targets.begin());
			if(bb->bb_type != bbt_unvisited || meta[address].local_visited == 2)
				continue;	// this should never happen

			bb->num_out_edges = 0;
			bb->out_edge_arg = 0;
			bb->edges.out_edges = NULL;
			bb->end_offset = address;	// initialize

			for(;;)
			{
				/*if(meta[address].procedure != 0 && address != proc.offset)
				{
					// We fell thru to the start of another procedure.
					bb->bb_type = bbt_pass_proc_internal;
					bb->edges.target = lookUpProc(address);
					break;
				}*/

				U8 isize = meta[address].insn_size;
				if(isize == 0)
					return;				// somehow we missed this insn in pass 1
				meta[address].local_visited = 2;	// we've visited this address
				if(isize == 31)
				{
					bb->end_offset += 1;
					bb->bb_type = bbt_invalid_opcode;
					break;
				}
				bb->end_offset += isize;
				bool wantNext = true;
				U8 nextAddress = address + isize;

std::cerr << std::hex << address << std::dec << std::endl;

				if(!dec->decode(&icode, image_size - address, dsz, image + address))
				{
					// This shouldn't happen - it was valid for pass 1!
					return;
				}
				if(dec->getState()->insn_size != isize)
				{
					// Something went seriously wrong, size changed between pass 1 & pass 2.
					return;
				}
				U8 insn_size = isize;

				U4 insn = icode.insn;
				IntelSyntaxInsn *insnT = dec->getDecoder()->getSyntax()->getInsn(insn);
				U4 insn_name = insnT->proto.name;

				if(insn_name == in__iret ||
					insn_name == in__retnum ||
					insn_name == in__ret ||
					insn_name == in__retfnum ||
					insn_name == in__retf
				)
				{
					wantNext = false;
					bb->bb_type = bbt_return;
				}
				else
				if(insn_name == in_sysenter ||
					insn_name == in_sysexit ||
					insn_name == in_syscall ||
					insn_name == in_sysret
				)
				{
					wantNext = false;
					bb->bb_type = bbt_exit_other;
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
						return;		// target is out of bounds
					if(meta[dest].local_visited == 0)
					{
						meta[dest].local_visited = 1;	// scheduled
						targets.push_front(lookUpBb(proc, dest));
					}
					bb->bb_type = bbt_cond_jump;
					bb->num_out_edges = 2;
					bb->edges.out_edges_many = new bb_t * [2];
					bb->edges.out_edges_many[1] = lookUpBb(proc, dest);
					bb->edges.out_edges_many[0] = NULL;
					if(meta[nextAddress].procedure != 0)
					{
						bb->out_edge_arg = nextAddress;
						break;
					}
					bb->edges.out_edges_many[0] = lookUpBb(proc, nextAddress);
					address = nextAddress;
					if(meta[address].local_visited != 0)
						break;
					meta[address].local_visited = 1;	// scheduled for now
					bb = bb->edges.out_edges_many[0];
					continue;
				}
				else
				if(insn_name == in_call)
				{
					/*U8 dest = address + origin + insn_size + (U8)(S8)(S4)(icode.imm);
					dest = (dest & rip_mask) - origin;	// rip_mask is determined by dsz, should really be determined by icode.osz
					if(dest >= image_size)
						return;		// target is out of bounds
					bb->bb_type = bbt_call_internal;
					bb->num_out_edges = 2;
					bb->edges.out_edges_many = new bb_t * [2];
					bb->edges.out_edges_many[1] = lookUpBb(proc, dest);
					bb->edges.out_edges_many[0] = NULL;
					if(meta[nextAddress].procedure != 0)
					{
						bb->out_edge_arg = nextAddress;
						break;
					}
					bb->edges.out_edges_many[0] = lookUpBb(proc, nextAddress);
					address = nextAddress;
					if(meta[address].local_visited != 0)
						break;
					meta[address].local_visited = 1;	// scheduled for now
					bb = bb->edges.out_edges_many[0];
					continue;*/
				}

				if(!wantNext)
					break;		// if set to false, user must have set bb->bb_type.
				address = nextAddress;
				if(address >= image_size)
				{
					return;		// we strayed beyond the bounds of the image
				}
				if(meta[address].procedure != 0)
				{
					// We fell thru to the start of a procedure.
					bb->bb_type = bbt_pass_proc_internal;
					bb->edges.target = lookUpProc(address);
					break;
				}
				if(meta[address].bb_type != 0 || meta[address].local_visited != 0)
				{
					bb_t *bb_target = lookUpBb(proc, address);

					// Pass control on to new BB.
					bb->bb_type = bbt_pass;
					bb->num_out_edges = 1;
					bb->edges.out_edges = bb_target;					

					if(meta[address].local_visited != 0)
						break;	// already been here, or is scheduled already
					meta[address].local_visited = 1;	// scheduled (not for long...)

					bb = bb_target;	// fall thru to new BB
				}
			}
		}

std::cerr << "success" << std::endl;
		proc.valid2 = true;

		// Clear local_visited flags.
		for(std::map<U8, bb_t>::iterator i = proc.basic_blocks.begin(); i != proc.basic_blocks.end(); ++i)
		{
			bb_t &bb = i->second;
			if(bb.bb_type != bbt_unvisited)
			{
				U8 ofs = bb.offset;
				do
				{
std::cerr << std::hex << " - " << ofs << std::dec << std::endl;
					meta[ofs].local_visited = 0;

					// Go to next instruction.
					U8 isize = meta[ofs].insn_size;
					if(isize == 0)
						break;
					if(isize == 31)
						isize = 1;
					ofs += isize;
				}	while(ofs < bb.end_offset);
			}
		}
	}

	// Pass 1 scan of machine code. Main objective: determine where the procedures are.
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

		for(size_t i = 0; i < image_size; ++i)
		{
			meta[i].local_visited = 0;
			// For pass 1 -- if we've EVER decoded this insn, don't revisit it.
			if(meta[i].insn_size != 0)
				meta[i].local_visited = 2;
		}

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

			/*for(size_t i = 0; i < image_size; ++i)
			{
				meta[i].local_visited = 0;
			}*/
			
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
						
						bool gotSwitchTable = false;
						switch_table_t *tmpSwitch = getSwitch(address);
						if(tmpSwitch != NULL)
						{
							if(tmpSwitch->out_edges.size() > 1)
								gotSwitchTable = true;
						}

						bool ok = false;
						if(!gotSwitchTable)
						{
							if(!ok)
								ok = handleSwitchCase2(icodes);
							if(!ok)
								ok = handleSwitchCase1(icodes);
						}
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
								if(swt.out_edges.size() > 1)
								{
									swt.switch_valid = true;
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

	// Returns true if we found and recognized a switch (type 2), false otherwise.
	// Type 2: (in my opinion there is a bug in Microsoft's 64-bit switch code when
	// the operand being switched is 32 bits).
	//   cmp reg1,imm_limit			; reg1 must be 32 bits or 64 bits
	//								; imm_limit must be <0x80000000
	//   ja  default_label
	//   lea reg2,[rel value]		; reg2 must be 64 bits
	//   optional : movsxd reg3,reg1; required if reg1 is 32bits in size.
	//                              ; reg3 is normally rax and must be 64 bits in size.
	//                              ; if reg3=rax & reg1=eax, VC++ uses CDQE here.
	//                              ; If missing, let reg3 = reg1 (64bit switch operand).
	//   optional : movzx reg4,[reg3+reg2+disp]		; use MOVZX table. if missing, let reg4=reg3.
	//   (MS BUG #1 : if above MOVZX is used, we should be clearing the high 32 bits of reg4 here!)
	//   mov reg5,[reg2+reg4*4+disp]; reg5 is 32 bits, and is normally ecx
	//   (MS BUG #2 : since above reg5 is 32bits, we should be doing MOVSX reg5q,reg5d here !!)
	//   add reg5q,reg2				; reg2 is also 64bits here
	//   JMP reg5q
	// For this and case 1, should be we be making sure memory accesses happen via DS & not FS or SS?
	bool handleSwitchCase2(icode_buffer_t &icodes)
	{
		if(dsz != 2)
			return false;	// this recognizer is strictly for 64-bit mode code

		// See if we have JMP reg5q. If yes, scan up, we want to see this:
		//   cmp reg1,imm {stop here} (mode 4)
		//   ...
		//   ja default_label (mode 3)
		//   ...
		//   mov reg5d,[reg2q+reg4q*4+disp] (mode 2)
		//   ...
		//   add reg5q,reg2q (mode 1)
		//   ...
		//   jmp reg5q {start here}
		// When we get to "stop here" we will know each of reg5, reg2, reg4 & reg1.
		// reg5 must not equal reg2, and reg2 must not equal reg4.

		U8 insn_size = icodes.buf[(icodes.index - 1) & (ICODE_BUFFER_SIZE - 1)].insn_len;
		U8 address = icodes.buf[(icodes.index - 1) & (ICODE_BUFFER_SIZE - 1)].address;
		ix_icode_t &icode0 = icodes.buf[(icodes.index - 1) & (ICODE_BUFFER_SIZE - 1)].code;
		U4 insn = icode0.insn;
		IntelSyntaxInsn *insnT = dec->getDecoder()->getSyntax()->getInsn(insn);
		U4 insn_name = insnT->proto.name;
		if(insn_name != in__jmpi)
			return false;
		if(icode0.argtype[0] != IntelArgTypes::Treg_gr)
			return false;
		if(icode0.argsize[0] != 8)
			return false;
		U1 reg5 = icode0.argvalue[0];
		U1 reg2 = 0;
		U1 reg4 = 0;
		U1 reg1 = 0;
		U1 reg1size = 0;
		U8 last_lookup_table = 0;

		U8 idx = (icodes.index - 1) & (ICODE_BUFFER_SIZE - 1);	// this is the JMP reg5 instruction
		U8 startIdx = idx;
		U8 endIdx = (icodes.index - 1 - icodes.size) & (ICODE_BUFFER_SIZE - 1);
		U8 cmp_value = 0;
		int mode = 0;
		bool is_jae = false;
		U8 default_label = 0;
		U8 idxJcc = 0;
		U8 idxMovReg5 = 0;
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
			switch(mode)
			{
			case 0:
				if(insn_name == in_add)
				{
					if(icode.argtype[0] == IntelArgTypes::Treg_gr &&
						icode.argsize[0] == 8 && icode.argvalue[0] == reg5 &&
						icode.argtype[1] == IntelArgTypes::Treg_gr &&
						icode.argsize[1] == 8
					)
					{
						reg2 = icode.argvalue[1];
						mode = 1;
					}
				}
				break;
			case 1:
				if(insn_name == in_mov)
				{
					if(icode.argtype[0] == IntelArgTypes::Treg_gr &&
						icode.argsize[0] == 4 && icode.argvalue[0] == reg5 &&
						icode.argtype[1] == IntelArgTypes::Tmem_ea &&
						icode.has_disp != 0 && icode.disp != 0 &&
						icode.ea.index_scale == 2 && icode.ea.base != 31 &&
						icode.ea.index != 31 &&
						icode.asz == 2 &&
						icode.rip_relative == 0
					)
					{
						if(icode.ea.base == reg2)
						{
							reg4 = icode.ea.index;
							last_lookup_table = icode.disp;
							mode = 2;
							idxMovReg5 = idx;
						}
					}
				}
				break;
			case 2:
				if(insn_name == in__ja || insn_name == in__jnc)
				{
					is_jae = (insn_name == insn_name == in__jnc);
					mode = 3;
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
					idxJcc = idx;
				}
				break;
			case 3:
				if(insn_name == in_cmp)
				{
					if(icode.argtype[0] == IntelArgTypes::Treg_gr &&
						(icode.argsize[0] == 4 || icode.argsize[0] == 8) &&
						icode.argtype[1] == IntelArgTypes::Timm_1st
					)
					{
						if(dsz == 2)
							cmp_value = (U8)(S8)(S4)(icode.imm);
						else
							cmp_value = (U8)(icode.imm);
						mode = 4;
						reg1 = icode.argvalue[0];
						reg1size = icode.argsize[0];
					}
				}
				break;
			}
			if(mode == 4)
				break;
		}

		if(mode != 4)
			return false;

		// Do some sanity checks.
		if(reg5 == reg2 || reg2 == reg4)
			return false;

		//   cmp reg1,imm_limit			; reg1 must be 32 bits or 64 bits
		//								; imm_limit must be <0x80000000
		//   ja  default_label
		// --- start ---
		//   lea reg2,[rel value]		; reg2 must be 64 bits
		//   optional : movsxd reg3,reg1; required if reg1 is 32bits in size.
		//                              ; reg3 is normally rax and must be 64 bits in size.
		//                              ; if reg3=rax & reg1=eax, VC++ uses CDQE here.
		//                              ; If missing, let reg3 = reg1 (64bit switch operand).
		//   optional : movzx reg4,[reg3+reg2+disp]		; use MOVZX table. if missing, let reg4=reg3.
		//   (MS BUG #1 : if above MOVZX is used, we should be clearing the high 32 bits of reg4 here!)
		// --- stop ---
		//   mov reg5,[reg2+reg4*4+disp]; reg5 is 32 bits, and is normally ecx
		//   (MS BUG #2 : since above reg5 is 32bits, we should be doing MOVSX reg5q,reg5d here !!)
		//   add reg5q,reg2				; reg2 is also 64bits here
		//   JMP reg5q
		idx = idxJcc;
		mode = 0;	// just after ja/jae
		// mode 0 : just after ja/jae
		// mode 1 : got lea from mode 0
		// mode 2 : reached idxMovReg5 from mode 1
		bool got_movsxd_reg3 = false;
		U1 reg3 = reg1;
		bool got_movzx_reg4 = false;
		U8 first_lookup_table = 0;		// valid only if got_movzx_reg4 is true
		U8 lea_reg2_value = 0;
		for(;;)
		{
			idx = (idx + 1) & (ICODE_BUFFER_SIZE - 1);
			if(idx == startIdx)
				return false;	// we somehow made it down to the JMP insn

			if(idx == idxMovReg5)
			{
				if(mode == 1)
					mode = 2;
				break;
			}

			ix_icode_t &icode = icodes.buf[idx & (ICODE_BUFFER_SIZE - 1)].code;
			U4 insn = icode.insn;
			IntelSyntaxInsn *insnT = dec->getDecoder()->getSyntax()->getInsn(insn);
			U4 insn_name = insnT->proto.name;

			if(mode == 0)
			{
				if(insn_name == in_lea && icode.argtype[0] == IntelArgTypes::Treg_gr &&
					icode.argtype[1] == IntelArgTypes::Tmem_ea_eal && icode.argsize[0] == 8 &&
					icode.argvalue[0] == reg2 && icode.has_disp != 0 && icode.rip_relative != 0 &&
					icode.ea.base == 31 && icode.ea.index == 31 && icode.argsize[1] == 8
				)
				{
					// OK, we got LEA reg2q,[rel <displacement>].
					mode = 1;
					U8 disp = origin + icodes.buf[idx & (ICODE_BUFFER_SIZE - 1)].address + (U8)(icodes.buf[idx & (ICODE_BUFFER_SIZE - 1)].insn_len);
					if((U8)(icode.disp) < TULL(0x80000000))
						disp += (U8)(icode.disp);
					else
						disp -= (U8)(U4)(TULL(0x100000000) - (U8)(U4)(icode.disp));
					lea_reg2_value = disp;
				}
			}
			else
			if(mode == 1)
			{
				if(!got_movsxd_reg3)
				{
					if((insn_name == in__sxacc && icode.osz == 2 /*cdqe*/) || insn_name == in_movsxd)
					{
						U1 dest = 0;
						U1 src = 0;
						bool ok = (insn_name == in__sxacc);
						if(!ok)
						{
							if(icode.argtype[0] == IntelArgTypes::Treg_gr &&
								icode.argsize[0] == 8 &&
								icode.argtype[1] == IntelArgTypes::Treg_gr &&
								icode.argsize[1] == 4
							)
							{
								ok = true;
								dest = icode.argvalue[0];
								src = icode.argvalue[1];
							}
						}
						if(ok && src == reg1)
						{
							got_movsxd_reg3 = true;
							reg3 = dest;
						}
					}
				}
				if(!got_movzx_reg4)
				{
					if(insn_name == in_movzx)
					{
						if(icode.argtype[0] == IntelArgTypes::Treg_gr &&
							icode.argsize[0] == 4 &&
							icode.argvalue[0] == reg4 &&
							icode.argtype[1] == IntelArgTypes::Tmem_ea &&
							icode.argsize[1] == 1 &&
							icode.asz == 2 &&
							icode.ea.index_scale == 0 &&
							icode.ea.base != 31 && icode.ea.index != 31
						)
						{
							if((icode.ea.base == reg3 && icode.ea.index == reg2) ||
								(icode.ea.base == reg2 && icode.ea.index == reg3)
							)
							{
								got_movzx_reg4 = true;
								first_lookup_table = icode.disp;	// 32bit offset
							}
						}
					}
				}
			}
		}

		// Check mode here.
		if(mode != 2)
			return false;
		if(!got_movzx_reg4 && reg4 != reg3)
			return false;

		if(got_movzx_reg4)
		{
			U8 movzx_offset = first_lookup_table + lea_reg2_value - origin;
			U8 disp = lea_reg2_value + last_lookup_table - origin;

			if(movzx_offset >= image_size)
				return false;
			if(cmp_value == 0)
				return false;
			U8 cmp_max = cmp_value;
			if(!is_jae)
				++cmp_max;
			swtable[address].default_target = default_label;
			swtable[address].switch_valid = false;
			for(U8 cmp_i = 0; cmp_i < cmp_max; ++cmp_i)
			{
				if(movzx_offset + cmp_i >= image_size)
					return false;
				U1 c = image[movzx_offset + cmp_i];
				U8 ofs = disp + (U8)(c) * 4;
				if(ofs + 3 >= image_size)
					return false;
				U4 target = *(U4 *)(image + ofs);
				/*if(target < origin)
					return false;
				target -= origin;*/
				if(target >= image_size)
					return false;
				swtable[address].out_edges[cmp_i] = target;
			}
			return true;
		}
		else
		{
			U8 disp = lea_reg2_value + last_lookup_table - origin;
			if(cmp_value == 0)
				return false;
			U8 cmp_max = cmp_value;
			if(!is_jae)
				++cmp_max;
			swtable[address].default_target = default_label;
			swtable[address].switch_valid = false;
			for(U8 cmp_i = 0; cmp_i < cmp_max; ++cmp_i)
			{
				U8 ofs = disp + cmp_i * 4;
				if(ofs + 3 >= image_size)
					return false;
				U4 target = *(U4 *)(image + ofs);
				/*if(target < origin)
					return false;
				target -= origin;*/
				if(target >= image_size)
					return false;
				swtable[address].out_edges[cmp_i] = target;
			}
			return true;
		}

		return false;
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
			swtable[address].switch_valid = false;
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
			swtable[address].switch_valid = false;
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

/*{
using namespace std;
FILE *ff = fopen("raw.bin", "wb");
fwrite(fd + 0x1000, 0x1000, 1, ff);
fclose(ff);
}*/

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

	std::cerr << "Prescanning " << targets.size() << " entrypoints..." << std::endl;
	IntelCodeScanner scanner(&dec, fd, fs, origin);
	scanner.prescan(targets, bits /* dsz */, bits /* ssz */, true /* do_xref */);

	std::cerr << "Scanning...";
	for(;;)
	{
		bool done = scanner.scan();
		if(done)
			break;
		done = scanner.scan();
		if(done)
			break;
		std::cerr << "." << std::flush;
	}
	std::cerr << " done" << std::endl;

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
					os << "\n; xref:";
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
				if(scanner.getMeta()[offset].indirect >= 2)
				{
					if((icode.argtype[0] == IntelArgTypes::Tmem_ea && icode.ea.index_scale != 0) || (icode.argtype[0] == IntelArgTypes::Treg_gr))
					{
						sprintf(tmp, "  ; [sw]");
						sw = true;
					}
				}
			}
			os << prefix << dest << suffix << "\n";

			if(sw && scanner.getSwitch(offset) != NULL)
			{
				switch_table_t *swt = scanner.getSwitch(offset);
				if(swt->switch_valid)
				{
					os << "; default => 0x" << std::hex << (origin + swt->default_target) << std::dec << "\n";
					for(std::map<U8, U8>::iterator swi = swt->out_edges.begin(); swi != swt->out_edges.end(); ++swi)
					{
						os << std::hex << "; case 0x" << swi->first << " => 0x" << (origin + swi->second) << std::dec << "\n";
					}
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
