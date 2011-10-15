// ixemulator.c
// Copyright (C) 2011 Willow Schlanger. All rights reserved.

// If GUEST64ENABLED is defined at compile time, i.e. via gcc -DGUEST64ENABLED -c ixemulator.c
// then we will allow the guest to enter 64bit mode. This requires the 64bit nasm muldiv64.asm
// file to be linked in, for large multiplication/division routines.

// Define PORTE9HACK to get port writes to 0xe9 to print to stdout.

#include "../common/include/types.h"
#include "ixdecoder.h"
#include <stddef.h>
#include <string.h>

enum
{
	CRUDASM_CPU_CACHE_SIZE_PAGES = 64,
	CRUDASM_XMM_SIZE_QWORDS = 2,
	CRUDASM_FPR_SIZE_QWORDS = 2,				// we support floating-point registers of up to 80 bits
	CRUDASM_MAX_ARG_SIZE_QWORDS = 2,
	CRUDASM_INTEL_MAX_ASM_ARGS = 4
};

struct crudasm_intel_descriptor_t
{
	U8 base;
	U8 limit;
	U1 readable : 1;
	U1 writable : 1;
	U1 : 6;
};

struct crudasm_basic_cpu_state_t
{
	U8 rflags;
	U8 gregs[16];
	U8 cregs[16];
	U8 dregs[16];
	U8 xmm[CRUDASM_XMM_SIZE_QWORDS * 16];
	U8 fpr[CRUDASM_FPR_SIZE_QWORDS * 8];		// 0..5=corresponding seg regs, 6=tr, 7=ldtr
	U2 sregs[8];	// 6=task register, 7=ldt register
	struct crudasm_intel_descriptor_t sreg_cache[8];
};

// memset() is used to zero this out prior to the execution of each instruction.
struct crudasm_basic_cpu_mask_t
{
	U8 rflags;
	U2 gregs;
	U2 cregs;
	U2 dregs;
	U2 xmm;
	U2 sreg_cache;					// 0..5=corresponding seg regs, 6=tr, 7=ldtr
	U1 fpr;
	U1 sregs;
};

struct crudasm_memory_cache_entry_t
{
	U8 phys_base;		// bits 11..0 = 0
	U2 dirty_size_bytes;
	U2 dirty_page_offset;
};

struct crudasm_memory_cache_t
{
	U1 cache[CRUDASM_CPU_CACHE_SIZE_PAGES * 4096];
	U4 cache_size;		// number of entries in use
	struct crudasm_memory_cache_entry_t entry[CRUDASM_CPU_CACHE_SIZE_PAGES];
};

struct crudasm_temp_node_t
{
	U8 value[CRUDASM_MAX_ARG_SIZE_QWORDS];
	U1 size;			// in bytes
};

struct crudasm_mem_arg_cache_t
{
	struct crudasm_temp_node_t node;	// value & size
	// linear is set by crudasm_intel_context_pre_exec() for ALL memory arguments.
	// It stores the offset of the EA.
	U8 linear;
	U8 phys[2];
	// If is_valid is 0, we have not yet accessed this argument OR the argument
	// is not eligable for memory caching. Prior to executing an instruction, all
	// the is_valid's are cleared to 0. This means the other fields are undefined
	// except for 'linear'.
	U1 is_valid : 1;
	// Need to do a write back when done executing the instruction (assuming success)?
	U1 need_write : 1;
};

// Objects of this type should be dynamically allocated because it is quite large.
struct crudasm_intel_context_t
{
	void *user_context;
	const char *error_desc;			// NULL on success
	struct crudasm_memory_cache_t mem_write_cache;
	struct crudasm_basic_cpu_mask_t basic_mask;
	struct crudasm_basic_cpu_state_t basic_state_cur;
	struct crudasm_basic_cpu_state_t basic_state_next;
	struct ix_icode_t icode;
	struct crudasm_mem_arg_cache_t mem_arg_cache[CRUDASM_INTEL_MAX_ASM_ARGS];
	
	// Really read from physical memory. size_bytes is 0..4096 inclusive and is such that we do not span two
	// 4KB pages, but only one physical 4kb page need be accessed. If the page in question is RAM, this just
	// reads from RAM. We can also read from ROM, memory-mapped I/O devices, or adaptor space (this includes
	// physical pages that are special such as video memory; internal CPU memory such as for the APICs, and
	// pages that are not present, which do NOT generate an error but return all 0xff's).
	// This should only return an error in case of an internal error, such as the case where multiple pages
	// are spanned. If size_bytes is 0, this does nothing and returns success (0).
	int (*do_read_physical)(struct crudasm_intel_context_t *context, U8 phys_addr, U2 size_bytes, void *dest);
	int (*do_write_physical)(struct crudasm_intel_context_t *context, U8 phys_addr, U2 size_bytes, void *src);

	int (*do_read_port)(struct crudasm_intel_context_t *context, U2 port, U1 size_bytes, U8 *result);
	int (*do_write_port)(struct crudasm_intel_context_t *context, U2 port, U1 size_bytes, U8 value);

	U8 rip, rip_next, rip_add;
	
	U8 cpu_except_errcode;
	U8 cpu_except_cr2;
	U8 rel;							// used by crudasm_read_arg() for 64-bit EAs
	U1 cpu_except_num : 5;
	U1 cpu_except_required : 1;
	U1 cpu_except_use_cr2 : 1;
	U1 cpu_soft_intr_required : 1;	// ignored if cpu_except_required != 0
	U1 cpu_soft_intr;				// valid only if cpu_soft_intr_required = 0 && cpu_except_required == 0
	U1 sim_abort : 1;				// set to 1 if we have to abort the simulation due to an internal error
	U1 dsz : 2;						// 0=16bit mode, 1=32bit mode, 2=64bit mode
	U1 seg_descr_limit_check : 1;	// if 1, check segment limits. if dsz == 2 this must be 0.
	U1 seg_max_limit_check : 1;		// if 1, check for wrap around, i.e. mov ax,[-1] should fail.
	U1 cpl : 2;						// in real mode this is 0. in vm86 mode it is 3. shortly after the switch
									// to protected mode from real mode, before CS is reloaded, it is also 0.
									// after CS has been loaded, it corresponds to the low 3 bits of the CS
									// selector at all times, until we switch to vm86 or real mode again.
	U1 ssz : 2;						// stack-address size, i.e. 0=use SP, 1=use ESP, 2=use RSP
};

//===============================================================================================//

enum { rAX = 0, rCX, rDX, rBX, rSP, rBP, rSI, rDI };
enum { rES = 0, rCS, rSS, rDS, rFS, rGS };

// sreg_value = 0..5 for corresponding segment register, or 6 for tr, 7 for ldtr.
static struct crudasm_intel_descriptor_t *get_cached_descriptor(struct crudasm_intel_context_t *context, U1 sreg_value)
{
	struct crudasm_intel_descriptor_t *cache = ((context->basic_mask.sreg_cache >> sreg_value) & 1) ? context->basic_state_next.sreg_cache : context->basic_state_cur.sreg_cache;
	return cache + (size_t)(sreg_value);
}

static void do_abort(struct crudasm_intel_context_t *context, const char *msg)
{
	context->error_desc = msg;
	context->sim_abort = 1;
}

static U8 read_rflags_all(struct crudasm_basic_cpu_state_t *cur, struct crudasm_basic_cpu_state_t *next, struct crudasm_basic_cpu_mask_t *mask)
{
	return (mask->rflags == 0) ? cur->rflags : next->rflags;
}

static U8 read_creg_value(struct crudasm_basic_cpu_state_t *cur, struct crudasm_basic_cpu_state_t *next, struct crudasm_basic_cpu_mask_t *mask, U1 index)
{
	if(index >= 16)
		return 0;
	return ((mask->cregs >> index) & 1) ? next->cregs[(size_t)(index)] : cur->cregs[(size_t)(index)];
}

static int really_write_seg_reg(struct crudasm_intel_context_t *context, U1 seg_reg, U2 value)
{
	U1 rvm = 1;  /* real mode */
	struct crudasm_intel_descriptor_t old_descr = *get_cached_descriptor(context, seg_reg);
	struct crudasm_intel_descriptor_t *pdescr = context->basic_state_next.sreg_cache + (size_t)(seg_reg);

	context->basic_mask.sreg_cache |= 1 << seg_reg;
	*pdescr = old_descr;

	if(read_creg_value(&context->basic_state_cur, &context->basic_state_next, &context->basic_mask, 0 /* cr0 */) & 1 /* protect-enable bit */)
		rvm = ((read_rflags_all(&context->basic_state_cur, &context->basic_state_next, &context->basic_mask) >> 17) & 1) != 0;  /* vm86 mode? */
	
	if(rvm)
	{
		pdescr->base = (U8)(value) << 4;
		return 0;
	}
	
	do_abort(context, "An attempt was made to alter a segment register in protected mode (not supported yet!)");
	return 1;
}

// Normally returns 0. Returns 1 if we have to repeat the instruction that must be repeated.
static int crudasm_intel_context_post_exec(struct crudasm_intel_context_t *context)
{
	size_t i, j;
	U8 a, b;
	struct crudasm_intel_encoding_t *encoding = crudasm_intel_encoding_table + context->icode.encoding;

	// FIXME, check for software interrupts here, i.e. INT n / INTO may trigger a change of CS:RIP.
	
	if(context->basic_mask.sregs != 0)
	{
		// Deal with segment register writes here.
		for(i = 0; i < 6; ++i)
		{
			if((context->basic_mask.sregs >> i) & 1)
			{
				if(really_write_seg_reg(context, i, context->basic_state_next.sregs[i]) != 0)
					return 0;		// exception generated
			}
		}

		for(i = 0; i < 6; ++i)
		{
			if((context->basic_mask.sregs >> i) & 1)
			{
				context->basic_state_cur.sregs[i] = context->basic_state_next.sregs[i];
			}
		}
	
		// FIXME, deal with writes to TR / LDTR here.
	}
	
	if(context->basic_mask.rflags != 0)
	{
		// FIXME, don't allow reserved or otherwise fixed rflags to be changed.
		context->basic_state_cur.rflags = context->basic_state_next.rflags;
	}
	if(context->basic_mask.gregs != 0)
	{
		for(i = 0; i < 16; ++i)
		{
			if((context->basic_mask.gregs >> i) & 1)
				context->basic_state_cur.gregs[i] = context->basic_state_next.gregs[i];
		}
	}
	if(context->basic_mask.cregs != 0)
	{
		for(i = 0; i < 16; ++i)
		{
			// FIXME, don't allow user to change "reserved" cregs.
			if((context->basic_mask.cregs >> i) & 1)
				context->basic_state_cur.cregs[i] = context->basic_state_next.cregs[i];
		}
	}
	if(context->basic_mask.dregs != 0)
	{
		for(i = 0; i < 16; ++i)
		{
			// FIXME, don't allow user to change "reserved" dregs.
			if((context->basic_mask.dregs >> i) & 1)
				context->basic_state_cur.dregs[i] = context->basic_state_next.dregs[i];
		}
	}
	if(context->basic_mask.xmm != 0)
	{
		for(i = 0; i < 16; ++i)
		{
			if((context->basic_mask.xmm >> i) & 1)
			{
				for(j = 0; j < CRUDASM_XMM_SIZE_QWORDS; ++j)
					context->basic_state_cur.xmm[CRUDASM_XMM_SIZE_QWORDS*i+j] = context->basic_state_next.xmm[CRUDASM_XMM_SIZE_QWORDS*i+j];
			}
		}
	}
	if(context->basic_mask.sreg_cache)
	{
		for(i = 0; i < 8; ++i)
		{
			if((context->basic_mask.sreg_cache >> i) & 1)
				context->basic_state_cur.sreg_cache[i] = context->basic_state_next.sreg_cache[i];
		}
	}
	if(context->basic_mask.fpr != 0)
	{
		for(i = 0; i < 8; ++i)
		{
			if((context->basic_mask.fpr >> i) & 1)
			{
				for(j = 0; j < CRUDASM_FPR_SIZE_QWORDS; ++j)
					context->basic_state_cur.fpr[CRUDASM_FPR_SIZE_QWORDS*i+j] = context->basic_state_next.fpr[CRUDASM_FPR_SIZE_QWORDS*i+j];
			}
		}
	}
	
	for(i = 0; i < context->mem_write_cache.cache_size; ++i)
	{
		if(context->do_write_physical(context, (U8)(context->mem_write_cache.entry[i].phys_base + context->mem_write_cache.entry[i].dirty_page_offset),
			context->mem_write_cache.entry[i].dirty_size_bytes,
			context->mem_write_cache.cache + 4096 * i + context->mem_write_cache.entry[i].dirty_page_offset
		) != 0)
		{
			do_abort(context, "Unable to write to physical memory.");
			return 0;
		}
	}
	for(i = 0; i <  CRUDASM_INTEL_MAX_ASM_ARGS; ++i)
	{
		if(context->mem_arg_cache[i].is_valid && context->mem_arg_cache[i].need_write)
		{
			a = context->mem_arg_cache[i].phys[0];
			b = a + context->mem_arg_cache[i].node.size - 1;
			if((a & ~TULL(0xfff)) == (b & ~TULL(0xfff)))
			{
				if(context->do_write_physical(context, a, context->mem_arg_cache[i].node.size, context->mem_arg_cache[i].node.value) != 0)
				{
					do_abort(context, "Unable to write to physical memory.");
					return 0;
				}
			}
			else
			{
				b = 0x1000 - (a & ~TULL(0xfff));
				if(context->do_write_physical(context, a, b, context->mem_arg_cache[i].node.value) != 0)
				{
					do_abort(context, "Unable to write to physical memory.");
					return 0;
				}
				if(context->do_write_physical(context, context->mem_arg_cache[i].phys[1], context->mem_arg_cache[i].node.size - b, (U1 *)(context->mem_arg_cache[i].node.value) + b) != 0)
				{
					do_abort(context, "Unable to write to physical memory.");
					return 0;
				}
			}
		}
	}

	// Check for repeat condition.
	if(context->icode.lockrep != 2 && context->icode.lockrep != 3)
		return 0;
	if(crudasm_intel_insns[(size_t)(encoding->insn)].flag_repable || crudasm_intel_insns[(size_t)(encoding->insn)].flag_repcond)
	{
		if(context->icode.asz == 0)
		{
			--*(U2 *)(&context->basic_state_cur.gregs[rCX]);
			if((U2)(context->basic_state_cur.gregs[rCX]) == 0)
			{
				return 0;
			}
		}
		else
		if(context->icode.asz == 1)
		{
			--*(U4 *)(&context->basic_state_cur.gregs[rCX]);
			if((U4)(context->basic_state_cur.gregs[rCX]) == 0)
				return 0;
		}
		else
		{
			--*(U8 *)(&context->basic_state_cur.gregs[rCX]);
			if(context->basic_state_cur.gregs[rCX] == 0)
				return 0;
		}
		// rCX has been decreased and is still nonzero.
		if(crudasm_intel_insns[(size_t)(encoding->insn)].flag_repable)
			return 1;	// plain rep -- so repeat!
		
		if(context->icode.lockrep == 2)
		{
			// repnz
			if(((context->basic_state_cur.rflags >> 6/*ZF*/) & 1) == 0)
				return 1;
		}
		else
		{
			// repz
			if(((context->basic_state_cur.rflags >> 6/*ZF*/) & 1) != 0)
				return 1;
		}
		
		return 0;		// repcond; cond not met
	}
	
	return 0;
}

static int default_read_physical(struct crudasm_intel_context_t *context, U8 phys_addr, U2 size_bytes, void *destT)
{
	U1 *dest = (U1 *)(dest);
	size_t i;
	for(i = 0; i < size_bytes; ++i)
		dest[i] = 0xff;
	return 0;
}

static int default_write_physical(struct crudasm_intel_context_t *context, U8 phys_addr, U2 size_bytes, void *srcT)
{
	return 0;
}

static void write_rflags_all(struct crudasm_basic_cpu_state_t *cur, struct crudasm_basic_cpu_state_t *next, struct crudasm_basic_cpu_mask_t *mask, U8 value)
{
	next->rflags = value;
	mask->rflags |= TULL(0xffffffffffffffff);
}

// Workaround for bad 64bit compiler shifts (certain versions of GCC).
static U8 SHL64(U8 src, U1 value)
{
	value &= 63;
	if(value == 0)
		return src;
	if(value < 32)
		return src << value;
	src <<= 16;
	src <<= 16;
	return src << (value - 32);
}

// Workaround for bad 64bit compiler shifts (certain versions of GCC).
static U8 SHR64U(U8 src, U1 value)
{
	value &= 63;
	if(value == 0)
		return src;
	if(value < 32)
		return src >> value;
	src >>= 16;
	src >>= 16;
	return src >> (value - 32);
}

static S8 SHR64S(S8 src, U1 value)
{
	value &= 63;
	if(value == 0)
		return src;
	if(value < 32)
		return src >> value;
	src >>= 16;
	src >>= 16;
	return src >> (value - 32);
}

static void write_rflag(struct crudasm_basic_cpu_state_t *cur, struct crudasm_basic_cpu_state_t *next, struct crudasm_basic_cpu_mask_t *mask, U1 shift, U1 value)
{
	U8 m;
	if(shift <= 63)
	{
		if(mask->rflags == 0)
			next->rflags = cur->rflags;
		m = SHL64(1, shift);
		mask->rflags |= m;
		if(value == 0)
			next->rflags &= ~m;
		else
			next->rflags |= m;
	}
}

static void read_xmm_value(struct crudasm_basic_cpu_state_t *cur, struct crudasm_basic_cpu_state_t *next, struct crudasm_basic_cpu_mask_t *mask, U1 index, U8 *result)
{
	size_t i;
	U8 *src = ((mask->xmm >> index) & 1) ? (next->xmm + CRUDASM_XMM_SIZE_QWORDS * (size_t)(index)) : (cur->xmm + CRUDASM_XMM_SIZE_QWORDS * (size_t)(index));
	for(i = 0; i < CRUDASM_XMM_SIZE_QWORDS; ++i)
		result[i] = src[i];
}

static void write_xmm_value(struct crudasm_basic_cpu_state_t *cur, struct crudasm_basic_cpu_state_t *next, struct crudasm_basic_cpu_mask_t *mask, U1 index, U8 *src)
{
	size_t i;
	U8 *result = next->xmm + CRUDASM_XMM_SIZE_QWORDS * (size_t)(index);
	mask->xmm |= SHL64(1, index);
	for(i = 0; i < CRUDASM_XMM_SIZE_QWORDS; ++i)
		result[i] = src[i];
}

static void read_mmx_value(struct crudasm_basic_cpu_state_t *cur, struct crudasm_basic_cpu_state_t *next, struct crudasm_basic_cpu_mask_t *mask, U1 index, U8 *result)
{
	// MMX registers are always exactly 8 bytes (64 bits).
	*result = ((mask->fpr >> index) & 1) ? next->fpr[(size_t)(index) * CRUDASM_FPR_SIZE_QWORDS] : cur->fpr[(size_t)(index) * CRUDASM_FPR_SIZE_QWORDS];
}

static void write_mmx_value(struct crudasm_basic_cpu_state_t *cur, struct crudasm_basic_cpu_state_t *next, struct crudasm_basic_cpu_mask_t *mask, U1 index, U8 value)
{
	// MMX registers are always exactly 8 bytes (64 bits).
	mask->fpr |= SHL64(1, index);
	next->fpr[(size_t)(index) * CRUDASM_FPR_SIZE_QWORDS] = value;
}

static U1 read_rflag(struct crudasm_basic_cpu_state_t *cur, struct crudasm_basic_cpu_state_t *next, struct crudasm_basic_cpu_mask_t *mask, U1 shift)
{
	U8 value = (mask->rflags == 0) ? cur->rflags : next->rflags;
	if(shift > 63)
		return 0;
	return SHR64U(value, shift) & 1;
}

// Note. After executing an instruction but before comitting it, we check for dirtied sregs, and we adjust cached information accordingly. At this time we may
// set the segment's accessed bit, etc.
static void write_sreg_value(struct crudasm_basic_cpu_state_t *cur, struct crudasm_basic_cpu_state_t *next, struct crudasm_basic_cpu_mask_t *mask, U1 index, U2 value)
{
	if(index < 8)
	{
		mask->sregs |= 1 << index;
		next->sregs[(size_t)(index)] = value;
	}
}

static U2 read_sreg_value(struct crudasm_basic_cpu_state_t *cur, struct crudasm_basic_cpu_state_t *next, struct crudasm_basic_cpu_mask_t *mask, U1 index)
{
	if(index >= 8)
		return 0;
	return ((mask->sregs >> index) & 1) ? next->sregs[(size_t)(index)] : cur->sregs[(size_t)(index)];
}

static void write_creg_value(struct crudasm_basic_cpu_state_t *cur, struct crudasm_basic_cpu_state_t *next, struct crudasm_basic_cpu_mask_t *mask, U1 index, U8 value)
{
	if(index < 16)
	{
		mask->cregs |= 1 << index;
		next->cregs[(size_t)(index)] = value;
	}
}

static void write_dreg_value(struct crudasm_basic_cpu_state_t *cur, struct crudasm_basic_cpu_state_t *next, struct crudasm_basic_cpu_mask_t *mask, U1 index, U8 value)
{
	if(index < 16)
	{
		mask->dregs |= 1 << index;
		next->dregs[(size_t)(index)] = value;
	}
}

static U8 read_dreg_value(struct crudasm_basic_cpu_state_t *cur, struct crudasm_basic_cpu_state_t *next, struct crudasm_basic_cpu_mask_t *mask, U1 index)
{
	if(index >= 16)
		return 0;
	return ((mask->dregs >> index) & 1) ? next->dregs[(size_t)(index)] : cur->dregs[(size_t)(index)];
}

static U8 read_gr_value(struct crudasm_basic_cpu_state_t *cur, struct crudasm_basic_cpu_state_t *next, struct crudasm_basic_cpu_mask_t *mask, U1 index, U1 size_bytes, U1 shift)
{
	U8 *src = ((mask->gregs >> index) & 1) ? next->gregs : cur->gregs;
	U8 value = src[(size_t)(index)] >> shift;
	if(size_bytes == 1)
		value = (U1)(value);
	else
	if(size_bytes == 2)
		value = (U2)(value);
	else
	if(size_bytes == 4)
		value = (U4)(value);
	return value;
}

void write_gr_value(struct crudasm_basic_cpu_state_t *cur, struct crudasm_basic_cpu_state_t *next, struct crudasm_basic_cpu_mask_t *mask, U1 index, U1 size_bytes, U1 shift, U8 value)
{
	U8 *preg = next->gregs + (size_t)(index);
	if(((mask->gregs >> index) & 1) == 0)
	{
		next->gregs[(size_t)(index)] = cur->gregs[(size_t)(index)];
		mask->gregs |= (1 << index);
	}
	if(size_bytes == 1)
		((U1 *)(preg))[(shift != 0) ? 1 : 0] = value;
	else
	if(size_bytes == 2)
		*(U2 *)(preg) = value;
	else
	if(size_bytes == 4)
		*(U4 *)(preg) = value;
	else
	if(size_bytes == 8)
		*preg = value;
}

// This is intended to be used by the emulator when it needs to write to memory. Rather than really writing to memory, the memory write is cached,
// so we can commit it later after the instruction succeeds without any need to roll back instructions. The cache is comitted upon success. After
// each instruction the cache must be cleared, in order to for example allow for memory reads from I/O mapped hardware to work normally.
int cache_write_physical(struct crudasm_intel_context_t *context, U8 phys_addr, U1 size, void *src)
{
	size_t index, i;

	if(size == 0)
		return 0;	// nothing to do (success)

	if((phys_addr & ~TULL(0xfff)) != ((phys_addr + size - 1) & ~TULL(0xfff)))
	{
		do_abort(context, "Internal error writing to memory cache.");
		return 1;
	}
	
	for(i = 0; i < context->mem_write_cache.cache_size; ++i)
	{
		if((phys_addr & ~TULL(0xfff)) == context->mem_write_cache.entry[i].phys_base)
		{
			// Already in the cache.
			memcpy(context->mem_write_cache.cache + 4096 * i + (phys_addr & 0xfff), src, size);
			
			if(context->mem_write_cache.entry[i].dirty_size_bytes == 0)
			{
				context->mem_write_cache.entry[i].dirty_size_bytes = size;
				context->mem_write_cache.entry[i].dirty_page_offset = phys_addr & 0xfff;
			}
			else
			{			
				index = context->mem_write_cache.entry[i].dirty_page_offset + context->mem_write_cache.entry[i].dirty_size_bytes;
				if((phys_addr & 0xfff) < context->mem_write_cache.entry[i].dirty_page_offset)
					context->mem_write_cache.entry[i].dirty_page_offset = phys_addr & 0xfff;
				if((phys_addr & 0xfff) + (size_t)(size) > index)
					index = (phys_addr & 0xfff) + (size_t)(size);
				context->mem_write_cache.entry[i].dirty_size_bytes = index - context->mem_write_cache.entry[i].dirty_page_offset;
			}
			
			return 0;
		}
	}
	
	if(context->mem_write_cache.cache_size == CRUDASM_CPU_CACHE_SIZE_PAGES)
	{
		do_abort(context, "The cache is full -- must increase CRUDASM_CPU_CACHE_SIZE_PAGES.");
		return 1;
	}
	
	index = context->mem_write_cache.cache_size;
	++context->mem_write_cache.cache_size;

	if(context->do_read_physical(context, phys_addr & ~TULL(0xfff), 0x1000, context->mem_write_cache.cache + 4096 * index) != 0)
		return 1;
	context->mem_write_cache.entry[index].phys_base = phys_addr & ~TULL(0xfff);
	context->mem_write_cache.entry[index].dirty_size_bytes = size;
	context->mem_write_cache.entry[index].dirty_page_offset = phys_addr & 0xfff;

	return 0;
}

// in: linear_page_aligned must be aligned on a 4kb boundery. cpl must be 0..3 inclusive. phys_page_out must be not null.
static int translate_linear(struct crudasm_intel_context_t *context, U8 linear_page_aligned, U1 cpl, U8 *phys_page_out, U1 rw /* bit 0 = read, bit 1 = write, bit 2 = don't fault */)
{
	U8 cr0, cr3, pdbr, tmp;
	U4 pte32, mask32;
	int need_write = 0;

	*phys_page_out = linear_page_aligned;
	
	cr0 = read_creg_value(&context->basic_state_cur, &context->basic_state_next, &context->basic_mask, 0 /* cr0 */);
	if((cr0 & 1) == 0)
		return 0;	// paging hardware is disabled in real mode
	
	if((rw & 2) != 0)
	{
		// We are attempting a memory WRITE linear translation.
		if(cpl == 3)
			need_write = 1;		// need write access
		else
		{
			if(cr0 & (1 << 16))
				need_write = 1;		// write-protect bit is set, disallow writes to read-only memory even in cpl < 3
		}
	}
	
	if(cr0 & (1 << 31))
	{
		// cr0.pg == 1 && cr0.pe == 1 (paging hardware is enabled).
		cr3 = read_creg_value(&context->basic_state_cur, &context->basic_state_next, &context->basic_mask, 3 /* cr3 */);
		pdbr = cr3 & ~TULL(0xfff);
		
		// FIXME, implement 3-level paging, e.g. PAE & 64bit paging support here.
		
		tmp = (linear_page_aligned >> 22) << 2;
		if(context->do_read_physical(context, pdbr + tmp, 4, &pte32))
			return 1;
		if((pte32 & 1) == 0)
		{
			if(rw & 4)
				return 1;
			// FIXME, generate exception here instead of abort.
			do_abort(context, "Page fault: page not present!");
			return 1;
		}
		if(cpl == 3 && (pte32 & 4) != 0)
		{
			if(rw & 4)
				return 1;
			// FIXME, generate exception here instead of abort.
			do_abort(context, "Page fault: attempt to access a system page from user mode!");
			return 1;
		}
		if(need_write && (rw & 2) != 0)
		{
			if(rw & 4)
				return 1;
			// FIXME, generate exception here instead of abort.
			do_abort(context, "Page fault: attempt to write to a read-only page!");
			return 1;
		}
		
		mask32 = 0;
		if((pte32 & (1 << 5)) == 0)
		{
			// Accessed bit needs to be set.
			mask32 |= (1 << 5);
		}
		if((rw & 2) != 0 && (pte32 & (1 << 6)) == 0)
		{
			// Are we sure we're supposed to set the dirty flag even for PDEs?
			// I am not so sure, but we do it here anyways... need to look it up.
			mask32 |= (1 << 6);
		}
		if(mask32 != 0)
		{
			pte32 |= mask32;
			if(cache_write_physical(context, pdbr + tmp, 4, &pte32))
				return 1;
		}
		
		// Ok, so far so good.
		pdbr = pte32 & ~TULL(0xfff);
		tmp = ((linear_page_aligned >> 12) & 1023) << 2;
		if(context->do_read_physical(context, pdbr + tmp, 4, &pte32))
			return 1;
		if((pte32 & 1) == 0)
		{
			if(rw & 4)
				return 1;
			// FIXME, generate exception here instead of abort.
			do_abort(context, "Page fault: page not present! (level 2)");
			return 1;
		}
		if(cpl == 3 && (pte32 & 4) != 0)
		{
			if(rw & 4)
				return 1;
			// FIXME, generate exception here instead of abort.
			do_abort(context, "Page fault: attempt to access a system page from user mode! (level 2)");
			return 1;
		}
		if(need_write && (rw & 2) != 0)
		{
			if(rw & 4)
				return 1;
			// FIXME, generate exception here instead of abort.
			do_abort(context, "Page fault: attempt to write to a read-only page! (level 2)");
			return 1;
		}
		
		mask32 = 0;
		if((pte32 & (1 << 5)) == 0)
		{
			// Accessed bit needs to be set.
			mask32 |= (1 << 5);
		}
		if((rw & 2) != 0 && (pte32 & (1 << 6)) == 0)
		{
			// Set the dirty bit (we're doing a write, and that bit was clear).
			mask32 |= (1 << 6);
		}
		if(mask32 != 0)
		{
			pte32 |= mask32;
			if(cache_write_physical(context, pdbr + tmp, 4, &pte32))
				return 1;
		}
		
		// Now we have the correct physical address.
		pdbr = pte32 & ~TULL(0xfff);
		*phys_page_out = pdbr;
	}
	
	return 0;
}

// size_bytes must be such that phys_addr does not exceed the physical 4kb page it is in.
// size_bytes must be 1 .. 8 inclusive.
static int read_physical(struct crudasm_intel_context_t *context, U8 phys_addr, U1 size_bytes, U1 *dest, int need_cache)
{
	// Check for a cache hit first. This is important for special memory, such as video adaptor space;
	// we don't want an instruction like add [bx],al in adaptor space to really read from that memory
	// too many times, it could be memory-mapped I/O hardware space !
	U8 page_base = phys_addr & ~TULL(0xfff);
	size_t i;
	
	for(i = 0; i < context->mem_write_cache.cache_size; ++i)
	{
		if(page_base == context->mem_write_cache.entry[i].phys_base)
		{
			memcpy(dest, context->mem_write_cache.cache + 4096 * i + (phys_addr & 0xfff), size_bytes);
			return 0;
		}
	}
	
	if(need_cache)
	{
		// Not in cache, but we want to put it in there.
		if(context->mem_write_cache.cache_size == CRUDASM_CPU_CACHE_SIZE_PAGES)
		{
			do_abort(context, "Unable to read physical memory: the cache is full! Increase CRUDASM_CPU_CACHE_SIZE_PAGES.");
			return 1;
		}
		i = context->mem_write_cache.cache_size;
		++context->mem_write_cache.cache_size;
		
		if(context->do_read_physical(context, page_base, 0x1000, context->mem_write_cache.cache + 4096 * i) != 0)
			return 1;
		context->mem_write_cache.entry[i].phys_base = page_base;
		context->mem_write_cache.entry[i].dirty_size_bytes = 0;		// nothing is dirty in this cache entry !
		context->mem_write_cache.entry[i].dirty_page_offset = 0;	// set to a known value

		memcpy(dest, context->mem_write_cache.cache + 4096 * i + (phys_addr & 0xfff), size_bytes);
		return 0;
	}
	
	// Not in cache. Leave it that way.
	return context->do_read_physical(context, phys_addr, size_bytes, dest);
}

// size_bytes must be exactly 1, 2, 4, or 8. cpl must be 0, 1, 2, or 3. The access need not be aligned.
// *** Actually it's OK for size_bytes to be up to 4096.
static int cache_write_linear(struct crudasm_intel_context_t *context, U8 linear, void *destT, U1 size_bytes, U1 cpl)
{
	U1 *dest = (U1 *)(destT);
	U8 first_page = (linear) & ~TULL(0xfff);
	U8 second_page = (linear + (size_t)(size_bytes - 1)) & ~TULL(0xfff);
	U8 first_phys, second_phys;
	if(first_page == second_page)
	{
		if(translate_linear(context, first_page, cpl, &first_phys, 2/*write only*/) != 0)
			return 1;
		if(cache_write_physical(context, first_phys + (linear & 0xfff), size_bytes, dest) != 0)
			return 1;
	}
	else
	{
		if(translate_linear(context, first_page, cpl, &first_phys, 2/*write only*/) != 0)
			return 1;
		if(translate_linear(context, second_page, cpl, &second_phys, 2/*write only*/) != 0)
			return 1;
		if(cache_write_physical(context, first_phys + (linear & 0xfff), 0x1000 - (linear & 0xfff), dest) != 0)
			return 1;
		if(cache_write_physical(context, second_phys, size_bytes - (0x1000 - (linear & 0xfff)), dest + (0x1000 - (linear & 0xfff))) != 0)
			return 1;
	}
	return 0;
}

// size_bytes may be (for example) 1, 2, 4, or 8. cpl must be 0, 1, 2, or 3. The access need not be aligned.
// size_bytes must be between 0 and 4096 bytes in size.
static int read_linear(struct crudasm_intel_context_t *context, U8 linear, void *destT, U1 size_bytes, U1 cpl, int need_cache)	
{
	U1 *dest = (U1 *)(destT);
	U8 first_page = (linear) & ~TULL(0xfff);
	U8 second_page = (linear + (size_t)(size_bytes - 1)) & ~TULL(0xfff);
	U8 first_phys, second_phys;
	if(first_page == second_page)
	{
		if(translate_linear(context, first_page, cpl, &first_phys, 1/*read only*/) != 0)
			return 1;
		if(read_physical(context, first_phys + (linear & 0xfff), size_bytes, dest, need_cache) != 0)
			return 1;
	}
	else
	{
		if(translate_linear(context, first_page, cpl, &first_phys, 1/*read only*/) != 0)
			return 1;
		if(translate_linear(context, second_page, cpl, &second_phys, 1/*read only*/) != 0)
			return 1;
		if(read_physical(context, first_phys + (linear & 0xfff), 0x1000 - (linear & 0xfff), dest, need_cache) != 0)
			return 1;
		if(read_physical(context, second_phys, size_bytes - (0x1000 - (linear & 0xfff)), dest + (0x1000 - (linear & 0xfff)), need_cache) != 0)
			return 1;
	}
	return 0;
}

static int access_cached_mem_arg(struct crudasm_intel_context_t *context, size_t arg_num, U1 write_mode, U1 sreg_value, size_t size_bytes, U8 *pOffsetLinear)
{
	U8 temp;
	U8 offset = context->mem_arg_cache[arg_num].linear;
	U2 selector = read_sreg_value(&context->basic_state_cur, &context->basic_state_next, &context->basic_mask, sreg_value);
	struct crudasm_intel_descriptor_t *descr = get_cached_descriptor(context, sreg_value);
	U1 rvm;
	
	if(size_bytes == 0)
		return 0;	// nothing to do - return success
	
	if(context->icode.argtype[arg_num] == crudasm_intel_argtype_mem_implict_std)
		sreg_value = rES;

	if(read_creg_value(&context->basic_state_cur, &context->basic_state_next, &context->basic_mask, 0 /* cr0 */) & 1 /* protect-enable bit */)
		rvm = ((read_rflags_all(&context->basic_state_cur, &context->basic_state_next, &context->basic_mask) >> 17) & 1) != 0;  /* vm86 mode? */
	else
		rvm = 1;  /* real mode ! */

	if(context->seg_max_limit_check != 0)
	{
		// This is intended to be used in 64bit mode, to catch e.g. mov ax,[-1].
		if(offset + (size_bytes - 1) < offset)
		{
			// Fixme, generate an exception here.
			do_abort(context, "A virtual read exceeded the maximum limit.");
			return 1;
		}
	}
	
	if(context->seg_descr_limit_check != 0 && descr->limit <= TULL(0xffffffff))
	{
		if(offset + (size_bytes - 1) > descr->limit)
		{
			// Fixme, generate an exception here.
			do_abort(context, "A virtual read exceeded the segment limit.");
			return 1;
		}
	}
	offset += descr->base;	// convert to linear address
	
	if(pOffsetLinear != NULL)
		*pOffsetLinear = offset;
	
	// Check access rights.
	if(!write_mode && !descr->readable)
	{
		if(sreg_value != 1 /* CS */ || !rvm)
		{
			// Fixme, generate an exception here.
			do_abort(context, "A virtual read was attempted from a non-readable segment.");
			return 1;
		}
	}
	if(write_mode && !descr->writable)
	{
		if(sreg_value != 1 /* CS */ || !rvm)
		{
			// Fixme, generate an exception here.
			do_abort(context, "A virtual write was attempted from a non-writable segment.");
			return 1;
		}
	}
	
	if(!context->mem_arg_cache[arg_num].is_valid)
	{
		context->mem_arg_cache[arg_num].is_valid = 1;
		context->mem_arg_cache[arg_num].need_write = (write_mode != 0) ? 1 : 0;
		
		if(translate_linear(context, offset & ~TULL(0xfff), context->cpl, context->mem_arg_cache[arg_num].phys + 0, (write_mode) ? 2 : 1) != 0)
			return 1;
		if((offset & ~TULL(0xfff)) != ((offset + size_bytes - 1) & ~TULL(0xfff)))
		{
			if(translate_linear(context, (offset + size_bytes - 1) & ~TULL(0xfff), context->cpl, context->mem_arg_cache[arg_num].phys + 1, (write_mode) ? 2 : 1) != 0)
				return 1;
		}
		else
			context->mem_arg_cache[arg_num].phys[1] = context->mem_arg_cache[arg_num].phys[0];
	}
	else
	if(write_mode && !context->mem_arg_cache[arg_num].need_write)
	{
		context->mem_arg_cache[arg_num].need_write = 1;

		if(translate_linear(context, offset & ~TULL(0xfff), context->cpl, &temp, 2) != 0)
			return 1;
		if((offset & ~TULL(0xfff)) != ((offset + size_bytes - 1) & ~TULL(0xfff)))
		{
			if(translate_linear(context, (offset + size_bytes - 1) & ~TULL(0xfff), context->cpl, &temp, 2) != 0)
				return 1;
		}
	}
	
	context->mem_arg_cache[arg_num].phys[0] |= (offset & 0xfff);
	
	context->mem_arg_cache[arg_num].node.size = size_bytes;
	
	return 0;
}

static int read_virtual(struct crudasm_intel_context_t *context, U1 sreg_value, U8 offset, void *destT, size_t size_bytes)
{
	U1 *dest = (U1 *)(destT);
	U2 selector = read_sreg_value(&context->basic_state_cur, &context->basic_state_next, &context->basic_mask, sreg_value);
	struct crudasm_intel_descriptor_t *descr = get_cached_descriptor(context, sreg_value);
	int rvm;
	U1 cpl;
	
	if(size_bytes == 0)
		return 0;	// success (nothing to do!)
	
	if(size_bytes > 8)
	{
		for(;;)
		{
			if(size_bytes <= 8)
				return read_virtual(context, selector, offset, dest, size_bytes);
			if(read_virtual(context, selector, offset, dest, 8) != 0)
				return 1;	// error detected
			offset += 8;
			dest += 8;
			size_bytes -= 8;
		}
	}
	
	// 1 <= size_bytes <= 8.
	
	if(size_bytes != 1 && size_bytes != 8 && size_bytes != 4 && size_bytes != 2)
	{
		// size_bytes is one of: 2, 3, 5, 6, 7
		while(size_bytes >= 4)
		{
			if(read_virtual(context, selector, offset, dest, 4) != 0)
				return 1;
			offset += 4;
			dest += 4;
			size_bytes -= 4;
		}

		// size_bytes is one of: 1, 2, 3
		while(size_bytes >= 2)
		{
			if(read_virtual(context, selector, offset, dest, 2) != 0)
				return 1;
			offset += 2;
			dest += 2;
			size_bytes -= 2;
		}
		
		// size_bytes is one of: 0, 1
		if(size_bytes == 1)
			return read_virtual(context, selector, offset, dest, 1);
		return 0;
	}
	
	// size_bytes is exactly one of these: 1, 2, 4, 8.

	if(read_creg_value(&context->basic_state_cur, &context->basic_state_next, &context->basic_mask, 0 /* cr0 */) & 1 /* protect-enable bit */)
		rvm = ((read_rflags_all(&context->basic_state_cur, &context->basic_state_next, &context->basic_mask) >> 17) & 1) != 0;  /* vm86 mode? */
	else
		rvm = 1;  /* real mode ! */

	if(context->seg_max_limit_check != 0)
	{
		// This is intended to be used in 64bit mode, to catch e.g. mov ax,[-1].
		if(offset + (size_bytes - 1) < offset)
		{
			// Fixme, generate an exception here.
			do_abort(context, "A virtual read exceeded the maximum limit.");
			return 1;
		}
	}
	
	if(context->seg_descr_limit_check != 0 && descr->limit <= TULL(0xffffffff))
	{
		if(offset + (size_bytes - 1) > descr->limit)
		{
			// Fixme, generate an exception here.
			do_abort(context, "A virtual read exceeded the segment limit.");
			return 1;
		}
	}
	offset += descr->base;	// convert to linear address
	
	// Check access rights.
	if(!descr->readable)
	{
		if(sreg_value != 1 /* CS */ || !rvm)
		{
			// Fixme, generate an exception here.
			do_abort(context, "A virtual read was attempted from a non-readable segment.");
			return 1;
		}
	}
	return read_linear(context, offset, dest, size_bytes, context->cpl /* virtual reads are at CPL */, 0/*need_cache*/);
}

static int cache_write_virtual(struct crudasm_intel_context_t *context, U1 sreg_value, U8 offset, void *destT, size_t size_bytes)
{
	U1 *dest = (U1 *)(destT);
	U2 selector = read_sreg_value(&context->basic_state_cur, &context->basic_state_next, &context->basic_mask, sreg_value);
	struct crudasm_intel_descriptor_t *descr = get_cached_descriptor(context, sreg_value);
	int rvm;
	U1 cpl;
	
	if(size_bytes == 0)
		return 0;	// success (nothing to do!)
	
	if(size_bytes > 8)
	{
		for(;;)
		{
			if(size_bytes <= 8)
				return cache_write_virtual(context, selector, offset, dest, size_bytes);
			if(cache_write_virtual(context, selector, offset, dest, 8) != 0)
				return 1;	// error detected
			offset += 8;
			dest += 8;
			size_bytes -= 8;
		}
	}
	
	// 1 <= size_bytes <= 8.
	
	if(size_bytes != 1 && size_bytes != 8 && size_bytes != 4 && size_bytes != 2)
	{
		// size_bytes is one of: 2, 3, 5, 6, 7
		while(size_bytes >= 4)
		{
			if(cache_write_virtual(context, selector, offset, dest, 4) != 0)
				return 1;
			offset += 4;
			dest += 4;
			size_bytes -= 4;
		}

		// size_bytes is one of: 1, 2, 3
		while(size_bytes >= 2)
		{
			if(cache_write_virtual(context, selector, offset, dest, 2) != 0)
				return 1;
			offset += 2;
			dest += 2;
			size_bytes -= 2;
		}
		
		// size_bytes is one of: 0, 1
		if(size_bytes == 1)
			return cache_write_virtual(context, selector, offset, dest, 1);
		return 0;
	}
	
	// size_bytes is exactly one of these: 1, 2, 4, 8.

	if(read_creg_value(&context->basic_state_cur, &context->basic_state_next, &context->basic_mask, 0 /* cr0 */) & 1 /* protect-enable bit */)
		rvm = ((read_rflags_all(&context->basic_state_cur, &context->basic_state_next, &context->basic_mask) >> 17) & 1) != 0;  /* vm86 mode? */
	else
		rvm = 1;  /* real mode ! */

	if(context->seg_max_limit_check != 0)
	{
		// This is intended to be used in 64bit mode, to catch e.g. mov ax,[-1].
		if(offset + (size_bytes - 1) < offset)
		{
			// Fixme, generate an exception here.
			do_abort(context, "A virtual read exceeded the maximum limit.");
			return 1;
		}
	}
	
	if(context->seg_descr_limit_check != 0 && descr->limit <= TULL(0xffffffff))
	{
		if(offset + (size_bytes - 1) > descr->limit)
		{
			// Fixme, generate an exception here.
			do_abort(context, "A virtual read exceeded the segment limit.");
			return 1;
		}
	}
	offset += descr->base;	// convert to linear address
	
	// Check access rights.
	if(!descr->writable)
	{
		if(sreg_value != 1 /* CS */ || !rvm)
		{
			// Fixme, generate an exception here.
			do_abort(context, "A virtual read was attempted from a non-readable segment.");
			return 1;
		}
	}

	return cache_write_linear(context, offset, dest, size_bytes, context->cpl /* virtual reads are at CPL */);
}

//===============================================================================================//

static U1 get_arg_size(struct crudasm_intel_context_t *context, size_t arg_num)
{
	U1 size = context->icode.argsize[arg_num];

	if(context->icode.argtype[arg_num] == crudasm_intel_argtype_mem_ea_dbl)
		size <<= 1;		// multiply size by two, since we need two bounds
	else
	if(context->icode.argtype[arg_num] == crudasm_intel_argtype_mem_ea_seg)
		size += 2;		// selector required
	else
	if(context->icode.argtype[arg_num] == crudasm_intel_argtype_mem_ea_lim)
	{
		// size is 2 bytes, for the limit. add appropriate size for the offset.
		// Note that if dsz is not 2 (i.e. we are not in 64 bit mode), then if
		// o16, only the low 24 bits is significant. We go ahead and update the
		// full 32 bits anyway in case of e.g. o16 lgdt [x] or whatever, and
		// similarly we write the full 32 bits in case of e.g. o16 sgdt [x]. We
		// do this even when dsz is not 2. This should not cause any problems.
		size += (context->dsz == 2) ? 8 : 4;
	}

	return size;
}

// in: dest->size is valid. note we also read context->rel and context->icode.
//     need_cache : set to 1 if this is the first time we've read the argument
//     [for e.g. add(x, x) where x is an argument it is OK for need_cache to be
//     1 for both argument reads] and we have not previously written to the argument.
//     This causes us to cache the argument if it is a memory access, so subsequent
//     reads will read from the cache and not e.g. again from a memory-mapped I/O device.
// out: dest->value is now valid on success (return code 0)
static int crudasm_read_arg(struct crudasm_intel_context_t *context, size_t arg_num, struct crudasm_temp_node_t *dest)
{
	const int need_cache = 0;		// not needed
	U8 tmp;
	U1 arg_size = get_arg_size(context, arg_num);
	/* assume icode.encoding is valid */
	struct crudasm_intel_encoding_t *encoding = crudasm_intel_encoding_table + context->icode.encoding;
	U2 selector;
	U8 offset, temp, linear;
	U1 seg, *mdest;

	if(arg_size == 0 || dest->size != arg_size || arg_size > sizeof(dest->value))
	{
		do_abort(context, "An attempt was made to access an argument with an invalid size.");
		return 1;
	}
	
	if(crudasm_intel_argtype__is_imm(context->icode.argtype[arg_num]))
	{
		if(context->icode.argtype[arg_num] == crudasm_intel_argtype_imm_implict)
			tmp = context->icode.argvalue[arg_num];		// zero extend from byte
		else
		if(context->icode.argtype[arg_num] == crudasm_intel_argtype_imm_2nd)
			tmp = context->icode.disp;
		else
		{
			tmp = context->icode.imm;
			if(context->icode.argsize[arg_num] == 8)
			{
				if(encoding->flag_imm64_disp != 0)
					tmp += (((U8)(context->icode.disp) << 16) << 16);
				else
				if(encoding->flag_imm64_sx32 != 0)
					tmp = (U8)(S8)(S4)(U4)(tmp);
			}
		}
		dest->value[0] = tmp;	// far direct immediates appear as two separate arguments
	}
	else
	if(crudasm_intel_argtype__is_reg(context->icode.argtype[arg_num]))
	{
		switch(context->icode.argtype[arg_num])
		{
			case crudasm_intel_argtype_reg_xmm:
				read_xmm_value(&context->basic_state_cur, &context->basic_state_next, &context->basic_mask, context->icode.argvalue[arg_num], dest->value);
				break;
			case crudasm_intel_argtype_reg_mmx:
				read_mmx_value(&context->basic_state_cur, &context->basic_state_next, &context->basic_mask, context->icode.argvalue[arg_num], dest->value);
				break;
			case crudasm_intel_argtype_reg_dr:
				dest->value[0] = read_dreg_value(&context->basic_state_cur, &context->basic_state_next, &context->basic_mask, context->icode.argvalue[arg_num]);
				break;
			case crudasm_intel_argtype_reg_cr:
				dest->value[0] = read_creg_value(&context->basic_state_cur, &context->basic_state_next, &context->basic_mask, context->icode.argvalue[arg_num]);
				break;
			case crudasm_intel_argtype_reg_st:
				do_abort(context, "UNIMPLEMENTED: An attempt was made to read from ST(i) which is currently unimplemented.");
				return 1;
			case crudasm_intel_argtype_reg_sr:
				dest->value[0] = read_sreg_value(&context->basic_state_cur, &context->basic_state_next, &context->basic_mask, context->icode.argvalue[arg_num]);
				break;
			case crudasm_intel_argtype_reg_gr:
				if(context->icode.argsize[arg_num] == 1 && encoding->argvalue[arg_num] == 0xff && !context->icode.rex_used && context->icode.argvalue[arg_num] >= 4)
					dest->value[0] = read_gr_value(&context->basic_state_cur, &context->basic_state_next, &context->basic_mask, context->icode.argvalue[arg_num] - 4, 1, 8);  /* ah, ch, dh, bh */
				else
				if(context->icode.argsize[arg_num] == 1 && context->icode.argvalue[arg_num] >= 16)
					dest->value[0] = read_gr_value(&context->basic_state_cur, &context->basic_state_next, &context->basic_mask, context->icode.argvalue[arg_num] - 16, 1, 8);  /* ah, ch, dh, bh */
				else
					dest->value[0] = read_gr_value(&context->basic_state_cur, &context->basic_state_next, &context->basic_mask, context->icode.argvalue[arg_num], context->icode.argsize[arg_num], 0);
				break;
		}
	}
	else
	if(crudasm_intel_argtype__is_mem(context->icode.argtype[arg_num]))
	{
		if(context->icode.argtype[arg_num] == crudasm_intel_argtype_mem_ea_eai || context->icode.argtype[arg_num] == crudasm_intel_argtype_mem_ea_fxs)
		{
			// User must manually provide code for INVLPG and related instructions, and for FXSAVE/FXRSTOR.
			// This is due to limitations in the way our crudasm_temp_node_t structure is implemented.
			do_abort(context, "An attempt was made to access a mem_ea_eai or mem_ea_fxs argument (not supported).");
			return 1;
		}
		
		if(context->mem_arg_cache[arg_num].is_valid)
		{
			if(dest->size != context->mem_arg_cache[arg_num].node.size)
			{
				do_abort(context, "The size of a cached memory argument has a mismatch.");
				return 1;
			}
			memcpy(dest->value, context->mem_arg_cache[arg_num].node.value, dest->size);
			return 0;
		}

		if(context->icode.argtype[arg_num] == crudasm_intel_argtype_mem_ea_eal)
		{
			dest->value[0] = context->mem_arg_cache[arg_num].linear;
			return 0;
		}
		
		seg = context->icode.ea.sreg;
		if(seg >= 6)
			seg -= 4;	// turn 6,7 into 2,3
		if(access_cached_mem_arg(context, arg_num, 0 /* read */, seg, arg_size, &linear) != 0)
			return 1;
		if(dest->size != context->mem_arg_cache[arg_num].node.size)  /* redundant check */
		{
			do_abort(context, "The size of a cached memory argument has a mismatch.");
			return 1;
		}
		
		mdest = (U1 *)context->mem_arg_cache[arg_num].node.value;

		// Read from physical memory to the argument cache.		
		while(arg_size != 0)
		{
			tmp = arg_size;
			if((linear & ~TULL(0xfff)) != ((linear + tmp - 1) & ~TULL(0xfff)))
				tmp = 0x1000 - (linear & ~TULL(0xfff));
			if(context->do_read_physical(context, context->mem_arg_cache[arg_num].phys[0], tmp, mdest) != 0)
				return 1;
			arg_size -= tmp;
			if(arg_size != 0)
			{
				mdest += tmp;
				if(context->do_read_physical(context, context->mem_arg_cache[arg_num].phys[1], arg_size, mdest) != 0)
					return 1;
			}
		}
		
		memcpy(dest->value, context->mem_arg_cache[arg_num].node.value, dest->size);
		return 0;
	}
	else
	{
		do_abort(context, "An attempt was made to access an argument with an unsupported type.");
		return 1;
	}
	
	return 0;
}

static int default_read_port(struct crudasm_intel_context_t *context, U2 port, U1 size_bytes, U8 *result)
{
	*result = TULL(0xffffffffffffffff);
	if(size_bytes == 1)
		*result = (U1)(*result);
	else
	if(size_bytes == 2)
		*result = (U2)(*result);
	else
	if(size_bytes == 4)
		*result = (U4)(*result);
	return 0;
}

int default_write_port(struct crudasm_intel_context_t *context, U2 port, U1 size_bytes, U8 value)
{
	return 0; // ignore writes
}

static void crudasm_intel_context_init(struct crudasm_intel_context_t *context, void *user_context)
{
	memset(context, 0, sizeof(context));
	context->user_context = user_context;
	context->do_read_physical = default_read_physical;
	context->do_write_physical = default_write_physical;
	context->do_read_port = default_read_port;
	context->do_write_port = default_write_port;
}

// When this is called, context->icode should already be valid.
// In: context->rel should be set correctly.
// Normally returns 0. Returns 1 if the instruction is repeated but rCX == 0.
static int crudasm_intel_context_pre_exec(struct crudasm_intel_context_t *context)
{
	U8 offset, temp, tmp;
	size_t i, arg_num;
	struct crudasm_intel_encoding_t *encoding = crudasm_intel_encoding_table + context->icode.encoding;
	int handled;

	context->error_desc = NULL;		// clear error message

	for(i = 0; i < CRUDASM_INTEL_MAX_ASM_ARGS; ++i)
	{
		arg_num = i;
		context->mem_arg_cache[i].is_valid = 0;
		if(crudasm_intel_argtype__is_mem(context->icode.argtype[i]))
		{
			// Start with an offset of 0.
			offset = 0;
			handled = 0;
		
			// Add in any base reg & index reg.
			if(context->icode.argtype[arg_num] == crudasm_intel_argtype_mem_implict_sts)
			{
				handled = 1;
				temp = read_gr_value(&context->basic_state_cur, &context->basic_state_next, &context->basic_mask, rSI, 8, 0);
				if(context->icode.asz == 2)
					offset += temp;
				else
				if(context->icode.asz == 0)
					offset = (U8)(U2)(offset + temp);	// not possible in 64 bit mode
				else
					offset += (U8)(U4)(temp);		// if a32, we'll truncate below. if db 0x67 in 64bit mode, we want to sign-extend (right?)
			}
			else
			if(context->icode.argtype[arg_num] == crudasm_intel_argtype_mem_implict_xls)
			{
				handled = 1;
				temp = read_gr_value(&context->basic_state_cur, &context->basic_state_next, &context->basic_mask, rBX, 8, 0);
				if(context->icode.asz == 2)
					offset += temp;
				else
				if(context->icode.asz == 0)
					offset = (U8)(U2)(offset + temp);	// not possible in 64 bit mode
				else
					offset += (U8)(U4)(temp);		// if a32, we'll truncate below. if db 0x67 in 64bit mode, we want to sign-extend (right?)
				temp = read_gr_value(&context->basic_state_cur, &context->basic_state_next, &context->basic_mask, rAX, 8, 0);
				offset += (U8)(U1)(temp);
			}
			else
			if(context->icode.argtype[arg_num] == crudasm_intel_argtype_mem_implict_std)
			{
				handled = 1;
				temp = read_gr_value(&context->basic_state_cur, &context->basic_state_next, &context->basic_mask, rDI, 8, 0);
				if(context->icode.asz == 2)
					offset += temp;
				else
				if(context->icode.asz == 0)
					offset = (U8)(U2)(offset + temp);	// not possible in 64 bit mode
				else
					offset += (U8)(U4)(temp);		// if a32, we'll truncate below. if db 0x67 in 64bit mode, we want to sign-extend (right?)
			}
			else
			if(context->icode.argtype[arg_num] != crudasm_intel_argtype_mem_fulldisp)
			{
				// Add base reg + (index reg << shift).
				if(context->icode.ea.base != 31)
				{
					// get base register
					temp = read_gr_value(&context->basic_state_cur, &context->basic_state_next, &context->basic_mask, context->icode.ea.base, 8, 0);
					if(context->icode.asz == 2)
						offset += temp;
					else
					if(context->icode.asz == 0)
						offset = (U8)(U2)(offset + temp);	// not possible in 64 bit mode
					else
						offset += (U8)(S8)(S4)(temp);		// if a32, we'll truncate below. if db 0x67 in 64bit mode, we want to sign-extend (right?)
				}
				if(context->icode.ea.index != 31)
				{
					// get base register
					temp = read_gr_value(&context->basic_state_cur, &context->basic_state_next, &context->basic_mask, context->icode.ea.index, 8, 0);
					temp <<= context->icode.ea.index_scale;
					if(context->icode.asz == 2)
						offset += temp;
					else
					if(context->icode.asz == 0)
						offset = (U8)(U2)(offset + temp);	// not possible in 64 bit mode
					else
						offset += (U8)(S8)(S4)(temp);		// if a32, we'll truncate below. if db 0x67 in 64bit mode, we want to sign-extend (right?)
				}
			}
			
			if(handled)
				;
			else
			if(context->icode.argtype[arg_num] != crudasm_intel_argtype_mem_fulldisp && context->icode.ea.disp8 != 0)
			{
				offset += (U8)(S8)(S1)(context->icode.disp);
			}
			else
			if(context->icode.argtype[arg_num] == crudasm_intel_argtype_mem_fulldisp && context->icode.asz == 2 && encoding->flag_imm64_disp != 0)
			{
				tmp = (U4)(context->icode.disp);
				tmp += (((U8)(context->icode.imm)) << 16) << 16;
				offset += tmp;
			}
			else
			{
				offset += (U8)(S8)(S4)(context->icode.disp);
			}
			
			// Handle RIP relative addressing. This can happen only in a64 mode.
			if(!handled && context->icode.rip_relative)
				offset += context->rel;
		
			if(context->icode.asz == 0)
				offset = (U2)(offset);
			else
			if(context->icode.asz == 1)
				offset = (U4)(offset);
			
			context->mem_arg_cache[i].linear = offset;	// this is an offset, not a linear address
		}
	}
	memset(&context->basic_mask, 0, sizeof(struct crudasm_basic_cpu_mask_t));
	
	// Check for skip condition.
	if(context->icode.lockrep != 2 && context->icode.lockrep != 3)
		return 0;
	if(crudasm_intel_insns[(size_t)(encoding->insn)].flag_repable || crudasm_intel_insns[(size_t)(encoding->insn)].flag_repcond)
	{
		if(context->icode.asz == 0)
		{
			if((U2)(context->basic_state_cur.gregs[rCX]) == 0)
				return 1;
		}
		else
		if(context->icode.asz == 1)
		{
			if((U4)(context->basic_state_cur.gregs[rCX]) == 0)
				return 1;
		}
		else
		{
			if(context->basic_state_cur.gregs[rCX] == 0)
				return 1;
		}
	}
	
	return 0;
}

static int crudasm_write_arg(struct crudasm_intel_context_t *context, size_t arg_num, struct crudasm_temp_node_t *src)
{
	struct crudasm_temp_node_t *dest = src;
	const int need_cache = 0;		// not needed
	U8 tmp;
	U1 arg_size = get_arg_size(context, arg_num);
	/* assume icode.encoding is valid */
	struct crudasm_intel_encoding_t *encoding = crudasm_intel_encoding_table + context->icode.encoding;
	U2 selector;
	U8 offset, temp, linear;
	U1 seg, *mdest;

	if(arg_size == 0 || dest->size != arg_size || arg_size > sizeof(dest->value))
	{
		do_abort(context, "An attempt was made to access an argument with an invalid size.");
		return 1;
	}
	
	if(crudasm_intel_argtype__is_imm(context->icode.argtype[arg_num]))
	{
		do_abort(context, "An attempt was made to write to an immediate argument.");
		return 1;
	}
	else
	if(crudasm_intel_argtype__is_reg(context->icode.argtype[arg_num]))
	{
		switch(context->icode.argtype[arg_num])
		{
			case crudasm_intel_argtype_reg_xmm:
				write_xmm_value(&context->basic_state_cur, &context->basic_state_next, &context->basic_mask, context->icode.argvalue[arg_num], dest->value);
				break;
			case crudasm_intel_argtype_reg_mmx:
				write_mmx_value(&context->basic_state_cur, &context->basic_state_next, &context->basic_mask, context->icode.argvalue[arg_num], dest->value[0]);
				break;
			case crudasm_intel_argtype_reg_dr:
				if(dest->size == 8)
					write_dreg_value(&context->basic_state_cur, &context->basic_state_next, &context->basic_mask, context->icode.argvalue[arg_num], dest->value[0]);
				else
				{
					tmp = read_dreg_value(&context->basic_state_cur, &context->basic_state_next, &context->basic_mask, context->icode.argvalue[arg_num]);
					*(U4 *)(&tmp) = dest->value[0];
					write_dreg_value(&context->basic_state_cur, &context->basic_state_next, &context->basic_mask, context->icode.argvalue[arg_num], tmp);
				}
				break;
			case crudasm_intel_argtype_reg_cr:
				if(dest->size == 8)
					write_creg_value(&context->basic_state_cur, &context->basic_state_next, &context->basic_mask, context->icode.argvalue[arg_num], dest->value[0]);
				else
				{
					tmp = read_creg_value(&context->basic_state_cur, &context->basic_state_next, &context->basic_mask, context->icode.argvalue[arg_num]);
					*(U4 *)(&tmp) = dest->value[0];
					write_creg_value(&context->basic_state_cur, &context->basic_state_next, &context->basic_mask, context->icode.argvalue[arg_num], tmp);
				}
				break;
			case crudasm_intel_argtype_reg_st:
				do_abort(context, "UNIMPLEMENTED: An attempt was made to read from ST(i) which is currently unimplemented.");
				return 1;
			case crudasm_intel_argtype_reg_sr:
				write_sreg_value(&context->basic_state_cur, &context->basic_state_next, &context->basic_mask, context->icode.argvalue[arg_num], dest->value[0]);
				break;
			case crudasm_intel_argtype_reg_gr:
				if(context->icode.argsize[arg_num] == 1 && encoding->argvalue[arg_num] == 0xff && !context->icode.rex_used && context->icode.argvalue[arg_num] >= 4)
					write_gr_value(&context->basic_state_cur, &context->basic_state_next, &context->basic_mask, context->icode.argvalue[arg_num] - 4, 1, 8, dest->value[0]);  /* ah, ch, dh, bh */
				else
				if(context->icode.argsize[arg_num] == 1 && context->icode.argvalue[arg_num] >= 16)
					write_gr_value(&context->basic_state_cur, &context->basic_state_next, &context->basic_mask, context->icode.argvalue[arg_num] - 16, 1, 8, dest->value[0]);  /* ah, ch, dh, bh */
				else
					write_gr_value(&context->basic_state_cur, &context->basic_state_next, &context->basic_mask, context->icode.argvalue[arg_num], context->icode.argsize[arg_num], 0, dest->value[0]);
				break;
		}
	}
	else
	if(crudasm_intel_argtype__is_mem(context->icode.argtype[arg_num]))
	{
		if(context->icode.argtype[arg_num] == crudasm_intel_argtype_mem_ea_eai || context->icode.argtype[arg_num] == crudasm_intel_argtype_mem_ea_fxs)
		{
			// User must manually provide code for INVLPG and related instructions, and for FXSAVE/FXRSTOR.
			// This is due to limitations in the way our crudasm_temp_node_t structure is implemented.
			do_abort(context, "An attempt was made to access a mem_ea_eai or mem_ea_fxs argument (not supported).");
			return 1;
		}
		
		if(context->mem_arg_cache[arg_num].is_valid && context->mem_arg_cache[arg_num].need_write)
		{
			// We've already written to this argument, so just update the cached data.
			if(dest->size != context->mem_arg_cache[arg_num].node.size)
			{
				do_abort(context, "The size of a cached memory argument has a mismatch.");
				return 1;
			}
			memcpy(context->mem_arg_cache[arg_num].node.value, dest->value, dest->size);
			return 0;
		}

		if(context->icode.argtype[arg_num] == crudasm_intel_argtype_mem_ea_eal)
		{
			do_abort(context, "An attempt was made to write to a read-only LEA argument.");
			return 1;
		}
		
		seg = context->icode.ea.sreg;
		if(seg >= 6)
			seg -= 4;	// turn 6,7 into 2,3
		if(access_cached_mem_arg(context, arg_num, 1 /* write */, seg, arg_size, &linear) != 0)
			return 1;

		if(dest->size != context->mem_arg_cache[arg_num].node.size)  /* redundant check */
		{
			do_abort(context, "The size of a cached memory argument has a mismatch.");
			return 1;
		}
		
		context->mem_arg_cache[arg_num].need_write = 1;		
		memcpy(context->mem_arg_cache[arg_num].node.value, dest->value, dest->size);
		return 0;
	}
	else
	{
		do_abort(context, "An attempt was made to access an argument with an unsupported type.");
		return 1;
	}
	
	return 0;
}

//===============================================================================================//

static struct crudasm_temp_node_t internal_get_flag(struct crudasm_intel_context_t *context, U1 flag)
{
	struct crudasm_temp_node_t r;
	r.size = 1; /* BIT */
	r.value[0] = read_rflag(&context->basic_state_cur, &context->basic_state_next, &context->basic_mask, flag) & 1;
	return r;
}

static int do_write_rflag_value(struct crudasm_intel_context_t *context, U1 flag, struct crudasm_temp_node_t *psrc)
{
	write_rflag(&context->basic_state_cur, &context->basic_state_next, &context->basic_mask, flag, psrc->value[0] & 1);
	return 0;
}

static struct crudasm_temp_node_t internal_get_extern_undefined(struct crudasm_intel_context_t *context)
{
	struct crudasm_temp_node_t r;
	r.size = 1; /* BIT */
	r.value[0] = 0; /* use 0 for undefined bits */
	return r;
}

static int special_do_assert(struct crudasm_intel_context_t *context, struct crudasm_temp_node_t cond, struct crudasm_temp_node_t level)
{
	U1 ok = cond.value[0] & 1;
	U1 code = level.value[0] & 0xff;
	if(ok)
		return 0;
	if(code < 32)
	{
		// FIXME, raise exception 'level' here.
		do_abort(context, "An instruction tried to generate an exception (not implemented!)");
	}
	else
		do_abort(context, "Internal emulation error!");
	return 1;
}

static struct crudasm_temp_node_t internal_make_num(struct crudasm_intel_context_t *context, U1 size, U8 msvalue, U8 lsvalue)
{
	struct crudasm_temp_node_t r;
	r.size = size;
	r.value[0] = lsvalue;
	r.value[1] = msvalue;
	return r;
}

static struct crudasm_temp_node_t internal_get_reg8(struct crudasm_intel_context_t *context, U1 num)
{
	struct crudasm_temp_node_t r;
	r.size = 1;
	r.value[0] = read_gr_value(&context->basic_state_cur, &context->basic_state_next, &context->basic_mask, num & 15, 1, (num < 16) ? 0 : 8);
	return r;
}

static struct crudasm_temp_node_t internal_get_reg16(struct crudasm_intel_context_t *context, U1 num)
{
	struct crudasm_temp_node_t r;
	r.size = 2;
	r.value[0] = read_gr_value(&context->basic_state_cur, &context->basic_state_next, &context->basic_mask, num, 2, 0);
	return r;
}

static struct crudasm_temp_node_t internal_get_reg32(struct crudasm_intel_context_t *context, U1 num)
{
	struct crudasm_temp_node_t r;
	r.size = 4;
	r.value[0] = read_gr_value(&context->basic_state_cur, &context->basic_state_next, &context->basic_mask, num, 4, 0);
	return r;
}

static struct crudasm_temp_node_t internal_get_reg64(struct crudasm_intel_context_t *context, U1 num)
{
	struct crudasm_temp_node_t r;
	r.size = 8;
	r.value[0] = read_gr_value(&context->basic_state_cur, &context->basic_state_next, &context->basic_mask, num, 8, 0);
	return r;
}

static int write_gr8_value(struct crudasm_intel_context_t *context, U1 regnum, struct crudasm_temp_node_t *src)
{
	write_gr_value(&context->basic_state_cur, &context->basic_state_next, &context->basic_mask, regnum & 15, 1, (regnum < 16) ? 0 : 8, src->value[0]);
	return 0;
}

static int write_gr16_value(struct crudasm_intel_context_t *context, U1 regnum, struct crudasm_temp_node_t *src)
{
	write_gr_value(&context->basic_state_cur, &context->basic_state_next, &context->basic_mask, regnum & 15, 2, 0, src->value[0]);
	return 0;
}

static int write_gr32_value(struct crudasm_intel_context_t *context, U1 regnum, struct crudasm_temp_node_t *src)
{
	write_gr_value(&context->basic_state_cur, &context->basic_state_next, &context->basic_mask, regnum & 15, 4, 0, src->value[0]);
	return 0;
}

static int write_gr64_value(struct crudasm_intel_context_t *context, U1 regnum, struct crudasm_temp_node_t *src)
{
	write_gr_value(&context->basic_state_cur, &context->basic_state_next, &context->basic_mask, regnum & 15, 8, 9, src->value[0]);
	return 0;
}

static struct crudasm_temp_node_t internal_get_argsize(struct crudasm_intel_context_t *context, size_t argnum)
{
	struct crudasm_temp_node_t r;
	r.size = 16;
	r.value[0] = get_arg_size(context, argnum);
	r.value[1] = 0;
	return r;
}

static struct crudasm_temp_node_t internal_get_extern_FLAGS(struct crudasm_intel_context_t *context)
{
	struct crudasm_temp_node_t r;
	r.size = 2;
	r.value[0] = (U2)read_rflags_all(&context->basic_state_cur, &context->basic_state_next, &context->basic_mask);
	return r;
}

static struct crudasm_temp_node_t internal_get_extern_EFLAGS(struct crudasm_intel_context_t *context)
{
	struct crudasm_temp_node_t r;
	r.size = 4;
	r.value[0] = (U4)read_rflags_all(&context->basic_state_cur, &context->basic_state_next, &context->basic_mask);
	return r;
}

static struct crudasm_temp_node_t internal_get_extern_RFLAGS(struct crudasm_intel_context_t *context)
{
	struct crudasm_temp_node_t r;
	r.size = 8;
	r.value[0] = read_rflags_all(&context->basic_state_cur, &context->basic_state_next, &context->basic_mask);
	return r;
}

//===============================================================================================//

static void do_write_extern_FLAGS(struct crudasm_intel_context_t *context, struct crudasm_temp_node_t *src)
{
	U8 tmp = read_rflags_all(&context->basic_state_cur, &context->basic_state_next, &context->basic_mask);
	*(U2 *)(&tmp) = src->value[0];
	write_rflags_all(&context->basic_state_cur, &context->basic_state_next, &context->basic_mask, tmp);
}

static void do_write_extern_EFLAGS(struct crudasm_intel_context_t *context, struct crudasm_temp_node_t *src)
{
	U8 tmp = read_rflags_all(&context->basic_state_cur, &context->basic_state_next, &context->basic_mask);
	*(U4 *)(&tmp) = src->value[0];
	write_rflags_all(&context->basic_state_cur, &context->basic_state_next, &context->basic_mask, tmp);
}

static void do_write_extern_RFLAGS(struct crudasm_intel_context_t *context, struct crudasm_temp_node_t *src)
{
	write_rflags_all(&context->basic_state_cur, &context->basic_state_next, &context->basic_mask, src->value[0]);
}

static int write_sr_value(struct crudasm_intel_context_t *context, U1 seg_reg, struct crudasm_temp_node_t *psrc)
{
	if(seg_reg > 6)
	{
		do_abort(context, "An attempt was made to write to an invalid segment register.");
		return 1;
	}
	write_sreg_value(&context->basic_state_cur, &context->basic_state_next, &context->basic_mask, seg_reg, (U2)psrc->value[0]);
	return 0;
}

static int special_do_reserve(struct crudasm_intel_context_t *context, struct crudasm_temp_node_t ssz_src)
{
	U8 rsp = read_gr_value(&context->basic_state_cur, &context->basic_state_next, &context->basic_mask, rSP, 8, 0);
	
	if(context->ssz == 0)
		rsp = (U2)((U2)(rsp) - (U2)(ssz_src.value[0]));
	else
	if(context->ssz == 1)
		rsp = (U4)((U4)(rsp) - (U4)(ssz_src.value[0]));
	else
		rsp -= ssz_src.value[0];
	
	write_gr_value(&context->basic_state_cur, &context->basic_state_next, &context->basic_mask, rSP, 2 << context->ssz, 0, rsp);
	return 0;
}

static int special_do_push(struct crudasm_intel_context_t *context, struct crudasm_temp_node_t dsrc)
{
	struct crudasm_temp_node_t *psrc = &dsrc;
	U8 rsp = read_gr_value(&context->basic_state_cur, &context->basic_state_next, &context->basic_mask, rSP, 8, 0);
	U8 delta = psrc->size;

	if(context->ssz == 0)
		rsp = (U2)(rsp);
	else
	if(context->ssz == 1)
		rsp = (U4)(rsp);
	
	if(rsp == 1)
	{
		do_abort(context, "push with rSP == 1, system shutdown! (a real machine would triple fault and reset)");
		return 1;
	}

	if(context->ssz == 0)
		rsp = (U2)(rsp - delta);
	else
	if(context->ssz == 1)
		rsp = (U4)(rsp - delta);
	else
		rsp = (U8)(rsp - delta);

	int result = cache_write_virtual(context, rSS, rsp, psrc->value, psrc->size);
	if(result != 0)
		return result;
	write_gr_value(&context->basic_state_cur, &context->basic_state_next, &context->basic_mask, rSP, psrc->size, 0, rsp);
	return 0;
}

static int do_pop(struct crudasm_intel_context_t *context, struct crudasm_temp_node_t *psrc)
{
	U8 rsp = read_gr_value(&context->basic_state_cur, &context->basic_state_next, &context->basic_mask, rSP, 8, 0);
	U8 delta = psrc->size;
	if(context->ssz == 0)
		rsp = (U2)(rsp);
	else
	if(context->ssz == 1)
		rsp = (U4)(rsp);
	int result = read_virtual(context, rSS, rsp, psrc->value, psrc->size);
	if(result != 0)
		return result;
	if(context->ssz == 0)
		rsp = (U2)(rsp + delta);	
	else
	if(context->ssz == 1)
		rsp = (U4)(rsp + delta);	
	else
		rsp = (U8)(rsp + delta);	
	write_gr_value(&context->basic_state_cur, &context->basic_state_next, &context->basic_mask, rSP, psrc->size, 0, rsp);
	return 0;
}

static int do_inport(struct crudasm_intel_context_t *context, struct crudasm_temp_node_t *target, U1 size)
{
	U2 port = (U2)target->value[0];
	U8 result = TULL(0xffffffffffffffff);
	// FIXME, check IOPL and/or IOBITMAP here !
	if(context->do_read_port(context, port, size, &result) != 0)
		return 1;
	target->size = size;
	target->value[0] = result;
	return 0;
}

static int special_do_outport(struct crudasm_intel_context_t *context, struct crudasm_temp_node_t port, struct crudasm_temp_node_t value)
{
	U2 portT = (U2)port.value[0];
	U8 valueT = value.value[0];
	if(value.size == 1)
		valueT = (U1)valueT;
	else
	if(value.size == 2)
		valueT = (U2)valueT;
	else
	if(value.size == 4)
		valueT = (U4)valueT;

	// FIXME, check IOPL and/or IOBITMAP here !
#ifdef PORTE9HACK	// debugging output hack, ala BOCHS
	if(portT == 0xe9)
		putchar((char)(valueT & 0xff));
#endif

	if(context->do_write_port(context, portT, value.size, valueT) != 0)
		return 1;
	return 0;
}

//===============================================================================================//

#ifdef GUEST64ENABLED
extern U8 muldiv64_a, muldiv64_b, muldiv64_result, muldiv64_high;
extern U1 muldiv64_overflow;
void muldiv64_smul();
void muldiv64_umul();
void muldiv64_sdiv();
void muldiv64_udiv();
#endif

static struct crudasm_temp_node_t internal_compute_3_udiv64(struct crudasm_intel_context_t *context, struct crudasm_temp_node_t a, struct crudasm_temp_node_t b, struct crudasm_temp_node_t c)
{
	struct crudasm_temp_node_t r;
	r.size = a.size;
#ifdef GUEST64ENABLED
	muldiv64_high = a.value[0];
	muldiv64_a = b.value[0];
	muldiv64_b = c.value[0];
	muldiv64_udiv();
	r.value[0] = muldiv64_result;
#else
	r.value[0] = 0;
#endif
	return r;
}

static struct crudasm_temp_node_t internal_compute_3_umod64(struct crudasm_intel_context_t *context, struct crudasm_temp_node_t a, struct crudasm_temp_node_t b, struct crudasm_temp_node_t c)
{
	struct crudasm_temp_node_t r;
	r.size = a.size;
#ifdef GUEST64ENABLED
	muldiv64_high = a.value[0];
	muldiv64_a = b.value[0];
	muldiv64_b = c.value[0];
	muldiv64_umod();
	r.value[0] = muldiv64_result;
#else
	r.value[0] = 0;
#endif
	return r;
}


static struct crudasm_temp_node_t internal_compute_3_sdiv64(struct crudasm_intel_context_t *context, struct crudasm_temp_node_t a, struct crudasm_temp_node_t b, struct crudasm_temp_node_t c)
{
	struct crudasm_temp_node_t r;
	r.size = a.size;
#ifdef GUEST64ENABLED
	muldiv64_high = a.value[0];
	muldiv64_a = b.value[0];
	muldiv64_b = c.value[0];
	muldiv64_sdiv();
	r.value[0] = muldiv64_result;
#else
	r.value[0] = 0;
#endif
	return r;
}

static struct crudasm_temp_node_t internal_compute_3_smod64(struct crudasm_intel_context_t *context, struct crudasm_temp_node_t a, struct crudasm_temp_node_t b, struct crudasm_temp_node_t c)
{
	struct crudasm_temp_node_t r;
	r.size = a.size;
#ifdef GUEST64ENABLED
	muldiv64_high = a.value[0];
	muldiv64_a = b.value[0];
	muldiv64_b = c.value[0];
	muldiv64_smod();
	r.value[0] = muldiv64_result;
#else
	r.value[0] = 0;
#endif
	return r;
}

static struct crudasm_temp_node_t internal_compute_2_umulhi(struct crudasm_intel_context_t *context, struct crudasm_temp_node_t a, struct crudasm_temp_node_t b)
{
	struct crudasm_temp_node_t result;
	result.size = a.size;
	
	if(result.size == 1)
		*(U1 *)(result.value) = (U1)(SHR64U((U8)(U2)( (U2)(U1)(a.value[0]) * (U2)(U1)(b.value[0]) ), 8));
	else
	if(result.size == 2)
		*(U2 *)(result.value) = (U2)(SHR64U((U8)(U4)( (U4)(U2)(a.value[0]) * (U4)(U2)(b.value[0]) ), 16));
	else
	if(result.size == 4)
		*(U4 *)(result.value) = (U4)(SHR64U((U8)( (U4)(U2)(a.value[0]) * (U4)(U2)(b.value[0]) ), 32));
	else
	{
#ifdef GUEST64ENABLED
		muldiv64_a = a.value[0];
		muldiv64_b = b.value[0];
		muldiv64_umul();
		result.value[0] = muldiv64_high;
#else
		result.value[0] = 0;
#endif
	}
	
	return result;
}

static struct crudasm_temp_node_t internal_compute_2_smulhi(struct crudasm_intel_context_t *context, struct crudasm_temp_node_t a, struct crudasm_temp_node_t b)
{
	struct crudasm_temp_node_t result;
	result.size = a.size;
	
	if(result.size == 1)
		*(S1 *)(result.value) = (S1)(SHR64U((U8)(U2)( (S2)(S1)(a.value[0]) * (S2)(S1)(b.value[0]) ), 8));
	else
	if(result.size == 2)
		*(S2 *)(result.value) = (S2)(SHR64U((U8)(U4)( (S4)(S2)(a.value[0]) * (S4)(S2)(b.value[0]) ), 16));
	else
	if(result.size == 4)
		*(S4 *)(result.value) = (S4)(SHR64U((U8)( (S4)(S2)(a.value[0]) * (S4)(S2)(b.value[0]) ), 32));
	else
	{
#ifdef GUEST64ENABLED
		muldiv64_a = a.value[0];
		muldiv64_b = b.value[0];
		muldiv64_smul();
		result.value[0] = muldiv64_high;
#else
		result.value[0] = 0;
#endif
	}
	
	return result;
}

static struct crudasm_temp_node_t internal_compute_2_cmul(struct crudasm_intel_context_t *context, struct crudasm_temp_node_t a, struct crudasm_temp_node_t b)
{
	struct crudasm_temp_node_t result;
	result.size = a.size;
	
	if(a.size == 1)
		*(S1 *)(result.value) = *(S1 *)(a.value) * *(S1 *)(b.value);
	else
	if(a.size == 2)
		*(S2 *)(result.value) = *(S2 *)(a.value) * *(S2 *)(b.value);
	else
	if(a.size == 4)
		*(S4 *)(result.value) = *(S4 *)(a.value) * *(S4 *)(b.value);
	else
		*(S8 *)(result.value) = *(S8 *)(a.value) * *(S8 *)(b.value);
	
	return result;
}

static struct crudasm_temp_node_t internal_compute_2__smul_overflow(struct crudasm_intel_context_t *context, struct crudasm_temp_node_t a, struct crudasm_temp_node_t b)
{
	struct crudasm_temp_node_t r;
	S2 c2;
	S4 c4;
	S8 c8;
	r.size = 1;
	
	if(a.size == 1)
	{
		c2 = (S2)(S1)(a.value[0]) * (S2)(S1)(b.value[0]);
		r.value[0] = (c2 != (S2)(S1)(c2)) ? 1 : 0;
	}
	else
	if(a.size == 2)
	{
		c4 = (S4)(S2)(a.value[0]) * (S4)(S2)(b.value[0]);
		r.value[0] = (c4 != (S4)(S2)(c4)) ? 1 : 0;
	}
	else
	if(a.size == 4)
	{
		c8 = (S8)(S4)(a.value[0]) * (S8)(S4)(b.value[0]);
		r.value[0] = (c8 != (S8)(S4)(c8)) ? 1 : 0;
	}
	else
	{
#ifdef GUEST64ENABLED
		muldiv64_a = a.value[0];
		muldiv64_b = b.value[0];
		muldiv64_smul();
		r.value[0] = muldiv64_overflow & 1;
#else
		r.value[0] = 0;		// not supported
#endif
	}
	
	return r;
}

static struct crudasm_temp_node_t internal_compute_2__umul_overflow(struct crudasm_intel_context_t *context, struct crudasm_temp_node_t a, struct crudasm_temp_node_t b)
{
	struct crudasm_temp_node_t r;
	U2 c2;
	U4 c4;
	U8 c8;
	r.size = 1;
	
	if(a.size == 1)
	{
		c2 = (U2)(U1)(a.value[0]) * (U2)(U1)(b.value[0]);
		r.value[0] = (c2 != (U2)(U1)(c2)) ? 1 : 0;
	}
	else
	if(a.size == 2)
	{
		c4 = (U4)(U2)(a.value[0]) * (U4)(U2)(b.value[0]);
		r.value[0] = (c4 != (U4)(U2)(c4)) ? 1 : 0;
	}
	else
	if(a.size == 4)
	{
		c8 = (U8)(U4)(a.value[0]) * (U8)(U4)(b.value[0]);
		r.value[0] = (c8 != (U8)(U4)(c8)) ? 1 : 0;
	}
	else
	{
#ifdef GUEST64ENABLED
		muldiv64_a = a.value[0];
		muldiv64_b = b.value[0];
		muldiv64_umul();
		r.value[0] = muldiv64_overflow & 1;
#else
		r.value[0] = 0;		// not supported
#endif
	}
	
	return r;
}

static struct crudasm_temp_node_t internal_compute_2_udiv(struct crudasm_intel_context_t *context, struct crudasm_temp_node_t a, struct crudasm_temp_node_t b)
{
	struct crudasm_temp_node_t result;
	result.size = a.size;
	
	if(result.size == 1)
		result.value[0] = (U1)(a.value[0]) / (U1)(b.value[0]);
	else
	if(result.size == 2)
		result.value[0] = (U2)(a.value[0]) / (U2)(b.value[0]);
	else
	if(result.size == 4)
		result.value[0] = (U4)(a.value[0]) / (U4)(b.value[0]);
	else
	{
#ifdef GUEST64ENABLED
		muldiv64_a = a.value[0];
		muldiv64_b = b.value[0];
		muldiv64_udiv();
		result.value[0] = muldiv64_result;
#else
		result.value[0] = 0;
#endif
	}
	
	return result;
}

static struct crudasm_temp_node_t internal_compute_2_umod(struct crudasm_intel_context_t *context, struct crudasm_temp_node_t a, struct crudasm_temp_node_t b)
{
	struct crudasm_temp_node_t result;
	result.size = a.size;
	
	if(result.size == 1)
		result.value[0] = (U1)(a.value[0]) % (U1)(b.value[0]);
	else
	if(result.size == 2)
		result.value[0] = (U2)(a.value[0]) % (U2)(b.value[0]);
	else
	if(result.size == 4)
		result.value[0] = (U4)(a.value[0]) % (U4)(b.value[0]);
	else
	{
#ifdef GUEST64ENABLED
		muldiv64_a = a.value[0];
		muldiv64_b = b.value[0];
		muldiv64_umod();
		result.value[0] = muldiv64_result;
#else
		result.value[0] = 0;
#endif
	}
	
	return result;
}

static struct crudasm_temp_node_t internal_compute_2_sdiv(struct crudasm_intel_context_t *context, struct crudasm_temp_node_t a, struct crudasm_temp_node_t b)
{
	struct crudasm_temp_node_t result;
	result.size = a.size;
	
	if(result.size == 1)
		result.value[0] = (S1)(a.value[0]) / (S1)(b.value[0]);
	else
	if(result.size == 2)
		result.value[0] = (S2)(a.value[0]) / (S2)(b.value[0]);
	else
	if(result.size == 4)
		result.value[0] = (S4)(a.value[0]) / (S4)(b.value[0]);
	else
	{
#ifdef GUEST64ENABLED
		muldiv64_a = a.value[0];
		muldiv64_b = b.value[0];
		muldiv64_sdiv();
		result.value[0] = muldiv64_result;
#else
		result.value[0] = 0;
#endif
	}
	
	return result;
}

static struct crudasm_temp_node_t internal_compute_2_smod(struct crudasm_intel_context_t *context, struct crudasm_temp_node_t a, struct crudasm_temp_node_t b)
{
	struct crudasm_temp_node_t result;
	result.size = a.size;
	
	if(result.size == 1)
		result.value[0] = (S1)(a.value[0]) % (S1)(b.value[0]);
	else
	if(result.size == 2)
		result.value[0] = (S2)(a.value[0]) % (S2)(b.value[0]);
	else
	if(result.size == 4)
		result.value[0] = (S4)(a.value[0]) % (S4)(b.value[0]);
	else
	{
#ifdef GUEST64ENABLED
		muldiv64_a = a.value[0];
		muldiv64_b = b.value[0];
		muldiv64_smod();
		result.value[0] = muldiv64_result;
#else
		result.value[0] = 0;
#endif
	}
	
	return result;
}

//===============================================================================================//

static struct crudasm_temp_node_t internal_compute_1_neg(struct crudasm_intel_context_t *context, struct crudasm_temp_node_t a)
{
	struct crudasm_temp_node_t result;
	result.size = a.size;
	result.value[0] = a.value[0];
	if(result.size == 1)
		result.value[0] = -(U1)(result.value[0]);
	else
	if(result.size == 2)
		result.value[0] = -(U2)(result.value[0]);
	else
	if(result.size == 4)
		result.value[0] = -(U4)(result.value[0]);
	else
		result.value[0] = -result.value[0];
	return result;
}

static struct crudasm_temp_node_t internal_compute_1_bitnot(struct crudasm_intel_context_t *context, struct crudasm_temp_node_t a)
{
	struct crudasm_temp_node_t result;
	result.size = a.size;
	result.value[0] = ~a.value[0];
	if(result.size == 1)
		result.value[0] = (U1)(result.value[0]);
	else
	if(result.size == 2)
		result.value[0] = (U2)(result.value[0]);
	else
	if(result.size == 4)
		result.value[0] = (U4)(result.value[0]);
	return result;
}

static struct crudasm_temp_node_t internal_compute_1_ofs16(struct crudasm_intel_context_t *context, struct crudasm_temp_node_t a)
{
	struct crudasm_temp_node_t result;
	result.size = 2;
	result.value[0] = *(U2 *)(a.value);
	return result;
}

static struct crudasm_temp_node_t internal_compute_1_ofs32(struct crudasm_intel_context_t *context, struct crudasm_temp_node_t a)
{
	struct crudasm_temp_node_t result;
	result.size = 4;
	result.value[0] = *(U4 *)(a.value);
	return result;
}

static struct crudasm_temp_node_t internal_compute_1_ofs64(struct crudasm_intel_context_t *context, struct crudasm_temp_node_t a)
{
	struct crudasm_temp_node_t result;
	result.size = 8;
	result.value[0] = *(U8 *)(a.value);
	return result;
}

static struct crudasm_temp_node_t internal_compute_1_seg16(struct crudasm_intel_context_t *context, struct crudasm_temp_node_t a)
{
	struct crudasm_temp_node_t result;
	result.size = 2;
	result.value[0] = *(U2 *)((U1 *)(a.value) + 2);
	return result;
}

static struct crudasm_temp_node_t internal_compute_1_seg32(struct crudasm_intel_context_t *context, struct crudasm_temp_node_t a)
{
	struct crudasm_temp_node_t result;
	result.size = 2;
	result.value[0] = *(U2 *)((U1 *)(a.value) + 4);
	return result;
}

static struct crudasm_temp_node_t internal_compute_1_seg64(struct crudasm_intel_context_t *context, struct crudasm_temp_node_t a)
{
	struct crudasm_temp_node_t result;
	result.size = 2;
	result.value[0] = (U2)(a.value[1]);
	return result;
}

static struct crudasm_temp_node_t internal_compute_1__lognot(struct crudasm_intel_context_t *context, struct crudasm_temp_node_t a)
{
	struct crudasm_temp_node_t result;
	result.size = 1;
	if(a.value[0] & 1)
		result.value[0] = 0;
	else
		result.value[0] = 1;
	return result;
}

static struct crudasm_temp_node_t internal_compute_1__lsb(struct crudasm_intel_context_t *context, struct crudasm_temp_node_t a)
{
	struct crudasm_temp_node_t result;
	result.size = 1;
	result.value[0] = a.value[0] & 1;
	return result;
}

static struct crudasm_temp_node_t internal_compute_1__parity(struct crudasm_intel_context_t *context, struct crudasm_temp_node_t a)
{
	struct crudasm_temp_node_t c;
	U1 d = (U1)(a.value[0]);
	U1 pf = 1;
	size_t i;
	c.size = 1;
	for(i = 0; i < 8; ++i)
		pf ^= (d >> i) & 1;
	c.value[0] = pf;
	return c;
}

static struct crudasm_temp_node_t internal_compute_1__sign(struct crudasm_intel_context_t *context, struct crudasm_temp_node_t a)
{
	size_t index = a.size * 8 - 1;
	U8 mask = SHL64(1, index);
	struct crudasm_temp_node_t result;
	result.size = 1;
	result.value[0] = ((a.value[0] & mask) != 0) ? 1 : 0;
	return result;
}

static struct crudasm_temp_node_t internal_compute_1__sign2(struct crudasm_intel_context_t *context, struct crudasm_temp_node_t a)
{
	size_t index = a.size * 8 - 2;
	U8 mask = SHL64(1, index);
	struct crudasm_temp_node_t result;
	result.size = 1;
	result.value[0] = ((a.value[0] & mask) != 0) ? 1 : 0;
	return result;
}

static struct crudasm_temp_node_t internal_compute_1__zero(struct crudasm_intel_context_t *context, struct crudasm_temp_node_t a)
{
	U1 *pvalue = (U1 *)(a.value);
	size_t i;
	struct crudasm_temp_node_t result;
	result.size = 0;
	for(i = 0; i < a.size; ++i)
	{
		if(pvalue[i] != 0)
		{
			result.value[0] = 0;	// not all zeros
			return result;
		}
	}
	result.value[0] = 1;	// is all zeros
	return result;
}

static struct crudasm_temp_node_t internal_compute_2_add(struct crudasm_intel_context_t *context, struct crudasm_temp_node_t a, struct crudasm_temp_node_t b)
{
	struct crudasm_temp_node_t result;
	result.size = a.size;
	
	if(a.size == 1)
		*(U1 *)(result.value) = *(U1 *)(a.value) + *(U1 *)(b.value);
	else
	if(a.size == 2)
		*(U2 *)(result.value) = *(U2 *)(a.value) + *(U2 *)(b.value);
	else
	if(a.size == 4)
		*(U4 *)(result.value) = *(U4 *)(a.value) + *(U4 *)(b.value);
	else
		*(U8 *)(result.value) = *(U8 *)(a.value) + *(U8 *)(b.value);
	
	return result;
}

static struct crudasm_temp_node_t internal_compute_2_sub(struct crudasm_intel_context_t *context, struct crudasm_temp_node_t a, struct crudasm_temp_node_t b)
{
	struct crudasm_temp_node_t result;
	result.size = a.size;
	
	if(a.size == 1)
		*(U1 *)(result.value) = *(U1 *)(a.value) - *(U1 *)(b.value);
	else
	if(a.size == 2)
		*(U2 *)(result.value) = *(U2 *)(a.value) - *(U2 *)(b.value);
	else
	if(a.size == 4)
		*(U4 *)(result.value) = *(U4 *)(a.value) - *(U4 *)(b.value);
	else
		*(U8 *)(result.value) = *(U8 *)(a.value) - *(U8 *)(b.value);
	
	return result;
}

static struct crudasm_temp_node_t internal_compute_2__eq(struct crudasm_intel_context_t *context, struct crudasm_temp_node_t a, struct crudasm_temp_node_t b)
{
	struct crudasm_temp_node_t result;
	result.size = 1;

	if(a.size == 1)
		result.value[0] = *(U1 *)(a.value) == *(U1 *)(b.value);
	else
	if(a.size == 2)
		result.value[0] = *(U2 *)(a.value) == *(U2 *)(b.value);
	else
	if(a.size == 4)
		result.value[0] = *(U4 *)(a.value) == *(U4 *)(b.value);
	else
		result.value[0] = *(U8 *)(a.value) == *(U8 *)(b.value);
	
	if(result.value[0] != 0)
		result.value[0] = 1;
	return result;
}

static struct crudasm_temp_node_t internal_compute_2__gtu(struct crudasm_intel_context_t *context, struct crudasm_temp_node_t a, struct crudasm_temp_node_t b)
{
	struct crudasm_temp_node_t result;
	result.size = 1;

	if(a.size == 1)
		result.value[0] = *(U1 *)(a.value) > *(U1 *)(b.value);
	else
	if(a.size == 2)
		result.value[0] = *(U2 *)(a.value) > *(U2 *)(b.value);
	else
	if(a.size == 4)
		result.value[0] = *(U4 *)(a.value) > *(U4 *)(b.value);
	else
		result.value[0] = *(U8 *)(a.value) > *(U8 *)(b.value);
	
	if(result.value[0] != 0)
		result.value[0] = 1;
	return result;
}

static struct crudasm_temp_node_t internal_compute_2__ltu(struct crudasm_intel_context_t *context, struct crudasm_temp_node_t a, struct crudasm_temp_node_t b)
{
	struct crudasm_temp_node_t result;
	result.size = 1;

	if(a.size == 1)
		result.value[0] = *(U1 *)(a.value) < *(U1 *)(b.value);
	else
	if(a.size == 2)
		result.value[0] = *(U2 *)(a.value) < *(U2 *)(b.value);
	else
	if(a.size == 4)
		result.value[0] = *(U4 *)(a.value) < *(U4 *)(b.value);
	else
		result.value[0] = *(U8 *)(a.value) < *(U8 *)(b.value);
	
	if(result.value[0] != 0)
		result.value[0] = 1;
	return result;
}

static struct crudasm_temp_node_t internal_compute_2_bitand(struct crudasm_intel_context_t *context, struct crudasm_temp_node_t a, struct crudasm_temp_node_t b)
{
	struct crudasm_temp_node_t result;

	result.size = a.size;
	result.value[0] = a.value[0] & b.value[0];
	
	if(a.size == 1)
		result.value[0] = (U1)(result.value[0]);
	else
	if(a.size == 2)
		result.value[0] = (U2)(result.value[0]);
	else
	if(a.size == 4)
		result.value[0] = (U4)(result.value[0]);

	return result;
}

static struct crudasm_temp_node_t internal_compute_2_bitor(struct crudasm_intel_context_t *context, struct crudasm_temp_node_t a, struct crudasm_temp_node_t b)
{
	struct crudasm_temp_node_t result;

	result.size = a.size;
	result.value[0] = a.value[0] | b.value[0];
	
	if(a.size == 1)
		result.value[0] = (U1)(result.value[0]);
	else
	if(a.size == 2)
		result.value[0] = (U2)(result.value[0]);
	else
	if(a.size == 4)
		result.value[0] = (U4)(result.value[0]);

	return result;
}

static struct crudasm_temp_node_t internal_compute_2_bitxor(struct crudasm_intel_context_t *context, struct crudasm_temp_node_t a, struct crudasm_temp_node_t b)
{
	struct crudasm_temp_node_t result;

	result.size = a.size;
	result.value[0] = a.value[0] ^ b.value[0];
	
	if(a.size == 1)
		result.value[0] = (U1)(result.value[0]);
	else
	if(a.size == 2)
		result.value[0] = (U2)(result.value[0]);
	else
	if(a.size == 4)
		result.value[0] = (U4)(result.value[0]);

	return result;
}

static struct crudasm_temp_node_t internal_compute_2__logor(struct crudasm_intel_context_t *context, struct crudasm_temp_node_t a, struct crudasm_temp_node_t b)
{
	struct crudasm_temp_node_t result;
	result.size = 1;
	result.value[0] = ((a.value[0] & 1) | (b.value[0] & 1)) ? 1 : 0;
	return result;
}

static struct crudasm_temp_node_t internal_compute_2__logxor(struct crudasm_intel_context_t *context, struct crudasm_temp_node_t a, struct crudasm_temp_node_t b)
{
	struct crudasm_temp_node_t result;
	result.size = 1;
	result.value[0] = ((a.value[0] & 1) ^ (b.value[0] & 1)) ? 1 : 0;
	return result;
}

static struct crudasm_temp_node_t internal_compute_2__add_af(struct crudasm_intel_context_t *context, struct crudasm_temp_node_t a, struct crudasm_temp_node_t b)
{
	struct crudasm_temp_node_t result;
	U1 aa = a.value[0] & 0x0f, bb = b.value[0] & 0x0f;
	U1 c = aa + bb;
	result.size = 1;
	result.value[0] = (c > 0x0f) ? 1 : 0;
	return result;
}

static struct crudasm_temp_node_t internal_compute_2__sub_af(struct crudasm_intel_context_t *context, struct crudasm_temp_node_t a, struct crudasm_temp_node_t b)
{
	struct crudasm_temp_node_t result;
	U1 aa = a.value[0] & 0x0f, bb = b.value[0] & 0x0f;
	result.size = 1;
	result.value[0] = aa < bb;
	return result;
}

static struct crudasm_temp_node_t internal_compute_2__sub_cf(struct crudasm_intel_context_t *context, struct crudasm_temp_node_t a, struct crudasm_temp_node_t b)
{
	struct crudasm_temp_node_t result;
	U1 aa1, bb1;
	U2 aa2, bb2;
	U4 aa4, bb4;
	U8 aa8, bb8;
	result.size = 1;
	
	if(a.size == 1)
	{
		aa1 = a.value[0];
		bb1 = b.value[0];
		result.value[0] = (aa1 < bb1) ? 1 : 0;
	}
	else
	if(a.size == 2)
	{
		aa2 = a.value[0];
		bb2 = b.value[0];
		result.value[0] = (aa2 < bb2) ? 1 : 0;
	}
	else
	if(a.size == 4)
	{
		aa4 = a.value[0];
		bb4 = b.value[0];
		result.value[0] = (aa4 < bb4) ? 1 : 0;
	}
	else
	{
		aa8 = a.value[0];
		bb8 = b.value[0];
		result.value[0] = (aa8 < bb8) ? 1 : 0;
	}

	return result;
}

static struct crudasm_temp_node_t internal_compute_2__sub_of(struct crudasm_intel_context_t *context, struct crudasm_temp_node_t a, struct crudasm_temp_node_t b)
{
	struct crudasm_temp_node_t result;
	U1 aa1, bb1, less1, sign1;
	S1 cc1;
	U2 aa2, bb2, less2, sign2;
	S2 cc2;
	U4 aa4, bb4, less4, sign4;
	S4 cc4;
	U8 aa8, bb8, less8, sign8;
	S8 cc8;
	result.size = 1;
	
	if(a.size == 1)
	{
		aa1 = a.value[0];
		bb1 = b.value[0];
		less1 = (aa1 < bb1) ? 1 : 0;
		cc1 = aa1 - bb1;
		sign1 = cc1 < 0;
		result.value[0] = (sign1 ^ less1) ? 1 : 0;
	}
	else
	if(a.size == 2)
	{
		aa2 = a.value[0];
		bb2 = b.value[0];
		less2 = (aa2 < bb2) ? 1 : 0;
		cc2 = aa2 - bb2;
		sign2 = cc2 < 0;
		result.value[0] = (sign2 ^ less2) ? 1 : 0;
	}
	else
	if(a.size == 4)
	{
		aa4 = a.value[0];
		bb4 = b.value[0];
		less4 = (aa4 < bb4) ? 1 : 0;
		cc4 = aa4 - bb4;
		sign4 = cc4 < 0;
		result.value[0] = (sign4 ^ less4) ? 1 : 0;
	}
	else
	{
		aa8 = a.value[0];
		bb8 = b.value[0];
		less8 = (aa8 < bb8) ? 1 : 0;
		cc8 = aa8 - bb8;
		sign8 = cc8 < 0;
		result.value[0] = (sign8 ^ less8) ? 1 : 0;
	}
	
	return result;
}

static struct crudasm_temp_node_t internal_compute_2__add_cf(struct crudasm_intel_context_t *context, struct crudasm_temp_node_t a, struct crudasm_temp_node_t b)
{
	struct crudasm_temp_node_t result;
	U1 aa1, bb1, cc1;
	U2 aa2, bb2, cc2;
	U4 aa4, bb4, cc4;
	U8 aa8, bb8, cc8;
	result.size = 1;

	if(a.size == 1)
	{
		aa1 = a.value[0];
		bb1 = b.value[0];
		cc1 = aa1 + bb1;
		result.value[0] = (cc1 < aa1) ? 1 : 0;
	}
	else
	if(a.size == 2)
	{
		aa2 = a.value[0];
		bb2 = b.value[0];
		cc2 = aa2 + bb2;
		result.value[0] = (cc2 < aa2) ? 1 : 0;
	}
	else
	if(a.size == 4)
	{
		aa4 = a.value[0];
		bb4 = b.value[0];
		cc4 = aa4 + bb4;
		result.value[0] = (cc4 < aa4) ? 1 : 0;
	}
	else
	{
		aa8 = a.value[0];
		bb8 = b.value[0];
		cc8 = aa8 + bb8;
		result.value[0] = (cc8 < aa8) ? 1 : 0;
	}

	return result;
}

static struct crudasm_temp_node_t internal_compute_2__add_of(struct crudasm_intel_context_t *context, struct crudasm_temp_node_t a, struct crudasm_temp_node_t b)
{
	struct crudasm_temp_node_t result;
	result.size = 1;
	if(a.size == 1 && (U1)(a.value[0]) == TULL(0x80) ||
		a.size == 2 && (U1)(a.value[0]) == TULL(0x8000) ||
		a.size == 4 && (U1)(a.value[0]) == TULL(0x80000000) ||
		a.size == 8 && (U1)(a.value[0]) == TULL(0x8000000000000000)
	)
		result.value[0] = 1;
	else
		result.value[0] = internal_compute_2__sub_of(context, a, internal_compute_1_neg(context, b)).value[0];
	return result;
}

static struct crudasm_temp_node_t internal_compute_3__adc_af(struct crudasm_intel_context_t *context, struct crudasm_temp_node_t a, struct crudasm_temp_node_t b, struct crudasm_temp_node_t c)
{
	struct crudasm_temp_node_t result;
	U1 aa, bb, cc, d;
	result.size = 1;
	aa = a.value[0] & 0x0f;
	bb = b.value[0] & 0x0f;
	cc = c.value[0] & 1;		// input cf -- 0 or 1
	d = aa + bb + cc;
	result.value[0] = (d > 0x0f) ? 1 : 0;
}

static struct crudasm_temp_node_t internal_compute_3__adc_cf(struct crudasm_intel_context_t *context, struct crudasm_temp_node_t a, struct crudasm_temp_node_t b, struct crudasm_temp_node_t c)
{
	struct crudasm_temp_node_t result, one;
	result.size = 1;
	result.value[0] = internal_compute_2__add_cf(context, a, b).value[0];
	one.size = a.size;
	one.value[0] = c.value[0] & 1;	
	if(internal_compute_2__add_cf(context, internal_compute_2_add(context, a, b), one).value[0] & 1)
		result.value[0] = 1;
	return result;
}

static struct crudasm_temp_node_t internal_compute_3__adc_of(struct crudasm_intel_context_t *context, struct crudasm_temp_node_t a, struct crudasm_temp_node_t b, struct crudasm_temp_node_t c)
{
	struct crudasm_temp_node_t result, one;
	result.size = 1;
	result.value[0] = internal_compute_2__add_of(context, a, b).value[0];
	one.size = a.size;
	one.value[0] = c.value[0] & 1;	
	if(internal_compute_2__add_of(context, internal_compute_2_add(context, a, b), one).value[0] & 1)
		result.value[0] = 1;
	return result;
}

static struct crudasm_temp_node_t internal_compute_3__sbb_af(struct crudasm_intel_context_t *context, struct crudasm_temp_node_t a, struct crudasm_temp_node_t b, struct crudasm_temp_node_t c)
{
	struct crudasm_temp_node_t result;
	U1 aa, bb;
	result.size = 1;
	aa = a.value[0] & 0x0f;
	bb = b.value[0] & 0x0f;
	result.value[0] = (aa < bb) ? 1 : 0;
	if(aa == bb)
	{
		if(c.value[0] & 1)
			result.value[0] = 1;
	}
	return result;
}

static struct crudasm_temp_node_t internal_compute_3__sbb_cf(struct crudasm_intel_context_t *context, struct crudasm_temp_node_t a, struct crudasm_temp_node_t b, struct crudasm_temp_node_t c)
{
	struct crudasm_temp_node_t result, one;
	result.size = 1;
	result.value[0] = internal_compute_2__sub_cf(context, a, b).value[0];
	one.size = 1;
	one.value[0] = c.value[0];
	if(internal_compute_2__sub_cf(context, internal_compute_2_sub(context, a, b), one).value[0] & 1)
		result.value[0] = 1;
	return result;
}

static struct crudasm_temp_node_t internal_compute_3__sbb_of(struct crudasm_intel_context_t *context, struct crudasm_temp_node_t a, struct crudasm_temp_node_t b, struct crudasm_temp_node_t c)
{
	struct crudasm_temp_node_t result, one;
	result.size = 1;
	result.value[0] = internal_compute_2__sub_of(context, a, b).value[0];
	one.size = 1;
	one.value[0] = c.value[0];
	if(internal_compute_2__sub_of(context, internal_compute_2_sub(context, a, b), one).value[0] & 1)
		result.value[0] = 1;
	return result;
}

static struct crudasm_temp_node_t internal_compute_3_quest(struct crudasm_intel_context_t *context, struct crudasm_temp_node_t a, struct crudasm_temp_node_t b, struct crudasm_temp_node_t c)
{
	return (a.value[0] & 1) ? b : c;
}

static struct crudasm_temp_node_t internal_compute_3__quest(struct crudasm_intel_context_t *context, struct crudasm_temp_node_t a, struct crudasm_temp_node_t b, struct crudasm_temp_node_t c)
{
	return internal_compute_3_quest(context, a, b, c);
}

static struct crudasm_temp_node_t internal_compute_2_shrs(struct crudasm_intel_context_t *context, struct crudasm_temp_node_t a, struct crudasm_temp_node_t b)
{
	static struct crudasm_temp_node_t r;
	r.size = a.size;
	U1 count = (U1)(b.value[0]);
	
	if(r.size == 1)
		*(S1 *)(r.value) = *(S1 *)(a.value) << count;
	else
	if(r.size == 2)
		*(S2 *)(r.value) = *(S2 *)(a.value) << count;
	else
	if(r.size == 4)
		*(S4 *)(r.value) = *(S4 *)(a.value) << count;
	else
		r.value[0] = SHR64S(a.value[0], count);
	
	return r;
}

static struct crudasm_temp_node_t internal_compute_2_shru(struct crudasm_intel_context_t *context, struct crudasm_temp_node_t a, struct crudasm_temp_node_t b)
{
	static struct crudasm_temp_node_t r;
	r.size = a.size;
	U1 count = (U1)(b.value[0]);
	
	if(r.size == 1)
		*(U1 *)(r.value) = *(U1 *)(a.value) >> count;
	else
	if(r.size == 2)
		*(U2 *)(r.value) = *(U2 *)(a.value) >> count;
	else
	if(r.size == 4)
		*(U4 *)(r.value) = *(U4 *)(a.value) >> count;
	else
		r.value[0] = SHR64U(a.value[0], count);
	
	return r;
}

static struct crudasm_temp_node_t internal_compute_2_shl(struct crudasm_intel_context_t *context, struct crudasm_temp_node_t a, struct crudasm_temp_node_t b)
{
	static struct crudasm_temp_node_t r;
	r.size = a.size;
	U1 count = (U1)(b.value[0]);
	
	if(r.size == 1)
		*(U1 *)(r.value) = *(U1 *)(a.value) << count;
	else
	if(r.size == 2)
		*(U2 *)(r.value) = *(U2 *)(a.value) << count;
	else
	if(r.size == 4)
		*(U4 *)(r.value) = *(U4 *)(a.value) << count;
	else
		r.value[0] = SHL64(a.value[0], count);
	
	return r;
}

static struct crudasm_temp_node_t internal_compute_2_rol(struct crudasm_intel_context_t *context, struct crudasm_temp_node_t a, struct crudasm_temp_node_t b)
{
	struct crudasm_temp_node_t x = a;
	U1 tmpcount = (U1)(b.value[0]);
	struct crudasm_temp_node_t tmpcf;
	struct crudasm_temp_node_t one;

	tmpcf.size = a.size;
	tmpcf.value[0] = 0;
	one.size = 1;
	one.value[0] = 1;
	
	while(tmpcount != 0)
	{
		tmpcf.value[0] = (U8)(U1)internal_compute_1__sign(context, x).value[0];
		x = internal_compute_2_add(context, internal_compute_2_shl(context, x, one), tmpcf);
		--tmpcount;
	}
	
	return x;
}

static struct crudasm_temp_node_t internal_compute_2_ror(struct crudasm_intel_context_t *context, struct crudasm_temp_node_t a, struct crudasm_temp_node_t b)
{
	struct crudasm_temp_node_t x = a;
	U1 tmpcount = (U1)b.value[0];
	struct crudasm_temp_node_t one;
	U8 adder, aa;
	
	one.size = 1;
	one.value[0] = 1;
	
	adder = SHL64(1, a.size * 8 - 1);
	
	while(tmpcount != 0)
	{
		if((x.value[0] & 1) != 0)
			aa = adder;
		else
			aa = 0;
		x = internal_compute_2_shru(context, x, one);
		x.value[0] += aa;
		
		--tmpcount;
	}
	
	return x;
}

static void do_rcr(struct crudasm_intel_context_t *context, struct crudasm_temp_node_t dest, struct crudasm_temp_node_t intmpcount, struct crudasm_temp_node_t incf, struct crudasm_temp_node_t *result, struct crudasm_temp_node_t *outcf)
{
	struct crudasm_temp_node_t tmpcf, adder, tmpcount, one;
	U8 bignum = SHL64(1, dest.size * 8 - 1);
	
	outcf->size = 1;
	outcf->value[0] = (U1)incf.value[0];
	*result = dest;
	
	tmpcf.size = 1;
	adder.size = dest.size;
	adder.value[0] = 0;
	tmpcount = intmpcount;
	tmpcount.value[0] = (U1)(tmpcount.value[0]);
	one.size = dest.size;
	one.value[0] = 1;
	
	while(tmpcount.value[0] != 0)
	{
		tmpcf.value[0] = result->value[0] & 1;
		adder.value[0] = (outcf->value[0] != 0) ? bignum : 0;
		
		*result = internal_compute_2_add(context, internal_compute_2_shru(context, *result, one), adder);
		outcf->value[0] = tmpcf.value[0];
		--tmpcount.value[0];
	}
}

static void do_rcl(struct crudasm_intel_context_t *context, struct crudasm_temp_node_t dest, struct crudasm_temp_node_t intmpcount, struct crudasm_temp_node_t incf, struct crudasm_temp_node_t *result, struct crudasm_temp_node_t *outcf)
{
	struct crudasm_temp_node_t tmpcf, adder, tmpcount, one;
	
	outcf->size = 1;
	outcf->value[0] = (U1)incf.value[0];
	*result = dest;
	
	tmpcf.size = 1;
	adder.size = dest.size;
	adder.value[0] = 0;
	tmpcount = intmpcount;
	tmpcount.value[0] = (U1)(tmpcount.value[0]);
	one.size = dest.size;
	one.value[0] = 1;
	
	while(tmpcount.value[0] != 0)
	{
		tmpcf.value[0] = internal_compute_1__sign(context, *result).value[0];
		adder.value[0] = (outcf->value[0] != 0) ? 1 : 0;
		
		*result = internal_compute_2_add(context, internal_compute_2_shl(context, *result, one), adder);
		outcf->value[0] = tmpcf.value[0];
		--tmpcount.value[0];
	}
}

static struct crudasm_temp_node_t internal_compute_3_rcl(struct crudasm_intel_context_t *context, struct crudasm_temp_node_t a, struct crudasm_temp_node_t b, struct crudasm_temp_node_t incf)
{
	struct crudasm_temp_node_t result, outcf;
	result.size = a.size;
	outcf.size = 1;
	do_rcl(context, a, b, incf, &result, &outcf);
	return result;
}

static struct crudasm_temp_node_t internal_compute_3__rcl_cf(struct crudasm_intel_context_t *context, struct crudasm_temp_node_t a, struct crudasm_temp_node_t b, struct crudasm_temp_node_t incf)
{
	struct crudasm_temp_node_t result, outcf;
	result.size = a.size;
	outcf.size = 1;
	do_rcl(context, a, b, incf, &result, &outcf);
	return outcf;
}

static struct crudasm_temp_node_t internal_compute_3_rcr(struct crudasm_intel_context_t *context, struct crudasm_temp_node_t a, struct crudasm_temp_node_t b, struct crudasm_temp_node_t incf)
{
	struct crudasm_temp_node_t result, outcf;
	result.size = a.size;
	outcf.size = 1;
	do_rcr(context, a, b, incf, &result, &outcf);
	return result;
}

static struct crudasm_temp_node_t internal_compute_3__rcr_cf(struct crudasm_intel_context_t *context, struct crudasm_temp_node_t a, struct crudasm_temp_node_t b, struct crudasm_temp_node_t incf)
{
	struct crudasm_temp_node_t result, outcf;
	result.size = a.size;
	outcf.size = 1;
	do_rcl(context, a, b, incf, &result, &outcf);
	return outcf;
}

static struct crudasm_temp_node_t internal_compute_2__shl_cf(struct crudasm_intel_context_t *context, struct crudasm_temp_node_t tmpdest, struct crudasm_temp_node_t tmpcount)
{
	struct crudasm_temp_node_t dest, one;
	struct crudasm_temp_node_t result, count;
	result.size = 1;
	count.size = 1;
	count.value[0] = (U1)tmpcount.value[0];

	// Return value is not used if tmpcount == 0 on input. So,
	// we just return 0 in that case.
	if(count.value[0] == 0)
	{
		result.value[0] = 0;
		return result;
	}
	
	dest = tmpdest;
	one.size = 1;
	one.value[0] = 1;
	
	while(count.value[0] != 0)
	{
		result.value[0] = internal_compute_1__sign(context, dest).value[0];
		dest = internal_compute_2_shl(context, dest, one);
		--count.value[0];
	}
	
	return result;
}

static struct crudasm_temp_node_t internal_compute_2__sar_cf(struct crudasm_intel_context_t *context, struct crudasm_temp_node_t tmpdest, struct crudasm_temp_node_t tmpcount)
{
	struct crudasm_temp_node_t dest, one;
	struct crudasm_temp_node_t result, count;
	result.size = 1;
	count.size = 1;
	count.value[0] = (U1)tmpcount.value[0];

	// Return value is not used if tmpcount == 0 on input. So,
	// we just return 0 in that case.
	if(count.value[0] == 0)
	{
		result.value[0] = 0;
		return result;
	}
	
	dest = tmpdest;
	one.size = 1;
	one.value[0] = 1;
	
	while(count.value[0] != 0)
	{
		result.value[0] = dest.value[0] & 1;
		dest = internal_compute_2_shrs(context, dest, one);
		--count.value[0];
	}
	
	return result;
}

static struct crudasm_temp_node_t internal_compute_2__shr_cf(struct crudasm_intel_context_t *context, struct crudasm_temp_node_t tmpdest, struct crudasm_temp_node_t tmpcount)
{
	struct crudasm_temp_node_t dest, one;
	struct crudasm_temp_node_t result, count;
	result.size = 1;
	count.size = 1;
	count.value[0] = (U1)tmpcount.value[0];

	// Return value is not used if tmpcount == 0 on input. So,
	// we just return 0 in that case.
	if(count.value[0] == 0)
	{
		result.value[0] = 0;
		return result;
	}
	
	dest = tmpdest;
	one.size = 1;
	one.value[0] = 1;
	
	while(count.value[0] != 0)
	{
		result.value[0] = dest.value[0] & 1;
		dest = internal_compute_2_shru(context, dest, one);
		--count.value[0];
	}
	
	return result;
}

static struct crudasm_temp_node_t internal_compute_tr(struct crudasm_intel_context_t *context, U1 size, struct crudasm_temp_node_t src)
{
	src.size = size;
	return src;
}

static struct crudasm_temp_node_t internal_compute_sx(struct crudasm_intel_context_t *context, U1 size, struct crudasm_temp_node_t src)
{
	if(size < src.size)
		;
	else
	if(src.size == 1)
		src.value[0] = (U8)(S8)(S1)(src.value[0]);
	else
	if(src.size == 2)
		src.value[0] = (U8)(S8)(S2)(src.value[0]);
	else
	if(src.size == 4)
		src.value[0] = (U8)(S8)(S4)(src.value[0]);
	// leave value alone if src.size == 8.
	src.size = size;
	return src;
}

static struct crudasm_temp_node_t internal_compute_zx(struct crudasm_intel_context_t *context, U1 size, struct crudasm_temp_node_t src)
{
	if(size < src.size)
		;
	else
	if(src.size == 1)
		src.value[0] = (U8)(U1)(src.value[0]);
	else
	if(src.size == 2)
		src.value[0] = (U8)(U2)(src.value[0]);
	else
	if(src.size == 4)
		src.value[0] = (U8)(U4)(src.value[0]);
	// leave value alone if src.size == 8.
	src.size = size;
	return src;
}

static void do_write_extern_RIPADD(struct crudasm_intel_context_t *context, struct crudasm_temp_node_t *src)
{
	context->rip_add = src->value[0];
}

static void do_write_extern_RIPNEXT(struct crudasm_intel_context_t *context, struct crudasm_temp_node_t *src)
{
	context->rip_next = src->value[0];
}

//===============================================================================================//

#include "../generated/out_intel_emulator.h"

//===============================================================================================//

#include <stdlib.h>
#include <stddef.h>

struct simpc_context
{
	void *user;
	U1 *ram;
	U1 *floppy;
	U8 ram_size;
};

static struct simpc_context *create_simpc_user_context(size_t ram_size)
{
	struct simpc_context *c = (struct simpc_context *)malloc(sizeof(struct simpc_context));
	memset(c, 0, sizeof(struct simpc_context));
	c->user = NULL;
	ram_size = (ram_size + 1024 * 1024 - 1) & ~(1024 * 1024);
	c->ram_size = ram_size;
	c->ram = (U1 *)malloc(ram_size);
	c->floppy = NULL;
	memset(c->ram, 0, ram_size);
	return c;
}

static void destroy_simpc_user_context(void *contextT)
{
	struct crudasm_intel_context_t *context = (struct crudasm_intel_context_t *)(contextT);
	struct simpc_context *c = (struct simpc_context *)context->user_context;
	free(c->floppy);
	free(c->ram);
	free(c);
}

static int simpc_read_physical(struct crudasm_intel_context_t *context, U8 phys_addr, U2 size_bytes, void *destT)
{
	struct simpc_context *c = (struct simpc_context *)context->user_context;
	if(size_bytes == 0)
		return 0;
	if(phys_addr + size_bytes - 1 >= c->ram_size)
		return default_read_physical(context, phys_addr, size_bytes, destT);
	memcpy(destT, c->ram + phys_addr, size_bytes);
	return 0;
}

static int simpc_write_physical(struct crudasm_intel_context_t *context, U8 phys_addr, U2 size_bytes, void *srcT)
{
	struct simpc_context *c = (struct simpc_context *)context->user_context;
	if(size_bytes == 0)
		return 0;
	if(phys_addr + size_bytes - 1 >= c->ram_size)
		return default_write_physical(context, phys_addr, size_bytes, srcT);
	memcpy(c->ram + phys_addr, srcT, size_bytes);
	return 0;
}

static void setup_simpc(struct crudasm_intel_context_t *context, size_t ram_size_bytes)
{
	crudasm_intel_context_init(context, create_simpc_user_context(ram_size_bytes));
	context->do_read_physical = simpc_read_physical;
	context->do_write_physical = simpc_write_physical;
}

static void dump_write_cache(FILE *ft, struct crudasm_intel_context_t *context)
{
	size_t i;
	for(i = 0; i < context->mem_write_cache.cache_size; ++i)
	{
		fprintf(ft, "delta memory physical %08x (%d bytes)\n", (U4)(context->mem_write_cache.entry[i].phys_base +
			context->mem_write_cache.entry[i].dirty_page_offset), (U4)(context->mem_write_cache.entry[i].dirty_size_bytes)
		);
	}
	for(i = 0; i < CRUDASM_INTEL_MAX_ASM_ARGS; ++i)
	{
		if(context->mem_arg_cache[i].is_valid && context->mem_arg_cache[i].need_write)
		{
			fprintf(ft, "write to memory, phys [%08x; %08x]\n", (U4)(context->mem_arg_cache[i].phys[0]), (U4)(context->mem_arg_cache[i].phys[1]));
		}
	}
}

static int fetch_read_linear(struct crudasm_intel_context_t *context, U8 linear, void *destT, U8 *size_bytesT, U1 cpl)
{
	U8 size_bytes = *size_bytesT;
	int need_page_2 = 1;

	U1 *dest = (U1 *)(destT);
	U8 first_page = (linear) & ~TULL(0xfff);
	U8 second_page = (linear + (size_t)(size_bytes - 1)) & ~TULL(0xfff);
	U8 first_phys, second_phys;
	if(first_page == second_page)
	{
		if(translate_linear(context, first_page, cpl, &first_phys, 1/*read only*/) != 0)
			return 1;
		if(context->do_read_physical(context, first_phys + (linear & 0xfff), size_bytes, dest) != 0)
			return 1;
	}
	else
	{
		if(translate_linear(context, first_page, cpl, &first_phys, 1/*read only*/) != 0)
			return 1;
		if(translate_linear(context, second_page, cpl, &second_phys, 1/*read only*/ | 4/*don't fault*/) != 0)
		{
			need_page_2 = 0;
			*size_bytesT = 0x1000 - (linear & 0xfff);
		}
		if(context->do_read_physical(context, first_phys + (linear & 0xfff), 0x1000 - (linear & 0xfff), dest) != 0)
			return 1;
		if(need_page_2)
		{
			if(context->do_read_physical(context, second_phys, size_bytes - (0x1000 - (linear & 0xfff)), dest + (0x1000 - (linear & 0xfff))) != 0)
				return 1;
		}
	}
	return 0;
}

// Returns 0..30 (number of bytes fetched)
static U1 crudasm_intel_fetch(struct crudasm_intel_context_t *context, U1 *code)
{
	U8 read_max = 30;
	U8 offset = context->rip;
	struct crudasm_intel_descriptor_t *descr = get_cached_descriptor(context, rCS);

	if(context->seg_max_limit_check != 0)
	{
		// This is intended to be used in 64bit mode, to catch e.g. mov ax,[-1].
		if(offset + (read_max - 1) < offset)
		{
			read_max = -offset;
		}
	}
	
	if(context->seg_descr_limit_check != 0 && descr->limit <= TULL(0xffffffff))
	{
		read_max = (U8)(descr->limit) + (U8)(1) - (U8)(offset);
	}

	if(read_max > 30)
		read_max = 30;
	if(read_max == 0)
		return 0;
	
	offset += descr->base;	// convert to linear address

	if(fetch_read_linear(context, offset, code, &read_max, context->cpl /* virtual reads are at CPL */) != 0)
		return 0;
	
	return read_max;
}

//===============================================================================================//

static void simpc_real_mode_initialize(struct crudasm_intel_context_t *context, const char *floppy_img_name)
{
	U1 vec1e[11] = {0xaf, 0x02, 0x25, 0x02, 0x12, 0x1b, 0xff, 0x6c, 0xf6, 0x0f, 0x08};
	U1 himem[] = {'0', '7', '/', '0', '5', '/', '0', '7', '\0', 0xfc, 0x24};
	U1 raw[10] = {0x08, 0x00, 0xfc, 0x00, 0x01, 0x74, 0x40, 0x00, 0x00, 0x00};
	int i;
	struct simpc_context *c = (struct simpc_context *)context->user_context;
	FILE *fi = fopen(floppy_img_name, "rb");
	if(fi == NULL)
	{
		fprintf(stderr, "Error: unable to read: %s\n", floppy_img_name);
		exit(1);
	}
	c->floppy = (U1 *)malloc(1474560);
	if(fread(c->floppy, 1474560, 1, fi) != 1)
	{
		fclose(fi);
		fprintf(stderr, "Error: unable to read: %s\n", floppy_img_name);
		exit(1);
	}
	fclose(fi);

	context->basic_state_cur.sregs[rCS] = 0xf000;
	context->basic_state_cur.sreg_cache[rCS].base = 0xf0000;
	context->rip = 0x0019 * 2;
	
	memset(c->ram + 0xa0000, 0xff, 0x100000 - 0xa0000);
	for(i = 0; i < 256; ++i)
	{
		c->ram[0xf0000 + i * 2] = 0x90;
		c->ram[0xf0000 + i * 2 + 1] = 0xcf;
		*(U2 *)(c->ram + 4 * i + 0) = 2 * i;
		*(U2 *)(c->ram + 4 * i + 2) = 0xf000;
	}
	*(U2 *)(c->ram + 0x1e * 4 + 0) = 0xefc7;
	*(U2 *)(c->ram + 0x1e * 4 + 2) = 0xf000;
	memcpy(c->ram + 0xfefc7, vec1e, 11);

	memcpy(c->ram + 0xffff5, himem, 11);
	memcpy(c->ram + 0xfe6f5, raw, 10);
	
	*(U2 *)(c->ram + 0x410) = 0x4225;	// int 0x11 value
	*(U2 *)(c->ram + 0x413) = 640;		// 640kb conventional ram
}

static int simple_read_disk(struct crudasm_intel_context_t *context)
{
	struct crudasm_temp_node_t tmp;
	U1 al, ch, cl, dh, dl;
	U4 sector, head, cyl, lba;
	U1 *src;
	struct simpc_context *c = (struct simpc_context *)context->user_context;
	U8 linear;
	
	tmp = internal_get_reg8(context, 16 + rCX /* CH */);
	ch = tmp.value[0];

	tmp = internal_get_reg8(context, rCX);
	cl = tmp.value[0];

	tmp = internal_get_reg8(context, rAX);
	al = tmp.value[0];

	tmp = internal_get_reg8(context, 16 + rDX /* DH */);
	dh = tmp.value[0];

	tmp = internal_get_reg8(context, rDX);
	dl = tmp.value[0];

	if(dl != 0)
		return 0;

	sector = (cl & 63) - 1;
	head = dh;
	cyl = (U4)(ch) + (((U4)(cl & 0xc0)) << 2);
	lba = 0x12 * 2 * cyl + 0x12 * head + sector;
	
	src = c->floppy + lba * 0x200;
	
	linear = read_sreg_value(&context->basic_state_cur, &context->basic_state_next, &context->basic_mask, rES);
	tmp = internal_get_reg16(context, rBX);
	linear = (linear << 4) + (U4)(tmp.value[0]);
	
	//if(cache_write_linear(context, linear, src, 512 * al, context->cpl) != 0)
	//	return 0;	// some error !
	memcpy(c->ram + linear, src, 512 * al);
	
//	fprintf(stderr, "lba=%08x\n", (U4)(lba));
//	fprintf(stderr, "->%02x<-%02x\n", c->ram[linear], src[0]);
		
	return 1;  /* success */
}

static int simpc_real_mode_bios_handler(struct crudasm_intel_context_t *context)
{
	U1 intr;
	struct crudasm_temp_node_t src;
	struct simpc_context *c = (struct simpc_context *)context->user_context;
	struct crudasm_temp_node_t tmp1, tmp2, tmp3, tmp;
	U2 fland = ~0, flor = 0;
	src.size = 2;
	if(context->basic_state_cur.sregs[rCS] == 0xf000 && context->rip < 0x200 && ((context->rip & 1) == 0))
	{
		intr = context->rip >> 1;
		switch(intr)
		{
			case 0x12:
				break;
			case 0x19:
				// Copy boot sector to 0:0x7c00
				memcpy(c->ram + 0x7c00, c->floppy, 512);
				// Put stack at 0:0x7c00 as well
				src.value[0] = 0;
				write_sr_value(context, rSS, &src);
				src.value[0] = 0x7c00;
				write_gr16_value(context, rSP, &src);
				// Clear DL (BIOS boot drive number).
				src.value[0] = 0;
				write_gr16_value(context, rDX, &src);
				// Push stack frame.
				src.value[0] = 0x0202;
				special_do_push(context, src);
				src.value[0] = 0x0000;
				special_do_push(context, src);
				src.value[0] = 0x7c00;
				special_do_push(context, src);
				break;
			case 0x13:
				// Check AH.
				tmp = internal_get_reg8(context, 16 /* AH */);
				switch((U1)tmp.value[0])
				{
					case 0x00:
						fland = ~1;	// clear CF
						tmp.size = 1;
						tmp.value[0] = 0;	// clear AH
						write_gr8_value(context, 16 /* AH */, &tmp);
						break;
					case 0x02:
						if(simple_read_disk(context))
						{
							fland = ~1;	// clear CF
							tmp.size = 1;
							tmp.value[0] = 0;	// clear AH
							write_gr8_value(context, 16 /* AH */, &tmp);
//							fprintf(stderr, "Ok\n");
							break;
						}
//						fprintf(stderr, "Err\n");
						// Disk read error.
						tmp.size = 1;
						tmp.value[0] = 1;	// some error
						write_gr8_value(context, 16 /* AH */, &tmp);
						tmp.size = 0;
						tmp.value[0] = 1;	// no sectors transfered
						write_gr8_value(context, 0 /* AL */, &tmp);
						flor = 1;
						break;
					default:
						fprintf(stderr, "Unsupported INT 0x13, AH = %02x\n", (U1)tmp.value[0]);
						return 1;
				}
				break;
			default:
				fprintf(stderr, "Unsupported interrupt - 0x%02x\n", (U4)(intr));
				return 1;
		}
	}

	tmp1.size = 2;
	tmp2.size = 2;
	tmp3.size = 2;
	do_pop(context, &tmp3);
	do_pop(context, &tmp2);
	do_pop(context, &tmp1);
	tmp1.value[0] = (tmp1.value[0] & fland) | flor;
	special_do_push(context, tmp1);
	special_do_push(context, tmp2);
	special_do_push(context, tmp3);
	return 0;
}

static int handle_special_instructions(struct crudasm_intel_context_t *context)
{
	struct simpc_context *c = (struct simpc_context *)context->user_context;
	struct crudasm_intel_encoding_t *encoding = crudasm_intel_encoding_table + context->icode.encoding;
	struct crudasm_temp_node_t tmp;
	U4 intr_descr;

	switch(encoding->insn)
	{
		case crudasm_intel_insn__jmpfd:
			tmp.size = 2;
			tmp.value[0] = context->icode.disp;
			write_sr_value(context, rCS, &tmp);
			tmp.size = 2;
			tmp.value[0] = context->icode.imm;
			context->rip_next = tmp.value[0];
			return 1;
		case crudasm_intel_insn_call:
			tmp.size = 2;
			tmp.value[0] = context->rip_next;
			special_do_push(context, tmp);
			context->rip_add = context->icode.imm;
			return 1;
		case crudasm_intel_insn_int:
			// Build stack frame.
			tmp = internal_get_extern_FLAGS(context);
			special_do_push(context, tmp);
			tmp.size = 2;
			tmp.value[0] = read_sreg_value(&context->basic_state_cur, &context->basic_state_next, &context->basic_mask, rCS);
			special_do_push(context, tmp);
			tmp.size = 2;
			tmp.value[0] = context->rip_next;
			special_do_push(context, tmp);
			
			if(read_linear(context, 4 * context->icode.imm, &intr_descr, 4, context->cpl, 0) != 0)
				return 1;
			tmp.size = 2;
			tmp.value[0] = (U2)(intr_descr >> 16);
			if(write_sr_value(context, rCS, &tmp) != 0)
				return 1;
			
			// Clear IF.
			tmp = internal_get_extern_FLAGS(context);
			tmp.value[0] &= ~0x200;
			do_write_extern_FLAGS(context, &tmp);
			
			context->rip_next = (U2)(intr_descr);
			return 1;
		case crudasm_intel_insn_cli:
			tmp.size = 1;
			tmp.value[0] = 0;
			if(do_write_rflag_value(context, 9, &tmp) != 0)
			{
				do_abort(context, "Unable to modify IF -- IOPL problem or VM86 mode?");
				return 1;
			}
			return 1;
		case crudasm_intel_insn_sti:
			tmp.size = 1;
			tmp.value[0] = 1;
			if(do_write_rflag_value(context, 9, &tmp) != 0)
			{
				do_abort(context, "Unable to modify IF -- IOPL problem or VM86 mode?");
				return 1;
			}
			return 1;
		case crudasm_intel_insn__iret:
			tmp.size = 2 << context->icode.osz;
			if(do_pop(context, &tmp) != 0)
			{
				do_abort(context, "Unable to POP for IRET.");
				return 1;
			}
			context->rip_next = tmp.value[0];
			if(do_pop(context, &tmp) != 0)
			{
				do_abort(context, "Unable to POP for IRET.");
				return 1;
			}
			write_sr_value(context, rCS, &tmp);
			if(do_pop(context, &tmp) != 0)
			{
				do_abort(context, "Unable to POP for IRET.");
				return 1;
			}
			do_write_extern_FLAGS(context, &tmp);
			return 1;
		case crudasm_intel_insn__ret:
			tmp.size = 2 << context->icode.osz;
			if(do_pop(context, &tmp) != 0)
			{
				do_abort(context, "Unable to POP for IRET.");
				return 1;
			}
			context->rip_next = tmp.value[0];
			return 1;
	}
	
	return 0;
}

//===============================================================================================//

int main(int argc, const char **argv)
{
	struct crudasm_intel_context_t *context = (struct crudasm_intel_context_t *)malloc(sizeof(struct crudasm_intel_context_t));
	FILE *fb;
	U1 fd[4096];
	const char *status;
	U8 insn_size;
	struct ix_decoder_state_t state;
	char dis[1024];
	int i, x;
	U1 code[30], code_size;
	U2 last_cs;
	U8 last_rip;
	
	setup_simpc(context, 2*1024*1024/*ram size*/);
	
	for(i = 0; i < 6; ++i)
	{
		context->basic_state_cur.sreg_cache[i].readable = 1;
		context->basic_state_cur.sreg_cache[i].writable = 1;
	}
	
	fb = fopen("testdis.txt", "wt");
	last_cs = 0;
	last_rip = ~TULL(0);
	
	//simpc_write_physical(context, 0, 4096, fd);
	simpc_real_mode_initialize(context, "test.img");
	
	for(;;)
	{
		if(context->basic_state_cur.sregs[rCS] == last_cs && context->rip == last_rip)
		{
			fprintf(stderr, "\nInfinite loop detected. STOP!\n");
			break;
		}
		last_cs = context->basic_state_cur.sregs[rCS];
		last_rip = context->rip;
		
		code_size = crudasm_intel_fetch(context, code);
		if(code_size == 0)
		{
			fprintf(stderr, "\nUnable to fetch CS:RIP=%04x:%08x%08x\n", last_cs, (U4)(SHR64U(last_rip, 32)), (U4)(last_rip));
			break;
		}
		
		status = crudasm_intel_decode(&context->icode, code_size, context->dsz, code, &state);
		if(*status != '\0')
		{
			fprintf(stderr, "\nUnable to decode CS:RIP=%04x:%08x%08x. Error: %s\n", last_cs, (U4)(SHR64U(last_rip, 32)), (U4)(last_rip), status);
			free(context);
			return 1;
		}
		insn_size = state.insn_size;
		context->rel = context->rip + insn_size;	// do this before pre_exec()
		///context->rip = pos;
		
		crudasm_intel_disasm(&context->icode, context->rel, dis, NULL, NULL, context->dsz);
		fprintf(fb, "%08x  %s\n", (int)(context->rip), dis);
	
		context->rip_add = 0;
		context->rip_next = context->rel;

		x = crudasm_intel_context_pre_exec(context);
		
		// Check for BIOS function here (if in real mode--vm86 mode will require QEMU & its BIOS).
		if(simpc_real_mode_bios_handler(context) != 0)
			break;
		
		if(x == 0)  /* don't skip instruction */
		{
			for(;;)
			{
				x = handle_special_instructions(context);
				
				if(x == 0)
				{
					if(crudasm_intel_execute(context) == -1)
					{
						crudasm_intel_disasm(&context->icode, context->rel, dis, NULL, NULL, context->dsz);
						fprintf(stderr, "\n%08x  %s\n", (int)(context->rip), dis);
						fprintf(stderr, "No semantics for above instruction -- simulation halted !\n");
						fclose(fb);
						destroy_simpc_user_context(context);
						free(context);
						return 1;
					}
				}
		
				if(context->sim_abort)
				{
					crudasm_intel_disasm(&context->icode, context->rel, dis, NULL, NULL, context->dsz);
					fprintf(stderr, "\n%08x  %s\n", (int)(context->rip), dis);
					fprintf(stderr, "Simulation halted: %s\n", (context->error_desc == NULL) ? "Unknown error." : context->error_desc);
					fclose(fb);
					destroy_simpc_user_context(context);
					free(context);
					return 1;
				}
				//dump_write_cache(stderr, context);

				// check for exceptions here. do not post_exec() if an exception has gone off.

				// if here, no exceptions have gone off.

				// post_exec() should check for software interrupts and deal with them.
				x = crudasm_intel_context_post_exec(context);
		
				// check for exceptions here as well.
				
				if(x == 0)
				{
					break;
				}
				x = crudasm_intel_context_pre_exec(context);
			}
		}

		// shouldn't "pos" be in context somewhere, and shouldn't we be fetching via a virtual read of CS or something?
		context->rip = context->rip_next + context->rip_add;
		if(context->dsz == 0)
			context->rip = (U2)(context->rip);
		else
		if(context->dsz == 1)
			context->rip = (U4)(context->rip);
		
		// Check for external IRQs here, if enabled and if last instruction was a ctrl xfer insn.
	}

	fclose(fb);
	destroy_simpc_user_context(context);
	free(context);
	return 0;
}

