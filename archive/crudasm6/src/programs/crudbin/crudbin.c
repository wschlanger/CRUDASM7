// crudbin.c - a simple binary disassembler (example program)
// Copyright (C) 2011 Willow Schlanger. All rights reserved.
//
// To build:
// gcc -o crudbin crudbin.c ../../core/decoder_internal.c ../../core/ixdecoder.c ../../core/ixdisasm.c

#include "../../core/ixdisasm.h"
#include "../../core/ixdecoder.h"
#include <stdlib.h>
#include <stddef.h>

int disassemble_buffer(U1 *fd, long fs, U1 dsz, U8 origin, FILE *fout)
{
	char disasm_line[1024];
	const char *status;
	struct ix_icode_t icode;
	struct ix_decoder_state_t state;
	U8 insn_size;
	U8 start_origin = origin;
	
	if(dsz == 2)
		fprintf(fout, "; org 0x%08x%08x\n", (U4)((origin >> 16) >> 16), (U4)(origin));
	else	
		fprintf(fout, "; org 0x%08x\n", (U4)(origin));
	fprintf(fout, "bits %d\n", 16 << dsz);
	
	while(fs > 0)
	{
		status = crudasm_intel_decode(&icode, fs, dsz, fd + (size_t)(origin - start_origin), &state);
		if(*status != '\0')
		{
			// Error decoding.
			fprintf(fout, "db 0x%02x\n", (U4)(U1)(fd[origin]));
			++origin;
			--fs;
		}
		else
		{
			insn_size = state.insn_size;
			
			crudasm_intel_disasm(&icode, origin + insn_size, disasm_line, NULL, NULL, dsz);
			
			fprintf(fout, "loc_");
			if(dsz == 2)
			{
				fprintf(fout, "%08x%08x", (U4)((origin >> 16) >> 16), (U4)(origin));
			}
			else
			{
				fprintf(fout, "%08x", (U4)(origin));
			}
			
			fprintf(fout, ":  %s\n", disasm_line);
			
			origin += insn_size;
			if(fs < insn_size)
				fs = 0;
			fs -= insn_size;
		}
	}
	
	return 0;
}

int disassemble_file(const char *filename, U1 dsz, U8 origin, FILE *fout)
{
	FILE *fi;
	char c;
	long fs;
	U1 *fd;
	int status;
	
	fi = fopen(filename, "rb");
	if(fi == NULL)
	{
		printf("error: unable to read file: %s\n", filename);
		return 1;
	}
	if(fgetc(fi) == EOF)
	{
		fclose(fi);
		printf("note: file has 0 size: %s\n", filename);
		return 1;
	}
	fseek(fi, -1, SEEK_END);
	fs = 1 + ftell(fi);
	rewind(fi);
	fd = (U1 *)malloc(fs);
	if(fd == NULL)
	{
		fclose(fi);
		printf("error: out of memory, tried to allocate %ld bytes!\n", fs);
		return 1;
	}
	if(fread(fd, fs, 1, fi) != 1)
	{
		free(fd);
		fclose(fi);
		printf("error: unable to read file: %s\n", filename);
		return 1;
	}
	
	fprintf(fout, "; disassembly of %s\n", filename);
	status = disassemble_buffer(fd, fs, dsz, origin, fout);	
	free(fd);	
	fclose(fi);
	return status;
}

void usage()
{
	printf("crudbin (uses CRUDASM6 engine by Willow Schlanger)\n\n");
	printf("usage: crudbin filename bits origin\n");
	printf("example: crudbin myprogram.com 16 0x100\n\n");
}

int main(int argc, const char **argv)
{
	const char *filename;
	char *tmp;
	long bits;
	U8 origin;
	U1 dsz;
	
	if(sizeof(U8) != 8)
	{
		printf("ERROR: sizeof(U8) != 8 !! Please edit ../../common/include/types.h\n");
		printf("for your platform and try again.\n\n");
		return 1;
	}
	
	if(argc != 4)
	{
		usage();
		return 1;
	}

	tmp = NULL;
	bits = strtol(argv[2], &tmp, 0);
	if(*tmp != '\0' || (bits != 16 && bits != 32 && bits != 64))
	{
		usage();
		printf("\nInvalid bits entered - %s (use 16, 32, or 64)\n", argv[2]);
		return 1;
	}
	if(bits == 16)
		dsz = 0;
	else
	if(bits == 32)
		dsz = 1;
	else
		dsz = 2;
	
	tmp = NULL;
	origin = strtoll(argv[3], &tmp, 0);
	if(*tmp != '\0')
	{
		usage();
		printf("\nInvalid origin entered - %s\n", argv[3]);
		return 1;
	}
	
	filename = argv[1];
	return disassemble_file(filename, dsz, origin, stdout);
}

