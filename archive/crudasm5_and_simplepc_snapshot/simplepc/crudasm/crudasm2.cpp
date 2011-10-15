// crudasm2.cpp
// Copyright (C) 2008 Willow Schlanger

// fixme--improve 64-bit support

#include "../x86s/types.h"
#include "../x86s/x86s_common.h"
#include "../x86s/x86s_decode.h"
#include "asmwriter.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>

#ifdef _MSC_VER
namespace std
{
}
#endif

using namespace std;
using namespace x86s;

// This decodes a buffer of data - each instruction - and prints out
// the result to stdout.
static void do_decode(U1 *data, U4 bufsize, int bits, U8 loadofs = 0)
{
	icode_t icode;
	decode_state_t s;
	UINT curofs;
	UINT bytesleft;
	
	s.icode = &icode;
	s.insn = data;
	if(bits == 16)
		s.dsz = argsize_16;
	else
	if(bits == 32)
		s.dsz = argsize_32;
	else
		s.dsz = argsize_64;
	
	char dest[1024];

	NasmWriter disasm;

	int x;
	for(;;)
	{
		curofs = s.insn - data;
		bytesleft = bufsize - curofs;
		s.end = (bytesleft <= 15) ? (s.insn + bytesleft) : (s.insn + 15);
		x = decode(s);
		if(x == 1)
		{
			// bugfix from release 0.05.
			//printf("abort: cs segment limit exceeded (or internal error)\n");
			for(int i = 0; i < bytesleft; ++i)
				printf("%08x  db 0x%02x\n", (U4)(loadofs + curofs + i), data[curofs + i]);
			//end bugfix
			return;
		}
		else
		if(x != 0)
		{
			//printf("abort: invalid opcode detected\n");
			//return;
			printf("%08x  db 0x%02x\n", (U4)(loadofs + curofs), data[curofs]);

			++s.insn;
			if(s.insn == (bufsize + data))
				break;
			continue;
		}

		if(s.encoding == 0xffffff || s.icode->insn >= insn__count)
		{
			printf("---");
			return;
		}
		
		printf("%08x  ", (U4)(loadofs + curofs));

		dest[0] = '\0';
		disasm.disasm(dest, &s, NULL, NULL, (U8)(loadofs) + (U8)(curofs) + (U8)(s.size));

		printf("%s\n", dest);
		
		s.insn += s.size;
		if(s.insn == (bufsize + data))
			break;
	}
}

void help()
{
	printf("crudasm1  Copyright (C) 2008 Willow Schlanger\n");
	printf("This program disassembles an x86 binary image file\n");
	printf("\n");
	printf("usage: crudasm1 file bits origin [bytes to skip]\n");
	printf("example: crudasm myprogram.com 16 0x100\n");
}

static U1 *read_file(const char *filename, U4 *size)
{
	U1 *data;
	FILE *f = fopen(filename, "rb");
	if(f == NULL)
		return NULL;
	if(feof(f))
	{
		fclose(f);
		return NULL;	// no data
	}
	fseek(f, -1, SEEK_END);
	*size = 1 + ftell(f);
	data = (U1 *)malloc(*size);
	rewind(f);
	if(fread(data, *size, 1, f) != 1)
	{
		free(data);
		fclose(f);
		return NULL;	// unable to read
	}
	fclose(f);
	return data;
}

int main(int argc, char **argv)
//int main()
{
//int argc = 4;
//char *argv[] = {"crudasm2", "C:\\Users\\User\\DOCUME~1\\code\\vm\\vm64\\vmdec\\frontend\\x86\\crudasm\\crudasm2.exe", "16", "0x100", 0};
	const char *filename;
	int bits = -1;
	if(argc < 4)
	{
		help();
		return 1;
	}
	filename = argv[1];
	if(!strcmp(argv[2], "16"))
		bits = 16;
	else
	if(!strcmp(argv[2], "32"))
		bits = 32;
	else
	if(!strcmp(argv[2], "64"))
	{
		printf("Warning: 64-bit mode is still experimental and will probably not work!\n");
		bits = 64;
	}
	else
	{
		help();
		printf("\nError: bits argument must be one of these: 16, 32, 64\n");
		return 1;
	}
	
	char *endp;
	U4 origin = strtol(argv[3], &endp, 0);
	if(*endp != '\0')
	{
		help();
		printf("\nError: bad origin given - use 0x for hex numbers\n");
		return 1;
	}

	U4 skip = 0;
	if(argc >= 5)
	{
		skip = strtol(argv[4], &endp, 0);
		if(*endp != '\0')
		{
			help();
			printf("\nError: bad skip count given - use 0x for hex numbers\n");
			return 1;
		}
	}
	
	//printf("%s %d %08x %08x\n", filename, bits, origin, skip);
	U4 size;
	U1 *dat = read_file(filename, &size);
	if(dat == NULL)
	{
		printf("Error: unable to open file \'%s\' or file has a size of 0 bytes\n", filename);
		return 1;
	}
	if(skip >= size)
	{
		free(dat);
		printf("Error: nothing to do!\n");
		return 1;
	}
	U1 *bin = dat + skip;
	size -= skip;
	
	do_decode(bin, size, bits, origin);
	
	free(dat);
	
	return 0;
}
