// notes.c
// Copyright (C) 2009 Willow Schlanger
// REVISION 2.

/*
commit   -- arg is BYTES
discard  -- arg is BYTES
restore  -- arg is BYTES
reserve  -- arg is BYTES
*/

// TAKE NOTE!
// Arguments are not comitted until AFTER an instruction successfully is
// executed by the emulator. Thus, consider this instruction:
// insn myexample(B4 x)
// {
//     x = 1;
//     EBX = EAX;
// }
// If the user executes this code:
//     myexample EAX
// then the first line really stores '1' in a temporary variable, so the
// above code functions AS IF it were written like so:
// insn myexample(B4 x)
// {
//     B4 tmp = 1;
//     EBX = EAX;
//     EAX = tmp;	// commit writes at the end of successful execution
// }
// That is, myexample(EAX) will set EBX to IN.EAX and not 1!

// need to do:
// - pusha, popa
// - int3
// - salc
// - wait
// - hlt (not used?)
// - ins, outs (not used?)
// - 64bit support not finished yet (i.e. need o64, a64 where applicable).
// - is ENTER right???

// old notes (obsolete).
// - literals have a size of 8 bytes, but are automatically concatinated
//   to a smaller size when that size can be inferred. For instance, if
//   we define cmul as P cmul(P x, P y) then do cmul(2, 3) the result will
//   have a size of B8. This should generate a warning, too. To suppress
//   the warning, use tr<> on both arguments. Use tr<B8> for both arguments
//   if the intended size really is 8 bytes.
// - Assignments always truncate literals; other than that, the right hand
//   side of the assignment must have a size matching the left-hand-side.
// - valid sizes: bit (same as B1 but value is exactly 0 or exactly 1).
//                B# [*##] (#>=1,##>=0)
//                <other identifier> [*##] (##>=0)
//                  Here, the other identifier must have been specified as
//                  an "args".
// - Note that args may define a new size (type) but that type must be a
//   positive number of sequential bytes in size. That is, the size may be
//   explictly given as "bit", or it may be implictly given as e.g. "P". If
//   the latter is chosen, the size must be B1, B2, B4, B8, B10, etc. It
//   cannot be overloaded as a bit.
// - Note that sizeof(bit) is not allowed. Nor can you do bit * 3 etc.
// - "undefined" is a reserved word, too.

/*
Pass 1 modifies the parse tree.
For example, tr<B2>(5) -> 5[size=2].
tr<B8>(5) = 5[size=8].
Untruncated number literals need a size.
<dest> = 5;
becomes (if <dest> has a size of B4)
<dest> = 5[size=4].
Note that a size of 0 means 'bit' (really 1 byte).
Note that floating-point literals are allowed, they simply have to be specified as
e.g. tr<B4>(0xcff12345) which might mean 3.14 in floating-point symbols.
"NUM" (numeric literal) nodes need a size.
Note that a size of -1 means the first argument's size, -2 the 2nd argument's size, etc.
These "templatized" sizes cannot be of "bit" type.

Parse Tree.
- Literal. This has an associated string, which is always in hexadecimal.
  Has a Size. May be: 0(bit),>0 (sequence of bytes).
  Note that if a literal is directly assigned somewhere, the destination has a size and the
  literal gets its size from there.
  i.e. X = 5 means X = 5[size=B2] if X.size = B2.
- Argsize.
  This has a size too. e.g. tr<B2>(sizeof(P)).
  By default, a literal has a size of -65536. This special size means "unknown".
- So, #s become Literal's; sizeof(...) becomes a literal if ... can be resolved,
  else it becomes an Argsize.
- tr<...>(Literal/Argsize) -> Literal/Argsize[size=...]
- You can't truncate to a bit. tr<bit>(x) can be effected with nz(and(x, 1)).
  In fact sx, zx, tr refuse bit destination sizes.
  Aside from this, if the dest size matches the src size the operation has no effect.
  sizeof(...) reports the size of ... in bytes but never reports 0 (i.e. a bit size).
- Identifier.
  This may be:
  a. an external identifier.
  b. an argument to an insn.
  c. a temporary created within an insn.
  In any case the identifier always has a size.
- Function
  This must be defined before use.
  It has a size.
  All arguments shall have a size at the time of evaluation. Sizes must match the
  signature constraints, for example, if one was add(AL, AX) that is in error
  because add is defined with arguments add(P x, P y) so both arguments must have
  the same size.
  The size of a function can be inferred from its arguments (and is sometimes fixed).
  A function also has a list of arguments.

The first pass is a "collect and check" pass. We collect symbollic information and
check the tree for accuracy, while annoting it and applying surgery to it.
*/

P cmul(P x, P y);	// c language product
P add(P x, P y);	// sum
bit sign(P src);	// most significant bit ?
bit zero(P src);	// is zero ?
bit parity(P src);	// only looks at low 8 bits
bit nz(P src);		// nonzero
bit ltu(P x, P y);	// unsigned<
bit leu(P x, P y);	// unsigned<=
bit geu(P x, P y);	// unsigned>=
bit gtu(P x, P y);	// unsigned>
bit lts(P x, P y);	// signed<
bit les(P x, P y);	// signed<=
bit ges(P x, P y);	// signed>=
bit gts(P x, P y);	// signed>
bit eq(P x, P y);	// ==
bit ne(P x, P y);	// !=
P quest(bit cond, P value1, P value2);
bit bitquest(bit cond, bit value1, bit value2);
P and(P x, P y);
P or(P x, P y);
P not(P src);

bit adc_af(P x, P y, bit in_cf);
bit adc_cf(P x, P y, bit in_cf);
bit adc_of(P x, P y, bit in_cf);
bit sbb_af(P x, P y, bit in_cf);
bit sbb_cf(P x, P y, bit in_cf);
bit sbb_of(P x, P y, bit in_cf);
bit add_af(P x, P y);
bit add_cf(P x, P y);
bit add_of(P x, P y);
bit sub_af(P x, P y);
bit sub_cf(P x, P y);
bit sub_of(P x, P y);
bit rcl_cf(P old_dest, B1 tmp_count, bit in_cf);
bit rcr_cf(P old_dest, B1 tmp_count, bit in_cf);
bit land(bit x, bit y);
bit lor(bit x, bit y);
bit lnot(bit src);
P neg(P src);
P rcl(P dest, B1 tmp_count, bit in_cf);
P rcr(P dest, B1 tmp_count, bit in_cf);
P rol(P dest, B1 tmp_count);
P ror(P dest, B1 tmp_count);
P sdiv(P x, P y);
P smod(P x, P y);
P udiv(P x, P y);
P umod(P x, P y);
P sub(P x, P y);
P xor(P x, P y);
P shl(P x, B1 c);
P shrs(P x, B1 c);
P shru(P x, B1 c);
bit smul_overflow(P src1, P src2);
bit umul_overflow(P src1, P src2);
bit shl_cf(P src, B1 count);
bit shr_cf(P src, B1 count);
bit sar_cf(P src, B1 count);

bit lsb(P src);	// least significant bit
bit lxor(bit x, bit y);
bit sign2(P src);	// 2nd most significant bit

// Note: for these two functions to work, 'src' must be a memory argument
// whose given size is the offset size.
B2 seg(P src);
P ofs(P src);

// This is a special symbol.
// It is given a parameter to an insn which is a hidden immediate argument.
// Depending on the value of that argument, xcond can become e.g. something
// like this: leu(CF, ZF) -- less than or equal unsigned.
bit xcond(B1 cc);

// Zero extends a bit to a byte. Once it has a size of B1 we can zx<> further
// in the usual way.
B1 bitzx(bit x);

extern B8 RIP;		// current RIP
extern B8 RIP1;		// (read) - current RIP plus insn length
extern B8 RIPC;		// (write) - value to add to RIP after insn executes,
					// 0 means only add current insn length. operand size
					// for the current insn determines how to mask RIP
					// after this addition is performed.
extern B1 RINT;		// if set, we generate the interrupt given (modify CS, RIP).
extern B8 RFLAGS;	// special symbol. read: has size of B8. write: only low osz bits used.

// covert aliasing?
// e.g. AH = 0; AL = 1; BX = AX; ???
// For emulation purposes we can simply follow the steps.
// For reverse engineering purposes ???

extern bit OF, SF, ZF, AF, PF, CF, DF, IF;
extern B1 AL, CL, DL, BL, AH, CH, DH, BH;
extern B2 AX, CX, DX, BX, SP, BP, SI, DI;
extern B4 EAX, ECX, EDX, EBX, ESP, EBP, ESI, EDI;
extern B8 RAX, RCX, RDX, RBX, RSP, RBP, RSI, RDI;	// add additional 64bit register names as needed
extern B2 ES, CS, SS, DS, FS, GS;

// These metasymbols have a size of B8 semantically.
// You can PUSH or POP these symbols, or assign to them.
// You should not try to read from them (result would have a size of B8).
// When push'd, pop'd, or assigned to, the true size depends on the stack
// address size. (CHECK THIS! Does ENTER really work that way?)
extern B8 RBPX, RSPX;

insn _aad(B1 imm)
{
	B1 tmp = AH;
	tmp = cmul(tmp, imm);
	tmp = add(tmp, AL);
	AL = tmp;
	AH = 0;
	OF = undefined;
	SF = sign(AL);
	ZF = zero(AL);
	AF = undefined;
	PF = parity(AL);
	CF = undefined;
}

insn _aam(B1 imm)
{
	assert(nz(imm));		// else division by zero exception
	B1 tmp = AL;
	AH = udiv(tmp, imm);
	AL = umod(tmp, imm);
	OF = undefined;
	SF = sign(AL);
	ZF = zero(AL);
	AF = undefined;
	PF = parity(AL);
	CF = undefined;
}

insn das()
{
	B1 tmpAL = AL;
	
	AF = lor(AF, gtu(and(tmpAL, 0x0f), 0x09));
	B1 count = quest(AF, tr<B1>(0x06), tr<B1>(0));
	bit tmpCF = bitquest(AF, lor(CF, ltu(AL, 0x06)), 0);
	AL = sub(AL, count);
	
	bit cond = lor(CF, gtu(tmpAL, 0x99));
	B1 fixup = quest(cond, tr<B1>(0x60), tr<B1>(0));
	AL = sub(AL, fixup);
	tmpCF = lor(tmpCF, cond);
	
	OF = 0;						// undocumented ?
	SF = sign(AL);
	ZF = zero(AL);
	PF = parity(AL);
	CF = tmpCF;
}

insn daa()
{
	B1 tmpAL = AL;

	AF = lor(AF, gtu(and(tmpAL, 0x0f), 0x09));
	B1 count = quest(AF, tr<B1>(0x06), tr<B1>(0));
	bit tmpCF = bitquest(AF, lor(CF, gtu(AL, 0xf9)), 0);
	AL = add(AL, count);
	
	bit cond = lor(CF, gtu(tmpAL, 0x99));
	B1 fixup = quest(cond, tr<B1>(0x60), tr<B1>(0));
	AL = add(AL, fixup);
	tmpCF = cond;
	
	OF = 0;						// undocumented ?
	SF = sign(AL);
	ZF = zero(AL);
	PF = parity(AL);
	CF = tmpCF;
}

insn aaa()
{
	AF = lor(AF, gtu(and(AL, 0x0f), 0x09));
	CF = AF;
	AX = add(AX, quest(AF, tr<B2>(0x0106), tr<B2>(0)));
	AL = and(AL, 0x0f);
	OF = 0;
	SF = 0;
	ZF = zero(AL);
	PF = parity(AL);
}

insn aas()
{
	AF = lor(AF, gtu(and(AL, 0x0f), 0x09));
	CF = AF;
	AX = sub(AX, quest(AF, tr<B2>(0x0106), tr<B2>(0)));
	AL = and(AL, 0x0f);
	OF = 0;
	SF = 0;
	ZF = zero(AL);
	PF = parity(AL);
}

insn _sxacc()
{
	o16: AX = sx<B2>(AL);
	o32: EAX = sx<B4>(AX);
}

insn _sxdax()
{
	o16: DX = quest(sign(AX), tr<B2>(0xffff), tr<B2>(0));
	o32: EDX = quest(sign(EAX), tr<B4>(0xffffffff), tr<B4>(0));
}

insn sahf()
{
	SF = nz(and(shru(AH, 7), 1));
	ZF = nz(and(shru(AH, 6), 1));
	AF = nz(and(shru(AH, 4), 1));
	PF = nz(and(shru(AH, 2), 1));
	CF = nz(and(AH, 1));
}

insn lahf()
{
	// bits 5 and 3 are zero; bit 1 is one.
	AH = 2;
	AH = or(AH, quest(CF, 1, tr<B1>(0)));
	AH = or(AH, quest(PF, 4, tr<B1>(0)));
	AH = or(AH, quest(AF, 16, tr<B1>(0)));
	AH = or(AH, quest(ZF, 64, tr<B1>(0)));
	AH = or(AH, quest(SF, 128, tr<B1>(0)));
}

insn cmc()
{
	CF = lnot(CF);
}

insn clc()
{
	CF = 0;
}

insn stc()
{
	CF = 1;
}

insn cld()
{
	DF = 0;
}

insn std()
{
	DF = 1;
}

insn lea(P dest, Q src)
{
	// Note: zx<> and sx<> will truncate if the destination
	// size is smaller than the source size.
	dest = zx<P>(src);				// ea itself copied
}

// --- begin imul ---

insn _cmul3(P dest, P src1, P src2)
{
	bit tmp = smul_overflow(src1, src2);
	dest = cmul(src1, src2);
	SF = undefined;
	ZF = undefined;
	AF = undefined;
	PF = undefined;
	CF = tmp;
	OF = tmp;
}

insn _cmul2(P dest, P src)
{
	bit tmp = smul_overflow(dest, src);
	dest = cmul(dest, src);
	SF = undefined;
	ZF = undefined;
	AF = undefined;
	PF = undefined;
	CF = tmp;
	OF = tmp;
}

insn _imulb(B1 arg)
{
	bit tmp = smul_overflow(AL, arg);
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
{
	bit tmp;
	P*2 result;

	o16: tmp = smul_overflow(AX, zx<B2>(arg));
	o16: result = cmul(sx<P*2>(AX), sx<P*2>(arg));
	o16: AX = tr<B2>(result);
	o16: DX = tr<B2>(shru(result, 16));

	o32: tmp = smul_overflow(EAX, zx<B4>(arg));
	o32: result = cmul(sx<P*2>(EAX), sx<P*2>(arg));
	o32: EAX = tr<B4>(result);
	o32: EDX = tr<B4>(shru(result, 32));

	SF = undefined;
	ZF = undefined;
	AF = undefined;
	PF = undefined;
	CF = tmp;
	OF = tmp;
}

// --- end imul ---

insn _mulb(B1 arg)
{
	bit tmp = umul_overflow(AL, arg);
	AX = cmul(zx<B2>(AL), zx<B2>(arg));
	SF = undefined;
	ZF = undefined;
	AF = undefined;
	PF = undefined;
	CF = tmp;
	OF = tmp;
}

insn mul(P arg)
{
	P*2 result;
	bit tmp;
	o16: result = cmul(zx<P*2>(AX), zx<P*2>(arg));
	o16: tmp = umul_overflow(AX, zx<B2>(arg));
	o16: AX = tr<B2>(result);
	o16: DX = tr<B2>(shru(result, 16));
	
	o32: result = cmul(zx<P*2>(EAX), zx<P*2>(arg));
	o32: tmp = umul_overflow(EAX, zx<B4>(arg));
	o32: EAX = tr<B4>(result);
	o32: EDX = tr<B4>(shru(result, 32));
	
	SF = undefined;
	ZF = undefined;
	AF = undefined;
	PF = undefined;
	CF = tmp;
	OF = tmp;
}

insn _divb(B1 arg)
{
	assert(nz(arg));
	B2 q = udiv(AX, zx<B2>(arg));
	B2 r = umod(AX, zx<B2>(arg));
	B1 s = tr<B1>(q);
	assert(eq(zx<B2>(s), q));		// make sure it fits
	AL = s;
	AH = tr<B1>(r);
	OF = undefined;
	SF = undefined;
	ZF = undefined;
	AF = undefined;
	PF = undefined;
	CF = undefined;
}

insn div(P arg)
{
	assert(nz(arg));
	
	P*2 p;
	P*2 q;
	P*2 r;
	P s;

	o16: p = or(shl(zx<P*2>(DX), 16), zx<P*2>(AX));
	o16: q = udiv(p, zx<P*2>(arg));
	o16: r = umod(p, zx<P*2>(arg));
	o16: s = tr<P>(q);
	o16: assert(eq(zx<P*2>(s), q));		// make sure it fits
	o16: AX = tr<B2>(s);		// this truncate has no effect but is required
	o16: DX = tr<B2>(r);

	o32: p = or(shl(zx<P*2>(EDX), 32), zx<P*2>(EAX));
	o32: q = udiv(p, zx<P*2>(arg));
	o32: r = umod(p, zx<P*2>(arg));
	o32: s = tr<P>(q);
	o32: assert(eq(zx<P*2>(s), q));		// make sure it fits
	o32: EAX = tr<B4>(s);
	o32: EDX = tr<B4>(r);

	OF = undefined;
	SF = undefined;
	ZF = undefined;
	AF = undefined;
	PF = undefined;
	CF = undefined;
}

insn _idivb(B1 arg)
{
	assert(nz(arg));
	B2 q = sdiv(AX, sx<B2>(arg));
	B2 r = smod(AX, sx<B2>(arg));
	B1 s = tr<B1>(q);
	assert(eq(sx<B2>(s), q));		// make sure it fits
	AL = s;
	AH = tr<B1>(r);
	OF = undefined;
	SF = undefined;
	ZF = undefined;
	AF = undefined;
	PF = undefined;
	CF = undefined;
}

insn idiv(P arg)
{
	assert(nz(arg));
	
	P*2 p;
	P*2 q;
	P*2 r;
	P s;

	o16: p = or(shl(zx<P*2>(DX), 16), zx<P*2>(AX));
	o16: q = sdiv(p, sx<P*2>(arg));
	o16: r = smod(p, sx<P*2>(arg));
	o16: s = tr<P>(q);
	o16: assert(eq(sx<P*2>(s), q));		// make sure it fits
	o16: AX = tr<B2>(s);
	o16: DX = tr<B2>(r);

	o32: p = or(shl(zx<P*2>(EDX), 32), zx<P*2>(EAX));
	o32: q = sdiv(p, sx<P*2>(arg));
	o32: r = smod(p, sx<P*2>(arg));
	o32: s = tr<P>(q);
	o32: assert(eq(sx<P*2>(s), q));		// make sure it fits
	o32: EAX = tr<B4>(s);
	o32: EDX = tr<B4>(r);

	OF = undefined;
	SF = undefined;
	ZF = undefined;
	AF = undefined;
	PF = undefined;
	CF = undefined;
}

insn test(P src1, P src2)
{
	OF = 0;
	AF = undefined;
	CF = 0;
	P result = and(src1, src2);
	SF = sign(result);
	ZF = zero(result);
	PF = parity(result);
}

insn _xchg(P x, P y)
{
	P z = x;
	x = y;
	y = z;
}

insn mov(P dest, P src)
{
	dest = src;
}

insn not(P x)
{
	x = not(x);
}

insn neg(P x)
{
	OF = sub_of(tr<P>(0), x);
	AF = sub_af(tr<P>(0), x);
	CF = sub_cf(tr<P>(0), x);
	x = neg(x);
	SF = sign(x);
	ZF = zero(x);
	PF = parity(x);
}

// arithmatic.

insn add(P dest, P src)
{
	OF = add_of(dest, src);
	AF = add_af(dest, src);
	CF = add_cf(dest, src);
	dest = add(dest, src);
	SF = sign(dest);
	ZF = zero(dest);
	PF = parity(dest);
}

insn or(P dest, P src)
{
	OF = 0;
	AF = undefined;
	CF = 0;
	dest = or(dest, src);
	SF = sign(dest);
	ZF = zero(dest);
	PF = parity(dest);
}

insn adc(P dest, P src)
{
	bit tmp = CF;
	OF = adc_of(dest, src, tmp);
	AF = adc_af(dest, src, tmp);
	CF = adc_cf(dest, src, tmp);
	
	P dest1 = add(dest, src);
	dest = quest(tmp, add(dest1, 1), dest1);
	
	SF = sign(dest);
	ZF = zero(dest);
	PF = parity(dest);
}

insn sbb(P dest, P src)
{
	bit tmp = CF;
	OF = sbb_of(dest, src, tmp);
	AF = sbb_af(dest, src, tmp);
	CF = sbb_cf(dest, src, tmp);

	P dest1 = sub(dest, src);
	dest = quest(tmp, sub(dest1, 1), dest1);

	SF = sign(dest);
	ZF = zero(dest);
	PF = parity(dest);
}

insn and(P dest, P src)
{
	OF = 0;
	AF = undefined;
	CF = 0;
	dest = and(dest, src);
	SF = sign(dest);
	ZF = zero(dest);
	PF = parity(dest);
}

insn sub(P dest, P src)
{
	OF = sub_of(dest, src);
	AF = sub_af(dest, src);
	CF = sub_cf(dest, src);
	dest = sub(dest, src);
	SF = sign(dest);
	ZF = zero(dest);
	PF = parity(dest);
}

insn xor(P dest, P src)
{
	OF = 0;
	AF = undefined;
	CF = 0;
	dest = xor(dest, src);
	SF = sign(dest);
	ZF = zero(dest);
	PF = parity(dest);
}

insn cmp(P dest, P src)
{
	OF = sub_of(dest, src);
	AF = sub_af(dest, src);
	CF = sub_cf(dest, src);
	P tmp = sub(dest, src);
	SF = sign(tmp);
	ZF = zero(tmp);
	PF = parity(tmp);
}

// shift and rotate.

// 1st 4 insn's : sf, zf, af, pf unaffected.
// of : affected only for single-bit rotates.
// cf : last bit shifted out

insn rol(P dest, B1 src)
{
	B1 count = src;
	B1 tmpmod = shl(tr<B1>(sizeof(P)), 3);
	B1 tmpcount = and(count, sub(tmpmod, 1));
	
	dest = rol(dest, tmpcount);
	
	bit tmp0 = zero(tmpcount);
	bit newcf = bitquest(tmp0, CF, lsb(dest));
	OF = bitquest(tmp0, OF, bitquest(eq(count, 1), lxor(sign(dest), newcf), undefined));
	CF = newcf;
}

insn ror(P dest, B1 src)
{
	B1 count = src;
	B1 tmpmod = shl(tr<B1>(sizeof(P)), 3);
	B1 tmpcount = and(count, sub(tmpmod, 1));
	
	dest = ror(dest, tmpcount);
	
	bit tmp0 = zero(tmpcount);
	bit newcf = bitquest(tmp0, CF, sign(dest));
	OF = bitquest(tmp0, OF, bitquest(eq(count, 1), lxor(sign(dest), sign2(dest)), undefined));
	CF = newcf;
}

insn rcl(P dest, B1 src)
{
	B1 count = src;
	B1 count1 = and(count, 31);
	B1 tmpmod = add(shl(tr<B1>(sizeof(P)), 3), 1);
	B1 tmpcount = umod(count1, tmpmod);
	
	P olddest = dest;
	dest = rcl(dest, tmpcount, CF);
	CF = rcl_cf(olddest, tmpcount, CF);
	OF = bitquest(zero(tmpcount), OF, bitquest(eq(count, 1), lxor(sign(dest), CF), undefined));
}

insn rcr(P dest, B1 src)
{
	B1 count = src;
	B1 count1 = and(count, 31);
	B1 tmpmod = add(shl(tr<B1>(sizeof(P)), 3), 1);
	B1 tmpcount = umod(count1, tmpmod);
	
	OF = bitquest(zero(tmpcount), OF, bitquest(eq(count, 1), lxor(sign(dest), CF), undefined));
	P olddest = dest;
	dest = rcr(dest, tmpcount, CF);
	CF = rcr_cf(olddest, tmpcount, CF);
}

insn shl(P dest, B1 src)
{
	B1 tmpcount = and(src, 31);
	bit count0 = zero(tmpcount);
	P tmpdest = dest;
	dest = shl(dest, tmpcount);
	CF = bitquest(count0, CF, shl_cf(tmpdest, tmpcount));
	OF = bitquest(eq(tmpcount, 1), lxor(sign(dest), CF), bitquest(count0, OF, undefined));
	SF = bitquest(count0, SF, sign(dest));
	ZF = bitquest(count0, ZF, zero(dest));
	AF = bitquest(count0, AF, undefined);
	PF = bitquest(count0, PF, parity(dest));
}

insn shr(P dest, B1 src)
{
	B1 tmpcount = and(src, 31);
	bit count0 = zero(tmpcount);
	P tmpdest = dest;
	dest = shru(dest, tmpcount);
	CF = bitquest(count0, CF, shr_cf(tmpdest, tmpcount));
	OF = bitquest(eq(tmpcount, 1), sign(tmpdest), bitquest(count0, OF, undefined));
	SF = bitquest(count0, SF, sign(dest));
	ZF = bitquest(count0, ZF, zero(dest));
	AF = bitquest(count0, AF, undefined);
	PF = bitquest(count0, PF, parity(dest));
}

insn _sal(P dest, B1 src)
{
	B1 tmpcount = and(src, 31);
	bit count0 = zero(tmpcount);
	P tmpdest = dest;
	dest = shl(dest, tmpcount);
	CF = bitquest(count0, CF, shl_cf(tmpdest, tmpcount));
	OF = bitquest(eq(tmpcount, 1), lxor(sign(dest), CF), bitquest(count0, OF, undefined));
	SF = bitquest(count0, SF, sign(dest));
	ZF = bitquest(count0, ZF, zero(dest));
	AF = bitquest(count0, AF, undefined);
	PF = bitquest(count0, PF, parity(dest));
}

insn sar(P dest, B1 src)
{
	B1 tmpcount = and(src, 31);
	bit count0 = zero(tmpcount);
	P tmpdest = dest;
	dest = shrs(dest, tmpcount);
	CF = bitquest(count0, CF, sar_cf(tmpdest, tmpcount));
	OF = bitquest(eq(tmpcount, 1), sign(tmpdest), bitquest(count0, OF, undefined));
	SF = bitquest(count0, SF, sign(dest));
	ZF = bitquest(count0, ZF, zero(dest));
	AF = bitquest(count0, AF, undefined);
	PF = bitquest(count0, PF, parity(dest));
}

insn inc(P arg)
{
	OF = add_of(arg, tr<P>(1));
	AF = add_af(arg, tr<P>(1));
	arg = add(arg, tr<P>(1));
	SF = sign(arg);
	ZF = zero(arg);
	PF = parity(arg);
}

insn dec(P arg)
{
	OF = sub_of(arg, tr<P>(1));
	AF = sub_af(arg, tr<P>(1));
	arg = sub(arg, tr<P>(1));
	SF = sign(arg);
	ZF = zero(arg);
	PF = parity(arg);
}

insn _xlat(B1 xs)
{
	AL = xs;
}

insn _pushsr(P arg)
{
	push(arg);
}

insn _popsr(P arg)
{
	pop(arg);
}

insn _push(P arg)
{
	push(arg);
}

insn _pop(P arg)
{
	pop(arg);		// note: can't pop cs
}

// Following two instructions required name updates
// in x86s.
insn _movsrv(B2 dest, P src)
{
	dest = tr<B2>(src);
}
insn _movvsr(P dest, B2 src)
{
	dest = zx<P>(src);
}

insn _pushf()
{
	push(RFLAGS);
}

insn _popf()
{
	pop(RFLAGS);
}

insn _jcc(P rel, B1 cc)
{
	// RIPC will look only at the low operand size bytes.
	RIPC = quest(xcond(cc), zx<B8>(rel), 0);
}

insn call(P rel)
{
	push(RIP1);
	RIPC = zx<B8>(rel);
}

insn _calli(P src)
{
	push(RIP1);
	RIP = zx<B8>(src);
}

insn _callfd(P ofsT, B2 segT)
{
	o16: reserve(tr<B8>(4));
	o32: reserve(tr<B8>(8));
	o64: reserve(tr<B8>(16));
	B8 tmp_cs = zx<B8>(CS);
	B8 tmp_rip1 = RIP1;
	// Do RIP before CS assignment, because RIP assigns don't really take
	// effect until after the instruction is successful.
	RIP = zx<B8>(ofsT);
	CS = segT;
	push(tmp_cs);
	push(tmp_rip1);
}

insn _callfi(P target)
{
	o16: reserve(tr<B8>(4));
	o32: reserve(tr<B8>(8));
	o64: reserve(tr<B8>(16));
	B8 tmp_cs = zx<B8>(CS);
	B8 tmp_rip1 = RIP1;
	RIP = zx<B8>(ofs(target));
	CS = seg(target);
	push(tmp_cs);
	push(tmp_rip1);
}

insn jmp(P rel)
{
	RIPC = zx<B8>(rel);
}

insn _jmpi(P src)
{
	RIP = zx<B8>(src);
}

insn _jmpfd(P ofsT, B2 segT)
{
	RIP = zx<B8>(ofsT);
	CS = segT;
}

insn _jmpfi(P target)
{
	CS = seg(target);
	RIP = zx<B8>(ofs(target));
}

insn _retnum(B2 x)
{
	//fixme--could put a restore here to make sure it would succeed..
	pop(RIP);		// pop(RIP) always uses operand size for # bytes to pop
	discard(zx<B8>(x));
}

insn _ret()
{
	pop(RIP);
}

insn _retfnum(B2 x)
{
	// make sure we can restore, first.
	o16: restore(add(tr<B8>(4), zx<B8>(x)));
	o32: restore(add(tr<B8>(8), zx<B8>(x)));
	o64: restore(add(tr<B8>(16), zx<B8>(x)));
	
	B8 tmp1;
	B8 tmp2;
	pop(tmp1);		// RIP
	pop(tmp2);		// CS
	
	push(tmp2);
	push(tmp1);
	CS = tr<B2>(tmp2);	// may fail. if it does, stack will be restored on account of the two push's above
	RIP = tmp1;
	o16: discard(tr<B8>(4));	// get rid of RIP, CS on stack
	o32: discard(tr<B8>(8));
	o64: discard(tr<B8>(16));
	discard(zx<B8>(x));
}

// this has an implict 16-bit zero argument. However, this is probably a bug and it should
// not be used.
insn _retf(B2 xx)
{
	B2 x = 0;
	// make sure we can restore, first.
	o16: restore(add(tr<B8>(4), zx<B8>(x)));
	o32: restore(add(tr<B8>(8), zx<B8>(x)));
	o64: restore(add(tr<B8>(16), zx<B8>(x)));
	
	B8 tmp1;
	B8 tmp2;
	pop(tmp1);		// RIP
	pop(tmp2);		// CS
	
	push(tmp2);
	push(tmp1);
	CS = tr<B2>(tmp2);	// may fail. if it does, stack will be restored on account of the two push's above
	RIP = tmp1;
	o16: discard(tr<B8>(4));	// get rid of RIP, CS on stack
	o32: discard(tr<B8>(8));
	o64: discard(tr<B8>(16));
	discard(zx<B8>(x));
}

insn int(B1 vec)
{
	///Below code is for real mode.
	///reserve(3);	// oops, should be in byte!
	///push(RFLAGS);
	///push(CS);
	///push(RIP1);
	///IF = 0;
	RINT = vec;
}

// Note: this instruction requires special handling. The code below is valid for real mode and for
// transitions to the same privilige level. Transitions to a lower (numerically higher) privilige
// level or to vm86 mode (RFLAGS.VM == 1) need to be handled specially. For p-mode operating
// systems, we can generally stop the analysis when an IRET is found as that marks the end of the
// system under study. For real-mode operating systems the semantics shown below is sufficient.
insn _iret()
{
	o16: restore(tr<B8>(6));		// make sure we can pop 3 before proceeding
	o32: restore(tr<B8>(12));		// make sure we can pop 3 before proceeding
	o64: restore(tr<B8>(24));		// make sure we can pop 3 before proceeding
	
	B8 tmp1;
	B8 tmp2;
	B8 tmp3;
	pop(tmp1);		// RIP
	pop(tmp2);		// CS
	pop(tmp3);		// RFLAGS
	
	push(tmp3);
	push(tmp2);
	push(tmp1);
	CS = tr<B2>(tmp2);	// may fail. if it does, stack will be restored on account of the 3 push's above
	RIP = tmp1;
	RFLAGS = tmp3;
	o16: discard(tr<B8>(6));	// get rid of RIP, CS on stack
	o32: discard(tr<B8>(12));
	o64: discard(tr<B8>(24));
}

insn _loopnz(P rel, Q c)
{
	c = sub(c, 1);
	bit cond = land(lnot(ZF), nz(c));
	RIPC = zx<B8>(quest(cond, rel, 0));
}

insn _loopz(P rel, Q c)
{
	c = sub(c, 1);
	bit cond = land(ZF, nz(c));
	RIPC = zx<B8>(quest(cond, rel, 0));
}

insn _loop(P rel, Q c)
{
	c = sub(c, 1);
	bit cond = nz(c);
	RIPC = zx<B8>(quest(cond, rel, 0));
}

insn _jrcxz(P rel, Q c)
{
	bit cond = zero(c);
	RIPC = zx<B8>(quest(cond, rel, 0));
}

insn cli()
{
	IF = 0;
}

insn sti()
{
	IF = 1;
}

// string instructions

insn _movs(P x, P y)
{
	x = y;
	
	B4 count = quest(lnot(DF), tr<B4>(sizeof(P)), neg(tr<B4>(sizeof(P))));
	a16: SI = add(SI, tr<B2>(count));
	a16: DI = add(DI, tr<B2>(count));
	a32: ESI = add(ESI, count);
	a32: EDI = add(EDI, count);
}

insn _cmps(P dest, P src)
{
	OF = sub_of(dest, src);
	AF = sub_af(dest, src);
	CF = sub_cf(dest, src);
	P tmp = sub(dest, src);
	SF = sign(tmp);
	ZF = zero(tmp);
	PF = parity(tmp);

	B4 count = quest(lnot(DF), tr<B4>(sizeof(P)), neg(tr<B4>(sizeof(P))));
	a16: SI = add(SI, tr<B2>(count));
	a16: DI = add(DI, tr<B2>(count));
	a32: ESI = add(ESI, count);
	a32: EDI = add(EDI, count);
}

insn _stos(P x, P y)
{
	x = y;

	B4 count = quest(lnot(DF), tr<B4>(sizeof(P)), neg(tr<B4>(sizeof(P))));
	a16: DI = add(DI, tr<B2>(count));
	a32: EDI = add(EDI, count);
}

insn _lods(P x, P y)
{
	x = y;

	B4 count = quest(lnot(DF), tr<B4>(sizeof(P)), neg(tr<B4>(sizeof(P))));
	a16: SI = add(SI, tr<B2>(count));
	a32: ESI = add(ESI, count);
}

insn _scas(P dest, P src)
{
	OF = sub_of(dest, src);
	AF = sub_af(dest, src);
	CF = sub_cf(dest, src);
	P tmp = sub(dest, src);
	SF = sign(tmp);
	ZF = zero(tmp);
	PF = parity(tmp);

	B4 count = quest(lnot(DF), tr<B4>(sizeof(P)), neg(tr<B4>(sizeof(P))));
	a16: DI = add(DI, tr<B2>(count));
	a32: EDI = add(EDI, count);
}

// *** Where are ins, outs ??? Not needed yet.

insn in(P x, Q y)
{
	inport(x, y);
}

insn out(P x, Q y)
{
	outport(x, y);
}

// note on les etc.--target size is offset size.
insn les(P x, P target)
{
	ES = seg(target);
	x = ofs(target);
}

insn lds(P x, P target)
{
	DS = seg(target);
	x = ofs(target);
}

// --- begin stuff for FreeDOS ---

insn enter(B2 x, B1 y)
{
	// FIXME--put a reserve() here to make sure state is consistent in case an exception is raised!
	assert(eq(y, 0));	// multilevel enter not supported by us!
	push(RBP);			// guess--operand size decides what to push
	RBPX = RSP;			// if so, shouldn't RBPX depend on operand size, not ssz ??
	commit(zx<B8>(x));	// commit arg size must be 8 bytes
}

// note: if the pop fails, rsp will not be rolled back to its old state.
// this is a subtle point and probably doesn't matter.
insn leave()
{
	RSPX = RBP;
	pop(RBPX);
}

insn lfs(P x, P target)
{
	FS = seg(target);
	x = ofs(target);
}

insn lgs(P x, P target)
{
	GS = seg(target);
	x = ofs(target);
}

insn lss(P x, P target)
{
	SS = seg(target);
	x = ofs(target);
}

insn _setcc(B1 dest, B1 cc)
{
	// One could use the terms bitcond() and bytecond() instead.
	dest = bitzx(xcond(cc));
}

insn movzx(P dest, Q src)
{
	dest = zx<P>(src);
}

insn movsx(P dest, Q src)
{
	dest = sx<P>(src);
}

insn _pusha()
{
	// Could be backwards.
	o16: push(AX);
	o16: push(CX);
	o16: push(DX);
	o16: push(BX);
	o16: push(SP);
	o16: push(BP);
	o16: push(SI);
	o16: push(DI);
	o32: push(EAX);
	o32: push(ECX);
	o32: push(EDX);
	o32: push(EBX);
	o32: push(ESP);
	o32: push(EBP);
	o32: push(ESI);
	o32: push(EDI);
}

insn _popa()
{
	o16: pop(DI);
	o16: pop(SI);
	o16: pop(BP);
	o16: discard(tr<B8>(2));
	o16: pop(BX);
	o16: pop(DX);
	o16: pop(CX);
	o16: pop(AX);
	o32: pop(EDI);
	o32: pop(ESI);
	o32: pop(EBP);
	o32: discard(tr<B8>(4));
	o32: pop(EBX);
	o32: pop(EDX);
	o32: pop(ECX);
	o32: pop(EAX);
}

