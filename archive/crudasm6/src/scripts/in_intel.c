// Copyright (C) 2010,2011 Willow Schlanger
// All rights reserved

/*
flags : lockable, lock_always, repable, repcont, ctrlxfer
tags  :
 asm_skip
	don't generate this encoding when assembling
 sx_byte
 	osz-sized immediates are encoded as a byte.
 	if used with imm64_sx32 then
 		o16: encoded byte is sign-extended to 16bits
 		o32: encoded byte is sign-extended to 32bits
 		o64: encoded byte is sign-extended to 64bits
 			but only low 32bits are stored in icode
 	if not used with imm64_sx32 then
 		o16: encoded byte is sign-extended to 16bits
 		o32/o64: encoded byte is sign-extended to 32bits
 	NOTE: at present, sx_byte is always used with imm64_sx32.
 imm64_sx32
	if an immediate has osz size and we have o64, then
		fetch 32 bits (or 8 bits if sx_byte is used)
		and store in icode. on read from icode, sign-extend
		the immediate to 64bits from 32bits.
 no_rex_w
 	even if rex.w is used, pretend rex.w = 0.
 	used by ins/outs.
 imm64_disp
 	if o64, disp is used together with imm in icode.
 	not compatible with sx_byte.
 	if an imm64 arg exists, upper 32bits stored in disp.
 	if a disp64 arg (memdisp) exists, upper 32bits of disp stored in imm.
 is64
 	in 64bit mode (dsz is 64 bits), use o64 unless 0x66 is used then, use o16.
 	used by the various push/pop instructions.
 no64
 	invalid in 64bit mode (used by single-byte INC/DEC, etc.)
 like_movsxd
 like_arpl
 	movsxd & arpl share the same opcode.
 reg_base
 	low 3 bits of 'reg' comes from low 3 bits of basecode, not reg/op
 has_cc
 	low 4 bits of basecode is a condition code
 relative
	imm's with value == 0xff are relative (used for ctrl xfers)
 reg_rm
 	low 3 bits of 'reg' comes from low 3 bits of 1st modr/m byte, not reg/op
 	if used, bit 4 comes from rex_b.
*/

//=================================================================================================
//=================================================================================================

insn aaa()
"37 void" ! no64
emu {
	AF = _logor(AF, _gtu(bitand(AL, tr<B1>(0x0f)), tr<B1>(0x09)));
	CF = AF;
	AX = add(AX, quest(AF, tr<B2>(0x0106), tr<B2>(0)));
	AL = bitand(AL, tr<B1>(0x0f));
	OF = 0;
	SF = 0;
	ZF = _zero(AL);
	PF = _parity(AL);
}

insn _aad(B1 arg)
"d5 imm" ! no64
dis all {
	write("aad");
	// ndisasm: show argument only if imm32 is not 0x0a.
	maybe_write_space_args_imm32(0x0a);
}
emu {
	B1 tmp = AH;
	tmp = cmul(tmp, arg);
	tmp = add(tmp, AL);
	AL = tmp;
	AH = 0;
	OF = undefined;
	SF = _sign(AL);
	ZF = _zero(AL);
	AF = undefined;
	PF = _parity(AL);
	CF = undefined;
}

insn _aam(B1 arg)
"d4 imm" ! no64
dis all {
	write("aam");
	// ndisasm: show argument only if imm32 is not 0x0a.
	maybe_write_space_args_imm32(0x0a);
}
emu {
	assert(_lognot(_zero(arg)), tr<B1>(0));		// else division by zero exception
	B1 tmp = AL;
	AH = udiv(tmp, arg);
	AL = umod(tmp, arg);
	OF = undefined;
	SF = _sign(AL);
	ZF = _zero(AL);
	AF = undefined;
	PF = _parity(AL);
	CF = undefined;
}

insn aas()
"3f void" ! no64
emu {
	AF = _logor(AF, _gtu(bitand(AL, tr<B1>(0x0f)), tr<B1>(0x09)));
	CF = AF;
	AX = sub(AX, quest(AF, tr<B2>(0x0106), tr<B2>(0)));
	AL = bitand(AL, tr<B1>(0x0f));
	OF = 0;
	SF = 0;
	ZF = _zero(AL);
	PF = _parity(AL);
}


insn das()
"2f void" ! no64
emu {
	B1 tmpAL = AL;
	
	AF = _logor(AF, _gtu(bitand(tmpAL, tr<B1>(0x0f)), tr<B1>(0x09)));
	B1 count = quest(AF, tr<B1>(0x06), tr<B1>(0));
	BIT tmpCF = _quest(AF, _logor(CF, _ltu(AL, tr<B1>(0x06))), tr<BIT>(0));
	AL = sub(AL, count);
	
	BIT cond = _logor(CF, _gtu(tmpAL, tr<B1>(0x99)));
	B1 fixup = quest(cond, tr<B1>(0x60), tr<B1>(0));
	AL = sub(AL, fixup);
	tmpCF = _logor(tmpCF, cond);
	
	OF = 0;						// undocumented ?
	SF = _sign(AL);
	ZF = _zero(AL);
	PF = _parity(AL);
	CF = tmpCF;
}

insn daa()
"27 void" ! no64
emu {
	B1 tmpAL = AL;

	AF = _logor(AF, _gtu(bitand(tmpAL, tr<B1>(0x0f)), tr<B1>(0x09)));
	B1 count = quest(AF, tr<B1>(0x06), tr<B1>(0));
	BIT tmpCF = _quest(AF, _logor(CF, _gtu(AL, tr<B1>(0xf9))), tr<BIT>(0));
	AL = add(AL, count);
	
	BIT cond = _logor(CF, _gtu(tmpAL, tr<B1>(0x99)));
	B1 fixup = quest(cond, tr<B1>(0x60), tr<B1>(0));
	AL = add(AL, fixup);
	tmpCF = cond;
	
	OF = 0;						// undocumented ?
	SF = _sign(AL);
	ZF = _zero(AL);
	PF = _parity(AL);
	CF = tmpCF;
}

//=================================================================================================
//=================================================================================================

insn lockable adc(P dest, P src)
"80 /2 rm,imm" ! P=B1
"82 /2 rm,imm" ! P=B1, asm_skip
"81 /2 rm,imm" ! P=osz, imm64_sx32
"83 /2 rm,imm" ! P=osz, imm64_sx32, sx_byte
"10 /r rm,r" ! P=B1
"12 /r r,rm" ! P=B1
"11 /r rm,r" ! P=osz
"13 /r r,rm" ! P=osz
"14 r:0,imm" ! P=B1
"15 r:0,imm" ! P=osz, imm64_sx32
emu {
	BIT tmp = CF;
	OF = _adc_of(dest, src, tmp);
	AF = _adc_af(dest, src, tmp);
	CF = _adc_cf(dest, src, tmp);
	dest = add(add(dest, src), zx<P>(tmp));
	SF = _sign(dest);
	ZF = _zero(dest);
	PF = _parity(dest);
}

//----

insn lockable add(P dest, P src)
"80 /0 rm,imm" ! P=B1
"82 /0 rm,imm" ! P=B1, asm_skip
"81 /0 rm,imm" ! P=osz, imm64_sx32
"83 /0 rm,imm" ! P=osz, imm64_sx32, sx_byte
"00 /r rm,r" ! P=B1
"02 /r r,rm" ! P=B1
"01 /r rm,r" ! P=osz
"03 /r r,rm" ! P=osz
"04 r:0,imm" ! P=B1
"05 r:0,imm" ! P=osz, imm64_sx32
emu {
	OF = _add_of(dest, src);
	AF = _add_af(dest, src);
	CF = _add_cf(dest, src);
	dest = add(dest, src);
	SF = _sign(dest);
	ZF = _zero(dest);
	PF = _parity(dest);
}

insn lockable or(P dest, P src)
"80 /1 rm,imm" ! P=B1
"82 /1 rm,imm" ! P=B1, asm_skip
"81 /1 rm,imm" ! P=osz, imm64_sx32
"83 /1 rm,imm" ! P=osz, imm64_sx32, sx_byte
"08 /r rm,r" ! P=B1
"0a /r r,rm" ! P=B1
"09 /r rm,r" ! P=osz
"0b /r r,rm" ! P=osz
"0c r:0,imm" ! P=B1
"0d r:0,imm" ! P=osz, imm64_sx32
emu {
	OF = 0;
	AF = undefined;
	CF = 0;
	dest = bitor(dest, src);
	SF = _sign(dest);
	ZF = _zero(dest);
	PF = _parity(dest);
}

insn lockable sbb(P dest, P src)
"80 /3 rm,imm" ! P=B1
"82 /3 rm,imm" ! P=B1, asm_skip
"81 /3 rm,imm" ! P=osz, imm64_sx32
"83 /3 rm,imm" ! P=osz, imm64_sx32, sx_byte
"18 /r rm,r" ! P=B1
"1a /r r,rm" ! P=B1
"19 /r rm,r" ! P=osz
"1b /r r,rm" ! P=osz
"1c r:0,imm" ! P=B1
"1d r:0,imm" ! P=osz, imm64_sx32
emu {
	BIT tmp = CF;
	OF = _sbb_of(dest, src, tmp);
	AF = _sbb_af(dest, src, tmp);
	CF = _sbb_cf(dest, src, tmp);
	dest = sub(sub(dest, src), zx<P>(tmp));
	SF = _sign(dest);
	ZF = _zero(dest);
	PF = _parity(dest);
}

insn lockable and(P dest, P src)
"80 /4 rm,imm" ! P=B1
"82 /4 rm,imm" ! P=B1, asm_skip
"81 /4 rm,imm" ! P=osz, imm64_sx32
"83 /4 rm,imm" ! P=osz, imm64_sx32, sx_byte
"20 /r rm,r" ! P=B1
"22 /r r,rm" ! P=B1
"21 /r rm,r" ! P=osz
"23 /r r,rm" ! P=osz
"24 r:0,imm" ! P=B1
"25 r:0,imm" ! P=osz, imm64_sx32
emu {
	OF = 0;
	AF = undefined;
	CF = 0;
	dest = bitand(dest, src);
	SF = _sign(dest);
	ZF = _zero(dest);
	PF = _parity(dest);
}

insn lockable sub(P dest, P src)
"80 /5 rm,imm" ! P=B1
"82 /5 rm,imm" ! P=B1, asm_skip
"81 /5 rm,imm" ! P=osz, imm64_sx32
"83 /5 rm,imm" ! P=osz, imm64_sx32, sx_byte
"28 /r rm,r" ! P=B1
"2a /r r,rm" ! P=B1
"29 /r rm,r" ! P=osz
"2b /r r,rm" ! P=osz
"2c r:0,imm" ! P=B1
"2d r:0,imm" ! P=osz, imm64_sx32
emu {
	OF = _sub_of(dest, src);
	AF = _sub_af(dest, src);
	CF = _sub_cf(dest, src);
	dest = sub(dest, src);
	SF = _sign(dest);
	ZF = _zero(dest);
	PF = _parity(dest);
}

insn lockable xor(P dest, P src)
"80 /6 rm,imm" ! P=B1
"82 /6 rm,imm" ! P=B1, asm_skip
"81 /6 rm,imm" ! P=osz, imm64_sx32
"83 /6 rm,imm" ! P=osz, imm64_sx32, sx_byte
"30 /r rm,r" ! P=B1
"32 /r r,rm" ! P=B1
"31 /r rm,r" ! P=osz
"33 /r r,rm" ! P=osz
"34 r:0,imm" ! P=B1
"35 r:0,imm" ! P=osz, imm64_sx32
emu {
	OF = 0;
	AF = undefined;
	CF = 0;
	dest = bitxor(dest, src);
	SF = _sign(dest);
	ZF = _zero(dest);
	PF = _parity(dest);
}

insn cmp(P dest, P src)
"80 /7 rm,imm" ! P=B1
"82 /7 rm,imm" ! P=B1, asm_skip
"81 /7 rm,imm" ! P=osz, imm64_sx32
"83 /7 rm,imm" ! P=osz, imm64_sx32, sx_byte
"38 /r rm,r" ! P=B1
"3a /r r,rm" ! P=B1
"39 /r rm,r" ! P=osz
"3b /r r,rm" ! P=osz
"3c r:0,imm" ! P=B1
"3d r:0,imm" ! P=osz, imm64_sx32
emu {
	OF = _sub_of(dest, src);
	AF = _sub_af(dest, src);
	CF = _sub_cf(dest, src);
	P tmp = sub(dest, src);
	SF = _sign(tmp);
	ZF = _zero(tmp);
	PF = _parity(tmp);
}

// -- end arithmatic opcodes ---

// shift and rotate. apparantly, not lockable.
// 1st 4 insn's : sf, zf, af, pf unaffected.
// of : affected only for single-bit rotates.
// cf : last bit shifted out

insn rol(P dest, B1 src)
"c0 /0 rm,imm" ! P=B1
"c1 /0 rm,imm" ! P=osz
"d0 /0 rm,imm_implict:1" ! P=B1
"d1 /0 rm,imm_implict:1" ! P=osz
"d2 /0 rm,r:1" ! P=B1
"d3 /0 rm,r:1" ! P=osz
emu {
	B1 count = src;
	B1 tmpmod = shl(tr<B1>(sizeof(P)), tr<B1>(3));
	B1 tmpcount = bitand(count, sub(tmpmod, tr<B1>(1)));
	
	dest = rol_(dest, tmpcount);
	
	BIT tmp0 = _zero(tmpcount);
	BIT newcf = _quest(tmp0, CF, _lsb(dest));
	OF = _quest(tmp0, OF, _quest(_eq(count, 1), _logxor(_sign(dest), newcf), undefined));
	CF = newcf;
}

insn ror(P dest, B1 src)
"c0 /1 rm,imm" ! P=B1
"c1 /1 rm,imm" ! P=osz
"d0 /1 rm,imm_implict:1" ! P=B1
"d1 /1 rm,imm_implict:1" ! P=osz
"d2 /1 rm,r:1" ! P=B1
"d3 /1 rm,r:1" ! P=osz
emu {
	B1 count = src;
	B1 tmpmod = shl(tr<B1>(sizeof(P)), tr<B1>(3));
	B1 tmpcount = bitand(count, sub(tmpmod, tr<B1>(1)));
	
	dest = ror_(dest, tmpcount);
	
	BIT tmp0 = _zero(tmpcount);
	BIT newcf = quest(tmp0, CF, _sign(dest));
	OF = _quest(tmp0, OF, _quest(_eq(count, tr<B1>(1)), _logxor(_sign(dest), _sign2(dest)), undefined));
	CF = newcf;
}

insn rcl(P dest, B1 src)
"c0 /2 rm,imm" ! P=B1
"c1 /2 rm,imm" ! P=osz
"d0 /2 rm,imm_implict:1" ! P=B1
"d1 /2 rm,imm_implict:1" ! P=osz
"d2 /2 rm,r:1" ! P=B1
"d3 /2 rm,r:1" ! P=osz
emu {
	B1 count = src;
	o16: B1 count1 = bitand(count, tr<B1>(31));
	o32: B1 count1 = bitand(count, tr<B1>(31));
	o64: B1 count1 = bitand(count, tr<B1>(63));	// If rex.w is used when not needed, we'll still AND by 63 here..
	B1 tmpmod = add(shl(tr<B1>(sizeof(P)), tr<B1>(3)), tr<B1>(1));
	B1 tmpcount = umod(count1, tmpmod);
	
	P olddest = dest;
	dest = rcl_(dest, tmpcount, CF);
	CF = _rcl_cf(olddest, tmpcount, CF);
	OF = _quest(_zero(tmpcount), OF, _quest(_eq(count, tr<B1>(1)), _logxor(_sign(dest), CF), undefined));
}

insn rcr(P dest, B1 src)
"c0 /3 rm,imm" ! P=B1
"c1 /3 rm,imm" ! P=osz
"d0 /3 rm,imm_implict:1" ! P=B1
"d1 /3 rm,imm_implict:1" ! P=osz
"d2 /3 rm,r:1" ! P=B1
"d3 /3 rm,r:1" ! P=osz
emu {
	B1 count = src;
	o16: B1 count1 = bitand(count, tr<B1>(31));
	o32: B1 count1 = bitand(count, tr<B1>(31));
	o64: B1 count1 = bitand(count, tr<B1>(63));	// If rex.w is used when not needed, we'll still AND by 63 here..
	B1 tmpmod = add(shl_(tr<B1>(sizeof(P)), tr<B1>(3)), tr<B1>(1));
	B1 tmpcount = umod(count1, tmpmod);
	
	OF = _quest(_zero(tmpcount), OF, _quest(_eq(count, tr<B1>(1)), _logxor(_sign(dest), CF), undefined));
	P olddest = dest;
	dest = rcr_(dest, tmpcount, CF);
	CF = _rcr_cf(olddest, tmpcount, CF);
}

insn shl(P dest, B1 src)
"c0 /4 rm,imm" ! P=B1
"c1 /4 rm,imm" ! P=osz
"d0 /4 rm,imm_implict:1" ! P=B1
"d1 /4 rm,imm_implict:1" ! P=osz
"d2 /4 rm,r:1" ! P=B1
"d3 /4 rm,r:1" ! P=osz
emu {
	o16: B1 tmpcount = bitand(src, tr<B1>(31));
	o32: B1 tmpcount = bitand(src, tr<B1>(31));
	o64: B1 tmpcount = bitand(src, tr<B1>(63));	// If rex.w is used when not needed, we'll still AND by 63 here..
	BIT count0 = _zero(tmpcount);
	P tmpdest = dest;
	dest = shl_(dest, tmpcount);
	CF = _quest(count0, CF, _shl_cf(tmpdest, tmpcount));
	OF = _quest(_eq(tmpcount, 1), _logxor(_sign(dest), CF), _quest(count0, OF, undefined));
	SF = _quest(count0, SF, _sign(dest));
	ZF = _quest(count0, ZF, _zero(dest));
	AF = _quest(count0, AF, undefined);
	PF = _quest(count0, PF, _parity(dest));
}

insn shr(P dest, B1 src)
"c0 /5 rm,imm" ! P=B1
"c1 /5 rm,imm" ! P=osz
"d0 /5 rm,imm_implict:1" ! P=B1
"d1 /5 rm,imm_implict:1" ! P=osz
"d2 /5 rm,r:1" ! P=B1
"d3 /5 rm,r:1" ! P=osz
emu {
	o16: B1 tmpcount = bitand(src, tr<B1>(31));
	o32: B1 tmpcount = bitand(src, tr<B1>(31));
	o64: B1 tmpcount = bitand(src, tr<B1>(63));	// If rex.w is used when not needed, we'll still AND by 63 here..
	BIT count0 = _zero(tmpcount);
	P tmpdest = dest;
	dest = shru_(dest, tmpcount);
	CF = _quest(count0, CF, _shr_cf(tmpdest, tmpcount));
	OF = _quest(_eq(tmpcount, 1), _sign(tmpdest), _quest(count0, OF, undefined));
	SF = _quest(count0, SF, _sign(dest));
	ZF = _quest(count0, ZF, _zero(dest));
	AF = _quest(count0, AF, undefined);
	PF = _quest(count0, PF, _parity(dest));
}

insn _sal(P dest, B1 src)
"c0 /6 rm,imm" ! P=B1, asm_skip
"c1 /6 rm,imm" ! P=osz, asm_skip
"d0 /6 rm,imm_implict:1" ! P=B1, asm_skip
"d1 /6 rm,imm_implict:1" ! P=osz, asm_skip
"d2 /6 rm,r:1" ! P=B1, asm_skip
"d3 /6 rm,r:1" ! P=osz, asm_skip
dis all {
	write("shl");
	space();
	write_args();
}
emu {
	o16: B1 tmpcount = bitand(src, tr<B1>(31));
	o32: B1 tmpcount = bitand(src, tr<B1>(31));
	o64: B1 tmpcount = bitand(src, tr<B1>(63));	// If rex.w is used when not needed, we'll still AND by 63 here..
	BIT count0 = _zero(tmpcount);
	P tmpdest = dest;
	dest = shl_(dest, tmpcount);
	CF = _quest(count0, CF, _shl_cf(tmpdest, tmpcount));
	OF = _quest(_eq(tmpcount, tr<B1>(1)), _logxor(_sign(dest), CF), _quest(count0, OF, undefined));
	SF = _quest(count0, SF, _sign(dest));
	ZF = _quest(count0, ZF, _zero(dest));
	AF = _quest(count0, AF, undefined);
	PF = _quest(count0, PF, _parity(dest));
}

insn sar(P dest, B1 src)
"c0 /7 rm,imm" ! P=B1
"c1 /7 rm,imm" ! P=osz
"d0 /7 rm,imm_implict:1" ! P=B1
"d1 /7 rm,imm_implict:1" ! P=osz
"d2 /7 rm,r:1" ! P=B1
"d3 /7 rm,r:1" ! P=osz
emu {
	o16: B1 tmpcount = bitand(src, tr<B1>(31));
	o32: B1 tmpcount = bitand(src, tr<B1>(31));
	o64: B1 tmpcount = bitand(src, tr<B1>(63));	// If rex.w is used when not needed, we'll still AND by 63 here..
	BIT count0 = _zero(tmpcount);
	P tmpdest = dest;
	dest = shrs_(dest, tmpcount);
	CF = _quest(count0, CF, _sar_cf(tmpdest, tmpcount));
	OF = _quest(_eq(tmpcount, 1), _sign(tmpdest), _quest(count0, OF, undefined));
	SF = _quest(count0, SF, _sign(dest));
	ZF = _quest(count0, ZF, _zero(dest));
	AF = _quest(count0, AF, undefined);
	PF = _quest(count0, PF, _parity(dest));
}

// --- end shift/rotate opcodes ---

// --- begin vmx instructions ---
// these have no associated semantics (i.e. no 'emu' section).

insn invept(P x, B16 y)
"66 0f 38 &80 /r /mdm r,mem" ! P=osz_ptr	// 66 0f 38 80

insn invvpid(P x, B16 y)
"66 0f 38 &81 /r /mdm r,mem" ! P=osz_ptr	// 66 0f 38 81

insn vmcall()
"0f 01 /0 /md3 /rm1 void"			// 0f 01 c1

insn vmclear(B8 x)
"nofx 66 0f c7 /6 /mdm mem"			// 66 0f c7 /6

insn vmlaunch()
"0f 01 /0 /md3 /rm2 void"			// 0f 01 c2

insn vmresume()
"0f 01 /0 /md3 /rm3 void"			// 0f 01 c3

insn vmptrld(B8 x)
"nofx no66 0f c7 /6 /mdm mem"			// 0f c7 /6

insn vmptrst(B8 x)
"nofx no66 0f c7 /7 /mdm mem"			// 0f c7 /7

insn vmread(P x, P y)
"nofx no66 0f 78 /r rm,r" ! P=osz_ptr		// 0f 78

insn vmwrite(P x, P y)
"nofx no66 0f 79 /r r,rm" ! P=osz_ptr		// 0f 79

insn vmxoff()
"0f 01 /0 /md3 /rm4 void"			// 0f 01 c4

insn vmxon(B8 x)
"no66 f3 0f c7 /6 /mdm mem"			// f3 0f c7 /6

// --- end vmx instructions ---

// --- begin experimental opcodes ---

insn fxtract()
//"d9 &f4 void" // 11 110 100
"d9 /6 /md3 /rm4 void"

insn phaddw(P x, P y)
"no66 0f 38 &01 /r reg_mmx,reg_mmx/mem" ! P=B8
"66 0f 38 &01 /r reg_xmm,reg_xmm/mem" ! P=B16

insn phaddd(P x, P y)
"no66 0f 38 &02 /r reg_mmx,reg_mmx/mem" ! P=B8
"66 0f 38 &02 /r reg_xmm,reg_xmm/mem" ! P=B16

insn f2xm1()
//"d9 &f0 void" //11 110 000
"d9 /6 /md3 /rm0 void"

insn fcos()
//"d9 &ff void" // 11 111 111
"d9 /7 /md3 /rm7 void"

// begin new instructions

insn movaps(P x, P y)
"0f 28 /r reg_xmm,reg_xmm/mem" ! P=B16
"0f 29 /r reg_xmm/mem,reg_xmm" ! P=B16

insn stmxcsr(B4 x)
"0f ae /3 /mdm mem"

insn ldmxcsr(B4 x)
"0f ae /2 /mdm mem"

insn movdqu(P x, P y)
"no66 f3 0f 6f /r reg_xmm,reg_xmm/mem" ! P=B16
"no66 f3 0f 7f /r reg_xmm/mem,reg_xmm" ! P=B16

insn _movsd2(P x, P y)
"f2 0f 10 /r reg_xmm,reg_xmm/mem" ! P=B8
"f2 0f 11 /r reg_xmm/mem,reg_xmm" ! P=B8
dis all {
	write("movsd");
	space();
	write_args();
}

insn movss(P x, P y)
"f3 0f 10 /r reg_xmm,reg_xmm/mem" ! P=B4
"f3 0f 11 /r reg_xmm/mem,reg_xmm" ! P=B4

insn movdqa(P x, P y)
"nofx 66 0f 6f /r reg_xmm,reg_xmm/mem" ! P=B16
"nofx 66 0f 7f /r reg_xmm/mem,reg_xmm" ! P=B16

insn prefetch(B1 x)
"0f 0d /0 /mdm mem"

insn prefetchw(B1 x)
"0f 0d /1 /mdm mem"

// end new instructions

// --- end experimental opcodes ---

// When accessing r:0, AL/AX/EAX/RAX is used at all times and neither rex.r nor any other
// rex bit will cause a different value besides the accumulator (register 0) to be used.
// Note that if a REX is used with 88, 8a, b0, or b8 then we're using an 8 bit general
// purpose register with REX. That register is always the LOW BYTE of one of the 16
// general purpose registers selected. If P=osz, then REX.W can be used for osz=64bit mode.
insn mov(P dest, P src)
"88 /r rm,r"			! P=B1
"8a /r r,rm"			! P=B1
"89 /r rm,r"			! P=osz
"8b /r r,rm"			! P=osz
"a0 r:0,mem_fulldisp"		! P=B1, imm64_disp
"a2 mem_fulldisp,r:0"		! P=B1, imm64_disp
"a1 r:0,mem_fulldisp"		! P=osz, imm64_disp
"a3 mem_fulldisp,r:0"		! P=osz, imm64_disp
"b0 r,imm"			! reg_base, P=B1
"b8 r,imm_both"			! reg_base, P=osz, imm64_disp
// Note: reportedly, on the 486, /1../7 are the same as /0 for c6. Not on my cpu, however.
"c6 /0 rm,imm"			! P=B1
"c7 /0 rm,imm"			! P=osz, imm64_sx32
emu {
	dest = src;
}

insn repable _ins(P dest, B2 src)
"6c mem_implict_std,r:2"	! P=B1, no_rex_w
"6d mem_implict_std,r:2"	! P=osz, no_rex_w
dis all {
	write_rep();		// if present
	write("ins");
	write_size_suffix_argsize(0);
}
emu {
	inport(dest, src);
	a16: B2 count = quest(_lognot(DF), tr<B2>(sizeof(P)), neg(tr<B2>(sizeof(P))));
	a16: DI = add(DI, count);
	a32: B4 count = quest(_lognot(DF), tr<B4>(sizeof(P)), neg(tr<B4>(sizeof(P))));
	a32: EDI = add(EDI, count);
	a64: B8 count = quest(_lognot(DF), tr<B8>(sizeof(P)), neg(tr<B8>(sizeof(P))));
	a64: RDI = add(RDI, count);
}

insn repable _outs(B2 dest, P src)
"6e r:2,mem_implict_sts"		! P=B1, no_rex_w
"6f r:2,mem_implict_sts"		! P=osz, no_rex_w
dis all {
	write_seg_reg();
	write_rep();		// if present
	write("outs");
	write_size_suffix_argsize(1);
}
emu {
	outport(dest, src);
	a16: B2 count = quest(_lognot(DF), tr<B2>(sizeof(P)), neg(tr<B2>(sizeof(P))));
	a16: SI = add(SI, count);
	a32: B4 count = quest(_lognot(DF), tr<B4>(sizeof(P)), neg(tr<B4>(sizeof(P))));
	a32: ESI = add(ESI, count);
	a64: B8 count = quest(_lognot(DF), tr<B8>(sizeof(P)), neg(tr<B8>(sizeof(P))));
	a64: RSI = add(RSI, count);
}

insn bound(P x, P y)
"62 /r /mdm r,mem_ea_dbl"		! P=osz, no64

// This instruction is valid only in protected mode.
// Not valid in 64-bit mode.
// arpl: valid only in protected mode. In real or vm86 mode it will #UD (int 6).
// arpl: in 64 bit mode this is movsxd, used with rex.w.

insn arpl(B2 x, B2 y)
"63 /r rm,r"			! like_arpl

// o16 : dest[15:0] = src[15:0] (not mentioned by intel)
// o32 : dest = src (discouraged)
// o64 : dest = sx<B8>(src) -- this is usual usage
insn movsxd(P dest, B4 src)
"63 /r r,rm"			! like_movsxd, P=osz

// This instruction is valid only in SMM. I suppose its 'defined behavior' in other modes
// is to generate #UD (interrupt 6).
insn ctrlxfer rsm()
"0f aa void"

// This always generates interrupt 6. It will never be defined, even in future processors.
insn ctrlxfer ud2()
"0f 0b void"

insn ctrlxfer sysenter()
"0f 34 void"

insn ctrlxfer sysexit()
"0f 35 void"

insn ctrlxfer syscall()
"0f 05 void"

insn ctrlxfer sysret()
"0f 07 void"

insn sgdt(B2 arg)
"0f 01 /0 /mdm mem_ea_lim"

insn sidt(B2 arg)
"0f 01 /1 /mdm mem_ea_lim"

insn lgdt(B2 arg)
"0f 01 /2 /mdm mem_ea_lim"

insn lidt(B2 arg)
"0f 01 /3 /mdm mem_ea_lim"

insn sldt(P x)
"0f 00 /0 /mdm mem"		! P=B2
"0f 00 /0 /md3 r"		! P=osz, reg_rm

insn lldt(B2 src)
"0f 00 /2 rm"

insn smsw(P x)
"0f 01 /4 /mdm mem"		! P=B2
"0f 01 /4 /md3 r"		! P=osz, reg_rm

insn lmsw(B2 src)
"0f 01 /6 rm"

insn str(P x)
"0f 00 /1 /mdm mem"		! P=B2
"0f 00 /1 /md3 r"		! P=osz, reg_rm

insn verr(B2 x)
"0f 00 /4 rm"

insn verw(B2 x)
"0f 00 /5 rm"

insn wbinvd()
"0f 09 void"

insn wrmsr()
"0f 30 void"

insn rdmsr()
"0f 32 void"

insn rdpmc()
"0f 33 void"

insn rdtsc()
"0f 31 void"

insn clts()
"0f 06 void"

insn cpuid()
"0f a2 void"

insn invd()
"0f 08 void"

insn invlpg(B1 src)
"0f 01 /7 /mdm mem_ea_eai"

// NOTE: When we decode, rm has become reg_gr/mem.
// This automatically uses the 'r/m' field of the modr/m,
// whereas the r operand uses the reg/op field when /r is used.
insn lar(P x, Q y)
"0f 02 /r /md3 r,rm"	! P=osz, Q=osz_max32
"0f 02 /r /mdm r,rm"	! P=osz, Q=B2

insn lsl(P x, Q y)
"0f 03 /r /md3 r,rm"	! P=osz, Q=osz_max32
"0f 03 /r /mdm r,rm"	! P=osz, Q=B2

insn ltr(B2 src)
"0f 00 /3 rm"

// Again, the rm's here are really reg's since we use /md3.
insn _movcr(P dest, P src)
"0f 20 /r /md3 rm,reg_cr"	! P=osz_ptr
"0f 22 /r /md3 reg_cr,rm"	! P=osz_ptr
dis all {
	write("mov");
	space();
	write_args();
}

insn _movdr(P dest, P src)
"0f 21 /r /md3 rm,reg_dr"	! P=osz_ptr
"0f 23 /r /md3 reg_dr,rm"	! P=osz_ptr
dis all {
	write("mov");
	space();
	write_args();
}

// Is this really no64?
insn les(P x, P target)
"c4 /r /mdm r,mem_ea_seg"	! P=osz, no64
emu {
	o16: ES = seg16(target);
	o16: x = ofs16(target);
	o32: ES = seg32(target);
	o32: x = ofs32(target);
	o64: ES = seg64(target);
	o64: x = ofs64(target);
}

// Is this really no64?
insn lds(P x, P target)
"c5 /r /mdm r,mem_ea_seg"	! P=osz, no64
emu {
	o16: DS = seg16(target);
	o16: x = ofs16(target);
	o32: DS = seg32(target);
	o32: x = ofs32(target);
	o64: DS = seg64(target);
	o64: x = ofs64(target);
}

insn lss(P x, P target)
"0f b2 /r /mdm r,mem_ea_seg"	! P=osz
emu {
	o16: SS = seg16(target);
	o16: x = ofs16(target);
	o32: SS = seg32(target);
	o32: x = ofs32(target);
	o64: SS = seg64(target);
	o64: x = ofs64(target);
}

insn lfs(P x, P target)
"0f b4 /r /mdm r,mem_ea_seg"	! P=osz
emu {
	o16: FS = seg16(target);
	o16: x = ofs16(target);
	o32: FS = seg32(target);
	o32: x = ofs32(target);
	o64: FS = seg64(target);
	o64: x = ofs64(target);
}

insn lgs(P x, P target)
"0f b5 /r /mdm r,mem_ea_seg"	! P=osz
emu {
	o16: GS = seg16(target);
	o16: x = ofs16(target);
	o32: GS = seg32(target);
	o32: x = ofs32(target);
	o64: GS = seg64(target);
	o64: x = ofs64(target);
}

// WARNING: NOT TESTED!
insn bsf(P dest, P src)
"0f bc /r r,rm"				! P=osz

// WARNING: NOT TESTED!
insn bsr(P dest, P src)
"0f bd /r r,rm"				! P=osz

// WARNING: NOT TESTED!
// -> bt dword [x],eax     -- won't suppress 'dword'.
// -> bt dword [x],byte 3  -- won't suppress 'byte'.
insn bt(P x, Q y)
"0f a3 /r rm,r"				! P=osz, Q=osz
"0f ba /4 rm,imm"			! P=osz, Q=B1

// WARNING: NOT TESTED!
insn lockable bts(P x, Q y)
"0f ab /r rm,r"				! P=osz, Q=osz
"0f ba /5 rm,imm"			! P=osz, Q=B1

// WARNING: NOT TESTED!
insn lockable btr(P x, Q y)
"0f b3 /r rm,r"				! P=osz, Q=osz
"0f ba /6 rm,imm"			! P=osz, Q=B1

// WARNING: NOT TESTED!
insn lockable btc(P x, Q y)
"0f bb /r rm,r"				! P=osz, Q=osz
"0f ba /7 rm,imm"			! P=osz, Q=B1

// WARNING: NOT TESTED!
insn bswap(P x)
"0f c8 r"				! P=osz_min32, reg_base

// WARNING: NOT TESTED!
// FIXME: OF, AF, CF, SF, PF ???
insn lockable cmpxchg(P x, P y)
//"0f b0 /r rm,r,r:0"			! P=B1
//"0f b1 /r rm,r,r:0"			! P=osz
"0f b0 /r rm,r"			! P=B1
"0f b1 /r rm,r"			! P=osz
dis all {
	write("cmpxchg");
	space();
	write_arg(0);
	comma();
	write_arg(1);
}

// WARNING: NOT TESTED!
insn lockable _cmpxchgxb(P dest)
"nofx no66 0f c7 /1 /mdm mem"	! P=osz_min64
"nofx 66 0f c7 /1 /mdm mem"		! P=osz_min64, asm_skip
dis all {
	write_osz64("cmpxchg16b", "cmpxchg8b");
	space();
	write_arg(0, false);	// don't write size (for nasm)
}

// WARNING: NOT TESTED!
insn movzx(P dest, Q src)
"0f b6 /r r,rm"				! P=osz, Q=B1
"0f b7 /r r,rm"				! P=osz_min32, Q=B2
emu {
	dest = zx<P>(src);
}


// WARNING: NOT TESTED!
insn movsx(P dest, Q src)
"0f be /r r,rm"				! P=osz, Q=B1
"0f bf /r r,rm"				! P=osz_min32, Q=B2
emu {
	dest = zx<P>(src);
}

// WARNING: NOT TESTED!
insn shld(P x, P y, B1 z)
"0f a4 /r rm,r,imm"			! P=osz
"0f a5 /r rm,r,r:1"			! P=osz

// WARNING: NOT TESTED!
insn shrd(P x, P y, B1 z)
"0f ac /r rm,r,imm"			! P=osz
"0f ad /r rm,r,r:1"			! P=osz

// WARNING: NOT TESTED!
insn lockable xadd(P dest, P src)
"0f c0 /r rm,r"				! P=B1
"0f c1 /r rm,r"				! P=osz

insn enter(B2 x, B1 y)
"c8 imm,imm"
emu {
	assert(_eq(y, tr<B1>(0)), tr<B1>(64));	// else unimplemented!
	s16: push(BP);
	s16: BP = SP;
	s16: reserve(x);
	s32: push(EBP);
	s32: EBP = ESP;
	s32: reserve(zx<B4>(x));
	s64: push(RBP);
	s64: RBP = RSP;
	s64: reserve(zx<B8>(x));
}

insn leave()
"c9 void"
emu {
	s16: SP = BP;
	s16: pop(BP);
	s32: ESP = EBP;
	s32: pop(EBP);
	s64: RSP = RBP;
	s64: pop(RBP);
}

insn hlt()
"f4 void"

insn cli()
"fa void"

insn sti()
"fb void"

insn in(P dest, Q src)
"e4 r:0,imm"		! P=B1, Q=B1		// in al,byte imm8
"e5 r:0,imm"		! P=osz_max32, Q=B1	// in ax/eax,byte imm8
"ec r:0,r:2"		! P=B1, Q=B2		// in al,dx
"ed r:0,r:2"		! P=osz_max32, Q=B2	// in ax/eax,dx
dis all {
	write("in");
	space();
	write_arg(0, false);
	comma();
	write_arg(1, false);
}
emu {
	inport(dest, src);
}

insn out(Q src1, P src2)
"e6 imm,r:0"		! Q=B1, P=B1		// out byte imm8,al
"e7 imm,r:0"		! Q=B1, P=osz_max32	// out byte imm8,ax/eax
"ee r:2,r:0"		! Q=B2, P=B1		// out dx,al
"ef r:2,r:0"		! Q=B2, P=osz_max32	// out dx,ax/eax
dis all {
	write("out");
	space();
	write_arg(0, false);
	comma();
	write_arg(1, false);
}
emu {
	outport(src1, src2);
}

// WARNING: NOT TESTED!
insn "cmovo" _cmovo(P x, P y)
"0f 40 /r r,rm"		! P=osz, has_cc

insn "cmovno" _cmovno(P x, P y)
"0f 41 /r r,rm"		! P=osz, has_cc

insn "cmovc" _cmovc(P x, P y)
"0f 42 /r r,rm"		! P=osz, has_cc

insn "cmovnc" _cmovnc(P x, P y)
"0f 43 /r r,rm"		! P=osz, has_cc

insn "cmovz" _cmovz(P x, P y)
"0f 44 /r r,rm"		! P=osz, has_cc

insn "cmovnz" _cmovnz(P x, P y)
"0f 45 /r r,rm"		! P=osz, has_cc

insn "cmovbe" _cmovbe(P x, P y)
"0f 46 /r r,rm"		! P=osz, has_cc

insn "cmova" _cmova(P x, P y)
"0f 47 /r r,rm"		! P=osz, has_cc

insn "cmovs" _cmovs(P x, P y)
"0f 48 /r r,rm"		! P=osz, has_cc

insn "cmovns" _cmovns(P x, P y)
"0f 49 /r r,rm"		! P=osz, has_cc

insn "cmovp" _cmovp(P x, P y)
"0f 4a /r r,rm"		! P=osz, has_cc

insn "cmovnp" _cmovnp(P x, P y)
"0f 4b /r r,rm"		! P=osz, has_cc

insn "cmovl" _cmovl(P x, P y)
"0f 4c /r r,rm"		! P=osz, has_cc

insn "cmovge" _cmovge(P x, P y)
"0f 4d /r r,rm"		! P=osz, has_cc

insn "cmovle" _cmovle(P x, P y)
"0f 4e /r r,rm"		! P=osz, has_cc

insn "cmovg" _cmovg(P x, P y)
"0f 4f /r r,rm"		! P=osz, has_cc

insn emms()
"0f 77 void"

// Note: table format depends on whether rex.w is used or not.
insn _fxrstor(B16 x)
"0f ae /1 /mdm mem_ea_fxs"
dis all {
	write_osz64("fxrstor64", "fxrstor");
	space();
	write_args();
}

// Note: table format depends on whether rex.w is used or not.
insn _fxsave(B16 x)
"0f ae /0 /mdm mem_ea_fxs"
dis all {
	write_osz64("fxsave64", "fxsave");
	space();
	write_args();
}

insn _pushsr(P x)
"06 reg_sr:0"				! no64, P=osz			// push es
"0e reg_sr:1"				! no64, P=osz			// push cs
"16 reg_sr:2"				! no64, P=osz			// push ss
"1e reg_sr:3"				! no64, P=osz			// push ds
"0f a0 reg_sr:4"			! is64, P=osz			// push fs
"0f a8 reg_sr:5"			! is64, P=osz			// push gs
dis all {
	write_stack_o16_o32_o64();
	write("push");
	space();
	write_args();
}
emu {
	push(x);
}

insn _popsr(P x)
"07 reg_sr:0"				! no64, P=osz			// pop es
"17 reg_sr:2"				! no64, P=osz			// pop ss
"1f reg_sr:3"				! no64, P=osz			// pop ds
"0f a1 reg_sr:4"			! is64, P=osz			// pop fs
"0f a9 reg_sr:5"			! is64, P=osz			// pop gs
dis all {
	write_stack_o16_o32_o64();
	write("pop");
	space();
	write_args();
}
emu {
	pop(x);
}

// 68, 6a -- Intel manual says "Stack pointer is incremented by the size of stack pointer."
//           For a PUSH instruction this is a curious comment, it wouldn't be the 1st time
//           an Intel manual is wrong though! (Even the AMD manuals can be wrong, though).
// Note that sign extension is apparantly used - if the manual can be trusted - when the
// argumnent size is < the stack pointer size.
// For example, if ss.d_b=1 in 16bit/32bit pmode, so the system uses ESP as the stack pointer,
// then if we do in 32bit pmode, o16 push word 0xffff, then according to the manual it is true
// that 0xffff will be pushed on the stack, which is a word. The o16 is what allowed us to
// encode it as a word! However, because ESP is being used and not SP, the stack pointer (ESP)
// is decremented by 32 bits! So 'push ax' is the same as push sx<B4>(AX) if ss.d_b=1. The
// [argument size] is determined by the operand size, but the true size pushed depends on ss.d_b.
// There is one caveat however: it is known to me that PUSH EBX works in 16bit real mode, and in
// this mode, SP will be decremented by 4 and not 2, so that POP EBX will restore EBX afterwords!
// So the manual is doubly wrong--but the encodings here are right as they capture the assembly
// language and machine code information itself.
// IGNORE THE ABOVE DESCRIPTION. IT IS WRONG.
insn _push(P x)
"50 r"						! P=osz, is64, reg_base
"68 imm"					! P=osz, is64, imm64_sx32	// was no64, but is valid in 64bit mode too !
"6a imm"					! P=osz, is64, sx_byte, imm64_sx32
"ff /6 rm"					! P=osz, is64
dis all {
	write_stack_o16_o32_o64();
	write("push");
	space();
	write_args();
}
emu {
	push(x);
}

// 8f -- intel486 supposedly treats 8f /1../7 same as 8f /0 (according to opcodes info
//       that comes with Ralph Brown's interrupt list). MS-DOS DEBUG.EXE reports the
//       same thing. However, the CPU I tested it on only recognized /0 and that's the
//       documented behavior so we'll only use that.
insn _pop(P x)
"58 r"						! P=osz, is64, reg_base
"8f /0 rm"					! P=osz, is64
dis all {
	write_stack_o16_o32_o64();
	write("pop");
	space();
	write_args();
}
emu {
	pop(x);
}

// NOTE: With _movsr*, when it's a memory operand it's always 16-bits. But, if it's a register,
// and it's a destination, it has size osz. (If it's a source, the processor ignores the upper
// 16 bits).
insn _movsrv(B2 dest, B2 src)
"8e /0 reg_sr:0,rm"
"8e /2 reg_sr:2,rm"
"8e /3 reg_sr:3,rm"
"8e /4 reg_sr:4,rm"
"8e /5 reg_sr:5,rm"
dis all {
	write("mov");
	space();
	write_args();
}
emu {
	dest = src;
}

insn _movvsr(P dest, B2 src)
"8c /0 /md3 rm,reg_sr:0"			! P=osz
"8c /1 /md3 rm,reg_sr:1"			! P=osz
"8c /2 /md3 rm,reg_sr:2"			! P=osz
"8c /3 /md3 rm,reg_sr:3"			! P=osz
"8c /4 /md3 rm,reg_sr:4"			! P=osz
"8c /5 /md3 rm,reg_sr:5"			! P=osz
"8c /0 /mdm rm,reg_sr:0"			! P=osz_seg
"8c /1 /mdm rm,reg_sr:1"			! P=osz_seg
"8c /2 /mdm rm,reg_sr:2"			! P=osz_seg
"8c /3 /mdm rm,reg_sr:3"			! P=osz_seg
"8c /4 /mdm rm,reg_sr:4"			! P=osz_seg
"8c /5 /mdm rm,reg_sr:5"			! P=osz_seg
dis all {
	write("mov");
	space();
	write_args();
}
emu {
	dest = zx<P>(src);
}

insn _pusha()
"60 void"						! no64
dis all {
	write("pusha");
	write_size_suffix_osz();		// 16 or 32 bits only possible
}

insn _popa()
"61 void"						! no64
dis all {
	write("popa");
	write_size_suffix_osz();		// 16 or 32 bits only possible
}

insn _pushf()
"9c void"						! is64
dis all {
	write("pushf");
	write_size_suffix_osz();
}
emu {
	o16: push(FLAGS);
	o32: push(EFLAGS);
	o64: push(RFLAGS);
}

insn _popf()
"9d void"						! is64
dis all {
	write("popf");
	write_size_suffix_osz();
}
emu {
	o16: pop(FLAGS);
	o32: pop(EFLAGS);
	o64: pop(RFLAGS);
}

// arguments given here matches disassembly by msdev.
// actually, when is rsi/rdi instead of esi/edi or si/di used? is it the operand size? i imagine its
// the address size.
// now wait a minute. address size defaults to 32 bits without a rex, correct? if so, uses esi/edi in 64 bit
// mode by default, right?
// believe it or not, it works like this: address size is 64 bits, use 67 for 32-bit not 16-bit address size.
// a16 is impossible in 64-bit mode.

// print suffixes here (instead of arguments), and sometimes a segment register prefix.

insn repable _movs(P x, P y)
"a4 mem_implict_std,mem_implict_sts"					! P=B1
"a5 mem_implict_std,mem_implict_sts"					! P=osz
dis all {
	write_seg_reg();
	write_rep();		// if present
	write("movs");
	write_size_suffix_argsize(0);
}
emu {
	x = y;

	a16: B2 count = quest(_lognot(DF), tr<B2>(sizeof(P)), neg(tr<B2>(sizeof(P))));
	a16: SI = add(SI, count);
	a16: DI = add(DI, count);
	a32: B4 count = quest(_lognot(DF), tr<B4>(sizeof(P)), neg(tr<B4>(sizeof(P))));
	a32: ESI = add(ESI, count);
	a32: EDI = add(EDI, count);
	a64: B8 count = quest(_lognot(DF), tr<B8>(sizeof(P)), neg(tr<B8>(sizeof(P))));
	a64: RSI = add(RSI, count);
	a64: RDI = add(RDI, count);
}

insn repcond _cmps(P dest, P src)
"a6 mem_implict_sts,mem_implict_std"					! P=B1
"a7 mem_implict_sts,mem_implict_std"					! P=osz
dis all {
	write_seg_reg();
	write_repcc();		// if present
	write("cmps");
	write_size_suffix_argsize(0);
}
emu {
	OF = _sub_of(dest, src);
	AF = _sub_af(dest, src);
	CF = _sub_cf(dest, src);
	P tmp = sub(dest, src);
	SF = _sign(tmp);
	ZF = _zero(tmp);
	PF = _parity(tmp);

	a16: B2 count = quest(_lognot(DF), tr<B2>(sizeof(P)), neg(tr<B2>(sizeof(P))));
	a16: SI = add(SI, count);
	a16: DI = add(DI, count);
	a32: B4 count = quest(_lognot(DF), tr<B4>(sizeof(P)), neg(tr<B4>(sizeof(P))));
	a32: ESI = add(ESI, count);
	a32: EDI = add(EDI, count);
	a64: B8 count = quest(_lognot(DF), tr<B8>(sizeof(P)), neg(tr<B8>(sizeof(P))));
	a64: RSI = add(RSI, count);
	a64: RDI = add(RDI, count);
}

insn repable _stos(P x, P y)
"aa mem_implict_std,r:0"					! P=B1
"ab mem_implict_std,r:0"					! P=osz
dis all {
	write_rep();		// if present
	write("stos");
	write_size_suffix_argsize(0);
}
emu {
	x = y;

	a16: B2 count = quest(_lognot(DF), tr<B2>(sizeof(P)), neg(tr<B2>(sizeof(P))));
	a16: DI = add(DI, count);
	a32: B4 count = quest(_lognot(DF), tr<B4>(sizeof(P)), neg(tr<B4>(sizeof(P))));
	a32: EDI = add(EDI, count);
	a64: B8 count = quest(_lognot(DF), tr<B8>(sizeof(P)), neg(tr<B8>(sizeof(P))));
	a64: RDI = add(RDI, count);
}


insn repable _lods(P x, P y)
"ac r:0,mem_implict_sts"					! P=B1
"ad r:0,mem_implict_sts"					! P=osz
dis all {
	write_seg_reg();
	write_rep();		// if present
	write("lods");		// is rep lods useful?
	write_size_suffix_argsize(0);
}
emu {
	x = y;

	a16: B2 count = quest(_lognot(DF), tr<B2>(sizeof(P)), neg(tr<B2>(sizeof(P))));
	a16: SI = add(SI, count);
	a32: B4 count = quest(_lognot(DF), tr<B4>(sizeof(P)), neg(tr<B4>(sizeof(P))));
	a32: ESI = add(ESI, count);
	a64: B8 count = quest(_lognot(DF), tr<B8>(sizeof(P)), neg(tr<B8>(sizeof(P))));
	a64: RSI = add(RSI, count);
}

insn repable _scas(P dest, P src)
"ae r:0,mem_implict_std"					! P=B1
"af r:0,mem_implict_std"					! P=osz
dis all {
	write_repcc();		// if present
	write("scas");
	write_size_suffix_argsize(0);
}
emu {
	OF = _sub_of(dest, src);
	AF = _sub_af(dest, src);
	CF = _sub_cf(dest, src);
	P tmp = sub(dest, src);
	SF = _sign(tmp);
	ZF = _zero(tmp);
	PF = _parity(tmp);

	a16: B2 count = quest(_lognot(DF), tr<B2>(sizeof(P)), neg(tr<B2>(sizeof(P))));
	a16: DI = add(DI, count);
	a32: B4 count = quest(_lognot(DF), tr<B4>(sizeof(P)), neg(tr<B4>(sizeof(P))));
	a32: EDI = add(EDI, count);
	a64: B8 count = quest(_lognot(DF), tr<B8>(sizeof(P)), neg(tr<B8>(sizeof(P))));
	a64: RDI = add(RDI, count);
}

// note: order of arguments is arbitrary.
insn _test(P src1, P src2)
"84 /r rm,r"						! P=B1
"85 /r rm,r"						! P=osz
"a8 r:0,imm"						! P=B1
"a9 r:0,imm"						! P=osz, imm64_sx32
"f6 /0 rm,imm"						! P=B1
"f7 /0 rm,imm"						! P=osz, imm64_sx32
// NOTE: on the machine I tested, /1 is treated as an undocumented alias for /0. This is an
// undocumented alias and could be removed at any time from future Intel processors; it's
// not documented in the Intel manual. I think I got the /1 idea from the opcodes listing
// that comes with Ralph Brown's interrupt list.
"f6 /1 rm,imm"						! P=B1, asm_skip
"f7 /1 rm,imm"						! P=osz, imm64_sx32, asm_skip
dis all {
	write("test");
	space();
	write_args();
}
emu {
	OF = 0;
	AF = undefined;
	CF = 0;
	P result = bitand(src1, src2);
	SF = _sign(result);
	ZF = _zero(result);
	PF = _parity(result);
}

// note: order of arguments is arbitrary. but, our code requires rm be first so we can allow lock.
// When disassembling, check to see if basecode is 0x90 and print nop if so -- if rex does not make
// the other argument r8.
// Also because the order of arguments is arbitrary, some disassemblers may flip the
// arguments over what we have here.
insn lock_always _xchg(P x, P y)
"86 /r rm,r"						! P=B1
"87 /r rm,r"						! P=osz
"90 r:0,r"						! reg_base, P=osz
dis all {
	do_nop_xchg("nop", "xchg", 1);	// if argvalue(1) is 0, nop else do xchg
}
emu {
	P z = x;
	x = y;
	y = z;
}

insn lockable not(P x)
"f6 /2 rm"							! P=B1
"f7 /2 rm"							! P=osz
emu {
	x = bitnot(x);
}

insn lockable neg(P x)
"f6 /3 rm"							! P=B1
"f7 /3 rm"							! P=osz
emu {
	OF = _sub_of(tr<P>(0), x);
	AF = _sub_af(tr<P>(0), x);
	CF = _sub_cf(tr<P>(0), x);
	x = neg(x);
	SF = _sign(x);
	ZF = _zero(x);
	PF = _parity(x);
}

insn lockable inc(P arg)
"40 r"								! P=osz, no64, reg_base
"fe /0 rm"							! P=B1
"ff /0 rm"							! P=osz
emu {
	OF = _add_of(arg, tr<P>(1));
	AF = _add_af(arg, tr<P>(1));
	arg = add(arg, tr<P>(1));
	SF = _sign(arg);
	ZF = _zero(arg);
	PF = _parity(arg);
}

insn lockable dec(P arg)
"48 r"								! P=osz, no64, reg_base
"fe /1 rm"							! P=B1
"ff /1 rm"							! P=osz
emu {
	OF = _sub_of(arg, tr<P>(1));
	AF = _sub_af(arg, tr<P>(1));
	arg = sub(arg, tr<P>(1));
	SF = _sign(arg);
	ZF = _zero(arg);
	PF = _parity(arg);
}

insn _xlat(B1 xs)
"d7 mem_implict_xls"
dis all {
	write_xlat_o16_o32_o64();
	write_seg_reg();				// if not default
	write("xlatb");
}
emu {
	AL = xs;
}

// The following refers to setCC instructions.
// Note: the Intel manual does not use /0 but totally ignores the reg/op field.
// In practice, /1../7 function the same as /0, but assemblers use /0 so we
// mainly expect to see /0. It is theoretically possible future processors could
// demand reg/op=0 for SETcc to work, but this is unlikely.

insn "seto" _seto(B1 dest)
"0f 90 /0 rm"				! has_cc
"0f 90 /1 rm"				! asm_skip, has_cc
"0f 90 /2 rm"				! asm_skip, has_cc
"0f 90 /3 rm"				! asm_skip, has_cc
"0f 90 /4 rm"				! asm_skip, has_cc
"0f 90 /5 rm"				! asm_skip, has_cc
"0f 90 /6 rm"				! asm_skip, has_cc
"0f 90 /7 rm"				! asm_skip, has_cc

insn "setno" _setno(B1 dest)
"0f 91 /0 rm"				! has_cc
"0f 91 /1 rm"				! asm_skip, has_cc
"0f 91 /2 rm"				! asm_skip, has_cc
"0f 91 /3 rm"				! asm_skip, has_cc
"0f 91 /4 rm"				! asm_skip, has_cc
"0f 91 /5 rm"				! asm_skip, has_cc
"0f 91 /6 rm"				! asm_skip, has_cc
"0f 91 /7 rm"				! asm_skip, has_cc

insn "setc" _setc(B1 dest)
"0f 92 /0 rm"				! has_cc
"0f 92 /1 rm"				! asm_skip, has_cc
"0f 92 /2 rm"				! asm_skip, has_cc
"0f 92 /3 rm"				! asm_skip, has_cc
"0f 92 /4 rm"				! asm_skip, has_cc
"0f 92 /5 rm"				! asm_skip, has_cc
"0f 92 /6 rm"				! asm_skip, has_cc
"0f 92 /7 rm"				! asm_skip, has_cc

insn "setnc" _setnc(B1 dest)
"0f 93 /0 rm"				! has_cc
"0f 93 /1 rm"				! asm_skip, has_cc
"0f 93 /2 rm"				! asm_skip, has_cc
"0f 93 /3 rm"				! asm_skip, has_cc
"0f 93 /4 rm"				! asm_skip, has_cc
"0f 93 /5 rm"				! asm_skip, has_cc
"0f 93 /6 rm"				! asm_skip, has_cc
"0f 93 /7 rm"				! asm_skip, has_cc

insn "setz" _setz(B1 dest)
"0f 94 /0 rm"				! has_cc
"0f 94 /1 rm"				! asm_skip, has_cc
"0f 94 /2 rm"				! asm_skip, has_cc
"0f 94 /3 rm"				! asm_skip, has_cc
"0f 94 /4 rm"				! asm_skip, has_cc
"0f 94 /5 rm"				! asm_skip, has_cc
"0f 94 /6 rm"				! asm_skip, has_cc
"0f 94 /7 rm"				! asm_skip, has_cc

insn "setnz" _setnz(B1 dest)
"0f 95 /0 rm"				! has_cc
"0f 95 /1 rm"				! asm_skip, has_cc
"0f 95 /2 rm"				! asm_skip, has_cc
"0f 95 /3 rm"				! asm_skip, has_cc
"0f 95 /4 rm"				! asm_skip, has_cc
"0f 95 /5 rm"				! asm_skip, has_cc
"0f 95 /6 rm"				! asm_skip, has_cc
"0f 95 /7 rm"				! asm_skip, has_cc

insn "setbe" _setbe(B1 dest)
"0f 96 /0 rm"				! has_cc
"0f 96 /1 rm"				! asm_skip, has_cc
"0f 96 /2 rm"				! asm_skip, has_cc
"0f 96 /3 rm"				! asm_skip, has_cc
"0f 96 /4 rm"				! asm_skip, has_cc
"0f 96 /5 rm"				! asm_skip, has_cc
"0f 96 /6 rm"				! asm_skip, has_cc
"0f 96 /7 rm"				! asm_skip, has_cc

insn "seta" _seta(B1 dest)
"0f 97 /0 rm"				! has_cc
"0f 97 /1 rm"				! asm_skip, has_cc
"0f 97 /2 rm"				! asm_skip, has_cc
"0f 97 /3 rm"				! asm_skip, has_cc
"0f 97 /4 rm"				! asm_skip, has_cc
"0f 97 /5 rm"				! asm_skip, has_cc
"0f 97 /6 rm"				! asm_skip, has_cc
"0f 97 /7 rm"				! asm_skip, has_cc

insn "sets" _sets(B1 dest)
"0f 98 /0 rm"				! has_cc
"0f 98 /1 rm"				! asm_skip, has_cc
"0f 98 /2 rm"				! asm_skip, has_cc
"0f 98 /3 rm"				! asm_skip, has_cc
"0f 98 /4 rm"				! asm_skip, has_cc
"0f 98 /5 rm"				! asm_skip, has_cc
"0f 98 /6 rm"				! asm_skip, has_cc
"0f 98 /7 rm"				! asm_skip, has_cc

insn "setns" _setns(B1 dest)
"0f 99 /0 rm"				! has_cc
"0f 99 /1 rm"				! asm_skip, has_cc
"0f 99 /2 rm"				! asm_skip, has_cc
"0f 99 /3 rm"				! asm_skip, has_cc
"0f 99 /4 rm"				! asm_skip, has_cc
"0f 99 /5 rm"				! asm_skip, has_cc
"0f 99 /6 rm"				! asm_skip, has_cc
"0f 99 /7 rm"				! asm_skip, has_cc

insn "setp" _setp(B1 dest)
"0f 9a /0 rm"				! has_cc
"0f 9a /1 rm"				! asm_skip, has_cc
"0f 9a /2 rm"				! asm_skip, has_cc
"0f 9a /3 rm"				! asm_skip, has_cc
"0f 9a /4 rm"				! asm_skip, has_cc
"0f 9a /5 rm"				! asm_skip, has_cc
"0f 9a /6 rm"				! asm_skip, has_cc
"0f 9a /7 rm"				! asm_skip, has_cc

insn "setnp" _setnp(B1 dest)
"0f 9b /0 rm"				! has_cc
"0f 9b /1 rm"				! asm_skip, has_cc
"0f 9b /2 rm"				! asm_skip, has_cc
"0f 9b /3 rm"				! asm_skip, has_cc
"0f 9b /4 rm"				! asm_skip, has_cc
"0f 9b /5 rm"				! asm_skip, has_cc
"0f 9b /6 rm"				! asm_skip, has_cc
"0f 9b /7 rm"				! asm_skip, has_cc

insn "setl" _setl(B1 dest)
"0f 9c /0 rm"				! has_cc
"0f 9c /1 rm"				! asm_skip, has_cc
"0f 9c /2 rm"				! asm_skip, has_cc
"0f 9c /3 rm"				! asm_skip, has_cc
"0f 9c /4 rm"				! asm_skip, has_cc
"0f 9c /5 rm"				! asm_skip, has_cc
"0f 9c /6 rm"				! asm_skip, has_cc
"0f 9c /7 rm"				! asm_skip, has_cc

insn "setge" _setge(B1 dest)
"0f 9d /0 rm"				! has_cc
"0f 9d /1 rm"				! asm_skip, has_cc
"0f 9d /2 rm"				! asm_skip, has_cc
"0f 9d /3 rm"				! asm_skip, has_cc
"0f 9d /4 rm"				! asm_skip, has_cc
"0f 9d /5 rm"				! asm_skip, has_cc
"0f 9d /6 rm"				! asm_skip, has_cc
"0f 9d /7 rm"				! asm_skip, has_cc

insn "setle" _setle(B1 dest)
"0f 9e /0 rm"				! has_cc
"0f 9e /1 rm"				! asm_skip, has_cc
"0f 9e /2 rm"				! asm_skip, has_cc
"0f 9e /3 rm"				! asm_skip, has_cc
"0f 9e /4 rm"				! asm_skip, has_cc
"0f 9e /5 rm"				! asm_skip, has_cc
"0f 9e /6 rm"				! asm_skip, has_cc
"0f 9e /7 rm"				! asm_skip, has_cc

insn "setg" _setg(B1 dest)
"0f 9f /0 rm"				! has_cc
"0f 9f /1 rm"				! asm_skip, has_cc
"0f 9f /2 rm"				! asm_skip, has_cc
"0f 9f /3 rm"				! asm_skip, has_cc
"0f 9f /4 rm"				! asm_skip, has_cc
"0f 9f /5 rm"				! asm_skip, has_cc
"0f 9f /6 rm"				! asm_skip, has_cc
"0f 9f /7 rm"				! asm_skip, has_cc

insn _nopmb(P dummy)
"0f 1f /0 rm"						! P=osz			// nasm-style
dis all {
	write("nop");
	space();
	write_args();
}
emu {
	// No operation.
}

insn _usalc()
"d6 void"
dis all {
	write("salc");
}

// WARNING: not tested!
// _sxacc = sign extend to accumulator. (r/e)ax <- s/x((r/e)ax.lo).
insn _sxacc()
"98 void"
dis all {
	write_osz("cbw", "cwde", "cdqe");
}
emu {
	o16: AX = sx<B2>(AL);
	o32: EAX = sx<B4>(AX);
	o64: RAX = sx<B8>(EAX);
}

// WARNING: not tested!
// _sxdax = sign extend to dax. (r/e)dx:(r/e)ax <- sx((r/e)ax).
insn _sxdax()
"99 void"
dis all {
	write_osz("cwd", "cdq", "cqo");
}
emu {
	o16: DX = sub(tr<B2>(0), zx<B2>(_sign(AX)));
	o32: EDX = sub(tr<B4>(0), zx<B4>(_sign(EAX)));
	o64: RDX = sub(tr<B8>(0), zx<B8>(_sign(RAX)));
}

// According to Wikipedia, some older Intel processors do not support sahf/lahf in 64-bit mode.
insn sahf()
"9e void"
emu {
	SF = _lognot(_zero(bitand(shru(AH, tr<B1>(7)), tr<B1>(1))));
	ZF = _lognot(_zero(bitand(shru(AH, tr<B1>(6)), tr<B1>(1))));
	AF = _lognot(_zero(bitand(shru(AH, tr<B1>(4)), tr<B1>(1))));
	PF = _lognot(_zero(bitand(shru(AH, tr<B1>(2)), tr<B1>(1))));
	CF = _lognot(_zero(bitand(AH, tr<B1>(1))));
}

// According to Wikipedia, some older Intel processors do not support sahf/lahf in 64-bit mode.
insn lahf()
"9f void"
emu {
	// bits 5 and 3 are zero; bit 1 is one.
	AH = bitor(tr<B1>(2), zx<B1>(CF));
	AH = bitor(AH, shl(zx<B1>(PF), tr<B1>(2)));
	AH = bitor(AH, shl(zx<B1>(AF), tr<B1>(4)));
	AH = bitor(AH, shl(zx<B1>(ZF), tr<B1>(6)));
	AH = bitor(AH, shl(zx<B1>(SF), tr<B1>(7)));
}

insn cmc()
"f5 void"
emu {
	CF = _lognot(CF);
}

// Note: given an emu fragment of "dest = src", if src is an immediate, it is automatically
// to the size of dest.

insn clc()
"f8 void"
emu {
	CF = 0;
}

insn stc()
"f9 void"
emu {
	CF = 1;
}

insn cld()
"fc void"
emu {
	DF = 0;
}

insn std()
"fd void"
emu {
	DF = 1;
}

insn lea(P dest, Q src)
"8d /r /mdm r,mem_ea_eal"				! P=osz, Q=asz
emu {
	dest = zx<P>(src);		// zx<> can truncate as well
}

insn into()
"ce void"						! no64
emu {
	assert(_lognot(OF), tr<B1>(4));				// else generate interrupt
}

// _cmul is officially signed but it can be used for unsigned too. destination operand has same size
// as source operands. _cmul is known as 'imul' in intel docs.

insn _cmul3(P dest, P src1, P src2)
"69 /r r,rm,imm"					! P=osz, imm64_sx32
"6b /r r,rm,imm"					! P=osz, sx_byte,imm64_sx32
dis all {
	write("imul");
	space();
	write_args();
}
emu {
	BIT tmp = _smul_overflow(src1, src2);
	dest = cmul(src1, src2);
	SF = undefined;
	ZF = undefined;
	AF = undefined;
	PF = undefined;
	CF = tmp;
	OF = tmp;
}

insn _cmul2(P dest, P src)
"0f af /r r,rm"						! P=osz
dis all {
	write("imul");
	space();
	write_args();
}
emu {
	BIT tmp = _smul_overflow(dest, src);
	dest = cmul(dest, src);
	SF = undefined;
	ZF = undefined;
	AF = undefined;
	PF = undefined;
	CF = tmp;
	OF = tmp;
}

insn _imulb(B1 arg)
"f6 /5 rm"
dis all {
	write("imul");
	space();
	write_arg(0, true, true);
}
emu {
	BIT tmp = _smul_overflow(AL, arg);
	AX = cmul(sx<B2>(AL), sx<B2>(arg));
	SF = undefined;
	ZF = undefined;
	AF = undefined;
	PF = undefined;
	CF = tmp;
	OF = tmp;
}

// Note -- in the future we may want to use low() and high() here for idiom recognition purposes.
insn imul(P arg)
"f7 /5 rm"							! P=osz
emu {
	o16: BIT tmp = _smul_overflow(AX, tr<B2>(arg));
	o16: DX = smulhi(AX, tr<B2>(arg));
	o16: AX = cmul(AX, tr<B2>(arg));

	o32: BIT tmp = _smul_overflow(EAX, tr<B4>(arg));
	o32: EDX = smulhi(EAX, tr<B4>(arg));
	o32: EAX = cmul(EAX, tr<B4>(arg));

	o64: BIT tmp = _smul_overflow(RAX, tr<B8>(arg));
	o64: RDX = smulhi(RAX, tr<B8>(arg));
	o64: RAX = cmul(RAX, tr<B8>(arg));

	SF = undefined;
	ZF = undefined;
	AF = undefined;
	PF = undefined;
	CF = tmp;
	OF = tmp;
}

insn _mulb(B1 arg)
"f6 /4 rm"
dis all {
	write("mul");
	space();
	write_arg(0, true, true);
}
emu {
	BIT tmp = _umul_overflow(AL, arg);
	AX = cmul(zx<B2>(AL), zx<B2>(arg));
	SF = undefined;
	ZF = undefined;
	AF = undefined;
	PF = undefined;
	CF = tmp;
	OF = tmp;
}

insn mul(P arg)
"f7 /4 rm"							! P=osz
emu {
	o16: BIT tmp = _umul_overflow(AX, arg);
	o16: DX = umulhi(AX, tr<B2>(arg));
	o16: AX = cmul(AX, tr<B2>(arg));

	o32: BIT tmp = _umul_overflow(EAX, tr<B4>(arg));
	o32: EDX = umulhi(EAX, tr<B4>(arg));
	o32: EAX = cmul(EAX, tr<B4>(arg));

	o64: BIT tmp = _umul_overflow(RAX, tr<B8>(arg));
	o64: RDX = umulhi(RAX, tr<B8>(arg));
	o64: RAX = cmul(RAX, tr<B8>(arg));

	SF = undefined;
	ZF = undefined;
	AF = undefined;
	PF = undefined;
	CF = tmp;
	OF = tmp;
}

insn _divb(B1 arg)
"f6 /6 rm"
dis all {
	write("div");
	space();
	write_arg(0, true, true);
}
emu {
	assert(_lognot(_zero(arg)), tr<B1>(0));
	B2 q = udiv(AX, zx<B2>(arg));
	B2 r = umod(AX, zx<B2>(arg));
	B1 s = tr<B1>(q);
	assert(_eq(zx<B2>(s), q), tr<B1>(0));		// make sure it fits
	AL = s;
	AH = tr<B1>(r);
	OF = undefined;
	SF = undefined;
	ZF = undefined;
	AF = undefined;
	PF = undefined;
	CF = undefined;
}

// FIXME, use double() here!
insn div(P arg)
"f7 /6 rm"							! P=osz
emu {
	assert(_lognot(_zero(arg)), tr<B1>(0));

	o16: B4 p = bitor(shl_(zx<B4>(DX), tr<B1>(16)), zx<B4>(AX));
	o16: B4 q = udiv(p, zx<B4>(arg));
	o16: B4 r = umod(p, zx<B4>(arg));
	o16: B2 s = tr<B2>(q);
	o16: assert(_eq(zx<B4>(s), q), tr<B1>(0));		// make sure it fits
	o16: AX = s;
	o16: DX = tr<B2>(r);

	o32: B8 p = bitor(shl_(zx<B8>(EDX), tr<B1>(32)), zx<B8>(EAX));
	o32: B8 q = udiv(p, zx<B8>(arg));
	o32: B8 r = umod(p, zx<B8>(arg));
	o32: B4 s = tr<B4>(q);
	o32: assert(_eq(zx<B8>(s), q), tr<B1>(0));		// make sure it fits
	o32: EAX = s;
	o32: EDX = tr<B4>(r);

	/*o64: B16 p = bitor(shl_(zx<B16>(RDX), tr<B1>(64)), zx<B16>(RAX));
	o64: B16 q = udiv(p, zx<B16>(arg));
	o64: B16 r = umod(p, zx<B16>(arg));
	o64: B8 s = tr<B8>(q);
	o64: assert(_eq(zx<B16>(s), q), tr<B1>(0));		// make sure it fits
	o64: RAX = s;
	o64: RDX = tr<B8>(r);*/
	
	// If there is a divide overflow, it will (at present) crash the emulator,
	// since we really execute assembly code to divide. FIXME!
	o64: B8 q = umod64(RDX, RAX, tr<B8>(arg));
	o64: B8 r = udiv64(RDX, RAX, tr<B8>(arg));
	o64: RDX = q;
	o64: RAX = r;

	OF = undefined;
	SF = undefined;
	ZF = undefined;
	AF = undefined;
	PF = undefined;
	CF = undefined;
}

insn _idivb(B1 arg)
"f6 /7 rm"
dis all {
	write("idiv");
	space();
	write_arg(0, true, true);
}
emu {
	assert(_lognot(_zero(arg)), tr<B1>(0));
	B2 q = sdiv(AX, sx<B2>(arg));
	B2 r = smod(AX, sx<B2>(arg));
	B1 s = tr<B1>(q);
	assert(_eq(sx<B2>(s), q), tr<B1>(0));		// make sure it fits
	AL = s;
	AH = tr<B1>(r);
	OF = undefined;
	SF = undefined;
	ZF = undefined;
	AF = undefined;
	PF = undefined;
	CF = undefined;
}

// FIXME: use double() here.
insn idiv(P arg)
"f7 /7 rm"							! P=osz
emu {
	assert(_lognot(_zero(arg)), tr<B1>(0));

	o16: B4 p = bitor(shl_(zx<B4>(DX), tr<B1>(16)), zx<B4>(AX));
	o16: B4 q = sdiv(p, sx<B4>(arg));
	o16: B4 r = smod(p, sx<B4>(arg));
	o16: B2 s = tr<B2>(q);
	o16: assert(_eq(sx<B4>(s), q), tr<B1>(0));		// make sure it fits
	o16: AX = s;
	o16: DX = tr<B2>(r);

	o32: B8 p = bitor(shl_(zx<B8>(EDX), tr<B1>(32)), zx<B8>(EAX));
	o32: B8 q = sdiv(p, sx<B8>(arg));
	o32: B8 r = smod(p, sx<B8>(arg));
	o32: B4 s = tr<B4>(q);
	o32: assert(_eq(sx<B8>(s), q), tr<B1>(0));		// make sure it fits
	o32: EAX = s;
	o32: EDX = tr<B4>(r);

	/*
	o64: B16 p = bitor(shl_(zx<B16>(RDX), tr<B1>(64)), zx<B16>(RAX));
	o64: B16 q = sdiv(p, sx<B16>(arg));
	o64: B16 r = smod(p, sx<B16>(arg));
	o64: B8 s = tr<B8>(q);
	o64: assert(_eq(sx<B16>(s), q), tr<B1>(0));		// make sure it fits
	o64: RAX = s;
	o64: RDX = tr<B8>(r);
	*/

	// If there is a divide overflow, it will (at present) crash the emulator,
	// since we really execute assembly code to divide. FIXME!
	o64: B8 q = smod64(RDX, RAX, tr<B8>(arg));
	o64: B8 r = sdiv64(RDX, RAX, tr<B8>(arg));
	o64: RDX = q;
	o64: RAX = r;

	OF = undefined;
	SF = undefined;
	ZF = undefined;
	AF = undefined;
	PF = undefined;
	CF = undefined;
}

// --- begin control transfer instructions ---

insn ctrlxfer _uint1()
"f1 void"
dis all {
	write("int1");
}

// For jCC, if not in 64bit mode, then we need to print e.g. "o32" if we're in 16bit mode but db 66 was used.
// Right ? Well maybe we can do e.g. "jo dword Label1" if o32 but in 16bit mode.

// {"o","no","c","nc","z","nz","be","a","s","ns","p","np","l","ge","le","g"}

//---

insn ctrlxfer "jo" _jo(P dest)
"70 imm"					! relative, has_cc, P=osz_64in64, sx_byte,imm64_sx32
"0f 80 imm"					! relative, has_cc, P=osz_64in64, imm64_sx32
emu {
	BIT cond = OF;
	RIPADD = quest(cond, zx<B8>(dest), tr<B8>(0));
}

insn ctrlxfer "jno" _jno(P dest)
"71 imm"					! relative, has_cc, P=osz_64in64, sx_byte,imm64_sx32
"0f 81 imm"					! relative, has_cc, P=osz_64in64, imm64_sx32
emu {
	BIT cond = _lognot(OF);
	RIPADD = quest(cond, zx<B8>(dest), tr<B8>(0));
}

insn ctrlxfer "jc" _jc(P dest)
"72 imm"					! relative, has_cc, P=osz_64in64, sx_byte,imm64_sx32
"0f 82 imm"					! relative, has_cc, P=osz_64in64, imm64_sx32
emu {
	BIT cond = CF;
	RIPADD = quest(cond, zx<B8>(dest), tr<B8>(0));
}

insn ctrlxfer "jnc" _jnc(P dest)
"73 imm"					! relative, has_cc, P=osz_64in64, sx_byte,imm64_sx32
"0f 83 imm"					! relative, has_cc, P=osz_64in64, imm64_sx32
emu {
	BIT cond = _lognot(CF);
	RIPADD = quest(cond, zx<B8>(dest), tr<B8>(0));
}

insn ctrlxfer "jz" _jz(P dest)
"74 imm"					! relative, has_cc, P=osz_64in64, sx_byte,imm64_sx32
"0f 84 imm"					! relative, has_cc, P=osz_64in64, imm64_sx32
emu {
	BIT cond = ZF;
	RIPADD = quest(cond, zx<B8>(dest), tr<B8>(0));
}

insn ctrlxfer "jnz" _jnz(P dest)
"75 imm"					! relative, has_cc, P=osz_64in64, sx_byte,imm64_sx32
"0f 85 imm"					! relative, has_cc, P=osz_64in64, imm64_sx32
emu {
	BIT cond = _lognot(ZF);
	RIPADD = quest(cond, zx<B8>(dest), tr<B8>(0));
}

insn ctrlxfer "jbe" _jbe(P dest)
"76 imm"					! relative, has_cc, P=osz_64in64, sx_byte,imm64_sx32
"0f 86 imm"					! relative, has_cc, P=osz_64in64, imm64_sx32
emu {
	BIT cond = _logor(CF, ZF);
	RIPADD = quest(cond, zx<B8>(dest), tr<B8>(0));
}

insn ctrlxfer "ja" _ja(P dest)
"77 imm"					! relative, has_cc, P=osz_64in64, sx_byte,imm64_sx32
"0f 87 imm"					! relative, has_cc, P=osz_64in64, imm64_sx32
emu {
	BIT cond = _lognot(_logor(CF, ZF));
	RIPADD = quest(cond, zx<B8>(dest), tr<B8>(0));
}

insn ctrlxfer "js" _js(P dest)
"78 imm"					! relative, has_cc, P=osz_64in64, sx_byte,imm64_sx32
"0f 88 imm"					! relative, has_cc, P=osz_64in64, imm64_sx32
emu {
	BIT cond = SF;
	RIPADD = quest(cond, zx<B8>(dest), tr<B8>(0));
}

insn ctrlxfer "jns" _jns(P dest)
"79 imm"					! relative, has_cc, P=osz_64in64, sx_byte,imm64_sx32
"0f 89 imm"					! relative, has_cc, P=osz_64in64, imm64_sx32
emu {
	BIT cond = _lognot(SF);
	RIPADD = quest(cond, zx<B8>(dest), tr<B8>(0));
}

insn ctrlxfer "jp" _jp(P dest)
"7a imm"					! relative, has_cc, P=osz_64in64, sx_byte,imm64_sx32
"0f 8a imm"					! relative, has_cc, P=osz_64in64, imm64_sx32
emu {
	BIT cond = PF;
	RIPADD = quest(cond, zx<B8>(dest), tr<B8>(0));
}

insn ctrlxfer "jnp" _jnp(P dest)
"7b imm"					! relative, has_cc, P=osz_64in64, sx_byte,imm64_sx32
"0f 8b imm"					! relative, has_cc, P=osz_64in64, imm64_sx32
emu {
	BIT cond = _lognot(PF);
	RIPADD = quest(cond, zx<B8>(dest), tr<B8>(0));
}

insn ctrlxfer "jl" _jl(P dest)
"7c imm"					! relative, has_cc, P=osz_64in64, sx_byte,imm64_sx32
"0f 8c imm"					! relative, has_cc, P=osz_64in64, imm64_sx32
emu {
	BIT cond = _logxor(SF, OF);
	RIPADD = quest(cond, zx<B8>(dest), tr<B8>(0));
}

insn ctrlxfer "jge" _jge(P dest)
"7d imm"					! relative, has_cc, P=osz_64in64, sx_byte,imm64_sx32
"0f 8d imm"					! relative, has_cc, P=osz_64in64, imm64_sx32
emu {
	BIT cond = _lognot(_logxor(SF, OF));
	RIPADD = quest(cond, zx<B8>(dest), tr<B8>(0));
}

insn ctrlxfer "jle" _jle(P dest)
"7e imm"					! relative, has_cc, P=osz_64in64, sx_byte,imm64_sx32
"0f 8e imm"					! relative, has_cc, P=osz_64in64, imm64_sx32
emu {
	BIT cond = _logor(ZF, _logxor(SF, OF));
	RIPADD = quest(cond, zx<B8>(dest), tr<B8>(0));
}

insn ctrlxfer "jg" _jg(P dest)
"7f imm"					! relative, has_cc, P=osz_64in64, sx_byte,imm64_sx32
"0f 8f imm"					! relative, has_cc, P=osz_64in64, imm64_sx32
emu {
	BIT cond = _lognot(_logor(ZF, _logxor(SF, OF)));
	RIPADD = quest(cond, zx<B8>(dest), tr<B8>(0));
}

//---

insn ctrlxfer call(P dest)
"e8 imm"							! relative, P=osz_64in64, imm64_sx32

insn ctrlxfer _calli(P dest)
"ff /2 rm"							! P=osz_64in64
dis all {
	write("call");
	space();
	write_args();
}

insn ctrlxfer _callfd(P xofs, B2 xseg)
"9a imm,imm"						! P=osz, no64
dis all {
	write("call");
	space();
	write_far_imm();
}

insn ctrlxfer _callfi(P dest)
"ff /3 /mdm mem_ea_seg"					! P=osz_64in64
dis all {
	write("call");
	space();
	write_args();
}

insn ctrlxfer jmp(P dest)
"e9 imm"							! relative, P=osz_64in64, imm64_sx32
"eb imm"							! relative, P=osz_64in64, sx_byte, imm64_sx32
emu {
	RIPADD = zx<B8>(dest);
}

insn ctrlxfer _jmpi(P dest)
"ff /4 rm"							! P=osz_64in64
dis all {
	write("jmp");
	space();
	write_args();
}

insn ctrlxfer _jmpfd(P xofs, B2 xseg)
"ea imm,imm"						! P=osz, no64
dis all {
	write("jmp");
	space();
	write_far_imm();
}

insn ctrlxfer _jmpfi(P dest)
"ff /5 /mdm mem_ea_seg"					! P=osz_64in64
dis all {
	write("jmp");
	space();
	write_args();
}

insn ctrlxfer _retnum(B2 num)
"c2 imm"
dis all {
	write("ret");
	space();
	write_args();
}

insn ctrlxfer _ret(B2 bogus)
"c3 imm_implict:0"
dis all {
	write("ret");
}

insn ctrlxfer _retfnum(B2 num)
"ca imm"
dis all {
	write("retf");
	space();
	write_args();
}

insn ctrlxfer _retf(B2 bogus)
"cb imm_implict:0"
dis all {
	write("retf");
}

insn ctrlxfer _int3(B1 magic)
"cc imm_implict:3"
dis all {
	write("int3");
}

insn ctrlxfer int(B1 vector)
"cd imm"


insn ctrlxfer _iret()
"cf void"
dis all {
	write("iret");
	write_size_suffix_osz();
}

insn ctrlxfer _loopnz(P dest, Q counter)
"e0 imm,r:1"						! P=osz_64in64, imm64_sx32, sx_byte, Q=asz, relative
dis all {
	write("loopnz");
	space();
	write_arg(0, true, false, true);
}

insn ctrlxfer _loopz(P dest, Q counter)
"e1 imm,r:1"						! P=osz_64in64, imm64_sx32, sx_byte, Q=asz, relative
dis all {
	write("loopz");
	space();
	write_arg(0, true, false, true);
}

insn ctrlxfer _loop(P dest, Q counter)
"e2 imm,r:1"						! P=osz_64in64, imm64_sx32, sx_byte, Q=asz, relative
dis all {
	write("loop");		// bugfix--was "loopnz"
	space();
	write_arg(0, true, false, true);
}
emu {
	a16: CX = sub(CX, tr<B2>(1));
	a16: BIT cond = _lognot(_zero(CX));
	a32: ECX = sub(ECX, tr<B4>(1));
	a32: BIT cond = _lognot(_zero(ECX));
	a64: RCX = sub(RCX, tr<B8>(1));
	a64: BIT cond = _lognot(_zero(RCX));
	RIPADD = quest(cond, zx<B8>(dest), tr<B8>(0));
}

insn ctrlxfer _jrcxz(P dest, Q counter)
"e3 imm,r:1"						! P=osz_64in64, imm64_sx32, sx_byte, Q=asz, relative
dis all {
	write_jrcxz_o16_o32_o64();
	write_asz("jcxz", "jecxz", "jrcxz");
	space();
	write_arg(0, true, false, true);
}

// --- end control transfer instructions ---

// begin new insns

// Is this the right size for ST(i) ?
#define STSIZE B10

// asm: fxch by itself is fxch st1 alias.
insn _fxch(STSIZE arg)
"d9 /1 /md3 reg_st" ! reg_rm
dis all {
	write("fxch");
	space();
	write_args();
}

insn fst(P arg)
"d9 /2 /mdm mem" ! P=B4
"dd /2 /mdm mem" ! P=B8
"dd /2 /md3 reg_st" ! reg_rm, P=STSIZE

insn fstp(P arg)
"d9 /3 /mdm mem" ! P=B4
"dd /3 /mdm mem" ! P=B8
"db /7 /mdm mem" ! P=B10
"dd /3 /md3 reg_st" ! P=STSIZE, reg_rm

// Note: fstcw -> wait; fnstcw.
// If insn is fwaitable, write "fn" by default; write "f" if preceded by fwait.
insn fwaitable "stcw" _fstcw(B2 arg)
"d9 /7 /mdm mem"

insn fldcw(B2 arg)
"d9 /5 /mdm mem"

insn fld(P arg)
"d9 /0 /mdm mem" ! P=B4
"dd /0 /mdm mem" ! P=B8
"db /5 /mdm mem" ! P=B10
"d9 /0 /md3 reg_st" ! reg_rm, P=STSIZE

insn "fmul" _fmul1(P arg)
"d8 /1 /mdm mem" ! P=B4
"dc /1 /mdm mem" ! P=B8

insn "fmul" _fmul2(STSIZE arg1, STSIZE arg2)
"d8 /1 /md3 reg_st:0,reg_st" ! reg_rm
"dc /1 /md3 reg_st,reg_st:0" ! reg_rm

// asm: fmulp by itself is same as fmulp st1,st0
insn "fmulp" _fmulp(STSIZE arg1, STSIZE arg2)
"de /1 /md3 reg_st,reg_st:0" ! reg_rm

insn fimul(P arg)
"da /1 /mdm mem" ! P=B4		// src is an integer
"de /1 /mdm mem" ! P=B2		// src is an integer

// end new insns

// NOTE--_wait should be last !
// This is a '_' insn because the assembler needs to match fwait and wait.
// Note: 9b is handled by the decoder, specially. It combines with 'fwaitable'
// instructions.
insn "wait" _wait()
"9b void"
