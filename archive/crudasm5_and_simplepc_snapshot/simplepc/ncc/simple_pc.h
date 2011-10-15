// simple_pc.h
// Copyright (C) 2009 Willow Schlanger

#ifndef l_simple_pc_h__included
#define l_simple_pc_h__included

#include "emulator.h"
#include "get_x86s.h"

class SimplePC
{
	public:
	U8 ram_size;				// in bytes
	U1 *ram;
	U1 *disk;
	U8 disk_sector_count;
	LinearMemory *linmem;
	SimpleExecutor *exec;
	SimpleEvaluator *eval;
	class IoHardware *iohw;
	GeneratedEmulator<SimpleExecutor, SimpleEvaluator> *emulator;
	SimplePC(U8 ram_size_t, U1 *diskT, U8 disk_sector_count_t);	// ram_size_t must be a multiple of 1 megabyte
	~SimplePC();
	
	void reset();
	void pre_exec();
	bool execute();				// if returns true, instruction was repeated
	void post_exec();
	bool handle_intr();
	
	x86s::icode_t icode;
	x86s::decode_state_t decode;
	
	private:
	bool read_disk();
	void setup_args();
	U8 get_mem_arg_offset(int i);
	U8 mem_offset;
	U8 strs_offset;
	U8 strd_offset;
};

#endif	// l_simple_pc_h__included
