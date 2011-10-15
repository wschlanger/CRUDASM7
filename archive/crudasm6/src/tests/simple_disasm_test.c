// simple_disasm_test.c
// Copyright (C) 2011 Willow Schlanger. All rights reserved.

#include "../core/ixdisasm.h"
#include "../core/ixdecoder.h"

int main(int argc, const char **argv)
{
	char buf1[32] = {0xb8, 0x01, 0x17};
	struct ix_icode_t icode;
	const char *status;
	char dis[1024];
	
	status = crudasm_intel_decode(&icode, 32, 0, buf1, NULL);
	crudasm_intel_disasm(&icode, 0, dis, NULL, NULL, 0 /* 16 bit mode */);
	printf("b8 01 17 -> [%s]\n", dis);
	
	return 0;
}

