// imetadata.h
// Copyright (C) 2009 Willow Schlanger

#ifndef l_imetadata_h__included
#define l_imetadata_h__included

#include "out_externals.h"
#include "get_insn_enums.h"

struct InsnMetaDataT
{
	unsigned char usesRIP : 1;
	unsigned char usesRIP1 : 1;
	unsigned char usesRIPC : 1;
	unsigned char usesRINT : 1;
	unsigned char usesRFLAGS : 1;
	unsigned char usesRBPX : 1;
	unsigned char usesRSPX : 1;
	unsigned char usesStack : 1;	// input : pop()/discard(), output: push()/commit()
};

struct InsnMetaData
{
	InsnMetaDataT data[2];			// [0]=input, [1]=output
};

// warning: osz, asz are in BITS here, i.e. 16, 32, or 64.
const int *get_ext_inputs(int insn, int osz, int asz);
const int *get_ext_outputs(int insn, int osz, int asz);
const char *get_arg_info(int insn, int osz, int asz);
const InsnMetaData *get_insn_metadata(int insn, int osz, int asz);

#endif	// l_imetadata_h__included
