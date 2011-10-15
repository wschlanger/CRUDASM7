// make_decoder.cpp
// Copyright (C) 2008 Willow Schlanger

#include "types.h"
#include "x86s_common.h"

#include <fstream>
#include <iostream>
#include <list>
#include <cassert>
#include <vector>

/*
Required prefixes:
- repeat : none, f2, f3
- op66 : none, 66
6 * 0x300 = 3072 [use 768 not 512: we need 3D Now! instructions too.

d9 f4       --- ?
66 0f 38 01 --- ?

Level 4. This comes FIRST.

There is one 4608-entry table. The table may refer to an encoding or a splitter, of which there are three levels.

Level 1. 8 entries, reg/op [ro0..ro7]
- roz : claim all 8, use 0 when encoding
- reg : claim all 8 (no table, again)
- def : default (no modr/m)
- 0..7 : table exists.
Level 2. 4 entries, all 4 memory types [mt0..mt3]
- mtr : claim 0,1,2 (register type)
- def : default (no table)
- 0..3 : claim only that #
Level 3. 8 entries, all 'mod' values. (this is a register when memory type is register). [md0..md7]
- 0..7 or def for default.

All of the above imply a modr/m.

We will generate a vector of U4's and then write this to x86c_decoder_table.c.
- Make sure has_d, has_s, has_w are all 0.
- If it has an imm_implict_cc argument, claim basecode + 0..0xf inclusive
- if it has a reg_basecode argument, claim basecode + 0..7 inclusive and
  make sure it's no64 (there are 16 64-bit numbers).

We have an array of 512 elements. Each element is a list. It's a list of Family's.
A Family is an 'opcode tree' using reg/op's, mt's, and mod's. Actually it's a flat list.

At most, there can be 8x8x4 = 3:3:2 = 256 entries to search.
*/

namespace x86s
{

#include "out_insn_enums.h"

// This is terminated with an element with an insn number of insn__count
encoding_t encodings[] =
{
#include "out_encodings.h"
};

struct FamilyMod
{
	int mod;		// reg number if reg type, 0..7 or -1
	int encoding;	// there is only one! 24 bits.
};

struct FamilyMem
{
	int memtype;	// 0..3, or: -1 for all. create 3 entries for mtr (0,1,2)
	std::list<FamilyMod> mod;
};

struct Family
{
	int regop;		// regop to claim, or -1
	std::list<FamilyMem> mem;
};

struct XFamily
{
	std::list<Family> f;
};

struct OpcodeFamily
{
	int repeat;		// nofx, f2, f3, default : 0, 1, 2, 3
	int op66;		// no66, 66, default : 0, 1, 2
	std::vector<XFamily> x;
};

struct OpcodeTree
{
	typedef std::list<OpcodeFamily> of_t;
	of_t of[256 * 3];	// 0x300 entries

	void done();

	// k = encoding index.
	void accept(int k, encoding_t *e, int basecode_ofs);
	
	// ro = reg/op
	// mt = mod
	// md = r/m
	void do_accept(int k, struct OpcodeFamily &f, U4 ro, U4 mt, U4 md, U4 nextbyte);

	// h = 0..255, next byte or 0 if none
	void add_to_table(std::vector<U4> &table, int claim_bc, OpcodeFamily &f, U4 h);

	void write_decoder_table_c(std::vector<U4> &table);
};

// When this returns, table[claim_bc] points to the correct table or encoding.
// table[claim_bc] might already be set, in which case it is not modified; in
// that case the high 8 bits must be nonzero. We can't 'settle' at a target that
// is already claimed.
void OpcodeTree::add_to_table(std::vector<U4> &table, int claim_bc, OpcodeFamily &f, U4 h)
{
	for(std::list<Family>::iterator i = f.x[h].f.begin(); i != f.x[h].f.end(); ++i)
	{
		for(std::list<FamilyMem>::iterator j = i->mem.begin(); j != i->mem.end(); ++j)
		{
			for(std::list<FamilyMod>::iterator k = j->mod.begin(); k != j->mod.end(); ++k)
			{
				int x = claim_bc;	// target
				
				if(f.x.size() == 256)
				{
					if(table[x] == 0x00ffffff)
					{
						table[x] = (0x04 << 24) + (U4)table.size();
						for(int m = 0; m < 256; ++m)
							table.push_back(0x00ffffff);
					}
					assert((table[x] >> 24) == 0x04);
					x = (table[x] & 0xffffff) + h;
				}
						
				if(i->regop != -1)
				{
					if(table[x] == 0x00ffffff)
					{
						table[x] = (0x01 << 24) + (U4)table.size();
						for(int h = 0; h < 8; ++h)
							table.push_back(0x00ffffff);
					}
					// table[x] is now valid.
					assert((table[x] >> 24) == 0x01);
					x = (table[x] & 0xffffff) + i->regop;
					// 'x' is now the index of the correct entry in the regop table.
				}
				
				if(j->memtype != -1)
				{
					if(table[x] == 0x00ffffff)
					{
						table[x] = (0x02 << 24) + (U4)table.size();
						for(int h = 0; h < 4; ++h)
							table.push_back(0x00ffffff);
					}
					// table[x] is now valid.
					assert((table[x] >> 24) == 0x02);
					x = (table[x] & 0xffffff) + j->memtype;
					// 'x' is now the index of the correct entry in the memtype table.
				}
				
				if(k->mod != -1)
				{
					if(table[x] == 0x00ffffff)
					{
						table[x] = (0x03 << 24) + (U4)table.size();
						for(int h = 0; h < 8; ++h)
							table.push_back(0x00ffffff);
					}
					// table[x] is now valid.
					assert((table[x] >> 24) == 0x03);
					x = (table[x] & 0xffffff) + k->mod;
					// 'x' is now the index of the correct entry in the memtype table.
				}
				
				if(table[x] != 0x00ffffff)
					throw "duplicate opcode detected while generating table";
				assert((k->encoding & 0xff000000) == 0);
				table[x] = k->encoding;		// high 8 bits are 0
			}
		}
	}
}

void OpcodeTree::write_decoder_table_c(std::vector<U4> &table)
{
	std::ofstream outf("out_decoder_table.h");

	outf << "// out_decoder_table.h  (note: this is an automatically generated file - do not edit!)" << std::endl;
	outf << "// Copyright (C) 2008 Willow Schlanger" << std::endl;
	outf << std::endl;

	if(table.empty())
	{
		std::cout << "Nothing to do!" << std::endl;
		return;
	}
	
	int linecount = 0;
	
	for(std::vector<U4>::iterator i = table.begin(); i != table.end(); ++i)
	{
		if(i != table.begin())
			outf << ",";
		if(linecount == 10)
		{
			linecount = 0;
			outf << std::endl;
		}
		outf << std::hex << "0x" << *i << std::dec;
		++linecount;
	}
	
	outf << std::endl;
	outf << std::endl;
}

// The opcode table is as follows: low 24 bits: encoding number, or
// 0x00ffffff for invalid opcode. high 8 bits: 0 for encoding, or table number (1, 2, or 3).
void OpcodeTree::done()
{
	// Create a base table with 0x1200 (4608) entries, all invalid opcodes to begin with.
	// offset = basecode + 0x300 * op66 + 0x600 * repeat
	std::vector<U4> table(3 * 2 * 0x300, 0x00ffffff);

	// Go thru each basecode.
	for(int bc = 0; bc < 0x300; ++bc)
	{
		if(of[bc].empty())
			continue;	// leave this opcode invalid
		// at least one opcode needs to be claimed, here.
		const bool rep_all = (of[bc].front().repeat == 3);	// true if we claim all 3 rep states
		const bool op66_all = (of[bc].front().op66 == 2);	// true if we claim all 2 op66 states
		// note. if rep_all && one_all then there is only one entry, etc.

		for(OpcodeTree::of_t::iterator ii = of[bc].begin(); ii != of[bc].end(); ++ii)
		{
			for(unsigned h = 0; h < ii->x.size(); ++h)
			{
				int j_i, j_f, k_i, k_f;
				if(op66_all)
				{
					j_i = 0;
					j_f = 2;
				}
				else
				{
					j_i = ii->op66;
					j_f = j_i + 1;
				}
				if(rep_all)
				{
					k_i = 0;
					k_f = 3;
				}
				else
				{
					k_i = ii->repeat;
					k_f = k_i + 1;
				}
				for(int j = j_i; j < j_f; ++j)
				{
					for(int k = k_i; k < k_f; ++k)
					{
						const int claim_bc = bc + 0x300 * j + 0x600 * k;
						//void add_to_table(std::vector<U4> &table, int claim_bc, OpcodeFamily &f, U4 h);
						add_to_table(table, claim_bc, *ii, h);
					}
				}
			}
		}
	}
	
	write_decoder_table_c(table);
}

// ro = reg/op
// mt = mod
// md = r/m
// This time if there's an exact match that means there's an opcode conflict!
void OpcodeTree::do_accept(int k, struct OpcodeFamily &f, U4 ro, U4 mt, U4 md, U4 nextbyte)
{
	// Go thru all Family's.
	// - if ro > 7, demand the family's regop == -1.
	// - if ro <= 7, demand the family's regop >= 0

	U4 nextindex = nextbyte;

	if(nextbyte == 511)
	{
		nextindex = 0;
		if(f.x.empty())
		{
			f.x.push_back(XFamily());
		}
		else
		if(f.x.size() != 1)
			throw "invalid next byte (1)";
	}
	else
	if(nextbyte >= 0 && nextbyte <= 255)
	{
		if(f.x.empty())
		{
			for(int i = 0; i < 256; ++i)
				f.x.push_back(XFamily());
		}
		else
		if(f.x.size() != 256)
			throw "invalid next byte (3)";
	}
	else
		throw "invalid next byte (2)";

	std::list<Family>::iterator a = f.x[nextindex].f.end();
	
	for(std::list<Family>::iterator i = f.x[nextindex].f.begin(); i != f.x[nextindex].f.end(); ++i)
	{
		if(ro > 7)
		{
			if(i->regop != -1)
				throw "invalid regop";
		}
		else
		{
			if(i->regop == -1)
				throw "invalid regop";
		}

		if(i->regop == -1)
		{
			assert(a == f.x[nextindex].f.end());
			assert(ro > 7);
			a = i;
		}
		else
		{
			assert(ro <= 7);
			if(ro == i->regop)
			{
				assert(a == f.x[nextindex].f.end());
				a = i;
			}
		}
	}
	
	// 'a' could be f.f.end(), or it could point to a prev entry with same regop.
	if(a == f.x[nextindex].f.end())
	{
		f.x[nextindex].f.push_front(Family());
		f.x[nextindex].f.front().regop = (ro <= 7) ? (int)ro : (int)-1;
		a = f.x[nextindex].f.begin();
	}
	
	// 'a' is now valid!
	// Now look at a->mem, a list of FamilyMem's.
	
	std::list<FamilyMem>::iterator b = a->mem.end();

	assert(mt != mod_mem);
	for(std::list<FamilyMem>::iterator i = a->mem.begin(); i != a->mem.end(); ++i)
	{
		if(mt == mod_def)
		{
			if(i->memtype != -1)
				throw "invalid memtype";
		}
		else
		{
			if(i->memtype == -1)
				throw "invalid memtype";
		}
		
		if(i->memtype == -1)
		{
			assert(b == a->mem.end());
			assert(mt == mod_def);
			b = i;
		}
		else
		{
			assert(mt != mod_def);
			if(mt == i->memtype)
				b = i;
		}
	}

	if(b == a->mem.end())
	{
		a->mem.push_front(FamilyMem());
		a->mem.front().memtype = (mt == mod_def) ? (int)-1 : (int)mt;
		b = a->mem.begin();
	}
	
	// 'b' is now valid.
	// Finally, go thru b->mod.
	// This time make sure a match isn't found, if it is we have a duplicate opcode!
	
	for(std::list<FamilyMod>::iterator i = b->mod.begin(); i != b->mod.end(); ++i)
	{
		if(md == rm_def)
		{
			if(i->mod != -1)
				throw "invalid mod";
		}
		else
		{
			if(i->mod == -1)
				throw "invalid mod";
		}
	
		if(i->mod != -1)
		{
			if(md == i->mod)
			{
				throw "duplicate opcode";
			}
		}
		else
		{
			assert(md == rm_def);
			throw "duplicate opcode";
		}
	}
	
	// Now add it and we're done!
	b->mod.push_front(FamilyMod());
	b->mod.front().mod = (md == rm_def) ? (int)-1 : (int)md;
	b->mod.front().encoding = k;	// save the encoding!
}

void OpcodeTree::accept(int k, encoding_t *e, int basecode_ofs)
{
	of_t *a = of + e->basecode + basecode_ofs;
	const U4 nextbyte = e->nextbyte;	// 511 for none
	
	// Now go thru all 'a' and see if there's a contradiction.
	int repeat = 3;		// default (no table)
	int op66 = 2;		// default (no table)
	
	//if(e->replock == replock_nofx || e->replock == replock_nofxl)
	if(e->suffix.fx == fx_none || e->suffix.fx == fx_none_lockable)
		repeat = 0;
	else
	if(e->suffix.fx == fx_f2)
		repeat = 1;
	else
	if(e->suffix.fx == fx_f3)
		repeat = 2;
	
	if(e->suffix.op66 == op66_no66)
		op66 = 0;
	else
	if(e->suffix.op66 == op66_op66)
		op66 = 1;
	
	// Go thru all 'a' looking for a match. Also abort if error encountered.
	OpcodeTree::of_t::iterator found = a->end();		// not found
	for(OpcodeTree::of_t::iterator i = a->begin(); i != a->end(); ++i)
	{
		if(repeat == i->repeat && op66 == i->op66)
		{
			assert(found == a->end());
			found = i;
		}
		
		if(repeat == 3)
		{
			// New insn is def.
			// All other insns must be def too.
			if(i->repeat != 3)
				throw "replock conflict";
		}
		else
		{
			// New insn is f2 or f3 or nofx.
			// ALl other insns must be f2, f3, or nofx.
			if(i->repeat == 3)
				throw "replock conflict";
		}
		
		if(op66 == 2)
		{
			// New insn is def.
			// All other insns must be def too.
			if(i->op66 != 2)
				throw "op66 conflict";
		}
		else
		{
			// New insn is no66 or op66.
			// All other insns must not be no66 or op66.
			if(i->op66 == 2)
				throw "op66 conflict";
		}
	}
	
	// If not found, add it.
	if(found == a->end())
	{
		a->push_front(OpcodeFamily());
		a->front().repeat = repeat;
		a->front().op66 = op66;
		found = a->begin();
	}
	
	if(e->suffix.mod == mod_mem)
	{
		do_accept(k, *found, e->suffix.ro, 0, e->suffix.rm, e->nextbyte);
		do_accept(k, *found, e->suffix.ro, 1, e->suffix.rm, e->nextbyte);
		do_accept(k, *found, e->suffix.ro, 2, e->suffix.rm, e->nextbyte);
	}
	else
		do_accept(k, *found, e->suffix.ro, e->suffix.mod, e->suffix.rm, e->nextbyte);
}

void make_decoder()
{
	OpcodeTree t;

	int k = 0;
	for(encoding_t *e = encodings; e->insn != insn__count; ++e, ++k)
	{
		try
		{
			char mode = 'd';	// default
			for(int i = 0; i < MAX_ARGS; ++i)
			{
				if(get_argtype_lo(e->argtype[i]) == argtype_void)
					break;
				if(get_argtype_lo(e->argtype[i]) == argtype_imm &&
					get_argtype_hi(e->argtype[i]) == argtypehi_imm_cc
				)
				{
					mode = 'c';	// condition codes
					break;
				}
				else
				if(get_argtype_lo(e->argtype[i]) == argtype_reg &&
					get_argtype_hi(e->argtype[i]) == argtypehi_reg_basecode
				)
				{
					mode = 'r';	// reg in base code
					break;
				}
			}
			
			int count = 1;
			
			if(mode == 'c')
				count = 16;
			else
			if(mode == 'r')
			{
				count = 8;
				//90..97 are valid in 64 bit mode; one can exchange with e.g. r8 by using rex.
			}
		
			for(int i = 0; i < count; ++i)
				t.accept(k, e, i);
		}
		catch(const char *s)
		{
			std::cout << "error: basecode=" << std::hex << e->basecode << std::dec << ": " << s << std::endl;
			return;
		}
	}
	
	t.done();
}

}	// namespace x86s

using namespace x86s;

int main()
{
	make_decoder();
	return 0;
}
