// test_out.cpp
// Copyright (C) 2008 Willow Schlanger

#include "types.h"
#include "x86s_common.h"

namespace x86s
{

#include "out_insn_enums.h"

// This is terminated with an element with an insn number of insn__count
encoding_t encodings[] =
{
#include "out_encodings.h"
};

int search_table[insn__string_count] =
{
#include "out_insn_search_table.h"
};

const char *insn_strings[insn__count] =
{
#include "out_insn_strings.h"
};

insn_def_t insns[insn__count] =
{
#include "out_insns.h"
};

U4 decoder_table[] =
{
#include "out_decoder_table.h"
};

}	// namespace x86s
