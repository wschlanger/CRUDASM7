// out_disassemble.h  (note: this is an automatically generated file - do not edit!)
// Copyright (C) 2008 Willow Schlanger

case insn__cmovcc:
	write("cmov");
	write_cc(argvalue(2));
	space();
	write_arg(0);
	comma();
	write_arg(1);
	break;
case insn__setcc:
	write("set");
	write_cc(argvalue(1));
	space();
	write_arg(0);
	break;
case insn__nopmb:
	write("nop");
	space();
	write_args();
	break;
case insn__usalc:
	write("salc");
	break;
case insn__uint1:
	write("int1");
	space();
	break;
case insn__aad:
	if((U1)get_imm64() == 0x0a)
		write("aad");
	else
	{
		write("aad");
		space();
		write_args();
	}
	break;
case insn__aam:
	if((U1)get_imm64() == 0x0a)
		write("aam");
	else
	{
		write("aam");
		space();
		write_args();
	}
	break;
case insn__sxacc:
	if(get_osz() == argsize_16)
		write("cbw");
	else
	if(get_osz() == argsize_32)
		write("cwde");
	else
		write("cdqe");
	break;
case insn__sxdax:
	if(get_osz() == argsize_16)
		write("cwd");
	else
	if(get_osz() == argsize_32)
		write("cdq");
	else
		write("cqo");
	return true;
	break;
case insn__cmul3:
	write("imul");
	space();
	write_args();
	break;
case insn__cmul2:
	write("imul");
	space();
	write_args();
	break;
case insn__imulb:
	write("imul");
	space();
	write_args();
	break;
case insn__mulb:
	write("mul");
	space();
	write_args();
	break;
case insn__divb:
	write("div");
	space();
	write_args();
	break;
case insn__idivb:
	write("idiv");
	space();
	write_args();
	break;
case insn__xchg:
	if(basecode() == 0x90 && argvalue(1) == 0)
		write("nop");
	else
	{
		write("xchg");
		space();
		write_args();
	}
	break;
case insn__sal:
	write("shl");
	space();
	write_args();
	break;
case insn__xlat:
	write_seg_reg();	// if not 7
	write("xlatb");
	break;
case insn__ins:
	write_rep();		// if present
	write("ins");
	write_size_suffix(argsize_lo(0));
	break;
case insn__outs:
	write_seg_reg();
	write_rep();		// if present
	write("outs");
	write_size_suffix(argsize_lo(1));
	break;
case insn__movs:
	write_seg_reg();
	write_rep();		// if present
	write("movs");
	write_size_suffix(argsize_lo(0));
	break;
case insn__cmps:
	write_seg_reg();
	write_repcc();		// if present
	write("cmps");
	write_size_suffix(argsize_lo(0));
	break;
case insn__stos:
	write_rep();		// if present
	write("stos");
	write_size_suffix(argsize_lo(0));
	break;
case insn__lods:
	write_seg_reg();
	write_rep();		// if present
	write("lods");		// is rep lods useful?
	write_size_suffix(argsize_lo(0));
	break;
case insn__scas:
	write_repcc();		// if present
	write("scas");
	write_size_suffix(argsize_lo(0));
	break;
case insn__pushsr:
	write_size();
	write("push");
	space();
	write_args();
	break;
case insn__popsr:
	write_size();
	write("pop");
	space();
	write_args();
	break;
case insn__push:
	write_size();
	write("push");
	space();
	write_args();
	break;
case insn__pop:
	write_size();
	write("pop");
	space();
	write_args();
	break;
case insn__movsrv:
	write("mov");
	space();
	write_args();
	break;
case insn__movvsr:
	write("mov");
	space();
	write_args();
	break;
case insn__pusha:
	write("pusha");
	write_size_suffix(get_osz());
	break;
case insn__popa:
	write("popa");
	write_size_suffix(get_osz());
	break;
case insn__pushf:
	write("pushf");
	write_size_suffix(get_osz());
	break;
case insn__popf:
	write("popf");
	write_size_suffix(get_osz());
	break;
case insn__jcc:
	// instead of e.g. o32 jmp target should we do e.g. jmp dword short target ?
	write_size();
	write("j");
	write_cc(argvalue(1));
	space();
	write_arg(0);
	break;
case insn__calli:
	write("call");
	space();
	write_args();
	break;
case insn__callfd:
	write("call");
	space();
	write_far_imm();
	break;
case insn__callfi:
	write("call");
	space();
	write_args();
	break;
case insn__jmpi:
	write("jmp");
	space();
	write_args();
	break;
case insn__jmpfd:
	write("jmp");
	space();
	write_far_imm();
	break;
case insn__jmpfi:
	write("jmp");
	space();
	write_args();
	break;
case insn__retnum:
	write("ret");
	space();
	write_args();
	break;
case insn__ret:
	write("ret");
	break;
case insn__retfnum:
	write("retf");
	space();
	write_args();
	break;
case insn__retf:
	write("retf");
	break;
case insn__int3:
	write("int3");
	break;
case insn__iret:
	//write_size();
	write("iret");
	write_size_suffix(get_osz());
	break;
case insn__loopnz:
	write("loopnz");
	space();
	write_arg(0);
	break;
case insn__loopz:
	write("loopz");
	space();
	write_arg(0);
	break;
case insn__loop:
	write("loop");
	space();
	write_arg(0);
	break;
case insn__jrcxz:
	write_size();
	if(get_asz() == argsize_16)
		write("jcxz");
	else
	if(get_asz() == argsize_32)
		write("jecxz");
	else
		write("jrcxz");
	space();
	write_arg(0);
	break;
case insn__arpl_movsxd:
	// This is probably not right. It's probably wired correctly for non-64-bit mode, need special code
	// in the decode to make movsxd work right.
	// Should abort if in 64-bit mode for now...
	if(get_osz() == argsize_64)
		write("movsxd");
	else
		write("arpl");
	space();
	write_args();
	break;
case insn__cmpxchgxb:
	// Is this right?
	if(get_osz() == argsize_64)
		write("cmpxchg16b");
	else
		write("cmpxchg8b");
	space();
	write_args();
	break;
case insn__movcr:
	write("mov");
	space();
	write_args();
	break;
case insn__movdr:
	write("mov");
	space();
	write_args();
	break;


