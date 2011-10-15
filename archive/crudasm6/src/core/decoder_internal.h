// decoder_internal.h
// Copyright (C) 2011 Willow Schlanger. All rights reserved.

#ifndef l_decoder_internal__crudasm_included
#define l_decoder_internal__crudasm_included

#include "../common/include/types.h"

#ifdef __cplusplus
extern "C"
{
#endif

#include "../generated/out_intel_encoding_info.h"

extern struct crudasm_intel_insn_t crudasm_intel_insns[];  /* see out_intel_insns.h */

#ifdef __cplusplus
}
#endif

#endif	// l_decoder_internal__crudasm_included

