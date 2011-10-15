// out_intel_emulator.h  (note: this file was automatically generated -- do not edit!)
// Copyright (C) 2011 Willow Schlanger. All rights reserved.


static int crudasm_intel_execute__aaa(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	temp_a = internal_compute_2__logor(context, internal_get_flag(context, 4), internal_compute_2__gtu(context, internal_compute_2_bitand(context, internal_get_reg8(context, 0), internal_make_num(context, 1, 0, 0x0f)), internal_make_num(context, 1, 0, 0x09)));
	do_write_rflag_value(context, 4, &temp_a);
	temp_a = internal_get_flag(context, 4);
	do_write_rflag_value(context, 0, &temp_a);
	temp_a = internal_compute_2_add(context, internal_get_reg16(context, 0), internal_compute_3_quest(context, internal_get_flag(context, 4), internal_make_num(context, 2, 0, 0x0106), internal_make_num(context, 2, 0, 0)));
	if(write_gr16_value(context, 0, &temp_a))
		return 1;
	temp_a = internal_compute_2_bitand(context, internal_get_reg8(context, 0), internal_make_num(context, 1, 0, 0x0f));
	if(write_gr8_value(context, 0, &temp_a))
		return 1;
	temp_a = internal_make_num(context, 1, 0, 0);
	do_write_rflag_value(context, 11, &temp_a);
	temp_a = internal_make_num(context, 1, 0, 0);
	do_write_rflag_value(context, 7, &temp_a);
	temp_a = internal_compute_1__zero(context, internal_get_reg8(context, 0));
	do_write_rflag_value(context, 6, &temp_a);
	temp_a = internal_compute_1__parity(context, internal_get_reg8(context, 0));
	do_write_rflag_value(context, 2, &temp_a);
	return 0;  /* success */
}


static int crudasm_intel_execute___aad(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	struct crudasm_temp_node_t tmp[1];
	struct crudasm_temp_node_t argtmp[1];
	tmp[0].size = 1;
	argtmp[0].size = get_arg_size(context, 0);
	temp_a = internal_get_reg8(context, 16);
	tmp[0] = temp_a;
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_2_cmul(context, tmp[0], argtmp[0]);
	tmp[0] = temp_a;
	temp_a = internal_compute_2_add(context, tmp[0], internal_get_reg8(context, 0));
	tmp[0] = temp_a;
	temp_a = tmp[0];
	if(write_gr8_value(context, 0, &temp_a))
		return 1;
	temp_a = internal_make_num(context, 1, 0, 0);
	if(write_gr8_value(context, 16, &temp_a))
		return 1;
	temp_a = internal_get_extern_undefined(context);
	do_write_rflag_value(context, 11, &temp_a);
	temp_a = internal_compute_1__sign(context, internal_get_reg8(context, 0));
	do_write_rflag_value(context, 7, &temp_a);
	temp_a = internal_compute_1__zero(context, internal_get_reg8(context, 0));
	do_write_rflag_value(context, 6, &temp_a);
	temp_a = internal_get_extern_undefined(context);
	do_write_rflag_value(context, 4, &temp_a);
	temp_a = internal_compute_1__parity(context, internal_get_reg8(context, 0));
	do_write_rflag_value(context, 2, &temp_a);
	temp_a = internal_get_extern_undefined(context);
	do_write_rflag_value(context, 0, &temp_a);
	return 0;  /* success */
}


static int crudasm_intel_execute___aam(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	struct crudasm_temp_node_t tmp[1];
	struct crudasm_temp_node_t argtmp[1];
	tmp[0].size = 1;
	argtmp[0].size = get_arg_size(context, 0);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	if(special_do_assert(context, internal_compute_1__lognot(context, internal_compute_1__zero(context, argtmp[0])), internal_make_num(context, 1, 0, 0)) != 0)
		return 1;
	temp_a = internal_get_reg8(context, 0);
	tmp[0] = temp_a;
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_2_udiv(context, tmp[0], argtmp[0]);
	if(write_gr8_value(context, 16, &temp_a))
		return 1;
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_2_umod(context, tmp[0], argtmp[0]);
	if(write_gr8_value(context, 0, &temp_a))
		return 1;
	temp_a = internal_get_extern_undefined(context);
	do_write_rflag_value(context, 11, &temp_a);
	temp_a = internal_compute_1__sign(context, internal_get_reg8(context, 0));
	do_write_rflag_value(context, 7, &temp_a);
	temp_a = internal_compute_1__zero(context, internal_get_reg8(context, 0));
	do_write_rflag_value(context, 6, &temp_a);
	temp_a = internal_get_extern_undefined(context);
	do_write_rflag_value(context, 4, &temp_a);
	temp_a = internal_compute_1__parity(context, internal_get_reg8(context, 0));
	do_write_rflag_value(context, 2, &temp_a);
	temp_a = internal_get_extern_undefined(context);
	do_write_rflag_value(context, 0, &temp_a);
	return 0;  /* success */
}


static int crudasm_intel_execute__aas(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	temp_a = internal_compute_2__logor(context, internal_get_flag(context, 4), internal_compute_2__gtu(context, internal_compute_2_bitand(context, internal_get_reg8(context, 0), internal_make_num(context, 1, 0, 0x0f)), internal_make_num(context, 1, 0, 0x09)));
	do_write_rflag_value(context, 4, &temp_a);
	temp_a = internal_get_flag(context, 4);
	do_write_rflag_value(context, 0, &temp_a);
	temp_a = internal_compute_2_sub(context, internal_get_reg16(context, 0), internal_compute_3_quest(context, internal_get_flag(context, 4), internal_make_num(context, 2, 0, 0x0106), internal_make_num(context, 2, 0, 0)));
	if(write_gr16_value(context, 0, &temp_a))
		return 1;
	temp_a = internal_compute_2_bitand(context, internal_get_reg8(context, 0), internal_make_num(context, 1, 0, 0x0f));
	if(write_gr8_value(context, 0, &temp_a))
		return 1;
	temp_a = internal_make_num(context, 1, 0, 0);
	do_write_rflag_value(context, 11, &temp_a);
	temp_a = internal_make_num(context, 1, 0, 0);
	do_write_rflag_value(context, 7, &temp_a);
	temp_a = internal_compute_1__zero(context, internal_get_reg8(context, 0));
	do_write_rflag_value(context, 6, &temp_a);
	temp_a = internal_compute_1__parity(context, internal_get_reg8(context, 0));
	do_write_rflag_value(context, 2, &temp_a);
	return 0;  /* success */
}


static int crudasm_intel_execute__das(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	struct crudasm_temp_node_t tmp[5];
	tmp[0].size = 1;
	tmp[1].size = 1;
	tmp[2].size = 1/*BIT*/;
	tmp[3].size = 1/*BIT*/;
	tmp[4].size = 1;
	temp_a = internal_get_reg8(context, 0);
	tmp[0] = temp_a;
	temp_a = internal_compute_2__logor(context, internal_get_flag(context, 4), internal_compute_2__gtu(context, internal_compute_2_bitand(context, tmp[0], internal_make_num(context, 1, 0, 0x0f)), internal_make_num(context, 1, 0, 0x09)));
	do_write_rflag_value(context, 4, &temp_a);
	temp_a = internal_compute_3_quest(context, internal_get_flag(context, 4), internal_make_num(context, 1, 0, 0x06), internal_make_num(context, 1, 0, 0));
	tmp[1] = temp_a;
	temp_a = internal_compute_3__quest(context, internal_get_flag(context, 4), internal_compute_2__logor(context, internal_get_flag(context, 0), internal_compute_2__ltu(context, internal_get_reg8(context, 0), internal_make_num(context, 1, 0, 0x06))), internal_make_num(context, 1, 0, 0));
	tmp[2] = temp_a;
	temp_a = internal_compute_2_sub(context, internal_get_reg8(context, 0), tmp[1]);
	if(write_gr8_value(context, 0, &temp_a))
		return 1;
	temp_a = internal_compute_2__logor(context, internal_get_flag(context, 0), internal_compute_2__gtu(context, tmp[0], internal_make_num(context, 1, 0, 0x99)));
	tmp[3] = temp_a;
	temp_a = internal_compute_3_quest(context, tmp[3], internal_make_num(context, 1, 0, 0x60), internal_make_num(context, 1, 0, 0));
	tmp[4] = temp_a;
	temp_a = internal_compute_2_sub(context, internal_get_reg8(context, 0), tmp[4]);
	if(write_gr8_value(context, 0, &temp_a))
		return 1;
	temp_a = internal_compute_2__logor(context, tmp[2], tmp[3]);
	tmp[2] = temp_a;
	temp_a = internal_make_num(context, 1, 0, 0);
	do_write_rflag_value(context, 11, &temp_a);
	temp_a = internal_compute_1__sign(context, internal_get_reg8(context, 0));
	do_write_rflag_value(context, 7, &temp_a);
	temp_a = internal_compute_1__zero(context, internal_get_reg8(context, 0));
	do_write_rflag_value(context, 6, &temp_a);
	temp_a = internal_compute_1__parity(context, internal_get_reg8(context, 0));
	do_write_rflag_value(context, 2, &temp_a);
	temp_a = tmp[2];
	do_write_rflag_value(context, 0, &temp_a);
	return 0;  /* success */
}


static int crudasm_intel_execute__daa(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	struct crudasm_temp_node_t tmp[5];
	tmp[0].size = 1;
	tmp[1].size = 1;
	tmp[2].size = 1/*BIT*/;
	tmp[3].size = 1/*BIT*/;
	tmp[4].size = 1;
	temp_a = internal_get_reg8(context, 0);
	tmp[0] = temp_a;
	temp_a = internal_compute_2__logor(context, internal_get_flag(context, 4), internal_compute_2__gtu(context, internal_compute_2_bitand(context, tmp[0], internal_make_num(context, 1, 0, 0x0f)), internal_make_num(context, 1, 0, 0x09)));
	do_write_rflag_value(context, 4, &temp_a);
	temp_a = internal_compute_3_quest(context, internal_get_flag(context, 4), internal_make_num(context, 1, 0, 0x06), internal_make_num(context, 1, 0, 0));
	tmp[1] = temp_a;
	temp_a = internal_compute_3__quest(context, internal_get_flag(context, 4), internal_compute_2__logor(context, internal_get_flag(context, 0), internal_compute_2__gtu(context, internal_get_reg8(context, 0), internal_make_num(context, 1, 0, 0xf9))), internal_make_num(context, 1, 0, 0));
	tmp[2] = temp_a;
	temp_a = internal_compute_2_add(context, internal_get_reg8(context, 0), tmp[1]);
	if(write_gr8_value(context, 0, &temp_a))
		return 1;
	temp_a = internal_compute_2__logor(context, internal_get_flag(context, 0), internal_compute_2__gtu(context, tmp[0], internal_make_num(context, 1, 0, 0x99)));
	tmp[3] = temp_a;
	temp_a = internal_compute_3_quest(context, tmp[3], internal_make_num(context, 1, 0, 0x60), internal_make_num(context, 1, 0, 0));
	tmp[4] = temp_a;
	temp_a = internal_compute_2_add(context, internal_get_reg8(context, 0), tmp[4]);
	if(write_gr8_value(context, 0, &temp_a))
		return 1;
	temp_a = tmp[3];
	tmp[2] = temp_a;
	temp_a = internal_make_num(context, 1, 0, 0);
	do_write_rflag_value(context, 11, &temp_a);
	temp_a = internal_compute_1__sign(context, internal_get_reg8(context, 0));
	do_write_rflag_value(context, 7, &temp_a);
	temp_a = internal_compute_1__zero(context, internal_get_reg8(context, 0));
	do_write_rflag_value(context, 6, &temp_a);
	temp_a = internal_compute_1__parity(context, internal_get_reg8(context, 0));
	do_write_rflag_value(context, 2, &temp_a);
	temp_a = tmp[2];
	do_write_rflag_value(context, 0, &temp_a);
	return 0;  /* success */
}


static int crudasm_intel_execute__adc(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	struct crudasm_temp_node_t tmp[1];
	struct crudasm_temp_node_t argtmp[2];
	tmp[0].size = 1/*BIT*/;
	argtmp[0].size = get_arg_size(context, 0);
	argtmp[1].size = get_arg_size(context, 1);
	temp_a = internal_get_flag(context, 0);
	tmp[0] = temp_a;
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	if(crudasm_read_arg(context, 1, argtmp + 1) != 0)
		return 1;
	temp_a = internal_compute_3__adc_of(context, argtmp[0], argtmp[1], tmp[0]);
	do_write_rflag_value(context, 11, &temp_a);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	if(crudasm_read_arg(context, 1, argtmp + 1) != 0)
		return 1;
	temp_a = internal_compute_3__adc_af(context, argtmp[0], argtmp[1], tmp[0]);
	do_write_rflag_value(context, 4, &temp_a);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	if(crudasm_read_arg(context, 1, argtmp + 1) != 0)
		return 1;
	temp_a = internal_compute_3__adc_cf(context, argtmp[0], argtmp[1], tmp[0]);
	do_write_rflag_value(context, 0, &temp_a);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	if(crudasm_read_arg(context, 1, argtmp + 1) != 0)
		return 1;
	temp_a = internal_compute_2_add(context, internal_compute_2_add(context, argtmp[0], argtmp[1]), internal_compute_zx(context, get_arg_size(context, 0), tmp[0]));
	if(crudasm_write_arg(context, 0, &temp_a) != 0)
		return 1;
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_1__sign(context, argtmp[0]);
	do_write_rflag_value(context, 7, &temp_a);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_1__zero(context, argtmp[0]);
	do_write_rflag_value(context, 6, &temp_a);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_1__parity(context, argtmp[0]);
	do_write_rflag_value(context, 2, &temp_a);
	return 0;  /* success */
}


static int crudasm_intel_execute__add(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	struct crudasm_temp_node_t argtmp[2];
	argtmp[0].size = get_arg_size(context, 0);
	argtmp[1].size = get_arg_size(context, 1);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	if(crudasm_read_arg(context, 1, argtmp + 1) != 0)
		return 1;
	temp_a = internal_compute_2__add_of(context, argtmp[0], argtmp[1]);
	do_write_rflag_value(context, 11, &temp_a);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	if(crudasm_read_arg(context, 1, argtmp + 1) != 0)
		return 1;
	temp_a = internal_compute_2__add_af(context, argtmp[0], argtmp[1]);
	do_write_rflag_value(context, 4, &temp_a);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	if(crudasm_read_arg(context, 1, argtmp + 1) != 0)
		return 1;
	temp_a = internal_compute_2__add_cf(context, argtmp[0], argtmp[1]);
	do_write_rflag_value(context, 0, &temp_a);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	if(crudasm_read_arg(context, 1, argtmp + 1) != 0)
		return 1;
	temp_a = internal_compute_2_add(context, argtmp[0], argtmp[1]);
	if(crudasm_write_arg(context, 0, &temp_a) != 0)
		return 1;
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_1__sign(context, argtmp[0]);
	do_write_rflag_value(context, 7, &temp_a);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_1__zero(context, argtmp[0]);
	do_write_rflag_value(context, 6, &temp_a);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_1__parity(context, argtmp[0]);
	do_write_rflag_value(context, 2, &temp_a);
	return 0;  /* success */
}


static int crudasm_intel_execute__or(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	struct crudasm_temp_node_t argtmp[2];
	argtmp[0].size = get_arg_size(context, 0);
	argtmp[1].size = get_arg_size(context, 1);
	temp_a = internal_make_num(context, 1, 0, 0);
	do_write_rflag_value(context, 11, &temp_a);
	temp_a = internal_get_extern_undefined(context);
	do_write_rflag_value(context, 4, &temp_a);
	temp_a = internal_make_num(context, 1, 0, 0);
	do_write_rflag_value(context, 0, &temp_a);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	if(crudasm_read_arg(context, 1, argtmp + 1) != 0)
		return 1;
	temp_a = internal_compute_2_bitor(context, argtmp[0], argtmp[1]);
	if(crudasm_write_arg(context, 0, &temp_a) != 0)
		return 1;
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_1__sign(context, argtmp[0]);
	do_write_rflag_value(context, 7, &temp_a);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_1__zero(context, argtmp[0]);
	do_write_rflag_value(context, 6, &temp_a);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_1__parity(context, argtmp[0]);
	do_write_rflag_value(context, 2, &temp_a);
	return 0;  /* success */
}


static int crudasm_intel_execute__sbb(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	struct crudasm_temp_node_t tmp[1];
	struct crudasm_temp_node_t argtmp[2];
	tmp[0].size = 1/*BIT*/;
	argtmp[0].size = get_arg_size(context, 0);
	argtmp[1].size = get_arg_size(context, 1);
	temp_a = internal_get_flag(context, 0);
	tmp[0] = temp_a;
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	if(crudasm_read_arg(context, 1, argtmp + 1) != 0)
		return 1;
	temp_a = internal_compute_3__sbb_of(context, argtmp[0], argtmp[1], tmp[0]);
	do_write_rflag_value(context, 11, &temp_a);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	if(crudasm_read_arg(context, 1, argtmp + 1) != 0)
		return 1;
	temp_a = internal_compute_3__sbb_af(context, argtmp[0], argtmp[1], tmp[0]);
	do_write_rflag_value(context, 4, &temp_a);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	if(crudasm_read_arg(context, 1, argtmp + 1) != 0)
		return 1;
	temp_a = internal_compute_3__sbb_cf(context, argtmp[0], argtmp[1], tmp[0]);
	do_write_rflag_value(context, 0, &temp_a);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	if(crudasm_read_arg(context, 1, argtmp + 1) != 0)
		return 1;
	temp_a = internal_compute_2_sub(context, internal_compute_2_sub(context, argtmp[0], argtmp[1]), internal_compute_zx(context, get_arg_size(context, 0), tmp[0]));
	if(crudasm_write_arg(context, 0, &temp_a) != 0)
		return 1;
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_1__sign(context, argtmp[0]);
	do_write_rflag_value(context, 7, &temp_a);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_1__zero(context, argtmp[0]);
	do_write_rflag_value(context, 6, &temp_a);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_1__parity(context, argtmp[0]);
	do_write_rflag_value(context, 2, &temp_a);
	return 0;  /* success */
}


static int crudasm_intel_execute__and(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	struct crudasm_temp_node_t argtmp[2];
	argtmp[0].size = get_arg_size(context, 0);
	argtmp[1].size = get_arg_size(context, 1);
	temp_a = internal_make_num(context, 1, 0, 0);
	do_write_rflag_value(context, 11, &temp_a);
	temp_a = internal_get_extern_undefined(context);
	do_write_rflag_value(context, 4, &temp_a);
	temp_a = internal_make_num(context, 1, 0, 0);
	do_write_rflag_value(context, 0, &temp_a);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	if(crudasm_read_arg(context, 1, argtmp + 1) != 0)
		return 1;
	temp_a = internal_compute_2_bitand(context, argtmp[0], argtmp[1]);
	if(crudasm_write_arg(context, 0, &temp_a) != 0)
		return 1;
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_1__sign(context, argtmp[0]);
	do_write_rflag_value(context, 7, &temp_a);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_1__zero(context, argtmp[0]);
	do_write_rflag_value(context, 6, &temp_a);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_1__parity(context, argtmp[0]);
	do_write_rflag_value(context, 2, &temp_a);
	return 0;  /* success */
}


static int crudasm_intel_execute__sub(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	struct crudasm_temp_node_t argtmp[2];
	argtmp[0].size = get_arg_size(context, 0);
	argtmp[1].size = get_arg_size(context, 1);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	if(crudasm_read_arg(context, 1, argtmp + 1) != 0)
		return 1;
	temp_a = internal_compute_2__sub_of(context, argtmp[0], argtmp[1]);
	do_write_rflag_value(context, 11, &temp_a);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	if(crudasm_read_arg(context, 1, argtmp + 1) != 0)
		return 1;
	temp_a = internal_compute_2__sub_af(context, argtmp[0], argtmp[1]);
	do_write_rflag_value(context, 4, &temp_a);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	if(crudasm_read_arg(context, 1, argtmp + 1) != 0)
		return 1;
	temp_a = internal_compute_2__sub_cf(context, argtmp[0], argtmp[1]);
	do_write_rflag_value(context, 0, &temp_a);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	if(crudasm_read_arg(context, 1, argtmp + 1) != 0)
		return 1;
	temp_a = internal_compute_2_sub(context, argtmp[0], argtmp[1]);
	if(crudasm_write_arg(context, 0, &temp_a) != 0)
		return 1;
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_1__sign(context, argtmp[0]);
	do_write_rflag_value(context, 7, &temp_a);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_1__zero(context, argtmp[0]);
	do_write_rflag_value(context, 6, &temp_a);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_1__parity(context, argtmp[0]);
	do_write_rflag_value(context, 2, &temp_a);
	return 0;  /* success */
}


static int crudasm_intel_execute__xor(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	struct crudasm_temp_node_t argtmp[2];
	argtmp[0].size = get_arg_size(context, 0);
	argtmp[1].size = get_arg_size(context, 1);
	temp_a = internal_make_num(context, 1, 0, 0);
	do_write_rflag_value(context, 11, &temp_a);
	temp_a = internal_get_extern_undefined(context);
	do_write_rflag_value(context, 4, &temp_a);
	temp_a = internal_make_num(context, 1, 0, 0);
	do_write_rflag_value(context, 0, &temp_a);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	if(crudasm_read_arg(context, 1, argtmp + 1) != 0)
		return 1;
	temp_a = internal_compute_2_bitxor(context, argtmp[0], argtmp[1]);
	if(crudasm_write_arg(context, 0, &temp_a) != 0)
		return 1;
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_1__sign(context, argtmp[0]);
	do_write_rflag_value(context, 7, &temp_a);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_1__zero(context, argtmp[0]);
	do_write_rflag_value(context, 6, &temp_a);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_1__parity(context, argtmp[0]);
	do_write_rflag_value(context, 2, &temp_a);
	return 0;  /* success */
}


static int crudasm_intel_execute__cmp(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	struct crudasm_temp_node_t tmp[1];
	struct crudasm_temp_node_t argtmp[2];
	tmp[0].size = get_arg_size(context, 0);
	argtmp[0].size = get_arg_size(context, 0);
	argtmp[1].size = get_arg_size(context, 1);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	if(crudasm_read_arg(context, 1, argtmp + 1) != 0)
		return 1;
	temp_a = internal_compute_2__sub_of(context, argtmp[0], argtmp[1]);
	do_write_rflag_value(context, 11, &temp_a);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	if(crudasm_read_arg(context, 1, argtmp + 1) != 0)
		return 1;
	temp_a = internal_compute_2__sub_af(context, argtmp[0], argtmp[1]);
	do_write_rflag_value(context, 4, &temp_a);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	if(crudasm_read_arg(context, 1, argtmp + 1) != 0)
		return 1;
	temp_a = internal_compute_2__sub_cf(context, argtmp[0], argtmp[1]);
	do_write_rflag_value(context, 0, &temp_a);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	if(crudasm_read_arg(context, 1, argtmp + 1) != 0)
		return 1;
	temp_a = internal_compute_2_sub(context, argtmp[0], argtmp[1]);
	tmp[0] = temp_a;
	temp_a = internal_compute_1__sign(context, tmp[0]);
	do_write_rflag_value(context, 7, &temp_a);
	temp_a = internal_compute_1__zero(context, tmp[0]);
	do_write_rflag_value(context, 6, &temp_a);
	temp_a = internal_compute_1__parity(context, tmp[0]);
	do_write_rflag_value(context, 2, &temp_a);
	return 0;  /* success */
}


static int crudasm_intel_execute__rol(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	struct crudasm_temp_node_t tmp[5];
	struct crudasm_temp_node_t argtmp[2];
	tmp[0].size = 1;
	tmp[1].size = 1;
	tmp[2].size = 1;
	tmp[3].size = 1/*BIT*/;
	tmp[4].size = 1/*BIT*/;
	argtmp[0].size = get_arg_size(context, 0);
	argtmp[1].size = get_arg_size(context, 1);
	if(crudasm_read_arg(context, 1, argtmp + 1) != 0)
		return 1;
	temp_a = argtmp[1];
	tmp[0] = temp_a;
	temp_a = internal_compute_2_shl(context, internal_compute_tr(context, 1, internal_get_argsize(context, 0)), internal_make_num(context, 1, 0, 3));
	tmp[1] = temp_a;
	temp_a = internal_compute_2_bitand(context, tmp[0], internal_compute_2_sub(context, tmp[1], internal_make_num(context, 1, 0, 1)));
	tmp[2] = temp_a;
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_2_rol(context, argtmp[0], tmp[2]);
	if(crudasm_write_arg(context, 0, &temp_a) != 0)
		return 1;
	temp_a = internal_compute_1__zero(context, tmp[2]);
	tmp[3] = temp_a;
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_3__quest(context, tmp[3], internal_get_flag(context, 0), internal_compute_1__lsb(context, argtmp[0]));
	tmp[4] = temp_a;
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_3__quest(context, tmp[3], internal_get_flag(context, 11), internal_compute_3__quest(context, internal_compute_2__eq(context, tmp[0], internal_make_num(context, 16, 0, 1)), internal_compute_2__logxor(context, internal_compute_1__sign(context, argtmp[0]), tmp[4]), internal_get_extern_undefined(context)));
	do_write_rflag_value(context, 11, &temp_a);
	temp_a = tmp[4];
	do_write_rflag_value(context, 0, &temp_a);
	return 0;  /* success */
}


static int crudasm_intel_execute__ror(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	struct crudasm_temp_node_t tmp[5];
	struct crudasm_temp_node_t argtmp[2];
	tmp[0].size = 1;
	tmp[1].size = 1;
	tmp[2].size = 1;
	tmp[3].size = 1/*BIT*/;
	tmp[4].size = 1/*BIT*/;
	argtmp[0].size = get_arg_size(context, 0);
	argtmp[1].size = get_arg_size(context, 1);
	if(crudasm_read_arg(context, 1, argtmp + 1) != 0)
		return 1;
	temp_a = argtmp[1];
	tmp[0] = temp_a;
	temp_a = internal_compute_2_shl(context, internal_compute_tr(context, 1, internal_get_argsize(context, 0)), internal_make_num(context, 1, 0, 3));
	tmp[1] = temp_a;
	temp_a = internal_compute_2_bitand(context, tmp[0], internal_compute_2_sub(context, tmp[1], internal_make_num(context, 1, 0, 1)));
	tmp[2] = temp_a;
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_2_ror(context, argtmp[0], tmp[2]);
	if(crudasm_write_arg(context, 0, &temp_a) != 0)
		return 1;
	temp_a = internal_compute_1__zero(context, tmp[2]);
	tmp[3] = temp_a;
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_3_quest(context, tmp[3], internal_get_flag(context, 0), internal_compute_1__sign(context, argtmp[0]));
	tmp[4] = temp_a;
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_3__quest(context, tmp[3], internal_get_flag(context, 11), internal_compute_3__quest(context, internal_compute_2__eq(context, tmp[0], internal_make_num(context, 1, 0, 1)), internal_compute_2__logxor(context, internal_compute_1__sign(context, argtmp[0]), internal_compute_1__sign2(context, argtmp[0])), internal_get_extern_undefined(context)));
	do_write_rflag_value(context, 11, &temp_a);
	temp_a = tmp[4];
	do_write_rflag_value(context, 0, &temp_a);
	return 0;  /* success */
}


static int crudasm_intel_execute_o16__rcl(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	struct crudasm_temp_node_t tmp[5];
	struct crudasm_temp_node_t argtmp[2];
	tmp[0].size = 1;
	tmp[1].size = 1;
	tmp[2].size = 1;
	tmp[3].size = 1;
	tmp[4].size = get_arg_size(context, 0);
	argtmp[0].size = get_arg_size(context, 0);
	argtmp[1].size = get_arg_size(context, 1);
	if(crudasm_read_arg(context, 1, argtmp + 1) != 0)
		return 1;
	temp_a = argtmp[1];
	tmp[0] = temp_a;
	temp_a = internal_compute_2_bitand(context, tmp[0], internal_make_num(context, 1, 0, 31));
	tmp[1] = temp_a;
	temp_a = internal_compute_2_add(context, internal_compute_2_shl(context, internal_compute_tr(context, 1, internal_get_argsize(context, 0)), internal_make_num(context, 1, 0, 3)), internal_make_num(context, 1, 0, 1));
	tmp[2] = temp_a;
	temp_a = internal_compute_2_umod(context, tmp[1], tmp[2]);
	tmp[3] = temp_a;
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = argtmp[0];
	tmp[4] = temp_a;
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_3_rcl(context, argtmp[0], tmp[3], internal_get_flag(context, 0));
	if(crudasm_write_arg(context, 0, &temp_a) != 0)
		return 1;
	temp_a = internal_compute_3__rcl_cf(context, tmp[4], tmp[3], internal_get_flag(context, 0));
	do_write_rflag_value(context, 0, &temp_a);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_3__quest(context, internal_compute_1__zero(context, tmp[3]), internal_get_flag(context, 11), internal_compute_3__quest(context, internal_compute_2__eq(context, tmp[0], internal_make_num(context, 1, 0, 1)), internal_compute_2__logxor(context, internal_compute_1__sign(context, argtmp[0]), internal_get_flag(context, 0)), internal_get_extern_undefined(context)));
	do_write_rflag_value(context, 11, &temp_a);
	return 0;  /* success */
}


static int crudasm_intel_execute_o32__rcl(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	struct crudasm_temp_node_t tmp[5];
	struct crudasm_temp_node_t argtmp[2];
	tmp[0].size = 1;
	tmp[1].size = 1;
	tmp[2].size = 1;
	tmp[3].size = 1;
	tmp[4].size = get_arg_size(context, 0);
	argtmp[0].size = get_arg_size(context, 0);
	argtmp[1].size = get_arg_size(context, 1);
	if(crudasm_read_arg(context, 1, argtmp + 1) != 0)
		return 1;
	temp_a = argtmp[1];
	tmp[0] = temp_a;
	temp_a = internal_compute_2_bitand(context, tmp[0], internal_make_num(context, 1, 0, 31));
	tmp[1] = temp_a;
	temp_a = internal_compute_2_add(context, internal_compute_2_shl(context, internal_compute_tr(context, 1, internal_get_argsize(context, 0)), internal_make_num(context, 1, 0, 3)), internal_make_num(context, 1, 0, 1));
	tmp[2] = temp_a;
	temp_a = internal_compute_2_umod(context, tmp[1], tmp[2]);
	tmp[3] = temp_a;
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = argtmp[0];
	tmp[4] = temp_a;
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_3_rcl(context, argtmp[0], tmp[3], internal_get_flag(context, 0));
	if(crudasm_write_arg(context, 0, &temp_a) != 0)
		return 1;
	temp_a = internal_compute_3__rcl_cf(context, tmp[4], tmp[3], internal_get_flag(context, 0));
	do_write_rflag_value(context, 0, &temp_a);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_3__quest(context, internal_compute_1__zero(context, tmp[3]), internal_get_flag(context, 11), internal_compute_3__quest(context, internal_compute_2__eq(context, tmp[0], internal_make_num(context, 1, 0, 1)), internal_compute_2__logxor(context, internal_compute_1__sign(context, argtmp[0]), internal_get_flag(context, 0)), internal_get_extern_undefined(context)));
	do_write_rflag_value(context, 11, &temp_a);
	return 0;  /* success */
}


static int crudasm_intel_execute_o64__rcl(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	struct crudasm_temp_node_t tmp[5];
	struct crudasm_temp_node_t argtmp[2];
	tmp[0].size = 1;
	tmp[1].size = 1;
	tmp[2].size = 1;
	tmp[3].size = 1;
	tmp[4].size = get_arg_size(context, 0);
	argtmp[0].size = get_arg_size(context, 0);
	argtmp[1].size = get_arg_size(context, 1);
	if(crudasm_read_arg(context, 1, argtmp + 1) != 0)
		return 1;
	temp_a = argtmp[1];
	tmp[0] = temp_a;
	temp_a = internal_compute_2_bitand(context, tmp[0], internal_make_num(context, 1, 0, 63));
	tmp[1] = temp_a;
	temp_a = internal_compute_2_add(context, internal_compute_2_shl(context, internal_compute_tr(context, 1, internal_get_argsize(context, 0)), internal_make_num(context, 1, 0, 3)), internal_make_num(context, 1, 0, 1));
	tmp[2] = temp_a;
	temp_a = internal_compute_2_umod(context, tmp[1], tmp[2]);
	tmp[3] = temp_a;
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = argtmp[0];
	tmp[4] = temp_a;
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_3_rcl(context, argtmp[0], tmp[3], internal_get_flag(context, 0));
	if(crudasm_write_arg(context, 0, &temp_a) != 0)
		return 1;
	temp_a = internal_compute_3__rcl_cf(context, tmp[4], tmp[3], internal_get_flag(context, 0));
	do_write_rflag_value(context, 0, &temp_a);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_3__quest(context, internal_compute_1__zero(context, tmp[3]), internal_get_flag(context, 11), internal_compute_3__quest(context, internal_compute_2__eq(context, tmp[0], internal_make_num(context, 1, 0, 1)), internal_compute_2__logxor(context, internal_compute_1__sign(context, argtmp[0]), internal_get_flag(context, 0)), internal_get_extern_undefined(context)));
	do_write_rflag_value(context, 11, &temp_a);
	return 0;  /* success */
}


static int crudasm_intel_execute_o16__rcr(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	struct crudasm_temp_node_t tmp[5];
	struct crudasm_temp_node_t argtmp[2];
	tmp[0].size = 1;
	tmp[1].size = 1;
	tmp[2].size = 1;
	tmp[3].size = 1;
	tmp[4].size = get_arg_size(context, 0);
	argtmp[0].size = get_arg_size(context, 0);
	argtmp[1].size = get_arg_size(context, 1);
	if(crudasm_read_arg(context, 1, argtmp + 1) != 0)
		return 1;
	temp_a = argtmp[1];
	tmp[0] = temp_a;
	temp_a = internal_compute_2_bitand(context, tmp[0], internal_make_num(context, 1, 0, 31));
	tmp[1] = temp_a;
	temp_a = internal_compute_2_add(context, internal_compute_2_shl(context, internal_compute_tr(context, 1, internal_get_argsize(context, 0)), internal_make_num(context, 1, 0, 3)), internal_make_num(context, 1, 0, 1));
	tmp[2] = temp_a;
	temp_a = internal_compute_2_umod(context, tmp[1], tmp[2]);
	tmp[3] = temp_a;
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_3__quest(context, internal_compute_1__zero(context, tmp[3]), internal_get_flag(context, 11), internal_compute_3__quest(context, internal_compute_2__eq(context, tmp[0], internal_make_num(context, 1, 0, 1)), internal_compute_2__logxor(context, internal_compute_1__sign(context, argtmp[0]), internal_get_flag(context, 0)), internal_get_extern_undefined(context)));
	do_write_rflag_value(context, 11, &temp_a);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = argtmp[0];
	tmp[4] = temp_a;
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_3_rcr(context, argtmp[0], tmp[3], internal_get_flag(context, 0));
	if(crudasm_write_arg(context, 0, &temp_a) != 0)
		return 1;
	temp_a = internal_compute_3__rcr_cf(context, tmp[4], tmp[3], internal_get_flag(context, 0));
	do_write_rflag_value(context, 0, &temp_a);
	return 0;  /* success */
}


static int crudasm_intel_execute_o32__rcr(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	struct crudasm_temp_node_t tmp[5];
	struct crudasm_temp_node_t argtmp[2];
	tmp[0].size = 1;
	tmp[1].size = 1;
	tmp[2].size = 1;
	tmp[3].size = 1;
	tmp[4].size = get_arg_size(context, 0);
	argtmp[0].size = get_arg_size(context, 0);
	argtmp[1].size = get_arg_size(context, 1);
	if(crudasm_read_arg(context, 1, argtmp + 1) != 0)
		return 1;
	temp_a = argtmp[1];
	tmp[0] = temp_a;
	temp_a = internal_compute_2_bitand(context, tmp[0], internal_make_num(context, 1, 0, 31));
	tmp[1] = temp_a;
	temp_a = internal_compute_2_add(context, internal_compute_2_shl(context, internal_compute_tr(context, 1, internal_get_argsize(context, 0)), internal_make_num(context, 1, 0, 3)), internal_make_num(context, 1, 0, 1));
	tmp[2] = temp_a;
	temp_a = internal_compute_2_umod(context, tmp[1], tmp[2]);
	tmp[3] = temp_a;
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_3__quest(context, internal_compute_1__zero(context, tmp[3]), internal_get_flag(context, 11), internal_compute_3__quest(context, internal_compute_2__eq(context, tmp[0], internal_make_num(context, 1, 0, 1)), internal_compute_2__logxor(context, internal_compute_1__sign(context, argtmp[0]), internal_get_flag(context, 0)), internal_get_extern_undefined(context)));
	do_write_rflag_value(context, 11, &temp_a);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = argtmp[0];
	tmp[4] = temp_a;
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_3_rcr(context, argtmp[0], tmp[3], internal_get_flag(context, 0));
	if(crudasm_write_arg(context, 0, &temp_a) != 0)
		return 1;
	temp_a = internal_compute_3__rcr_cf(context, tmp[4], tmp[3], internal_get_flag(context, 0));
	do_write_rflag_value(context, 0, &temp_a);
	return 0;  /* success */
}


static int crudasm_intel_execute_o64__rcr(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	struct crudasm_temp_node_t tmp[5];
	struct crudasm_temp_node_t argtmp[2];
	tmp[0].size = 1;
	tmp[1].size = 1;
	tmp[2].size = 1;
	tmp[3].size = 1;
	tmp[4].size = get_arg_size(context, 0);
	argtmp[0].size = get_arg_size(context, 0);
	argtmp[1].size = get_arg_size(context, 1);
	if(crudasm_read_arg(context, 1, argtmp + 1) != 0)
		return 1;
	temp_a = argtmp[1];
	tmp[0] = temp_a;
	temp_a = internal_compute_2_bitand(context, tmp[0], internal_make_num(context, 1, 0, 63));
	tmp[1] = temp_a;
	temp_a = internal_compute_2_add(context, internal_compute_2_shl(context, internal_compute_tr(context, 1, internal_get_argsize(context, 0)), internal_make_num(context, 1, 0, 3)), internal_make_num(context, 1, 0, 1));
	tmp[2] = temp_a;
	temp_a = internal_compute_2_umod(context, tmp[1], tmp[2]);
	tmp[3] = temp_a;
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_3__quest(context, internal_compute_1__zero(context, tmp[3]), internal_get_flag(context, 11), internal_compute_3__quest(context, internal_compute_2__eq(context, tmp[0], internal_make_num(context, 1, 0, 1)), internal_compute_2__logxor(context, internal_compute_1__sign(context, argtmp[0]), internal_get_flag(context, 0)), internal_get_extern_undefined(context)));
	do_write_rflag_value(context, 11, &temp_a);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = argtmp[0];
	tmp[4] = temp_a;
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_3_rcr(context, argtmp[0], tmp[3], internal_get_flag(context, 0));
	if(crudasm_write_arg(context, 0, &temp_a) != 0)
		return 1;
	temp_a = internal_compute_3__rcr_cf(context, tmp[4], tmp[3], internal_get_flag(context, 0));
	do_write_rflag_value(context, 0, &temp_a);
	return 0;  /* success */
}


static int crudasm_intel_execute_o16__shl(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	struct crudasm_temp_node_t tmp[3];
	struct crudasm_temp_node_t argtmp[2];
	tmp[0].size = 1;
	tmp[1].size = 1/*BIT*/;
	tmp[2].size = get_arg_size(context, 0);
	argtmp[0].size = get_arg_size(context, 0);
	argtmp[1].size = get_arg_size(context, 1);
	if(crudasm_read_arg(context, 1, argtmp + 1) != 0)
		return 1;
	temp_a = internal_compute_2_bitand(context, argtmp[1], internal_make_num(context, 1, 0, 31));
	tmp[0] = temp_a;
	temp_a = internal_compute_1__zero(context, tmp[0]);
	tmp[1] = temp_a;
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = argtmp[0];
	tmp[2] = temp_a;
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_2_shl(context, argtmp[0], tmp[0]);
	if(crudasm_write_arg(context, 0, &temp_a) != 0)
		return 1;
	temp_a = internal_compute_3__quest(context, tmp[1], internal_get_flag(context, 0), internal_compute_2__shl_cf(context, tmp[2], tmp[0]));
	do_write_rflag_value(context, 0, &temp_a);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_3__quest(context, internal_compute_2__eq(context, tmp[0], internal_make_num(context, 16, 0, 1)), internal_compute_2__logxor(context, internal_compute_1__sign(context, argtmp[0]), internal_get_flag(context, 0)), internal_compute_3__quest(context, tmp[1], internal_get_flag(context, 11), internal_get_extern_undefined(context)));
	do_write_rflag_value(context, 11, &temp_a);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_3__quest(context, tmp[1], internal_get_flag(context, 7), internal_compute_1__sign(context, argtmp[0]));
	do_write_rflag_value(context, 7, &temp_a);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_3__quest(context, tmp[1], internal_get_flag(context, 6), internal_compute_1__zero(context, argtmp[0]));
	do_write_rflag_value(context, 6, &temp_a);
	temp_a = internal_compute_3__quest(context, tmp[1], internal_get_flag(context, 4), internal_get_extern_undefined(context));
	do_write_rflag_value(context, 4, &temp_a);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_3__quest(context, tmp[1], internal_get_flag(context, 2), internal_compute_1__parity(context, argtmp[0]));
	do_write_rflag_value(context, 2, &temp_a);
	return 0;  /* success */
}


static int crudasm_intel_execute_o32__shl(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	struct crudasm_temp_node_t tmp[3];
	struct crudasm_temp_node_t argtmp[2];
	tmp[0].size = 1;
	tmp[1].size = 1/*BIT*/;
	tmp[2].size = get_arg_size(context, 0);
	argtmp[0].size = get_arg_size(context, 0);
	argtmp[1].size = get_arg_size(context, 1);
	if(crudasm_read_arg(context, 1, argtmp + 1) != 0)
		return 1;
	temp_a = internal_compute_2_bitand(context, argtmp[1], internal_make_num(context, 1, 0, 31));
	tmp[0] = temp_a;
	temp_a = internal_compute_1__zero(context, tmp[0]);
	tmp[1] = temp_a;
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = argtmp[0];
	tmp[2] = temp_a;
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_2_shl(context, argtmp[0], tmp[0]);
	if(crudasm_write_arg(context, 0, &temp_a) != 0)
		return 1;
	temp_a = internal_compute_3__quest(context, tmp[1], internal_get_flag(context, 0), internal_compute_2__shl_cf(context, tmp[2], tmp[0]));
	do_write_rflag_value(context, 0, &temp_a);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_3__quest(context, internal_compute_2__eq(context, tmp[0], internal_make_num(context, 16, 0, 1)), internal_compute_2__logxor(context, internal_compute_1__sign(context, argtmp[0]), internal_get_flag(context, 0)), internal_compute_3__quest(context, tmp[1], internal_get_flag(context, 11), internal_get_extern_undefined(context)));
	do_write_rflag_value(context, 11, &temp_a);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_3__quest(context, tmp[1], internal_get_flag(context, 7), internal_compute_1__sign(context, argtmp[0]));
	do_write_rflag_value(context, 7, &temp_a);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_3__quest(context, tmp[1], internal_get_flag(context, 6), internal_compute_1__zero(context, argtmp[0]));
	do_write_rflag_value(context, 6, &temp_a);
	temp_a = internal_compute_3__quest(context, tmp[1], internal_get_flag(context, 4), internal_get_extern_undefined(context));
	do_write_rflag_value(context, 4, &temp_a);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_3__quest(context, tmp[1], internal_get_flag(context, 2), internal_compute_1__parity(context, argtmp[0]));
	do_write_rflag_value(context, 2, &temp_a);
	return 0;  /* success */
}


static int crudasm_intel_execute_o64__shl(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	struct crudasm_temp_node_t tmp[3];
	struct crudasm_temp_node_t argtmp[2];
	tmp[0].size = 1;
	tmp[1].size = 1/*BIT*/;
	tmp[2].size = get_arg_size(context, 0);
	argtmp[0].size = get_arg_size(context, 0);
	argtmp[1].size = get_arg_size(context, 1);
	if(crudasm_read_arg(context, 1, argtmp + 1) != 0)
		return 1;
	temp_a = internal_compute_2_bitand(context, argtmp[1], internal_make_num(context, 1, 0, 63));
	tmp[0] = temp_a;
	temp_a = internal_compute_1__zero(context, tmp[0]);
	tmp[1] = temp_a;
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = argtmp[0];
	tmp[2] = temp_a;
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_2_shl(context, argtmp[0], tmp[0]);
	if(crudasm_write_arg(context, 0, &temp_a) != 0)
		return 1;
	temp_a = internal_compute_3__quest(context, tmp[1], internal_get_flag(context, 0), internal_compute_2__shl_cf(context, tmp[2], tmp[0]));
	do_write_rflag_value(context, 0, &temp_a);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_3__quest(context, internal_compute_2__eq(context, tmp[0], internal_make_num(context, 16, 0, 1)), internal_compute_2__logxor(context, internal_compute_1__sign(context, argtmp[0]), internal_get_flag(context, 0)), internal_compute_3__quest(context, tmp[1], internal_get_flag(context, 11), internal_get_extern_undefined(context)));
	do_write_rflag_value(context, 11, &temp_a);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_3__quest(context, tmp[1], internal_get_flag(context, 7), internal_compute_1__sign(context, argtmp[0]));
	do_write_rflag_value(context, 7, &temp_a);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_3__quest(context, tmp[1], internal_get_flag(context, 6), internal_compute_1__zero(context, argtmp[0]));
	do_write_rflag_value(context, 6, &temp_a);
	temp_a = internal_compute_3__quest(context, tmp[1], internal_get_flag(context, 4), internal_get_extern_undefined(context));
	do_write_rflag_value(context, 4, &temp_a);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_3__quest(context, tmp[1], internal_get_flag(context, 2), internal_compute_1__parity(context, argtmp[0]));
	do_write_rflag_value(context, 2, &temp_a);
	return 0;  /* success */
}


static int crudasm_intel_execute_o16__shr(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	struct crudasm_temp_node_t tmp[3];
	struct crudasm_temp_node_t argtmp[2];
	tmp[0].size = 1;
	tmp[1].size = 1/*BIT*/;
	tmp[2].size = get_arg_size(context, 0);
	argtmp[0].size = get_arg_size(context, 0);
	argtmp[1].size = get_arg_size(context, 1);
	if(crudasm_read_arg(context, 1, argtmp + 1) != 0)
		return 1;
	temp_a = internal_compute_2_bitand(context, argtmp[1], internal_make_num(context, 1, 0, 31));
	tmp[0] = temp_a;
	temp_a = internal_compute_1__zero(context, tmp[0]);
	tmp[1] = temp_a;
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = argtmp[0];
	tmp[2] = temp_a;
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_2_shru(context, argtmp[0], tmp[0]);
	if(crudasm_write_arg(context, 0, &temp_a) != 0)
		return 1;
	temp_a = internal_compute_3__quest(context, tmp[1], internal_get_flag(context, 0), internal_compute_2__shr_cf(context, tmp[2], tmp[0]));
	do_write_rflag_value(context, 0, &temp_a);
	temp_a = internal_compute_3__quest(context, internal_compute_2__eq(context, tmp[0], internal_make_num(context, 16, 0, 1)), internal_compute_1__sign(context, tmp[2]), internal_compute_3__quest(context, tmp[1], internal_get_flag(context, 11), internal_get_extern_undefined(context)));
	do_write_rflag_value(context, 11, &temp_a);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_3__quest(context, tmp[1], internal_get_flag(context, 7), internal_compute_1__sign(context, argtmp[0]));
	do_write_rflag_value(context, 7, &temp_a);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_3__quest(context, tmp[1], internal_get_flag(context, 6), internal_compute_1__zero(context, argtmp[0]));
	do_write_rflag_value(context, 6, &temp_a);
	temp_a = internal_compute_3__quest(context, tmp[1], internal_get_flag(context, 4), internal_get_extern_undefined(context));
	do_write_rflag_value(context, 4, &temp_a);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_3__quest(context, tmp[1], internal_get_flag(context, 2), internal_compute_1__parity(context, argtmp[0]));
	do_write_rflag_value(context, 2, &temp_a);
	return 0;  /* success */
}


static int crudasm_intel_execute_o32__shr(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	struct crudasm_temp_node_t tmp[3];
	struct crudasm_temp_node_t argtmp[2];
	tmp[0].size = 1;
	tmp[1].size = 1/*BIT*/;
	tmp[2].size = get_arg_size(context, 0);
	argtmp[0].size = get_arg_size(context, 0);
	argtmp[1].size = get_arg_size(context, 1);
	if(crudasm_read_arg(context, 1, argtmp + 1) != 0)
		return 1;
	temp_a = internal_compute_2_bitand(context, argtmp[1], internal_make_num(context, 1, 0, 31));
	tmp[0] = temp_a;
	temp_a = internal_compute_1__zero(context, tmp[0]);
	tmp[1] = temp_a;
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = argtmp[0];
	tmp[2] = temp_a;
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_2_shru(context, argtmp[0], tmp[0]);
	if(crudasm_write_arg(context, 0, &temp_a) != 0)
		return 1;
	temp_a = internal_compute_3__quest(context, tmp[1], internal_get_flag(context, 0), internal_compute_2__shr_cf(context, tmp[2], tmp[0]));
	do_write_rflag_value(context, 0, &temp_a);
	temp_a = internal_compute_3__quest(context, internal_compute_2__eq(context, tmp[0], internal_make_num(context, 16, 0, 1)), internal_compute_1__sign(context, tmp[2]), internal_compute_3__quest(context, tmp[1], internal_get_flag(context, 11), internal_get_extern_undefined(context)));
	do_write_rflag_value(context, 11, &temp_a);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_3__quest(context, tmp[1], internal_get_flag(context, 7), internal_compute_1__sign(context, argtmp[0]));
	do_write_rflag_value(context, 7, &temp_a);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_3__quest(context, tmp[1], internal_get_flag(context, 6), internal_compute_1__zero(context, argtmp[0]));
	do_write_rflag_value(context, 6, &temp_a);
	temp_a = internal_compute_3__quest(context, tmp[1], internal_get_flag(context, 4), internal_get_extern_undefined(context));
	do_write_rflag_value(context, 4, &temp_a);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_3__quest(context, tmp[1], internal_get_flag(context, 2), internal_compute_1__parity(context, argtmp[0]));
	do_write_rflag_value(context, 2, &temp_a);
	return 0;  /* success */
}


static int crudasm_intel_execute_o64__shr(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	struct crudasm_temp_node_t tmp[3];
	struct crudasm_temp_node_t argtmp[2];
	tmp[0].size = 1;
	tmp[1].size = 1/*BIT*/;
	tmp[2].size = get_arg_size(context, 0);
	argtmp[0].size = get_arg_size(context, 0);
	argtmp[1].size = get_arg_size(context, 1);
	if(crudasm_read_arg(context, 1, argtmp + 1) != 0)
		return 1;
	temp_a = internal_compute_2_bitand(context, argtmp[1], internal_make_num(context, 1, 0, 63));
	tmp[0] = temp_a;
	temp_a = internal_compute_1__zero(context, tmp[0]);
	tmp[1] = temp_a;
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = argtmp[0];
	tmp[2] = temp_a;
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_2_shru(context, argtmp[0], tmp[0]);
	if(crudasm_write_arg(context, 0, &temp_a) != 0)
		return 1;
	temp_a = internal_compute_3__quest(context, tmp[1], internal_get_flag(context, 0), internal_compute_2__shr_cf(context, tmp[2], tmp[0]));
	do_write_rflag_value(context, 0, &temp_a);
	temp_a = internal_compute_3__quest(context, internal_compute_2__eq(context, tmp[0], internal_make_num(context, 16, 0, 1)), internal_compute_1__sign(context, tmp[2]), internal_compute_3__quest(context, tmp[1], internal_get_flag(context, 11), internal_get_extern_undefined(context)));
	do_write_rflag_value(context, 11, &temp_a);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_3__quest(context, tmp[1], internal_get_flag(context, 7), internal_compute_1__sign(context, argtmp[0]));
	do_write_rflag_value(context, 7, &temp_a);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_3__quest(context, tmp[1], internal_get_flag(context, 6), internal_compute_1__zero(context, argtmp[0]));
	do_write_rflag_value(context, 6, &temp_a);
	temp_a = internal_compute_3__quest(context, tmp[1], internal_get_flag(context, 4), internal_get_extern_undefined(context));
	do_write_rflag_value(context, 4, &temp_a);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_3__quest(context, tmp[1], internal_get_flag(context, 2), internal_compute_1__parity(context, argtmp[0]));
	do_write_rflag_value(context, 2, &temp_a);
	return 0;  /* success */
}


static int crudasm_intel_execute_o16___sal(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	struct crudasm_temp_node_t tmp[3];
	struct crudasm_temp_node_t argtmp[2];
	tmp[0].size = 1;
	tmp[1].size = 1/*BIT*/;
	tmp[2].size = get_arg_size(context, 0);
	argtmp[0].size = get_arg_size(context, 0);
	argtmp[1].size = get_arg_size(context, 1);
	if(crudasm_read_arg(context, 1, argtmp + 1) != 0)
		return 1;
	temp_a = internal_compute_2_bitand(context, argtmp[1], internal_make_num(context, 1, 0, 31));
	tmp[0] = temp_a;
	temp_a = internal_compute_1__zero(context, tmp[0]);
	tmp[1] = temp_a;
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = argtmp[0];
	tmp[2] = temp_a;
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_2_shl(context, argtmp[0], tmp[0]);
	if(crudasm_write_arg(context, 0, &temp_a) != 0)
		return 1;
	temp_a = internal_compute_3__quest(context, tmp[1], internal_get_flag(context, 0), internal_compute_2__shl_cf(context, tmp[2], tmp[0]));
	do_write_rflag_value(context, 0, &temp_a);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_3__quest(context, internal_compute_2__eq(context, tmp[0], internal_make_num(context, 1, 0, 1)), internal_compute_2__logxor(context, internal_compute_1__sign(context, argtmp[0]), internal_get_flag(context, 0)), internal_compute_3__quest(context, tmp[1], internal_get_flag(context, 11), internal_get_extern_undefined(context)));
	do_write_rflag_value(context, 11, &temp_a);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_3__quest(context, tmp[1], internal_get_flag(context, 7), internal_compute_1__sign(context, argtmp[0]));
	do_write_rflag_value(context, 7, &temp_a);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_3__quest(context, tmp[1], internal_get_flag(context, 6), internal_compute_1__zero(context, argtmp[0]));
	do_write_rflag_value(context, 6, &temp_a);
	temp_a = internal_compute_3__quest(context, tmp[1], internal_get_flag(context, 4), internal_get_extern_undefined(context));
	do_write_rflag_value(context, 4, &temp_a);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_3__quest(context, tmp[1], internal_get_flag(context, 2), internal_compute_1__parity(context, argtmp[0]));
	do_write_rflag_value(context, 2, &temp_a);
	return 0;  /* success */
}


static int crudasm_intel_execute_o32___sal(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	struct crudasm_temp_node_t tmp[3];
	struct crudasm_temp_node_t argtmp[2];
	tmp[0].size = 1;
	tmp[1].size = 1/*BIT*/;
	tmp[2].size = get_arg_size(context, 0);
	argtmp[0].size = get_arg_size(context, 0);
	argtmp[1].size = get_arg_size(context, 1);
	if(crudasm_read_arg(context, 1, argtmp + 1) != 0)
		return 1;
	temp_a = internal_compute_2_bitand(context, argtmp[1], internal_make_num(context, 1, 0, 31));
	tmp[0] = temp_a;
	temp_a = internal_compute_1__zero(context, tmp[0]);
	tmp[1] = temp_a;
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = argtmp[0];
	tmp[2] = temp_a;
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_2_shl(context, argtmp[0], tmp[0]);
	if(crudasm_write_arg(context, 0, &temp_a) != 0)
		return 1;
	temp_a = internal_compute_3__quest(context, tmp[1], internal_get_flag(context, 0), internal_compute_2__shl_cf(context, tmp[2], tmp[0]));
	do_write_rflag_value(context, 0, &temp_a);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_3__quest(context, internal_compute_2__eq(context, tmp[0], internal_make_num(context, 1, 0, 1)), internal_compute_2__logxor(context, internal_compute_1__sign(context, argtmp[0]), internal_get_flag(context, 0)), internal_compute_3__quest(context, tmp[1], internal_get_flag(context, 11), internal_get_extern_undefined(context)));
	do_write_rflag_value(context, 11, &temp_a);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_3__quest(context, tmp[1], internal_get_flag(context, 7), internal_compute_1__sign(context, argtmp[0]));
	do_write_rflag_value(context, 7, &temp_a);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_3__quest(context, tmp[1], internal_get_flag(context, 6), internal_compute_1__zero(context, argtmp[0]));
	do_write_rflag_value(context, 6, &temp_a);
	temp_a = internal_compute_3__quest(context, tmp[1], internal_get_flag(context, 4), internal_get_extern_undefined(context));
	do_write_rflag_value(context, 4, &temp_a);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_3__quest(context, tmp[1], internal_get_flag(context, 2), internal_compute_1__parity(context, argtmp[0]));
	do_write_rflag_value(context, 2, &temp_a);
	return 0;  /* success */
}


static int crudasm_intel_execute_o64___sal(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	struct crudasm_temp_node_t tmp[3];
	struct crudasm_temp_node_t argtmp[2];
	tmp[0].size = 1;
	tmp[1].size = 1/*BIT*/;
	tmp[2].size = get_arg_size(context, 0);
	argtmp[0].size = get_arg_size(context, 0);
	argtmp[1].size = get_arg_size(context, 1);
	if(crudasm_read_arg(context, 1, argtmp + 1) != 0)
		return 1;
	temp_a = internal_compute_2_bitand(context, argtmp[1], internal_make_num(context, 1, 0, 63));
	tmp[0] = temp_a;
	temp_a = internal_compute_1__zero(context, tmp[0]);
	tmp[1] = temp_a;
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = argtmp[0];
	tmp[2] = temp_a;
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_2_shl(context, argtmp[0], tmp[0]);
	if(crudasm_write_arg(context, 0, &temp_a) != 0)
		return 1;
	temp_a = internal_compute_3__quest(context, tmp[1], internal_get_flag(context, 0), internal_compute_2__shl_cf(context, tmp[2], tmp[0]));
	do_write_rflag_value(context, 0, &temp_a);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_3__quest(context, internal_compute_2__eq(context, tmp[0], internal_make_num(context, 1, 0, 1)), internal_compute_2__logxor(context, internal_compute_1__sign(context, argtmp[0]), internal_get_flag(context, 0)), internal_compute_3__quest(context, tmp[1], internal_get_flag(context, 11), internal_get_extern_undefined(context)));
	do_write_rflag_value(context, 11, &temp_a);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_3__quest(context, tmp[1], internal_get_flag(context, 7), internal_compute_1__sign(context, argtmp[0]));
	do_write_rflag_value(context, 7, &temp_a);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_3__quest(context, tmp[1], internal_get_flag(context, 6), internal_compute_1__zero(context, argtmp[0]));
	do_write_rflag_value(context, 6, &temp_a);
	temp_a = internal_compute_3__quest(context, tmp[1], internal_get_flag(context, 4), internal_get_extern_undefined(context));
	do_write_rflag_value(context, 4, &temp_a);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_3__quest(context, tmp[1], internal_get_flag(context, 2), internal_compute_1__parity(context, argtmp[0]));
	do_write_rflag_value(context, 2, &temp_a);
	return 0;  /* success */
}


static int crudasm_intel_execute_o16__sar(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	struct crudasm_temp_node_t tmp[3];
	struct crudasm_temp_node_t argtmp[2];
	tmp[0].size = 1;
	tmp[1].size = 1/*BIT*/;
	tmp[2].size = get_arg_size(context, 0);
	argtmp[0].size = get_arg_size(context, 0);
	argtmp[1].size = get_arg_size(context, 1);
	if(crudasm_read_arg(context, 1, argtmp + 1) != 0)
		return 1;
	temp_a = internal_compute_2_bitand(context, argtmp[1], internal_make_num(context, 1, 0, 31));
	tmp[0] = temp_a;
	temp_a = internal_compute_1__zero(context, tmp[0]);
	tmp[1] = temp_a;
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = argtmp[0];
	tmp[2] = temp_a;
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_2_shrs(context, argtmp[0], tmp[0]);
	if(crudasm_write_arg(context, 0, &temp_a) != 0)
		return 1;
	temp_a = internal_compute_3__quest(context, tmp[1], internal_get_flag(context, 0), internal_compute_2__sar_cf(context, tmp[2], tmp[0]));
	do_write_rflag_value(context, 0, &temp_a);
	temp_a = internal_compute_3__quest(context, internal_compute_2__eq(context, tmp[0], internal_make_num(context, 16, 0, 1)), internal_compute_1__sign(context, tmp[2]), internal_compute_3__quest(context, tmp[1], internal_get_flag(context, 11), internal_get_extern_undefined(context)));
	do_write_rflag_value(context, 11, &temp_a);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_3__quest(context, tmp[1], internal_get_flag(context, 7), internal_compute_1__sign(context, argtmp[0]));
	do_write_rflag_value(context, 7, &temp_a);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_3__quest(context, tmp[1], internal_get_flag(context, 6), internal_compute_1__zero(context, argtmp[0]));
	do_write_rflag_value(context, 6, &temp_a);
	temp_a = internal_compute_3__quest(context, tmp[1], internal_get_flag(context, 4), internal_get_extern_undefined(context));
	do_write_rflag_value(context, 4, &temp_a);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_3__quest(context, tmp[1], internal_get_flag(context, 2), internal_compute_1__parity(context, argtmp[0]));
	do_write_rflag_value(context, 2, &temp_a);
	return 0;  /* success */
}


static int crudasm_intel_execute_o32__sar(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	struct crudasm_temp_node_t tmp[3];
	struct crudasm_temp_node_t argtmp[2];
	tmp[0].size = 1;
	tmp[1].size = 1/*BIT*/;
	tmp[2].size = get_arg_size(context, 0);
	argtmp[0].size = get_arg_size(context, 0);
	argtmp[1].size = get_arg_size(context, 1);
	if(crudasm_read_arg(context, 1, argtmp + 1) != 0)
		return 1;
	temp_a = internal_compute_2_bitand(context, argtmp[1], internal_make_num(context, 1, 0, 31));
	tmp[0] = temp_a;
	temp_a = internal_compute_1__zero(context, tmp[0]);
	tmp[1] = temp_a;
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = argtmp[0];
	tmp[2] = temp_a;
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_2_shrs(context, argtmp[0], tmp[0]);
	if(crudasm_write_arg(context, 0, &temp_a) != 0)
		return 1;
	temp_a = internal_compute_3__quest(context, tmp[1], internal_get_flag(context, 0), internal_compute_2__sar_cf(context, tmp[2], tmp[0]));
	do_write_rflag_value(context, 0, &temp_a);
	temp_a = internal_compute_3__quest(context, internal_compute_2__eq(context, tmp[0], internal_make_num(context, 16, 0, 1)), internal_compute_1__sign(context, tmp[2]), internal_compute_3__quest(context, tmp[1], internal_get_flag(context, 11), internal_get_extern_undefined(context)));
	do_write_rflag_value(context, 11, &temp_a);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_3__quest(context, tmp[1], internal_get_flag(context, 7), internal_compute_1__sign(context, argtmp[0]));
	do_write_rflag_value(context, 7, &temp_a);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_3__quest(context, tmp[1], internal_get_flag(context, 6), internal_compute_1__zero(context, argtmp[0]));
	do_write_rflag_value(context, 6, &temp_a);
	temp_a = internal_compute_3__quest(context, tmp[1], internal_get_flag(context, 4), internal_get_extern_undefined(context));
	do_write_rflag_value(context, 4, &temp_a);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_3__quest(context, tmp[1], internal_get_flag(context, 2), internal_compute_1__parity(context, argtmp[0]));
	do_write_rflag_value(context, 2, &temp_a);
	return 0;  /* success */
}


static int crudasm_intel_execute_o64__sar(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	struct crudasm_temp_node_t tmp[3];
	struct crudasm_temp_node_t argtmp[2];
	tmp[0].size = 1;
	tmp[1].size = 1/*BIT*/;
	tmp[2].size = get_arg_size(context, 0);
	argtmp[0].size = get_arg_size(context, 0);
	argtmp[1].size = get_arg_size(context, 1);
	if(crudasm_read_arg(context, 1, argtmp + 1) != 0)
		return 1;
	temp_a = internal_compute_2_bitand(context, argtmp[1], internal_make_num(context, 1, 0, 63));
	tmp[0] = temp_a;
	temp_a = internal_compute_1__zero(context, tmp[0]);
	tmp[1] = temp_a;
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = argtmp[0];
	tmp[2] = temp_a;
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_2_shrs(context, argtmp[0], tmp[0]);
	if(crudasm_write_arg(context, 0, &temp_a) != 0)
		return 1;
	temp_a = internal_compute_3__quest(context, tmp[1], internal_get_flag(context, 0), internal_compute_2__sar_cf(context, tmp[2], tmp[0]));
	do_write_rflag_value(context, 0, &temp_a);
	temp_a = internal_compute_3__quest(context, internal_compute_2__eq(context, tmp[0], internal_make_num(context, 16, 0, 1)), internal_compute_1__sign(context, tmp[2]), internal_compute_3__quest(context, tmp[1], internal_get_flag(context, 11), internal_get_extern_undefined(context)));
	do_write_rflag_value(context, 11, &temp_a);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_3__quest(context, tmp[1], internal_get_flag(context, 7), internal_compute_1__sign(context, argtmp[0]));
	do_write_rflag_value(context, 7, &temp_a);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_3__quest(context, tmp[1], internal_get_flag(context, 6), internal_compute_1__zero(context, argtmp[0]));
	do_write_rflag_value(context, 6, &temp_a);
	temp_a = internal_compute_3__quest(context, tmp[1], internal_get_flag(context, 4), internal_get_extern_undefined(context));
	do_write_rflag_value(context, 4, &temp_a);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_3__quest(context, tmp[1], internal_get_flag(context, 2), internal_compute_1__parity(context, argtmp[0]));
	do_write_rflag_value(context, 2, &temp_a);
	return 0;  /* success */
}


static int crudasm_intel_execute__mov(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	struct crudasm_temp_node_t argtmp[2];
	argtmp[0].size = get_arg_size(context, 0);
	argtmp[1].size = get_arg_size(context, 1);
	if(crudasm_read_arg(context, 1, argtmp + 1) != 0)
		return 1;
	temp_a = argtmp[1];
	if(crudasm_write_arg(context, 0, &temp_a) != 0)
		return 1;
	return 0;  /* success */
}


static int crudasm_intel_execute_a16___ins(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	struct crudasm_temp_node_t tmp[1];
	struct crudasm_temp_node_t argtmp[2];
	tmp[0].size = 2;
	argtmp[0].size = get_arg_size(context, 0);
	argtmp[1].size = get_arg_size(context, 1);
	if(crudasm_read_arg(context, 1, argtmp + 1) != 0)
		return 1;
	temp_a = argtmp[1];
	if(do_inport(context, &temp_a, get_arg_size(context, 0)) != 0)
		return 1;
	if(crudasm_write_arg(context, 0, &temp_a) != 0)
		return 1;
	temp_a = internal_compute_3_quest(context, internal_compute_1__lognot(context, internal_get_flag(context, 10)), internal_compute_tr(context, 2, internal_get_argsize(context, 0)), internal_compute_1_neg(context, internal_compute_tr(context, 2, internal_get_argsize(context, 0))));
	tmp[0] = temp_a;
	temp_a = internal_compute_2_add(context, internal_get_reg16(context, 7), tmp[0]);
	if(write_gr16_value(context, 7, &temp_a))
		return 1;
	return 0;  /* success */
}


static int crudasm_intel_execute_a32___ins(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	struct crudasm_temp_node_t tmp[1];
	struct crudasm_temp_node_t argtmp[2];
	tmp[0].size = 4;
	argtmp[0].size = get_arg_size(context, 0);
	argtmp[1].size = get_arg_size(context, 1);
	if(crudasm_read_arg(context, 1, argtmp + 1) != 0)
		return 1;
	temp_a = argtmp[1];
	if(do_inport(context, &temp_a, get_arg_size(context, 0)) != 0)
		return 1;
	if(crudasm_write_arg(context, 0, &temp_a) != 0)
		return 1;
	temp_a = internal_compute_3_quest(context, internal_compute_1__lognot(context, internal_get_flag(context, 10)), internal_compute_tr(context, 4, internal_get_argsize(context, 0)), internal_compute_1_neg(context, internal_compute_tr(context, 4, internal_get_argsize(context, 0))));
	tmp[0] = temp_a;
	temp_a = internal_compute_2_add(context, internal_get_reg32(context, 7), tmp[0]);
	if(write_gr32_value(context, 7, &temp_a))
		return 1;
	return 0;  /* success */
}


static int crudasm_intel_execute_a64___ins(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	struct crudasm_temp_node_t tmp[1];
	struct crudasm_temp_node_t argtmp[2];
	tmp[0].size = 8;
	argtmp[0].size = get_arg_size(context, 0);
	argtmp[1].size = get_arg_size(context, 1);
	if(crudasm_read_arg(context, 1, argtmp + 1) != 0)
		return 1;
	temp_a = argtmp[1];
	if(do_inport(context, &temp_a, get_arg_size(context, 0)) != 0)
		return 1;
	if(crudasm_write_arg(context, 0, &temp_a) != 0)
		return 1;
	temp_a = internal_compute_3_quest(context, internal_compute_1__lognot(context, internal_get_flag(context, 10)), internal_compute_tr(context, 8, internal_get_argsize(context, 0)), internal_compute_1_neg(context, internal_compute_tr(context, 8, internal_get_argsize(context, 0))));
	tmp[0] = temp_a;
	temp_a = internal_compute_2_add(context, internal_get_reg64(context, 7), tmp[0]);
	if(write_gr64_value(context, 7, &temp_a))
		return 1;
	return 0;  /* success */
}


static int crudasm_intel_execute_a16___outs(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	struct crudasm_temp_node_t tmp[1];
	struct crudasm_temp_node_t argtmp[2];
	tmp[0].size = 2;
	argtmp[0].size = get_arg_size(context, 0);
	argtmp[1].size = get_arg_size(context, 1);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	if(crudasm_read_arg(context, 1, argtmp + 1) != 0)
		return 1;
	if(special_do_outport(context, argtmp[0], argtmp[1]) != 0)
		return 1;
	temp_a = internal_compute_3_quest(context, internal_compute_1__lognot(context, internal_get_flag(context, 10)), internal_compute_tr(context, 2, internal_get_argsize(context, 1)), internal_compute_1_neg(context, internal_compute_tr(context, 2, internal_get_argsize(context, 1))));
	tmp[0] = temp_a;
	temp_a = internal_compute_2_add(context, internal_get_reg16(context, 6), tmp[0]);
	if(write_gr16_value(context, 6, &temp_a))
		return 1;
	return 0;  /* success */
}


static int crudasm_intel_execute_a32___outs(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	struct crudasm_temp_node_t tmp[1];
	struct crudasm_temp_node_t argtmp[2];
	tmp[0].size = 4;
	argtmp[0].size = get_arg_size(context, 0);
	argtmp[1].size = get_arg_size(context, 1);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	if(crudasm_read_arg(context, 1, argtmp + 1) != 0)
		return 1;
	if(special_do_outport(context, argtmp[0], argtmp[1]) != 0)
		return 1;
	temp_a = internal_compute_3_quest(context, internal_compute_1__lognot(context, internal_get_flag(context, 10)), internal_compute_tr(context, 4, internal_get_argsize(context, 1)), internal_compute_1_neg(context, internal_compute_tr(context, 4, internal_get_argsize(context, 1))));
	tmp[0] = temp_a;
	temp_a = internal_compute_2_add(context, internal_get_reg32(context, 6), tmp[0]);
	if(write_gr32_value(context, 6, &temp_a))
		return 1;
	return 0;  /* success */
}


static int crudasm_intel_execute_a64___outs(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	struct crudasm_temp_node_t tmp[1];
	struct crudasm_temp_node_t argtmp[2];
	tmp[0].size = 8;
	argtmp[0].size = get_arg_size(context, 0);
	argtmp[1].size = get_arg_size(context, 1);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	if(crudasm_read_arg(context, 1, argtmp + 1) != 0)
		return 1;
	if(special_do_outport(context, argtmp[0], argtmp[1]) != 0)
		return 1;
	temp_a = internal_compute_3_quest(context, internal_compute_1__lognot(context, internal_get_flag(context, 10)), internal_compute_tr(context, 8, internal_get_argsize(context, 1)), internal_compute_1_neg(context, internal_compute_tr(context, 8, internal_get_argsize(context, 1))));
	tmp[0] = temp_a;
	temp_a = internal_compute_2_add(context, internal_get_reg64(context, 6), tmp[0]);
	if(write_gr64_value(context, 6, &temp_a))
		return 1;
	return 0;  /* success */
}


static int crudasm_intel_execute_o16__les(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	struct crudasm_temp_node_t argtmp[2];
	argtmp[0].size = get_arg_size(context, 0);
	argtmp[1].size = get_arg_size(context, 1);
	if(crudasm_read_arg(context, 1, argtmp + 1) != 0)
		return 1;
	temp_a = internal_compute_1_seg16(context, argtmp[1]);
	if(write_sr_value(context, 0, &temp_a))
		return 1;
	if(crudasm_read_arg(context, 1, argtmp + 1) != 0)
		return 1;
	temp_a = internal_compute_1_ofs16(context, argtmp[1]);
	if(crudasm_write_arg(context, 0, &temp_a) != 0)
		return 1;
	return 0;  /* success */
}


static int crudasm_intel_execute_o32__les(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	struct crudasm_temp_node_t argtmp[2];
	argtmp[0].size = get_arg_size(context, 0);
	argtmp[1].size = get_arg_size(context, 1);
	if(crudasm_read_arg(context, 1, argtmp + 1) != 0)
		return 1;
	temp_a = internal_compute_1_seg32(context, argtmp[1]);
	if(write_sr_value(context, 0, &temp_a))
		return 1;
	if(crudasm_read_arg(context, 1, argtmp + 1) != 0)
		return 1;
	temp_a = internal_compute_1_ofs32(context, argtmp[1]);
	if(crudasm_write_arg(context, 0, &temp_a) != 0)
		return 1;
	return 0;  /* success */
}


static int crudasm_intel_execute_o64__les(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	struct crudasm_temp_node_t argtmp[2];
	argtmp[0].size = get_arg_size(context, 0);
	argtmp[1].size = get_arg_size(context, 1);
	if(crudasm_read_arg(context, 1, argtmp + 1) != 0)
		return 1;
	temp_a = internal_compute_1_seg64(context, argtmp[1]);
	if(write_sr_value(context, 0, &temp_a))
		return 1;
	if(crudasm_read_arg(context, 1, argtmp + 1) != 0)
		return 1;
	temp_a = internal_compute_1_ofs64(context, argtmp[1]);
	if(crudasm_write_arg(context, 0, &temp_a) != 0)
		return 1;
	return 0;  /* success */
}


static int crudasm_intel_execute_o16__lds(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	struct crudasm_temp_node_t argtmp[2];
	argtmp[0].size = get_arg_size(context, 0);
	argtmp[1].size = get_arg_size(context, 1);
	if(crudasm_read_arg(context, 1, argtmp + 1) != 0)
		return 1;
	temp_a = internal_compute_1_seg16(context, argtmp[1]);
	if(write_sr_value(context, 3, &temp_a))
		return 1;
	if(crudasm_read_arg(context, 1, argtmp + 1) != 0)
		return 1;
	temp_a = internal_compute_1_ofs16(context, argtmp[1]);
	if(crudasm_write_arg(context, 0, &temp_a) != 0)
		return 1;
	return 0;  /* success */
}


static int crudasm_intel_execute_o32__lds(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	struct crudasm_temp_node_t argtmp[2];
	argtmp[0].size = get_arg_size(context, 0);
	argtmp[1].size = get_arg_size(context, 1);
	if(crudasm_read_arg(context, 1, argtmp + 1) != 0)
		return 1;
	temp_a = internal_compute_1_seg32(context, argtmp[1]);
	if(write_sr_value(context, 3, &temp_a))
		return 1;
	if(crudasm_read_arg(context, 1, argtmp + 1) != 0)
		return 1;
	temp_a = internal_compute_1_ofs32(context, argtmp[1]);
	if(crudasm_write_arg(context, 0, &temp_a) != 0)
		return 1;
	return 0;  /* success */
}


static int crudasm_intel_execute_o64__lds(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	struct crudasm_temp_node_t argtmp[2];
	argtmp[0].size = get_arg_size(context, 0);
	argtmp[1].size = get_arg_size(context, 1);
	if(crudasm_read_arg(context, 1, argtmp + 1) != 0)
		return 1;
	temp_a = internal_compute_1_seg64(context, argtmp[1]);
	if(write_sr_value(context, 3, &temp_a))
		return 1;
	if(crudasm_read_arg(context, 1, argtmp + 1) != 0)
		return 1;
	temp_a = internal_compute_1_ofs64(context, argtmp[1]);
	if(crudasm_write_arg(context, 0, &temp_a) != 0)
		return 1;
	return 0;  /* success */
}


static int crudasm_intel_execute_o16__lss(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	struct crudasm_temp_node_t argtmp[2];
	argtmp[0].size = get_arg_size(context, 0);
	argtmp[1].size = get_arg_size(context, 1);
	if(crudasm_read_arg(context, 1, argtmp + 1) != 0)
		return 1;
	temp_a = internal_compute_1_seg16(context, argtmp[1]);
	if(write_sr_value(context, 2, &temp_a))
		return 1;
	if(crudasm_read_arg(context, 1, argtmp + 1) != 0)
		return 1;
	temp_a = internal_compute_1_ofs16(context, argtmp[1]);
	if(crudasm_write_arg(context, 0, &temp_a) != 0)
		return 1;
	return 0;  /* success */
}


static int crudasm_intel_execute_o32__lss(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	struct crudasm_temp_node_t argtmp[2];
	argtmp[0].size = get_arg_size(context, 0);
	argtmp[1].size = get_arg_size(context, 1);
	if(crudasm_read_arg(context, 1, argtmp + 1) != 0)
		return 1;
	temp_a = internal_compute_1_seg32(context, argtmp[1]);
	if(write_sr_value(context, 2, &temp_a))
		return 1;
	if(crudasm_read_arg(context, 1, argtmp + 1) != 0)
		return 1;
	temp_a = internal_compute_1_ofs32(context, argtmp[1]);
	if(crudasm_write_arg(context, 0, &temp_a) != 0)
		return 1;
	return 0;  /* success */
}


static int crudasm_intel_execute_o64__lss(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	struct crudasm_temp_node_t argtmp[2];
	argtmp[0].size = get_arg_size(context, 0);
	argtmp[1].size = get_arg_size(context, 1);
	if(crudasm_read_arg(context, 1, argtmp + 1) != 0)
		return 1;
	temp_a = internal_compute_1_seg64(context, argtmp[1]);
	if(write_sr_value(context, 2, &temp_a))
		return 1;
	if(crudasm_read_arg(context, 1, argtmp + 1) != 0)
		return 1;
	temp_a = internal_compute_1_ofs64(context, argtmp[1]);
	if(crudasm_write_arg(context, 0, &temp_a) != 0)
		return 1;
	return 0;  /* success */
}


static int crudasm_intel_execute_o16__lfs(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	struct crudasm_temp_node_t argtmp[2];
	argtmp[0].size = get_arg_size(context, 0);
	argtmp[1].size = get_arg_size(context, 1);
	if(crudasm_read_arg(context, 1, argtmp + 1) != 0)
		return 1;
	temp_a = internal_compute_1_seg16(context, argtmp[1]);
	if(write_sr_value(context, 4, &temp_a))
		return 1;
	if(crudasm_read_arg(context, 1, argtmp + 1) != 0)
		return 1;
	temp_a = internal_compute_1_ofs16(context, argtmp[1]);
	if(crudasm_write_arg(context, 0, &temp_a) != 0)
		return 1;
	return 0;  /* success */
}


static int crudasm_intel_execute_o32__lfs(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	struct crudasm_temp_node_t argtmp[2];
	argtmp[0].size = get_arg_size(context, 0);
	argtmp[1].size = get_arg_size(context, 1);
	if(crudasm_read_arg(context, 1, argtmp + 1) != 0)
		return 1;
	temp_a = internal_compute_1_seg32(context, argtmp[1]);
	if(write_sr_value(context, 4, &temp_a))
		return 1;
	if(crudasm_read_arg(context, 1, argtmp + 1) != 0)
		return 1;
	temp_a = internal_compute_1_ofs32(context, argtmp[1]);
	if(crudasm_write_arg(context, 0, &temp_a) != 0)
		return 1;
	return 0;  /* success */
}


static int crudasm_intel_execute_o64__lfs(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	struct crudasm_temp_node_t argtmp[2];
	argtmp[0].size = get_arg_size(context, 0);
	argtmp[1].size = get_arg_size(context, 1);
	if(crudasm_read_arg(context, 1, argtmp + 1) != 0)
		return 1;
	temp_a = internal_compute_1_seg64(context, argtmp[1]);
	if(write_sr_value(context, 4, &temp_a))
		return 1;
	if(crudasm_read_arg(context, 1, argtmp + 1) != 0)
		return 1;
	temp_a = internal_compute_1_ofs64(context, argtmp[1]);
	if(crudasm_write_arg(context, 0, &temp_a) != 0)
		return 1;
	return 0;  /* success */
}


static int crudasm_intel_execute_o16__lgs(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	struct crudasm_temp_node_t argtmp[2];
	argtmp[0].size = get_arg_size(context, 0);
	argtmp[1].size = get_arg_size(context, 1);
	if(crudasm_read_arg(context, 1, argtmp + 1) != 0)
		return 1;
	temp_a = internal_compute_1_seg16(context, argtmp[1]);
	if(write_sr_value(context, 5, &temp_a))
		return 1;
	if(crudasm_read_arg(context, 1, argtmp + 1) != 0)
		return 1;
	temp_a = internal_compute_1_ofs16(context, argtmp[1]);
	if(crudasm_write_arg(context, 0, &temp_a) != 0)
		return 1;
	return 0;  /* success */
}


static int crudasm_intel_execute_o32__lgs(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	struct crudasm_temp_node_t argtmp[2];
	argtmp[0].size = get_arg_size(context, 0);
	argtmp[1].size = get_arg_size(context, 1);
	if(crudasm_read_arg(context, 1, argtmp + 1) != 0)
		return 1;
	temp_a = internal_compute_1_seg32(context, argtmp[1]);
	if(write_sr_value(context, 5, &temp_a))
		return 1;
	if(crudasm_read_arg(context, 1, argtmp + 1) != 0)
		return 1;
	temp_a = internal_compute_1_ofs32(context, argtmp[1]);
	if(crudasm_write_arg(context, 0, &temp_a) != 0)
		return 1;
	return 0;  /* success */
}


static int crudasm_intel_execute_o64__lgs(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	struct crudasm_temp_node_t argtmp[2];
	argtmp[0].size = get_arg_size(context, 0);
	argtmp[1].size = get_arg_size(context, 1);
	if(crudasm_read_arg(context, 1, argtmp + 1) != 0)
		return 1;
	temp_a = internal_compute_1_seg64(context, argtmp[1]);
	if(write_sr_value(context, 5, &temp_a))
		return 1;
	if(crudasm_read_arg(context, 1, argtmp + 1) != 0)
		return 1;
	temp_a = internal_compute_1_ofs64(context, argtmp[1]);
	if(crudasm_write_arg(context, 0, &temp_a) != 0)
		return 1;
	return 0;  /* success */
}


static int crudasm_intel_execute__movzx(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	struct crudasm_temp_node_t argtmp[2];
	argtmp[0].size = get_arg_size(context, 0);
	argtmp[1].size = get_arg_size(context, 1);
	if(crudasm_read_arg(context, 1, argtmp + 1) != 0)
		return 1;
	temp_a = internal_compute_zx(context, get_arg_size(context, 0), argtmp[1]);
	if(crudasm_write_arg(context, 0, &temp_a) != 0)
		return 1;
	return 0;  /* success */
}


static int crudasm_intel_execute__movsx(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	struct crudasm_temp_node_t argtmp[2];
	argtmp[0].size = get_arg_size(context, 0);
	argtmp[1].size = get_arg_size(context, 1);
	if(crudasm_read_arg(context, 1, argtmp + 1) != 0)
		return 1;
	temp_a = internal_compute_zx(context, get_arg_size(context, 0), argtmp[1]);
	if(crudasm_write_arg(context, 0, &temp_a) != 0)
		return 1;
	return 0;  /* success */
}


static int crudasm_intel_execute_s16__enter(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	struct crudasm_temp_node_t argtmp[2];
	argtmp[0].size = get_arg_size(context, 0);
	argtmp[1].size = get_arg_size(context, 1);
	if(crudasm_read_arg(context, 1, argtmp + 1) != 0)
		return 1;
	if(special_do_assert(context, internal_compute_2__eq(context, argtmp[1], internal_make_num(context, 1, 0, 0)), internal_make_num(context, 1, 0, 64)) != 0)
		return 1;
	if(special_do_push(context, internal_get_reg16(context, 5)) != 0)
		return 1;
	temp_a = internal_get_reg16(context, 4);
	if(write_gr16_value(context, 5, &temp_a))
		return 1;
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	if(special_do_reserve(context, argtmp[0]) != 0)
		return 1;
	return 0;  /* success */
}


static int crudasm_intel_execute_s32__enter(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	struct crudasm_temp_node_t argtmp[2];
	argtmp[0].size = get_arg_size(context, 0);
	argtmp[1].size = get_arg_size(context, 1);
	if(crudasm_read_arg(context, 1, argtmp + 1) != 0)
		return 1;
	if(special_do_assert(context, internal_compute_2__eq(context, argtmp[1], internal_make_num(context, 1, 0, 0)), internal_make_num(context, 1, 0, 64)) != 0)
		return 1;
	if(special_do_push(context, internal_get_reg32(context, 5)) != 0)
		return 1;
	temp_a = internal_get_reg32(context, 4);
	if(write_gr32_value(context, 5, &temp_a))
		return 1;
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	if(special_do_reserve(context, internal_compute_zx(context, 4, argtmp[0])) != 0)
		return 1;
	return 0;  /* success */
}


static int crudasm_intel_execute_s64__enter(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	struct crudasm_temp_node_t argtmp[2];
	argtmp[0].size = get_arg_size(context, 0);
	argtmp[1].size = get_arg_size(context, 1);
	if(crudasm_read_arg(context, 1, argtmp + 1) != 0)
		return 1;
	if(special_do_assert(context, internal_compute_2__eq(context, argtmp[1], internal_make_num(context, 1, 0, 0)), internal_make_num(context, 1, 0, 64)) != 0)
		return 1;
	if(special_do_push(context, internal_get_reg64(context, 5)) != 0)
		return 1;
	temp_a = internal_get_reg64(context, 4);
	if(write_gr64_value(context, 5, &temp_a))
		return 1;
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	if(special_do_reserve(context, internal_compute_zx(context, 8, argtmp[0])) != 0)
		return 1;
	return 0;  /* success */
}


static int crudasm_intel_execute_s16__leave(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	temp_a = internal_get_reg16(context, 5);
	if(write_gr16_value(context, 4, &temp_a))
		return 1;
	temp_a.size = 2;
	if(do_pop(context, &temp_a) != 0)
		return 1;
	if(write_gr16_value(context, 5, &temp_a))
		return 1;
	return 0;  /* success */
}


static int crudasm_intel_execute_s32__leave(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	temp_a = internal_get_reg32(context, 5);
	if(write_gr32_value(context, 4, &temp_a))
		return 1;
	temp_a.size = 4;
	if(do_pop(context, &temp_a) != 0)
		return 1;
	if(write_gr32_value(context, 5, &temp_a))
		return 1;
	return 0;  /* success */
}


static int crudasm_intel_execute_s64__leave(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	temp_a = internal_get_reg64(context, 5);
	if(write_gr64_value(context, 4, &temp_a))
		return 1;
	temp_a.size = 8;
	if(do_pop(context, &temp_a) != 0)
		return 1;
	if(write_gr64_value(context, 5, &temp_a))
		return 1;
	return 0;  /* success */
}


static int crudasm_intel_execute__in(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	struct crudasm_temp_node_t argtmp[2];
	argtmp[0].size = get_arg_size(context, 0);
	argtmp[1].size = get_arg_size(context, 1);
	if(crudasm_read_arg(context, 1, argtmp + 1) != 0)
		return 1;
	temp_a = argtmp[1];
	if(do_inport(context, &temp_a, get_arg_size(context, 0)) != 0)
		return 1;
	if(crudasm_write_arg(context, 0, &temp_a) != 0)
		return 1;
	return 0;  /* success */
}


static int crudasm_intel_execute__out(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	struct crudasm_temp_node_t argtmp[2];
	argtmp[0].size = get_arg_size(context, 0);
	argtmp[1].size = get_arg_size(context, 1);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	if(crudasm_read_arg(context, 1, argtmp + 1) != 0)
		return 1;
	if(special_do_outport(context, argtmp[0], argtmp[1]) != 0)
		return 1;
	return 0;  /* success */
}


static int crudasm_intel_execute___pushsr(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	struct crudasm_temp_node_t argtmp[1];
	argtmp[0].size = get_arg_size(context, 0);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	if(special_do_push(context, argtmp[0]) != 0)
		return 1;
	return 0;  /* success */
}


static int crudasm_intel_execute___popsr(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	struct crudasm_temp_node_t argtmp[1];
	argtmp[0].size = get_arg_size(context, 0);
	temp_a.size = get_arg_size(context, 0);
	if(do_pop(context, &temp_a) != 0)
		return 1;
	if(crudasm_write_arg(context, 0, &temp_a) != 0)
		return 1;
	return 0;  /* success */
}


static int crudasm_intel_execute___push(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	struct crudasm_temp_node_t argtmp[1];
	argtmp[0].size = get_arg_size(context, 0);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	if(special_do_push(context, argtmp[0]) != 0)
		return 1;
	return 0;  /* success */
}


static int crudasm_intel_execute___pop(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	struct crudasm_temp_node_t argtmp[1];
	argtmp[0].size = get_arg_size(context, 0);
	temp_a.size = get_arg_size(context, 0);
	if(do_pop(context, &temp_a) != 0)
		return 1;
	if(crudasm_write_arg(context, 0, &temp_a) != 0)
		return 1;
	return 0;  /* success */
}


static int crudasm_intel_execute___movsrv(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	struct crudasm_temp_node_t argtmp[2];
	argtmp[0].size = get_arg_size(context, 0);
	argtmp[1].size = get_arg_size(context, 1);
	if(crudasm_read_arg(context, 1, argtmp + 1) != 0)
		return 1;
	temp_a = argtmp[1];
	if(crudasm_write_arg(context, 0, &temp_a) != 0)
		return 1;
	return 0;  /* success */
}


static int crudasm_intel_execute___movvsr(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	struct crudasm_temp_node_t argtmp[2];
	argtmp[0].size = get_arg_size(context, 0);
	argtmp[1].size = get_arg_size(context, 1);
	if(crudasm_read_arg(context, 1, argtmp + 1) != 0)
		return 1;
	temp_a = internal_compute_zx(context, get_arg_size(context, 0), argtmp[1]);
	if(crudasm_write_arg(context, 0, &temp_a) != 0)
		return 1;
	return 0;  /* success */
}


static int crudasm_intel_execute_o16___pushf(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	if(special_do_push(context, internal_get_extern_FLAGS(context)) != 0)
		return 1;
	return 0;  /* success */
}


static int crudasm_intel_execute_o32___pushf(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	if(special_do_push(context, internal_get_extern_EFLAGS(context)) != 0)
		return 1;
	return 0;  /* success */
}


static int crudasm_intel_execute_o64___pushf(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	if(special_do_push(context, internal_get_extern_RFLAGS(context)) != 0)
		return 1;
	return 0;  /* success */
}


static int crudasm_intel_execute_o16___popf(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	temp_a.size = 2;
	if(do_pop(context, &temp_a) != 0)
		return 1;
	do_write_extern_FLAGS(context, &temp_a);
	return 0;  /* success */
}


static int crudasm_intel_execute_o32___popf(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	temp_a.size = 4;
	if(do_pop(context, &temp_a) != 0)
		return 1;
	do_write_extern_EFLAGS(context, &temp_a);
	return 0;  /* success */
}


static int crudasm_intel_execute_o64___popf(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	temp_a.size = 8;
	if(do_pop(context, &temp_a) != 0)
		return 1;
	do_write_extern_RFLAGS(context, &temp_a);
	return 0;  /* success */
}


static int crudasm_intel_execute_a16___movs(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	struct crudasm_temp_node_t tmp[1];
	struct crudasm_temp_node_t argtmp[2];
	tmp[0].size = 2;
	argtmp[0].size = get_arg_size(context, 0);
	argtmp[1].size = get_arg_size(context, 1);
	if(crudasm_read_arg(context, 1, argtmp + 1) != 0)
		return 1;
	temp_a = argtmp[1];
	if(crudasm_write_arg(context, 0, &temp_a) != 0)
		return 1;
	temp_a = internal_compute_3_quest(context, internal_compute_1__lognot(context, internal_get_flag(context, 10)), internal_compute_tr(context, 2, internal_get_argsize(context, 0)), internal_compute_1_neg(context, internal_compute_tr(context, 2, internal_get_argsize(context, 0))));
	tmp[0] = temp_a;
	temp_a = internal_compute_2_add(context, internal_get_reg16(context, 6), tmp[0]);
	if(write_gr16_value(context, 6, &temp_a))
		return 1;
	temp_a = internal_compute_2_add(context, internal_get_reg16(context, 7), tmp[0]);
	if(write_gr16_value(context, 7, &temp_a))
		return 1;
	return 0;  /* success */
}


static int crudasm_intel_execute_a32___movs(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	struct crudasm_temp_node_t tmp[1];
	struct crudasm_temp_node_t argtmp[2];
	tmp[0].size = 4;
	argtmp[0].size = get_arg_size(context, 0);
	argtmp[1].size = get_arg_size(context, 1);
	if(crudasm_read_arg(context, 1, argtmp + 1) != 0)
		return 1;
	temp_a = argtmp[1];
	if(crudasm_write_arg(context, 0, &temp_a) != 0)
		return 1;
	temp_a = internal_compute_3_quest(context, internal_compute_1__lognot(context, internal_get_flag(context, 10)), internal_compute_tr(context, 4, internal_get_argsize(context, 0)), internal_compute_1_neg(context, internal_compute_tr(context, 4, internal_get_argsize(context, 0))));
	tmp[0] = temp_a;
	temp_a = internal_compute_2_add(context, internal_get_reg32(context, 6), tmp[0]);
	if(write_gr32_value(context, 6, &temp_a))
		return 1;
	temp_a = internal_compute_2_add(context, internal_get_reg32(context, 7), tmp[0]);
	if(write_gr32_value(context, 7, &temp_a))
		return 1;
	return 0;  /* success */
}


static int crudasm_intel_execute_a64___movs(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	struct crudasm_temp_node_t tmp[1];
	struct crudasm_temp_node_t argtmp[2];
	tmp[0].size = 8;
	argtmp[0].size = get_arg_size(context, 0);
	argtmp[1].size = get_arg_size(context, 1);
	if(crudasm_read_arg(context, 1, argtmp + 1) != 0)
		return 1;
	temp_a = argtmp[1];
	if(crudasm_write_arg(context, 0, &temp_a) != 0)
		return 1;
	temp_a = internal_compute_3_quest(context, internal_compute_1__lognot(context, internal_get_flag(context, 10)), internal_compute_tr(context, 8, internal_get_argsize(context, 0)), internal_compute_1_neg(context, internal_compute_tr(context, 8, internal_get_argsize(context, 0))));
	tmp[0] = temp_a;
	temp_a = internal_compute_2_add(context, internal_get_reg64(context, 6), tmp[0]);
	if(write_gr64_value(context, 6, &temp_a))
		return 1;
	temp_a = internal_compute_2_add(context, internal_get_reg64(context, 7), tmp[0]);
	if(write_gr64_value(context, 7, &temp_a))
		return 1;
	return 0;  /* success */
}


static int crudasm_intel_execute_a16___cmps(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	struct crudasm_temp_node_t tmp[2];
	struct crudasm_temp_node_t argtmp[2];
	tmp[0].size = get_arg_size(context, 0);
	tmp[1].size = 2;
	argtmp[0].size = get_arg_size(context, 0);
	argtmp[1].size = get_arg_size(context, 1);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	if(crudasm_read_arg(context, 1, argtmp + 1) != 0)
		return 1;
	temp_a = internal_compute_2__sub_of(context, argtmp[0], argtmp[1]);
	do_write_rflag_value(context, 11, &temp_a);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	if(crudasm_read_arg(context, 1, argtmp + 1) != 0)
		return 1;
	temp_a = internal_compute_2__sub_af(context, argtmp[0], argtmp[1]);
	do_write_rflag_value(context, 4, &temp_a);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	if(crudasm_read_arg(context, 1, argtmp + 1) != 0)
		return 1;
	temp_a = internal_compute_2__sub_cf(context, argtmp[0], argtmp[1]);
	do_write_rflag_value(context, 0, &temp_a);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	if(crudasm_read_arg(context, 1, argtmp + 1) != 0)
		return 1;
	temp_a = internal_compute_2_sub(context, argtmp[0], argtmp[1]);
	tmp[0] = temp_a;
	temp_a = internal_compute_1__sign(context, tmp[0]);
	do_write_rflag_value(context, 7, &temp_a);
	temp_a = internal_compute_1__zero(context, tmp[0]);
	do_write_rflag_value(context, 6, &temp_a);
	temp_a = internal_compute_1__parity(context, tmp[0]);
	do_write_rflag_value(context, 2, &temp_a);
	temp_a = internal_compute_3_quest(context, internal_compute_1__lognot(context, internal_get_flag(context, 10)), internal_compute_tr(context, 2, internal_get_argsize(context, 0)), internal_compute_1_neg(context, internal_compute_tr(context, 2, internal_get_argsize(context, 0))));
	tmp[1] = temp_a;
	temp_a = internal_compute_2_add(context, internal_get_reg16(context, 6), tmp[1]);
	if(write_gr16_value(context, 6, &temp_a))
		return 1;
	temp_a = internal_compute_2_add(context, internal_get_reg16(context, 7), tmp[1]);
	if(write_gr16_value(context, 7, &temp_a))
		return 1;
	return 0;  /* success */
}


static int crudasm_intel_execute_a32___cmps(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	struct crudasm_temp_node_t tmp[2];
	struct crudasm_temp_node_t argtmp[2];
	tmp[0].size = get_arg_size(context, 0);
	tmp[1].size = 4;
	argtmp[0].size = get_arg_size(context, 0);
	argtmp[1].size = get_arg_size(context, 1);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	if(crudasm_read_arg(context, 1, argtmp + 1) != 0)
		return 1;
	temp_a = internal_compute_2__sub_of(context, argtmp[0], argtmp[1]);
	do_write_rflag_value(context, 11, &temp_a);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	if(crudasm_read_arg(context, 1, argtmp + 1) != 0)
		return 1;
	temp_a = internal_compute_2__sub_af(context, argtmp[0], argtmp[1]);
	do_write_rflag_value(context, 4, &temp_a);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	if(crudasm_read_arg(context, 1, argtmp + 1) != 0)
		return 1;
	temp_a = internal_compute_2__sub_cf(context, argtmp[0], argtmp[1]);
	do_write_rflag_value(context, 0, &temp_a);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	if(crudasm_read_arg(context, 1, argtmp + 1) != 0)
		return 1;
	temp_a = internal_compute_2_sub(context, argtmp[0], argtmp[1]);
	tmp[0] = temp_a;
	temp_a = internal_compute_1__sign(context, tmp[0]);
	do_write_rflag_value(context, 7, &temp_a);
	temp_a = internal_compute_1__zero(context, tmp[0]);
	do_write_rflag_value(context, 6, &temp_a);
	temp_a = internal_compute_1__parity(context, tmp[0]);
	do_write_rflag_value(context, 2, &temp_a);
	temp_a = internal_compute_3_quest(context, internal_compute_1__lognot(context, internal_get_flag(context, 10)), internal_compute_tr(context, 4, internal_get_argsize(context, 0)), internal_compute_1_neg(context, internal_compute_tr(context, 4, internal_get_argsize(context, 0))));
	tmp[1] = temp_a;
	temp_a = internal_compute_2_add(context, internal_get_reg32(context, 6), tmp[1]);
	if(write_gr32_value(context, 6, &temp_a))
		return 1;
	temp_a = internal_compute_2_add(context, internal_get_reg32(context, 7), tmp[1]);
	if(write_gr32_value(context, 7, &temp_a))
		return 1;
	return 0;  /* success */
}


static int crudasm_intel_execute_a64___cmps(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	struct crudasm_temp_node_t tmp[2];
	struct crudasm_temp_node_t argtmp[2];
	tmp[0].size = get_arg_size(context, 0);
	tmp[1].size = 8;
	argtmp[0].size = get_arg_size(context, 0);
	argtmp[1].size = get_arg_size(context, 1);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	if(crudasm_read_arg(context, 1, argtmp + 1) != 0)
		return 1;
	temp_a = internal_compute_2__sub_of(context, argtmp[0], argtmp[1]);
	do_write_rflag_value(context, 11, &temp_a);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	if(crudasm_read_arg(context, 1, argtmp + 1) != 0)
		return 1;
	temp_a = internal_compute_2__sub_af(context, argtmp[0], argtmp[1]);
	do_write_rflag_value(context, 4, &temp_a);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	if(crudasm_read_arg(context, 1, argtmp + 1) != 0)
		return 1;
	temp_a = internal_compute_2__sub_cf(context, argtmp[0], argtmp[1]);
	do_write_rflag_value(context, 0, &temp_a);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	if(crudasm_read_arg(context, 1, argtmp + 1) != 0)
		return 1;
	temp_a = internal_compute_2_sub(context, argtmp[0], argtmp[1]);
	tmp[0] = temp_a;
	temp_a = internal_compute_1__sign(context, tmp[0]);
	do_write_rflag_value(context, 7, &temp_a);
	temp_a = internal_compute_1__zero(context, tmp[0]);
	do_write_rflag_value(context, 6, &temp_a);
	temp_a = internal_compute_1__parity(context, tmp[0]);
	do_write_rflag_value(context, 2, &temp_a);
	temp_a = internal_compute_3_quest(context, internal_compute_1__lognot(context, internal_get_flag(context, 10)), internal_compute_tr(context, 8, internal_get_argsize(context, 0)), internal_compute_1_neg(context, internal_compute_tr(context, 8, internal_get_argsize(context, 0))));
	tmp[1] = temp_a;
	temp_a = internal_compute_2_add(context, internal_get_reg64(context, 6), tmp[1]);
	if(write_gr64_value(context, 6, &temp_a))
		return 1;
	temp_a = internal_compute_2_add(context, internal_get_reg64(context, 7), tmp[1]);
	if(write_gr64_value(context, 7, &temp_a))
		return 1;
	return 0;  /* success */
}


static int crudasm_intel_execute_a16___stos(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	struct crudasm_temp_node_t tmp[1];
	struct crudasm_temp_node_t argtmp[2];
	tmp[0].size = 2;
	argtmp[0].size = get_arg_size(context, 0);
	argtmp[1].size = get_arg_size(context, 1);
	if(crudasm_read_arg(context, 1, argtmp + 1) != 0)
		return 1;
	temp_a = argtmp[1];
	if(crudasm_write_arg(context, 0, &temp_a) != 0)
		return 1;
	temp_a = internal_compute_3_quest(context, internal_compute_1__lognot(context, internal_get_flag(context, 10)), internal_compute_tr(context, 2, internal_get_argsize(context, 0)), internal_compute_1_neg(context, internal_compute_tr(context, 2, internal_get_argsize(context, 0))));
	tmp[0] = temp_a;
	temp_a = internal_compute_2_add(context, internal_get_reg16(context, 7), tmp[0]);
	if(write_gr16_value(context, 7, &temp_a))
		return 1;
	return 0;  /* success */
}


static int crudasm_intel_execute_a32___stos(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	struct crudasm_temp_node_t tmp[1];
	struct crudasm_temp_node_t argtmp[2];
	tmp[0].size = 4;
	argtmp[0].size = get_arg_size(context, 0);
	argtmp[1].size = get_arg_size(context, 1);
	if(crudasm_read_arg(context, 1, argtmp + 1) != 0)
		return 1;
	temp_a = argtmp[1];
	if(crudasm_write_arg(context, 0, &temp_a) != 0)
		return 1;
	temp_a = internal_compute_3_quest(context, internal_compute_1__lognot(context, internal_get_flag(context, 10)), internal_compute_tr(context, 4, internal_get_argsize(context, 0)), internal_compute_1_neg(context, internal_compute_tr(context, 4, internal_get_argsize(context, 0))));
	tmp[0] = temp_a;
	temp_a = internal_compute_2_add(context, internal_get_reg32(context, 7), tmp[0]);
	if(write_gr32_value(context, 7, &temp_a))
		return 1;
	return 0;  /* success */
}


static int crudasm_intel_execute_a64___stos(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	struct crudasm_temp_node_t tmp[1];
	struct crudasm_temp_node_t argtmp[2];
	tmp[0].size = 8;
	argtmp[0].size = get_arg_size(context, 0);
	argtmp[1].size = get_arg_size(context, 1);
	if(crudasm_read_arg(context, 1, argtmp + 1) != 0)
		return 1;
	temp_a = argtmp[1];
	if(crudasm_write_arg(context, 0, &temp_a) != 0)
		return 1;
	temp_a = internal_compute_3_quest(context, internal_compute_1__lognot(context, internal_get_flag(context, 10)), internal_compute_tr(context, 8, internal_get_argsize(context, 0)), internal_compute_1_neg(context, internal_compute_tr(context, 8, internal_get_argsize(context, 0))));
	tmp[0] = temp_a;
	temp_a = internal_compute_2_add(context, internal_get_reg64(context, 7), tmp[0]);
	if(write_gr64_value(context, 7, &temp_a))
		return 1;
	return 0;  /* success */
}


static int crudasm_intel_execute_a16___lods(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	struct crudasm_temp_node_t tmp[1];
	struct crudasm_temp_node_t argtmp[2];
	tmp[0].size = 2;
	argtmp[0].size = get_arg_size(context, 0);
	argtmp[1].size = get_arg_size(context, 1);
	if(crudasm_read_arg(context, 1, argtmp + 1) != 0)
		return 1;
	temp_a = argtmp[1];
	if(crudasm_write_arg(context, 0, &temp_a) != 0)
		return 1;
	temp_a = internal_compute_3_quest(context, internal_compute_1__lognot(context, internal_get_flag(context, 10)), internal_compute_tr(context, 2, internal_get_argsize(context, 0)), internal_compute_1_neg(context, internal_compute_tr(context, 2, internal_get_argsize(context, 0))));
	tmp[0] = temp_a;
	temp_a = internal_compute_2_add(context, internal_get_reg16(context, 6), tmp[0]);
	if(write_gr16_value(context, 6, &temp_a))
		return 1;
	return 0;  /* success */
}


static int crudasm_intel_execute_a32___lods(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	struct crudasm_temp_node_t tmp[1];
	struct crudasm_temp_node_t argtmp[2];
	tmp[0].size = 4;
	argtmp[0].size = get_arg_size(context, 0);
	argtmp[1].size = get_arg_size(context, 1);
	if(crudasm_read_arg(context, 1, argtmp + 1) != 0)
		return 1;
	temp_a = argtmp[1];
	if(crudasm_write_arg(context, 0, &temp_a) != 0)
		return 1;
	temp_a = internal_compute_3_quest(context, internal_compute_1__lognot(context, internal_get_flag(context, 10)), internal_compute_tr(context, 4, internal_get_argsize(context, 0)), internal_compute_1_neg(context, internal_compute_tr(context, 4, internal_get_argsize(context, 0))));
	tmp[0] = temp_a;
	temp_a = internal_compute_2_add(context, internal_get_reg32(context, 6), tmp[0]);
	if(write_gr32_value(context, 6, &temp_a))
		return 1;
	return 0;  /* success */
}


static int crudasm_intel_execute_a64___lods(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	struct crudasm_temp_node_t tmp[1];
	struct crudasm_temp_node_t argtmp[2];
	tmp[0].size = 8;
	argtmp[0].size = get_arg_size(context, 0);
	argtmp[1].size = get_arg_size(context, 1);
	if(crudasm_read_arg(context, 1, argtmp + 1) != 0)
		return 1;
	temp_a = argtmp[1];
	if(crudasm_write_arg(context, 0, &temp_a) != 0)
		return 1;
	temp_a = internal_compute_3_quest(context, internal_compute_1__lognot(context, internal_get_flag(context, 10)), internal_compute_tr(context, 8, internal_get_argsize(context, 0)), internal_compute_1_neg(context, internal_compute_tr(context, 8, internal_get_argsize(context, 0))));
	tmp[0] = temp_a;
	temp_a = internal_compute_2_add(context, internal_get_reg64(context, 6), tmp[0]);
	if(write_gr64_value(context, 6, &temp_a))
		return 1;
	return 0;  /* success */
}


static int crudasm_intel_execute_a16___scas(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	struct crudasm_temp_node_t tmp[2];
	struct crudasm_temp_node_t argtmp[2];
	tmp[0].size = get_arg_size(context, 0);
	tmp[1].size = 2;
	argtmp[0].size = get_arg_size(context, 0);
	argtmp[1].size = get_arg_size(context, 1);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	if(crudasm_read_arg(context, 1, argtmp + 1) != 0)
		return 1;
	temp_a = internal_compute_2__sub_of(context, argtmp[0], argtmp[1]);
	do_write_rflag_value(context, 11, &temp_a);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	if(crudasm_read_arg(context, 1, argtmp + 1) != 0)
		return 1;
	temp_a = internal_compute_2__sub_af(context, argtmp[0], argtmp[1]);
	do_write_rflag_value(context, 4, &temp_a);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	if(crudasm_read_arg(context, 1, argtmp + 1) != 0)
		return 1;
	temp_a = internal_compute_2__sub_cf(context, argtmp[0], argtmp[1]);
	do_write_rflag_value(context, 0, &temp_a);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	if(crudasm_read_arg(context, 1, argtmp + 1) != 0)
		return 1;
	temp_a = internal_compute_2_sub(context, argtmp[0], argtmp[1]);
	tmp[0] = temp_a;
	temp_a = internal_compute_1__sign(context, tmp[0]);
	do_write_rflag_value(context, 7, &temp_a);
	temp_a = internal_compute_1__zero(context, tmp[0]);
	do_write_rflag_value(context, 6, &temp_a);
	temp_a = internal_compute_1__parity(context, tmp[0]);
	do_write_rflag_value(context, 2, &temp_a);
	temp_a = internal_compute_3_quest(context, internal_compute_1__lognot(context, internal_get_flag(context, 10)), internal_compute_tr(context, 2, internal_get_argsize(context, 0)), internal_compute_1_neg(context, internal_compute_tr(context, 2, internal_get_argsize(context, 0))));
	tmp[1] = temp_a;
	temp_a = internal_compute_2_add(context, internal_get_reg16(context, 7), tmp[1]);
	if(write_gr16_value(context, 7, &temp_a))
		return 1;
	return 0;  /* success */
}


static int crudasm_intel_execute_a32___scas(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	struct crudasm_temp_node_t tmp[2];
	struct crudasm_temp_node_t argtmp[2];
	tmp[0].size = get_arg_size(context, 0);
	tmp[1].size = 4;
	argtmp[0].size = get_arg_size(context, 0);
	argtmp[1].size = get_arg_size(context, 1);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	if(crudasm_read_arg(context, 1, argtmp + 1) != 0)
		return 1;
	temp_a = internal_compute_2__sub_of(context, argtmp[0], argtmp[1]);
	do_write_rflag_value(context, 11, &temp_a);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	if(crudasm_read_arg(context, 1, argtmp + 1) != 0)
		return 1;
	temp_a = internal_compute_2__sub_af(context, argtmp[0], argtmp[1]);
	do_write_rflag_value(context, 4, &temp_a);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	if(crudasm_read_arg(context, 1, argtmp + 1) != 0)
		return 1;
	temp_a = internal_compute_2__sub_cf(context, argtmp[0], argtmp[1]);
	do_write_rflag_value(context, 0, &temp_a);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	if(crudasm_read_arg(context, 1, argtmp + 1) != 0)
		return 1;
	temp_a = internal_compute_2_sub(context, argtmp[0], argtmp[1]);
	tmp[0] = temp_a;
	temp_a = internal_compute_1__sign(context, tmp[0]);
	do_write_rflag_value(context, 7, &temp_a);
	temp_a = internal_compute_1__zero(context, tmp[0]);
	do_write_rflag_value(context, 6, &temp_a);
	temp_a = internal_compute_1__parity(context, tmp[0]);
	do_write_rflag_value(context, 2, &temp_a);
	temp_a = internal_compute_3_quest(context, internal_compute_1__lognot(context, internal_get_flag(context, 10)), internal_compute_tr(context, 4, internal_get_argsize(context, 0)), internal_compute_1_neg(context, internal_compute_tr(context, 4, internal_get_argsize(context, 0))));
	tmp[1] = temp_a;
	temp_a = internal_compute_2_add(context, internal_get_reg32(context, 7), tmp[1]);
	if(write_gr32_value(context, 7, &temp_a))
		return 1;
	return 0;  /* success */
}


static int crudasm_intel_execute_a64___scas(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	struct crudasm_temp_node_t tmp[2];
	struct crudasm_temp_node_t argtmp[2];
	tmp[0].size = get_arg_size(context, 0);
	tmp[1].size = 8;
	argtmp[0].size = get_arg_size(context, 0);
	argtmp[1].size = get_arg_size(context, 1);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	if(crudasm_read_arg(context, 1, argtmp + 1) != 0)
		return 1;
	temp_a = internal_compute_2__sub_of(context, argtmp[0], argtmp[1]);
	do_write_rflag_value(context, 11, &temp_a);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	if(crudasm_read_arg(context, 1, argtmp + 1) != 0)
		return 1;
	temp_a = internal_compute_2__sub_af(context, argtmp[0], argtmp[1]);
	do_write_rflag_value(context, 4, &temp_a);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	if(crudasm_read_arg(context, 1, argtmp + 1) != 0)
		return 1;
	temp_a = internal_compute_2__sub_cf(context, argtmp[0], argtmp[1]);
	do_write_rflag_value(context, 0, &temp_a);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	if(crudasm_read_arg(context, 1, argtmp + 1) != 0)
		return 1;
	temp_a = internal_compute_2_sub(context, argtmp[0], argtmp[1]);
	tmp[0] = temp_a;
	temp_a = internal_compute_1__sign(context, tmp[0]);
	do_write_rflag_value(context, 7, &temp_a);
	temp_a = internal_compute_1__zero(context, tmp[0]);
	do_write_rflag_value(context, 6, &temp_a);
	temp_a = internal_compute_1__parity(context, tmp[0]);
	do_write_rflag_value(context, 2, &temp_a);
	temp_a = internal_compute_3_quest(context, internal_compute_1__lognot(context, internal_get_flag(context, 10)), internal_compute_tr(context, 8, internal_get_argsize(context, 0)), internal_compute_1_neg(context, internal_compute_tr(context, 8, internal_get_argsize(context, 0))));
	tmp[1] = temp_a;
	temp_a = internal_compute_2_add(context, internal_get_reg64(context, 7), tmp[1]);
	if(write_gr64_value(context, 7, &temp_a))
		return 1;
	return 0;  /* success */
}


static int crudasm_intel_execute___test(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	struct crudasm_temp_node_t tmp[1];
	struct crudasm_temp_node_t argtmp[2];
	tmp[0].size = get_arg_size(context, 0);
	argtmp[0].size = get_arg_size(context, 0);
	argtmp[1].size = get_arg_size(context, 1);
	temp_a = internal_make_num(context, 1, 0, 0);
	do_write_rflag_value(context, 11, &temp_a);
	temp_a = internal_get_extern_undefined(context);
	do_write_rflag_value(context, 4, &temp_a);
	temp_a = internal_make_num(context, 1, 0, 0);
	do_write_rflag_value(context, 0, &temp_a);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	if(crudasm_read_arg(context, 1, argtmp + 1) != 0)
		return 1;
	temp_a = internal_compute_2_bitand(context, argtmp[0], argtmp[1]);
	tmp[0] = temp_a;
	temp_a = internal_compute_1__sign(context, tmp[0]);
	do_write_rflag_value(context, 7, &temp_a);
	temp_a = internal_compute_1__zero(context, tmp[0]);
	do_write_rflag_value(context, 6, &temp_a);
	temp_a = internal_compute_1__parity(context, tmp[0]);
	do_write_rflag_value(context, 2, &temp_a);
	return 0;  /* success */
}


static int crudasm_intel_execute___xchg(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	struct crudasm_temp_node_t tmp[1];
	struct crudasm_temp_node_t argtmp[2];
	tmp[0].size = get_arg_size(context, 0);
	argtmp[0].size = get_arg_size(context, 0);
	argtmp[1].size = get_arg_size(context, 1);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = argtmp[0];
	tmp[0] = temp_a;
	if(crudasm_read_arg(context, 1, argtmp + 1) != 0)
		return 1;
	temp_a = argtmp[1];
	if(crudasm_write_arg(context, 0, &temp_a) != 0)
		return 1;
	temp_a = tmp[0];
	if(crudasm_write_arg(context, 1, &temp_a) != 0)
		return 1;
	return 0;  /* success */
}


static int crudasm_intel_execute__not(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	struct crudasm_temp_node_t argtmp[1];
	argtmp[0].size = get_arg_size(context, 0);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_1_bitnot(context, argtmp[0]);
	if(crudasm_write_arg(context, 0, &temp_a) != 0)
		return 1;
	return 0;  /* success */
}


static int crudasm_intel_execute__neg(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	struct crudasm_temp_node_t argtmp[1];
	argtmp[0].size = get_arg_size(context, 0);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_2__sub_of(context, internal_make_num(context, get_arg_size(context, 0), 0, 0), argtmp[0]);
	do_write_rflag_value(context, 11, &temp_a);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_2__sub_af(context, internal_make_num(context, get_arg_size(context, 0), 0, 0), argtmp[0]);
	do_write_rflag_value(context, 4, &temp_a);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_2__sub_cf(context, internal_make_num(context, get_arg_size(context, 0), 0, 0), argtmp[0]);
	do_write_rflag_value(context, 0, &temp_a);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_1_neg(context, argtmp[0]);
	if(crudasm_write_arg(context, 0, &temp_a) != 0)
		return 1;
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_1__sign(context, argtmp[0]);
	do_write_rflag_value(context, 7, &temp_a);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_1__zero(context, argtmp[0]);
	do_write_rflag_value(context, 6, &temp_a);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_1__parity(context, argtmp[0]);
	do_write_rflag_value(context, 2, &temp_a);
	return 0;  /* success */
}


static int crudasm_intel_execute__inc(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	struct crudasm_temp_node_t argtmp[1];
	argtmp[0].size = get_arg_size(context, 0);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_2__add_of(context, argtmp[0], internal_make_num(context, get_arg_size(context, 0), 0, 1));
	do_write_rflag_value(context, 11, &temp_a);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_2__add_af(context, argtmp[0], internal_make_num(context, get_arg_size(context, 0), 0, 1));
	do_write_rflag_value(context, 4, &temp_a);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_2_add(context, argtmp[0], internal_make_num(context, get_arg_size(context, 0), 0, 1));
	if(crudasm_write_arg(context, 0, &temp_a) != 0)
		return 1;
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_1__sign(context, argtmp[0]);
	do_write_rflag_value(context, 7, &temp_a);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_1__zero(context, argtmp[0]);
	do_write_rflag_value(context, 6, &temp_a);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_1__parity(context, argtmp[0]);
	do_write_rflag_value(context, 2, &temp_a);
	return 0;  /* success */
}


static int crudasm_intel_execute__dec(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	struct crudasm_temp_node_t argtmp[1];
	argtmp[0].size = get_arg_size(context, 0);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_2__sub_of(context, argtmp[0], internal_make_num(context, get_arg_size(context, 0), 0, 1));
	do_write_rflag_value(context, 11, &temp_a);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_2__sub_af(context, argtmp[0], internal_make_num(context, get_arg_size(context, 0), 0, 1));
	do_write_rflag_value(context, 4, &temp_a);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_2_sub(context, argtmp[0], internal_make_num(context, get_arg_size(context, 0), 0, 1));
	if(crudasm_write_arg(context, 0, &temp_a) != 0)
		return 1;
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_1__sign(context, argtmp[0]);
	do_write_rflag_value(context, 7, &temp_a);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_1__zero(context, argtmp[0]);
	do_write_rflag_value(context, 6, &temp_a);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_1__parity(context, argtmp[0]);
	do_write_rflag_value(context, 2, &temp_a);
	return 0;  /* success */
}


static int crudasm_intel_execute___xlat(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	struct crudasm_temp_node_t argtmp[1];
	argtmp[0].size = get_arg_size(context, 0);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = argtmp[0];
	if(write_gr8_value(context, 0, &temp_a))
		return 1;
	return 0;  /* success */
}


static int crudasm_intel_execute___nopmb(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	struct crudasm_temp_node_t argtmp[1];
	argtmp[0].size = get_arg_size(context, 0);
	return 0;  /* success */
}


static int crudasm_intel_execute_o16___sxacc(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	temp_a = internal_compute_sx(context, 2, internal_get_reg8(context, 0));
	if(write_gr16_value(context, 0, &temp_a))
		return 1;
	return 0;  /* success */
}


static int crudasm_intel_execute_o32___sxacc(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	temp_a = internal_compute_sx(context, 4, internal_get_reg16(context, 0));
	if(write_gr32_value(context, 0, &temp_a))
		return 1;
	return 0;  /* success */
}


static int crudasm_intel_execute_o64___sxacc(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	temp_a = internal_compute_sx(context, 8, internal_get_reg32(context, 0));
	if(write_gr64_value(context, 0, &temp_a))
		return 1;
	return 0;  /* success */
}


static int crudasm_intel_execute_o16___sxdax(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	temp_a = internal_compute_2_sub(context, internal_make_num(context, 2, 0, 0), internal_compute_zx(context, 2, internal_compute_1__sign(context, internal_get_reg16(context, 0))));
	if(write_gr16_value(context, 2, &temp_a))
		return 1;
	return 0;  /* success */
}


static int crudasm_intel_execute_o32___sxdax(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	temp_a = internal_compute_2_sub(context, internal_make_num(context, 4, 0, 0), internal_compute_zx(context, 4, internal_compute_1__sign(context, internal_get_reg32(context, 0))));
	if(write_gr32_value(context, 2, &temp_a))
		return 1;
	return 0;  /* success */
}


static int crudasm_intel_execute_o64___sxdax(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	temp_a = internal_compute_2_sub(context, internal_make_num(context, 8, 0, 0), internal_compute_zx(context, 8, internal_compute_1__sign(context, internal_get_reg64(context, 0))));
	if(write_gr64_value(context, 2, &temp_a))
		return 1;
	return 0;  /* success */
}


static int crudasm_intel_execute__sahf(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	temp_a = internal_compute_1__lognot(context, internal_compute_1__zero(context, internal_compute_2_bitand(context, internal_compute_2_shru(context, internal_get_reg8(context, 16), internal_make_num(context, 1, 0, 7)), internal_make_num(context, 1, 0, 1))));
	do_write_rflag_value(context, 7, &temp_a);
	temp_a = internal_compute_1__lognot(context, internal_compute_1__zero(context, internal_compute_2_bitand(context, internal_compute_2_shru(context, internal_get_reg8(context, 16), internal_make_num(context, 1, 0, 6)), internal_make_num(context, 1, 0, 1))));
	do_write_rflag_value(context, 6, &temp_a);
	temp_a = internal_compute_1__lognot(context, internal_compute_1__zero(context, internal_compute_2_bitand(context, internal_compute_2_shru(context, internal_get_reg8(context, 16), internal_make_num(context, 1, 0, 4)), internal_make_num(context, 1, 0, 1))));
	do_write_rflag_value(context, 4, &temp_a);
	temp_a = internal_compute_1__lognot(context, internal_compute_1__zero(context, internal_compute_2_bitand(context, internal_compute_2_shru(context, internal_get_reg8(context, 16), internal_make_num(context, 1, 0, 2)), internal_make_num(context, 1, 0, 1))));
	do_write_rflag_value(context, 2, &temp_a);
	temp_a = internal_compute_1__lognot(context, internal_compute_1__zero(context, internal_compute_2_bitand(context, internal_get_reg8(context, 16), internal_make_num(context, 1, 0, 1))));
	do_write_rflag_value(context, 0, &temp_a);
	return 0;  /* success */
}


static int crudasm_intel_execute__lahf(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	temp_a = internal_compute_2_bitor(context, internal_make_num(context, 1, 0, 2), internal_compute_zx(context, 1, internal_get_flag(context, 0)));
	if(write_gr8_value(context, 16, &temp_a))
		return 1;
	temp_a = internal_compute_2_bitor(context, internal_get_reg8(context, 16), internal_compute_2_shl(context, internal_compute_zx(context, 1, internal_get_flag(context, 2)), internal_make_num(context, 1, 0, 2)));
	if(write_gr8_value(context, 16, &temp_a))
		return 1;
	temp_a = internal_compute_2_bitor(context, internal_get_reg8(context, 16), internal_compute_2_shl(context, internal_compute_zx(context, 1, internal_get_flag(context, 4)), internal_make_num(context, 1, 0, 4)));
	if(write_gr8_value(context, 16, &temp_a))
		return 1;
	temp_a = internal_compute_2_bitor(context, internal_get_reg8(context, 16), internal_compute_2_shl(context, internal_compute_zx(context, 1, internal_get_flag(context, 6)), internal_make_num(context, 1, 0, 6)));
	if(write_gr8_value(context, 16, &temp_a))
		return 1;
	temp_a = internal_compute_2_bitor(context, internal_get_reg8(context, 16), internal_compute_2_shl(context, internal_compute_zx(context, 1, internal_get_flag(context, 7)), internal_make_num(context, 1, 0, 7)));
	if(write_gr8_value(context, 16, &temp_a))
		return 1;
	return 0;  /* success */
}


static int crudasm_intel_execute__cmc(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	temp_a = internal_compute_1__lognot(context, internal_get_flag(context, 0));
	do_write_rflag_value(context, 0, &temp_a);
	return 0;  /* success */
}


static int crudasm_intel_execute__clc(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	temp_a = internal_make_num(context, 1, 0, 0);
	do_write_rflag_value(context, 0, &temp_a);
	return 0;  /* success */
}


static int crudasm_intel_execute__stc(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	temp_a = internal_make_num(context, 1, 0, 1);
	do_write_rflag_value(context, 0, &temp_a);
	return 0;  /* success */
}


static int crudasm_intel_execute__cld(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	temp_a = internal_make_num(context, 1, 0, 0);
	do_write_rflag_value(context, 10, &temp_a);
	return 0;  /* success */
}


static int crudasm_intel_execute__std(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	temp_a = internal_make_num(context, 1, 0, 1);
	do_write_rflag_value(context, 10, &temp_a);
	return 0;  /* success */
}


static int crudasm_intel_execute__lea(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	struct crudasm_temp_node_t argtmp[2];
	argtmp[0].size = get_arg_size(context, 0);
	argtmp[1].size = get_arg_size(context, 1);
	if(crudasm_read_arg(context, 1, argtmp + 1) != 0)
		return 1;
	temp_a = internal_compute_zx(context, get_arg_size(context, 0), argtmp[1]);
	if(crudasm_write_arg(context, 0, &temp_a) != 0)
		return 1;
	return 0;  /* success */
}


static int crudasm_intel_execute__into(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	if(special_do_assert(context, internal_compute_1__lognot(context, internal_get_flag(context, 11)), internal_make_num(context, 1, 0, 4)) != 0)
		return 1;
	return 0;  /* success */
}


static int crudasm_intel_execute___cmul3(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	struct crudasm_temp_node_t tmp[1];
	struct crudasm_temp_node_t argtmp[3];
	tmp[0].size = 1/*BIT*/;
	argtmp[0].size = get_arg_size(context, 0);
	argtmp[1].size = get_arg_size(context, 1);
	argtmp[2].size = get_arg_size(context, 2);
	if(crudasm_read_arg(context, 1, argtmp + 1) != 0)
		return 1;
	if(crudasm_read_arg(context, 2, argtmp + 2) != 0)
		return 1;
	temp_a = internal_compute_2__smul_overflow(context, argtmp[1], argtmp[2]);
	tmp[0] = temp_a;
	if(crudasm_read_arg(context, 1, argtmp + 1) != 0)
		return 1;
	if(crudasm_read_arg(context, 2, argtmp + 2) != 0)
		return 1;
	temp_a = internal_compute_2_cmul(context, argtmp[1], argtmp[2]);
	if(crudasm_write_arg(context, 0, &temp_a) != 0)
		return 1;
	temp_a = internal_get_extern_undefined(context);
	do_write_rflag_value(context, 7, &temp_a);
	temp_a = internal_get_extern_undefined(context);
	do_write_rflag_value(context, 6, &temp_a);
	temp_a = internal_get_extern_undefined(context);
	do_write_rflag_value(context, 4, &temp_a);
	temp_a = internal_get_extern_undefined(context);
	do_write_rflag_value(context, 2, &temp_a);
	temp_a = tmp[0];
	do_write_rflag_value(context, 0, &temp_a);
	temp_a = tmp[0];
	do_write_rflag_value(context, 11, &temp_a);
	return 0;  /* success */
}


static int crudasm_intel_execute___cmul2(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	struct crudasm_temp_node_t tmp[1];
	struct crudasm_temp_node_t argtmp[2];
	tmp[0].size = 1/*BIT*/;
	argtmp[0].size = get_arg_size(context, 0);
	argtmp[1].size = get_arg_size(context, 1);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	if(crudasm_read_arg(context, 1, argtmp + 1) != 0)
		return 1;
	temp_a = internal_compute_2__smul_overflow(context, argtmp[0], argtmp[1]);
	tmp[0] = temp_a;
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	if(crudasm_read_arg(context, 1, argtmp + 1) != 0)
		return 1;
	temp_a = internal_compute_2_cmul(context, argtmp[0], argtmp[1]);
	if(crudasm_write_arg(context, 0, &temp_a) != 0)
		return 1;
	temp_a = internal_get_extern_undefined(context);
	do_write_rflag_value(context, 7, &temp_a);
	temp_a = internal_get_extern_undefined(context);
	do_write_rflag_value(context, 6, &temp_a);
	temp_a = internal_get_extern_undefined(context);
	do_write_rflag_value(context, 4, &temp_a);
	temp_a = internal_get_extern_undefined(context);
	do_write_rflag_value(context, 2, &temp_a);
	temp_a = tmp[0];
	do_write_rflag_value(context, 0, &temp_a);
	temp_a = tmp[0];
	do_write_rflag_value(context, 11, &temp_a);
	return 0;  /* success */
}


static int crudasm_intel_execute___imulb(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	struct crudasm_temp_node_t tmp[1];
	struct crudasm_temp_node_t argtmp[1];
	tmp[0].size = 1/*BIT*/;
	argtmp[0].size = get_arg_size(context, 0);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_2__smul_overflow(context, internal_get_reg8(context, 0), argtmp[0]);
	tmp[0] = temp_a;
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_2_cmul(context, internal_compute_sx(context, 2, internal_get_reg8(context, 0)), internal_compute_sx(context, 2, argtmp[0]));
	if(write_gr16_value(context, 0, &temp_a))
		return 1;
	temp_a = internal_get_extern_undefined(context);
	do_write_rflag_value(context, 7, &temp_a);
	temp_a = internal_get_extern_undefined(context);
	do_write_rflag_value(context, 6, &temp_a);
	temp_a = internal_get_extern_undefined(context);
	do_write_rflag_value(context, 4, &temp_a);
	temp_a = internal_get_extern_undefined(context);
	do_write_rflag_value(context, 2, &temp_a);
	temp_a = tmp[0];
	do_write_rflag_value(context, 0, &temp_a);
	temp_a = tmp[0];
	do_write_rflag_value(context, 11, &temp_a);
	return 0;  /* success */
}


static int crudasm_intel_execute_o16__imul(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	struct crudasm_temp_node_t tmp[1];
	struct crudasm_temp_node_t argtmp[1];
	tmp[0].size = 1/*BIT*/;
	argtmp[0].size = get_arg_size(context, 0);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_2__smul_overflow(context, internal_get_reg16(context, 0), internal_compute_tr(context, 2, argtmp[0]));
	tmp[0] = temp_a;
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_2_smulhi(context, internal_get_reg16(context, 0), internal_compute_tr(context, 2, argtmp[0]));
	if(write_gr16_value(context, 2, &temp_a))
		return 1;
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_2_cmul(context, internal_get_reg16(context, 0), internal_compute_tr(context, 2, argtmp[0]));
	if(write_gr16_value(context, 0, &temp_a))
		return 1;
	temp_a = internal_get_extern_undefined(context);
	do_write_rflag_value(context, 7, &temp_a);
	temp_a = internal_get_extern_undefined(context);
	do_write_rflag_value(context, 6, &temp_a);
	temp_a = internal_get_extern_undefined(context);
	do_write_rflag_value(context, 4, &temp_a);
	temp_a = internal_get_extern_undefined(context);
	do_write_rflag_value(context, 2, &temp_a);
	temp_a = tmp[0];
	do_write_rflag_value(context, 0, &temp_a);
	temp_a = tmp[0];
	do_write_rflag_value(context, 11, &temp_a);
	return 0;  /* success */
}


static int crudasm_intel_execute_o32__imul(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	struct crudasm_temp_node_t tmp[1];
	struct crudasm_temp_node_t argtmp[1];
	tmp[0].size = 1/*BIT*/;
	argtmp[0].size = get_arg_size(context, 0);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_2__smul_overflow(context, internal_get_reg32(context, 0), internal_compute_tr(context, 4, argtmp[0]));
	tmp[0] = temp_a;
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_2_smulhi(context, internal_get_reg32(context, 0), internal_compute_tr(context, 4, argtmp[0]));
	if(write_gr32_value(context, 2, &temp_a))
		return 1;
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_2_cmul(context, internal_get_reg32(context, 0), internal_compute_tr(context, 4, argtmp[0]));
	if(write_gr32_value(context, 0, &temp_a))
		return 1;
	temp_a = internal_get_extern_undefined(context);
	do_write_rflag_value(context, 7, &temp_a);
	temp_a = internal_get_extern_undefined(context);
	do_write_rflag_value(context, 6, &temp_a);
	temp_a = internal_get_extern_undefined(context);
	do_write_rflag_value(context, 4, &temp_a);
	temp_a = internal_get_extern_undefined(context);
	do_write_rflag_value(context, 2, &temp_a);
	temp_a = tmp[0];
	do_write_rflag_value(context, 0, &temp_a);
	temp_a = tmp[0];
	do_write_rflag_value(context, 11, &temp_a);
	return 0;  /* success */
}


static int crudasm_intel_execute_o64__imul(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	struct crudasm_temp_node_t tmp[1];
	struct crudasm_temp_node_t argtmp[1];
	tmp[0].size = 1/*BIT*/;
	argtmp[0].size = get_arg_size(context, 0);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_2__smul_overflow(context, internal_get_reg64(context, 0), internal_compute_tr(context, 8, argtmp[0]));
	tmp[0] = temp_a;
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_2_smulhi(context, internal_get_reg64(context, 0), internal_compute_tr(context, 8, argtmp[0]));
	if(write_gr64_value(context, 2, &temp_a))
		return 1;
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_2_cmul(context, internal_get_reg64(context, 0), internal_compute_tr(context, 8, argtmp[0]));
	if(write_gr64_value(context, 0, &temp_a))
		return 1;
	temp_a = internal_get_extern_undefined(context);
	do_write_rflag_value(context, 7, &temp_a);
	temp_a = internal_get_extern_undefined(context);
	do_write_rflag_value(context, 6, &temp_a);
	temp_a = internal_get_extern_undefined(context);
	do_write_rflag_value(context, 4, &temp_a);
	temp_a = internal_get_extern_undefined(context);
	do_write_rflag_value(context, 2, &temp_a);
	temp_a = tmp[0];
	do_write_rflag_value(context, 0, &temp_a);
	temp_a = tmp[0];
	do_write_rflag_value(context, 11, &temp_a);
	return 0;  /* success */
}


static int crudasm_intel_execute___mulb(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	struct crudasm_temp_node_t tmp[1];
	struct crudasm_temp_node_t argtmp[1];
	tmp[0].size = 1/*BIT*/;
	argtmp[0].size = get_arg_size(context, 0);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_2__umul_overflow(context, internal_get_reg8(context, 0), argtmp[0]);
	tmp[0] = temp_a;
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_2_cmul(context, internal_compute_zx(context, 2, internal_get_reg8(context, 0)), internal_compute_zx(context, 2, argtmp[0]));
	if(write_gr16_value(context, 0, &temp_a))
		return 1;
	temp_a = internal_get_extern_undefined(context);
	do_write_rflag_value(context, 7, &temp_a);
	temp_a = internal_get_extern_undefined(context);
	do_write_rflag_value(context, 6, &temp_a);
	temp_a = internal_get_extern_undefined(context);
	do_write_rflag_value(context, 4, &temp_a);
	temp_a = internal_get_extern_undefined(context);
	do_write_rflag_value(context, 2, &temp_a);
	temp_a = tmp[0];
	do_write_rflag_value(context, 0, &temp_a);
	temp_a = tmp[0];
	do_write_rflag_value(context, 11, &temp_a);
	return 0;  /* success */
}


static int crudasm_intel_execute_o16__mul(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	struct crudasm_temp_node_t tmp[1];
	struct crudasm_temp_node_t argtmp[1];
	tmp[0].size = 1/*BIT*/;
	argtmp[0].size = get_arg_size(context, 0);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_2__umul_overflow(context, internal_get_reg16(context, 0), argtmp[0]);
	tmp[0] = temp_a;
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_2_umulhi(context, internal_get_reg16(context, 0), internal_compute_tr(context, 2, argtmp[0]));
	if(write_gr16_value(context, 2, &temp_a))
		return 1;
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_2_cmul(context, internal_get_reg16(context, 0), internal_compute_tr(context, 2, argtmp[0]));
	if(write_gr16_value(context, 0, &temp_a))
		return 1;
	temp_a = internal_get_extern_undefined(context);
	do_write_rflag_value(context, 7, &temp_a);
	temp_a = internal_get_extern_undefined(context);
	do_write_rflag_value(context, 6, &temp_a);
	temp_a = internal_get_extern_undefined(context);
	do_write_rflag_value(context, 4, &temp_a);
	temp_a = internal_get_extern_undefined(context);
	do_write_rflag_value(context, 2, &temp_a);
	temp_a = tmp[0];
	do_write_rflag_value(context, 0, &temp_a);
	temp_a = tmp[0];
	do_write_rflag_value(context, 11, &temp_a);
	return 0;  /* success */
}


static int crudasm_intel_execute_o32__mul(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	struct crudasm_temp_node_t tmp[1];
	struct crudasm_temp_node_t argtmp[1];
	tmp[0].size = 1/*BIT*/;
	argtmp[0].size = get_arg_size(context, 0);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_2__umul_overflow(context, internal_get_reg32(context, 0), internal_compute_tr(context, 4, argtmp[0]));
	tmp[0] = temp_a;
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_2_umulhi(context, internal_get_reg32(context, 0), internal_compute_tr(context, 4, argtmp[0]));
	if(write_gr32_value(context, 2, &temp_a))
		return 1;
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_2_cmul(context, internal_get_reg32(context, 0), internal_compute_tr(context, 4, argtmp[0]));
	if(write_gr32_value(context, 0, &temp_a))
		return 1;
	temp_a = internal_get_extern_undefined(context);
	do_write_rflag_value(context, 7, &temp_a);
	temp_a = internal_get_extern_undefined(context);
	do_write_rflag_value(context, 6, &temp_a);
	temp_a = internal_get_extern_undefined(context);
	do_write_rflag_value(context, 4, &temp_a);
	temp_a = internal_get_extern_undefined(context);
	do_write_rflag_value(context, 2, &temp_a);
	temp_a = tmp[0];
	do_write_rflag_value(context, 0, &temp_a);
	temp_a = tmp[0];
	do_write_rflag_value(context, 11, &temp_a);
	return 0;  /* success */
}


static int crudasm_intel_execute_o64__mul(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	struct crudasm_temp_node_t tmp[1];
	struct crudasm_temp_node_t argtmp[1];
	tmp[0].size = 1/*BIT*/;
	argtmp[0].size = get_arg_size(context, 0);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_2__umul_overflow(context, internal_get_reg64(context, 0), internal_compute_tr(context, 8, argtmp[0]));
	tmp[0] = temp_a;
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_2_umulhi(context, internal_get_reg64(context, 0), internal_compute_tr(context, 8, argtmp[0]));
	if(write_gr64_value(context, 2, &temp_a))
		return 1;
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_2_cmul(context, internal_get_reg64(context, 0), internal_compute_tr(context, 8, argtmp[0]));
	if(write_gr64_value(context, 0, &temp_a))
		return 1;
	temp_a = internal_get_extern_undefined(context);
	do_write_rflag_value(context, 7, &temp_a);
	temp_a = internal_get_extern_undefined(context);
	do_write_rflag_value(context, 6, &temp_a);
	temp_a = internal_get_extern_undefined(context);
	do_write_rflag_value(context, 4, &temp_a);
	temp_a = internal_get_extern_undefined(context);
	do_write_rflag_value(context, 2, &temp_a);
	temp_a = tmp[0];
	do_write_rflag_value(context, 0, &temp_a);
	temp_a = tmp[0];
	do_write_rflag_value(context, 11, &temp_a);
	return 0;  /* success */
}


static int crudasm_intel_execute___divb(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	struct crudasm_temp_node_t tmp[3];
	struct crudasm_temp_node_t argtmp[1];
	tmp[0].size = 2;
	tmp[1].size = 2;
	tmp[2].size = 1;
	argtmp[0].size = get_arg_size(context, 0);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	if(special_do_assert(context, internal_compute_1__lognot(context, internal_compute_1__zero(context, argtmp[0])), internal_make_num(context, 1, 0, 0)) != 0)
		return 1;
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_2_udiv(context, internal_get_reg16(context, 0), internal_compute_zx(context, 2, argtmp[0]));
	tmp[0] = temp_a;
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_2_umod(context, internal_get_reg16(context, 0), internal_compute_zx(context, 2, argtmp[0]));
	tmp[1] = temp_a;
	temp_a = internal_compute_tr(context, 1, tmp[0]);
	tmp[2] = temp_a;
	if(special_do_assert(context, internal_compute_2__eq(context, internal_compute_zx(context, 2, tmp[2]), tmp[0]), internal_make_num(context, 1, 0, 0)) != 0)
		return 1;
	temp_a = tmp[2];
	if(write_gr8_value(context, 0, &temp_a))
		return 1;
	temp_a = internal_compute_tr(context, 1, tmp[1]);
	if(write_gr8_value(context, 16, &temp_a))
		return 1;
	temp_a = internal_get_extern_undefined(context);
	do_write_rflag_value(context, 11, &temp_a);
	temp_a = internal_get_extern_undefined(context);
	do_write_rflag_value(context, 7, &temp_a);
	temp_a = internal_get_extern_undefined(context);
	do_write_rflag_value(context, 6, &temp_a);
	temp_a = internal_get_extern_undefined(context);
	do_write_rflag_value(context, 4, &temp_a);
	temp_a = internal_get_extern_undefined(context);
	do_write_rflag_value(context, 2, &temp_a);
	temp_a = internal_get_extern_undefined(context);
	do_write_rflag_value(context, 0, &temp_a);
	return 0;  /* success */
}


static int crudasm_intel_execute_o16__div(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	struct crudasm_temp_node_t tmp[4];
	struct crudasm_temp_node_t argtmp[1];
	tmp[0].size = 4;
	tmp[1].size = 4;
	tmp[2].size = 4;
	tmp[3].size = 2;
	argtmp[0].size = get_arg_size(context, 0);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	if(special_do_assert(context, internal_compute_1__lognot(context, internal_compute_1__zero(context, argtmp[0])), internal_make_num(context, 1, 0, 0)) != 0)
		return 1;
	temp_a = internal_compute_2_bitor(context, internal_compute_2_shl(context, internal_compute_zx(context, 4, internal_get_reg16(context, 2)), internal_make_num(context, 1, 0, 16)), internal_compute_zx(context, 4, internal_get_reg16(context, 0)));
	tmp[0] = temp_a;
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_2_udiv(context, tmp[0], internal_compute_zx(context, 4, argtmp[0]));
	tmp[1] = temp_a;
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_2_umod(context, tmp[0], internal_compute_zx(context, 4, argtmp[0]));
	tmp[2] = temp_a;
	temp_a = internal_compute_tr(context, 2, tmp[1]);
	tmp[3] = temp_a;
	if(special_do_assert(context, internal_compute_2__eq(context, internal_compute_zx(context, 4, tmp[3]), tmp[1]), internal_make_num(context, 1, 0, 0)) != 0)
		return 1;
	temp_a = tmp[3];
	if(write_gr16_value(context, 0, &temp_a))
		return 1;
	temp_a = internal_compute_tr(context, 2, tmp[2]);
	if(write_gr16_value(context, 2, &temp_a))
		return 1;
	temp_a = internal_get_extern_undefined(context);
	do_write_rflag_value(context, 11, &temp_a);
	temp_a = internal_get_extern_undefined(context);
	do_write_rflag_value(context, 7, &temp_a);
	temp_a = internal_get_extern_undefined(context);
	do_write_rflag_value(context, 6, &temp_a);
	temp_a = internal_get_extern_undefined(context);
	do_write_rflag_value(context, 4, &temp_a);
	temp_a = internal_get_extern_undefined(context);
	do_write_rflag_value(context, 2, &temp_a);
	temp_a = internal_get_extern_undefined(context);
	do_write_rflag_value(context, 0, &temp_a);
	return 0;  /* success */
}


static int crudasm_intel_execute_o32__div(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	struct crudasm_temp_node_t tmp[4];
	struct crudasm_temp_node_t argtmp[1];
	tmp[0].size = 8;
	tmp[1].size = 8;
	tmp[2].size = 8;
	tmp[3].size = 4;
	argtmp[0].size = get_arg_size(context, 0);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	if(special_do_assert(context, internal_compute_1__lognot(context, internal_compute_1__zero(context, argtmp[0])), internal_make_num(context, 1, 0, 0)) != 0)
		return 1;
	temp_a = internal_compute_2_bitor(context, internal_compute_2_shl(context, internal_compute_zx(context, 8, internal_get_reg32(context, 2)), internal_make_num(context, 1, 0, 32)), internal_compute_zx(context, 8, internal_get_reg32(context, 0)));
	tmp[0] = temp_a;
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_2_udiv(context, tmp[0], internal_compute_zx(context, 8, argtmp[0]));
	tmp[1] = temp_a;
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_2_umod(context, tmp[0], internal_compute_zx(context, 8, argtmp[0]));
	tmp[2] = temp_a;
	temp_a = internal_compute_tr(context, 4, tmp[1]);
	tmp[3] = temp_a;
	if(special_do_assert(context, internal_compute_2__eq(context, internal_compute_zx(context, 8, tmp[3]), tmp[1]), internal_make_num(context, 1, 0, 0)) != 0)
		return 1;
	temp_a = tmp[3];
	if(write_gr32_value(context, 0, &temp_a))
		return 1;
	temp_a = internal_compute_tr(context, 4, tmp[2]);
	if(write_gr32_value(context, 2, &temp_a))
		return 1;
	temp_a = internal_get_extern_undefined(context);
	do_write_rflag_value(context, 11, &temp_a);
	temp_a = internal_get_extern_undefined(context);
	do_write_rflag_value(context, 7, &temp_a);
	temp_a = internal_get_extern_undefined(context);
	do_write_rflag_value(context, 6, &temp_a);
	temp_a = internal_get_extern_undefined(context);
	do_write_rflag_value(context, 4, &temp_a);
	temp_a = internal_get_extern_undefined(context);
	do_write_rflag_value(context, 2, &temp_a);
	temp_a = internal_get_extern_undefined(context);
	do_write_rflag_value(context, 0, &temp_a);
	return 0;  /* success */
}


static int crudasm_intel_execute_o64__div(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	struct crudasm_temp_node_t tmp[2];
	struct crudasm_temp_node_t argtmp[1];
	tmp[0].size = 8;
	tmp[1].size = 8;
	argtmp[0].size = get_arg_size(context, 0);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	if(special_do_assert(context, internal_compute_1__lognot(context, internal_compute_1__zero(context, argtmp[0])), internal_make_num(context, 1, 0, 0)) != 0)
		return 1;
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_3_umod64(context, internal_get_reg64(context, 2), internal_get_reg64(context, 0), internal_compute_tr(context, 8, argtmp[0]));
	tmp[0] = temp_a;
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_3_udiv64(context, internal_get_reg64(context, 2), internal_get_reg64(context, 0), internal_compute_tr(context, 8, argtmp[0]));
	tmp[1] = temp_a;
	temp_a = tmp[0];
	if(write_gr64_value(context, 2, &temp_a))
		return 1;
	temp_a = tmp[1];
	if(write_gr64_value(context, 0, &temp_a))
		return 1;
	temp_a = internal_get_extern_undefined(context);
	do_write_rflag_value(context, 11, &temp_a);
	temp_a = internal_get_extern_undefined(context);
	do_write_rflag_value(context, 7, &temp_a);
	temp_a = internal_get_extern_undefined(context);
	do_write_rflag_value(context, 6, &temp_a);
	temp_a = internal_get_extern_undefined(context);
	do_write_rflag_value(context, 4, &temp_a);
	temp_a = internal_get_extern_undefined(context);
	do_write_rflag_value(context, 2, &temp_a);
	temp_a = internal_get_extern_undefined(context);
	do_write_rflag_value(context, 0, &temp_a);
	return 0;  /* success */
}


static int crudasm_intel_execute___idivb(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	struct crudasm_temp_node_t tmp[3];
	struct crudasm_temp_node_t argtmp[1];
	tmp[0].size = 2;
	tmp[1].size = 2;
	tmp[2].size = 1;
	argtmp[0].size = get_arg_size(context, 0);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	if(special_do_assert(context, internal_compute_1__lognot(context, internal_compute_1__zero(context, argtmp[0])), internal_make_num(context, 1, 0, 0)) != 0)
		return 1;
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_2_sdiv(context, internal_get_reg16(context, 0), internal_compute_sx(context, 2, argtmp[0]));
	tmp[0] = temp_a;
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_2_smod(context, internal_get_reg16(context, 0), internal_compute_sx(context, 2, argtmp[0]));
	tmp[1] = temp_a;
	temp_a = internal_compute_tr(context, 1, tmp[0]);
	tmp[2] = temp_a;
	if(special_do_assert(context, internal_compute_2__eq(context, internal_compute_sx(context, 2, tmp[2]), tmp[0]), internal_make_num(context, 1, 0, 0)) != 0)
		return 1;
	temp_a = tmp[2];
	if(write_gr8_value(context, 0, &temp_a))
		return 1;
	temp_a = internal_compute_tr(context, 1, tmp[1]);
	if(write_gr8_value(context, 16, &temp_a))
		return 1;
	temp_a = internal_get_extern_undefined(context);
	do_write_rflag_value(context, 11, &temp_a);
	temp_a = internal_get_extern_undefined(context);
	do_write_rflag_value(context, 7, &temp_a);
	temp_a = internal_get_extern_undefined(context);
	do_write_rflag_value(context, 6, &temp_a);
	temp_a = internal_get_extern_undefined(context);
	do_write_rflag_value(context, 4, &temp_a);
	temp_a = internal_get_extern_undefined(context);
	do_write_rflag_value(context, 2, &temp_a);
	temp_a = internal_get_extern_undefined(context);
	do_write_rflag_value(context, 0, &temp_a);
	return 0;  /* success */
}


static int crudasm_intel_execute_o16__idiv(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	struct crudasm_temp_node_t tmp[4];
	struct crudasm_temp_node_t argtmp[1];
	tmp[0].size = 4;
	tmp[1].size = 4;
	tmp[2].size = 4;
	tmp[3].size = 2;
	argtmp[0].size = get_arg_size(context, 0);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	if(special_do_assert(context, internal_compute_1__lognot(context, internal_compute_1__zero(context, argtmp[0])), internal_make_num(context, 1, 0, 0)) != 0)
		return 1;
	temp_a = internal_compute_2_bitor(context, internal_compute_2_shl(context, internal_compute_zx(context, 4, internal_get_reg16(context, 2)), internal_make_num(context, 1, 0, 16)), internal_compute_zx(context, 4, internal_get_reg16(context, 0)));
	tmp[0] = temp_a;
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_2_sdiv(context, tmp[0], internal_compute_sx(context, 4, argtmp[0]));
	tmp[1] = temp_a;
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_2_smod(context, tmp[0], internal_compute_sx(context, 4, argtmp[0]));
	tmp[2] = temp_a;
	temp_a = internal_compute_tr(context, 2, tmp[1]);
	tmp[3] = temp_a;
	if(special_do_assert(context, internal_compute_2__eq(context, internal_compute_sx(context, 4, tmp[3]), tmp[1]), internal_make_num(context, 1, 0, 0)) != 0)
		return 1;
	temp_a = tmp[3];
	if(write_gr16_value(context, 0, &temp_a))
		return 1;
	temp_a = internal_compute_tr(context, 2, tmp[2]);
	if(write_gr16_value(context, 2, &temp_a))
		return 1;
	temp_a = internal_get_extern_undefined(context);
	do_write_rflag_value(context, 11, &temp_a);
	temp_a = internal_get_extern_undefined(context);
	do_write_rflag_value(context, 7, &temp_a);
	temp_a = internal_get_extern_undefined(context);
	do_write_rflag_value(context, 6, &temp_a);
	temp_a = internal_get_extern_undefined(context);
	do_write_rflag_value(context, 4, &temp_a);
	temp_a = internal_get_extern_undefined(context);
	do_write_rflag_value(context, 2, &temp_a);
	temp_a = internal_get_extern_undefined(context);
	do_write_rflag_value(context, 0, &temp_a);
	return 0;  /* success */
}


static int crudasm_intel_execute_o32__idiv(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	struct crudasm_temp_node_t tmp[4];
	struct crudasm_temp_node_t argtmp[1];
	tmp[0].size = 8;
	tmp[1].size = 8;
	tmp[2].size = 8;
	tmp[3].size = 4;
	argtmp[0].size = get_arg_size(context, 0);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	if(special_do_assert(context, internal_compute_1__lognot(context, internal_compute_1__zero(context, argtmp[0])), internal_make_num(context, 1, 0, 0)) != 0)
		return 1;
	temp_a = internal_compute_2_bitor(context, internal_compute_2_shl(context, internal_compute_zx(context, 8, internal_get_reg32(context, 2)), internal_make_num(context, 1, 0, 32)), internal_compute_zx(context, 8, internal_get_reg32(context, 0)));
	tmp[0] = temp_a;
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_2_sdiv(context, tmp[0], internal_compute_sx(context, 8, argtmp[0]));
	tmp[1] = temp_a;
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_2_smod(context, tmp[0], internal_compute_sx(context, 8, argtmp[0]));
	tmp[2] = temp_a;
	temp_a = internal_compute_tr(context, 4, tmp[1]);
	tmp[3] = temp_a;
	if(special_do_assert(context, internal_compute_2__eq(context, internal_compute_sx(context, 8, tmp[3]), tmp[1]), internal_make_num(context, 1, 0, 0)) != 0)
		return 1;
	temp_a = tmp[3];
	if(write_gr32_value(context, 0, &temp_a))
		return 1;
	temp_a = internal_compute_tr(context, 4, tmp[2]);
	if(write_gr32_value(context, 2, &temp_a))
		return 1;
	temp_a = internal_get_extern_undefined(context);
	do_write_rflag_value(context, 11, &temp_a);
	temp_a = internal_get_extern_undefined(context);
	do_write_rflag_value(context, 7, &temp_a);
	temp_a = internal_get_extern_undefined(context);
	do_write_rflag_value(context, 6, &temp_a);
	temp_a = internal_get_extern_undefined(context);
	do_write_rflag_value(context, 4, &temp_a);
	temp_a = internal_get_extern_undefined(context);
	do_write_rflag_value(context, 2, &temp_a);
	temp_a = internal_get_extern_undefined(context);
	do_write_rflag_value(context, 0, &temp_a);
	return 0;  /* success */
}


static int crudasm_intel_execute_o64__idiv(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	struct crudasm_temp_node_t tmp[2];
	struct crudasm_temp_node_t argtmp[1];
	tmp[0].size = 8;
	tmp[1].size = 8;
	argtmp[0].size = get_arg_size(context, 0);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	if(special_do_assert(context, internal_compute_1__lognot(context, internal_compute_1__zero(context, argtmp[0])), internal_make_num(context, 1, 0, 0)) != 0)
		return 1;
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_3_smod64(context, internal_get_reg64(context, 2), internal_get_reg64(context, 0), internal_compute_tr(context, 8, argtmp[0]));
	tmp[0] = temp_a;
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_3_sdiv64(context, internal_get_reg64(context, 2), internal_get_reg64(context, 0), internal_compute_tr(context, 8, argtmp[0]));
	tmp[1] = temp_a;
	temp_a = tmp[0];
	if(write_gr64_value(context, 2, &temp_a))
		return 1;
	temp_a = tmp[1];
	if(write_gr64_value(context, 0, &temp_a))
		return 1;
	temp_a = internal_get_extern_undefined(context);
	do_write_rflag_value(context, 11, &temp_a);
	temp_a = internal_get_extern_undefined(context);
	do_write_rflag_value(context, 7, &temp_a);
	temp_a = internal_get_extern_undefined(context);
	do_write_rflag_value(context, 6, &temp_a);
	temp_a = internal_get_extern_undefined(context);
	do_write_rflag_value(context, 4, &temp_a);
	temp_a = internal_get_extern_undefined(context);
	do_write_rflag_value(context, 2, &temp_a);
	temp_a = internal_get_extern_undefined(context);
	do_write_rflag_value(context, 0, &temp_a);
	return 0;  /* success */
}


static int crudasm_intel_execute___jo(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	struct crudasm_temp_node_t tmp[1];
	struct crudasm_temp_node_t argtmp[1];
	tmp[0].size = 1/*BIT*/;
	argtmp[0].size = get_arg_size(context, 0);
	temp_a = internal_get_flag(context, 11);
	tmp[0] = temp_a;
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_3_quest(context, tmp[0], internal_compute_zx(context, 8, argtmp[0]), internal_make_num(context, 8, 0, 0));
	do_write_extern_RIPADD(context, &temp_a);
	return 0;  /* success */
}


static int crudasm_intel_execute___jno(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	struct crudasm_temp_node_t tmp[1];
	struct crudasm_temp_node_t argtmp[1];
	tmp[0].size = 1/*BIT*/;
	argtmp[0].size = get_arg_size(context, 0);
	temp_a = internal_compute_1__lognot(context, internal_get_flag(context, 11));
	tmp[0] = temp_a;
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_3_quest(context, tmp[0], internal_compute_zx(context, 8, argtmp[0]), internal_make_num(context, 8, 0, 0));
	do_write_extern_RIPADD(context, &temp_a);
	return 0;  /* success */
}


static int crudasm_intel_execute___jc(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	struct crudasm_temp_node_t tmp[1];
	struct crudasm_temp_node_t argtmp[1];
	tmp[0].size = 1/*BIT*/;
	argtmp[0].size = get_arg_size(context, 0);
	temp_a = internal_get_flag(context, 0);
	tmp[0] = temp_a;
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_3_quest(context, tmp[0], internal_compute_zx(context, 8, argtmp[0]), internal_make_num(context, 8, 0, 0));
	do_write_extern_RIPADD(context, &temp_a);
	return 0;  /* success */
}


static int crudasm_intel_execute___jnc(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	struct crudasm_temp_node_t tmp[1];
	struct crudasm_temp_node_t argtmp[1];
	tmp[0].size = 1/*BIT*/;
	argtmp[0].size = get_arg_size(context, 0);
	temp_a = internal_compute_1__lognot(context, internal_get_flag(context, 0));
	tmp[0] = temp_a;
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_3_quest(context, tmp[0], internal_compute_zx(context, 8, argtmp[0]), internal_make_num(context, 8, 0, 0));
	do_write_extern_RIPADD(context, &temp_a);
	return 0;  /* success */
}


static int crudasm_intel_execute___jz(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	struct crudasm_temp_node_t tmp[1];
	struct crudasm_temp_node_t argtmp[1];
	tmp[0].size = 1/*BIT*/;
	argtmp[0].size = get_arg_size(context, 0);
	temp_a = internal_get_flag(context, 6);
	tmp[0] = temp_a;
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_3_quest(context, tmp[0], internal_compute_zx(context, 8, argtmp[0]), internal_make_num(context, 8, 0, 0));
	do_write_extern_RIPADD(context, &temp_a);
	return 0;  /* success */
}


static int crudasm_intel_execute___jnz(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	struct crudasm_temp_node_t tmp[1];
	struct crudasm_temp_node_t argtmp[1];
	tmp[0].size = 1/*BIT*/;
	argtmp[0].size = get_arg_size(context, 0);
	temp_a = internal_compute_1__lognot(context, internal_get_flag(context, 6));
	tmp[0] = temp_a;
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_3_quest(context, tmp[0], internal_compute_zx(context, 8, argtmp[0]), internal_make_num(context, 8, 0, 0));
	do_write_extern_RIPADD(context, &temp_a);
	return 0;  /* success */
}


static int crudasm_intel_execute___jbe(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	struct crudasm_temp_node_t tmp[1];
	struct crudasm_temp_node_t argtmp[1];
	tmp[0].size = 1/*BIT*/;
	argtmp[0].size = get_arg_size(context, 0);
	temp_a = internal_compute_2__logor(context, internal_get_flag(context, 0), internal_get_flag(context, 6));
	tmp[0] = temp_a;
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_3_quest(context, tmp[0], internal_compute_zx(context, 8, argtmp[0]), internal_make_num(context, 8, 0, 0));
	do_write_extern_RIPADD(context, &temp_a);
	return 0;  /* success */
}


static int crudasm_intel_execute___ja(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	struct crudasm_temp_node_t tmp[1];
	struct crudasm_temp_node_t argtmp[1];
	tmp[0].size = 1/*BIT*/;
	argtmp[0].size = get_arg_size(context, 0);
	temp_a = internal_compute_1__lognot(context, internal_compute_2__logor(context, internal_get_flag(context, 0), internal_get_flag(context, 6)));
	tmp[0] = temp_a;
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_3_quest(context, tmp[0], internal_compute_zx(context, 8, argtmp[0]), internal_make_num(context, 8, 0, 0));
	do_write_extern_RIPADD(context, &temp_a);
	return 0;  /* success */
}


static int crudasm_intel_execute___js(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	struct crudasm_temp_node_t tmp[1];
	struct crudasm_temp_node_t argtmp[1];
	tmp[0].size = 1/*BIT*/;
	argtmp[0].size = get_arg_size(context, 0);
	temp_a = internal_get_flag(context, 7);
	tmp[0] = temp_a;
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_3_quest(context, tmp[0], internal_compute_zx(context, 8, argtmp[0]), internal_make_num(context, 8, 0, 0));
	do_write_extern_RIPADD(context, &temp_a);
	return 0;  /* success */
}


static int crudasm_intel_execute___jns(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	struct crudasm_temp_node_t tmp[1];
	struct crudasm_temp_node_t argtmp[1];
	tmp[0].size = 1/*BIT*/;
	argtmp[0].size = get_arg_size(context, 0);
	temp_a = internal_compute_1__lognot(context, internal_get_flag(context, 7));
	tmp[0] = temp_a;
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_3_quest(context, tmp[0], internal_compute_zx(context, 8, argtmp[0]), internal_make_num(context, 8, 0, 0));
	do_write_extern_RIPADD(context, &temp_a);
	return 0;  /* success */
}


static int crudasm_intel_execute___jp(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	struct crudasm_temp_node_t tmp[1];
	struct crudasm_temp_node_t argtmp[1];
	tmp[0].size = 1/*BIT*/;
	argtmp[0].size = get_arg_size(context, 0);
	temp_a = internal_get_flag(context, 2);
	tmp[0] = temp_a;
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_3_quest(context, tmp[0], internal_compute_zx(context, 8, argtmp[0]), internal_make_num(context, 8, 0, 0));
	do_write_extern_RIPADD(context, &temp_a);
	return 0;  /* success */
}


static int crudasm_intel_execute___jnp(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	struct crudasm_temp_node_t tmp[1];
	struct crudasm_temp_node_t argtmp[1];
	tmp[0].size = 1/*BIT*/;
	argtmp[0].size = get_arg_size(context, 0);
	temp_a = internal_compute_1__lognot(context, internal_get_flag(context, 2));
	tmp[0] = temp_a;
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_3_quest(context, tmp[0], internal_compute_zx(context, 8, argtmp[0]), internal_make_num(context, 8, 0, 0));
	do_write_extern_RIPADD(context, &temp_a);
	return 0;  /* success */
}


static int crudasm_intel_execute___jl(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	struct crudasm_temp_node_t tmp[1];
	struct crudasm_temp_node_t argtmp[1];
	tmp[0].size = 1/*BIT*/;
	argtmp[0].size = get_arg_size(context, 0);
	temp_a = internal_compute_2__logxor(context, internal_get_flag(context, 7), internal_get_flag(context, 11));
	tmp[0] = temp_a;
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_3_quest(context, tmp[0], internal_compute_zx(context, 8, argtmp[0]), internal_make_num(context, 8, 0, 0));
	do_write_extern_RIPADD(context, &temp_a);
	return 0;  /* success */
}


static int crudasm_intel_execute___jge(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	struct crudasm_temp_node_t tmp[1];
	struct crudasm_temp_node_t argtmp[1];
	tmp[0].size = 1/*BIT*/;
	argtmp[0].size = get_arg_size(context, 0);
	temp_a = internal_compute_1__lognot(context, internal_compute_2__logxor(context, internal_get_flag(context, 7), internal_get_flag(context, 11)));
	tmp[0] = temp_a;
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_3_quest(context, tmp[0], internal_compute_zx(context, 8, argtmp[0]), internal_make_num(context, 8, 0, 0));
	do_write_extern_RIPADD(context, &temp_a);
	return 0;  /* success */
}


static int crudasm_intel_execute___jle(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	struct crudasm_temp_node_t tmp[1];
	struct crudasm_temp_node_t argtmp[1];
	tmp[0].size = 1/*BIT*/;
	argtmp[0].size = get_arg_size(context, 0);
	temp_a = internal_compute_2__logor(context, internal_get_flag(context, 6), internal_compute_2__logxor(context, internal_get_flag(context, 7), internal_get_flag(context, 11)));
	tmp[0] = temp_a;
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_3_quest(context, tmp[0], internal_compute_zx(context, 8, argtmp[0]), internal_make_num(context, 8, 0, 0));
	do_write_extern_RIPADD(context, &temp_a);
	return 0;  /* success */
}


static int crudasm_intel_execute___jg(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	struct crudasm_temp_node_t tmp[1];
	struct crudasm_temp_node_t argtmp[1];
	tmp[0].size = 1/*BIT*/;
	argtmp[0].size = get_arg_size(context, 0);
	temp_a = internal_compute_1__lognot(context, internal_compute_2__logor(context, internal_get_flag(context, 6), internal_compute_2__logxor(context, internal_get_flag(context, 7), internal_get_flag(context, 11))));
	tmp[0] = temp_a;
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_3_quest(context, tmp[0], internal_compute_zx(context, 8, argtmp[0]), internal_make_num(context, 8, 0, 0));
	do_write_extern_RIPADD(context, &temp_a);
	return 0;  /* success */
}


static int crudasm_intel_execute__jmp(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	struct crudasm_temp_node_t argtmp[1];
	argtmp[0].size = get_arg_size(context, 0);
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_zx(context, 8, argtmp[0]);
	do_write_extern_RIPADD(context, &temp_a);
	return 0;  /* success */
}


static int crudasm_intel_execute_a16___loop(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	struct crudasm_temp_node_t tmp[1];
	struct crudasm_temp_node_t argtmp[2];
	tmp[0].size = 1/*BIT*/;
	argtmp[0].size = get_arg_size(context, 0);
	argtmp[1].size = get_arg_size(context, 1);
	temp_a = internal_compute_2_sub(context, internal_get_reg16(context, 1), internal_make_num(context, 2, 0, 1));
	if(write_gr16_value(context, 1, &temp_a))
		return 1;
	temp_a = internal_compute_1__lognot(context, internal_compute_1__zero(context, internal_get_reg16(context, 1)));
	tmp[0] = temp_a;
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_3_quest(context, tmp[0], internal_compute_zx(context, 8, argtmp[0]), internal_make_num(context, 8, 0, 0));
	do_write_extern_RIPADD(context, &temp_a);
	return 0;  /* success */
}


static int crudasm_intel_execute_a32___loop(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	struct crudasm_temp_node_t tmp[1];
	struct crudasm_temp_node_t argtmp[2];
	tmp[0].size = 1/*BIT*/;
	argtmp[0].size = get_arg_size(context, 0);
	argtmp[1].size = get_arg_size(context, 1);
	temp_a = internal_compute_2_sub(context, internal_get_reg32(context, 1), internal_make_num(context, 4, 0, 1));
	if(write_gr32_value(context, 1, &temp_a))
		return 1;
	temp_a = internal_compute_1__lognot(context, internal_compute_1__zero(context, internal_get_reg32(context, 1)));
	tmp[0] = temp_a;
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_3_quest(context, tmp[0], internal_compute_zx(context, 8, argtmp[0]), internal_make_num(context, 8, 0, 0));
	do_write_extern_RIPADD(context, &temp_a);
	return 0;  /* success */
}


static int crudasm_intel_execute_a64___loop(struct crudasm_intel_context_t *context)  {
	struct crudasm_temp_node_t temp_a;
	struct crudasm_temp_node_t tmp[1];
	struct crudasm_temp_node_t argtmp[2];
	tmp[0].size = 1/*BIT*/;
	argtmp[0].size = get_arg_size(context, 0);
	argtmp[1].size = get_arg_size(context, 1);
	temp_a = internal_compute_2_sub(context, internal_get_reg64(context, 1), internal_make_num(context, 8, 0, 1));
	if(write_gr64_value(context, 1, &temp_a))
		return 1;
	temp_a = internal_compute_1__lognot(context, internal_compute_1__zero(context, internal_get_reg64(context, 1)));
	tmp[0] = temp_a;
	if(crudasm_read_arg(context, 0, argtmp + 0) != 0)
		return 1;
	temp_a = internal_compute_3_quest(context, tmp[0], internal_compute_zx(context, 8, argtmp[0]), internal_make_num(context, 8, 0, 0));
	do_write_extern_RIPADD(context, &temp_a);
	return 0;  /* success */
}

static int crudasm_intel_execute(struct crudasm_intel_context_t *context)  {
	int osz = 16 << context->icode.osz, asz = 16 << context->icode.asz, ssz = 16 << context->ssz;
	switch(crudasm_intel_encoding_table[context->icode.encoding].insn)  {
		case crudasm_intel_insn_aaa:
			if(1)
				return crudasm_intel_execute__aaa(context);
			return 2;  /* internal error */
		case crudasm_intel_insn__aad:
			if(1)
				return crudasm_intel_execute___aad(context);
			return 2;  /* internal error */
		case crudasm_intel_insn__aam:
			if(1)
				return crudasm_intel_execute___aam(context);
			return 2;  /* internal error */
		case crudasm_intel_insn_aas:
			if(1)
				return crudasm_intel_execute__aas(context);
			return 2;  /* internal error */
		case crudasm_intel_insn_das:
			if(1)
				return crudasm_intel_execute__das(context);
			return 2;  /* internal error */
		case crudasm_intel_insn_daa:
			if(1)
				return crudasm_intel_execute__daa(context);
			return 2;  /* internal error */
		case crudasm_intel_insn_adc:
			if(1)
				return crudasm_intel_execute__adc(context);
			return 2;  /* internal error */
		case crudasm_intel_insn_add:
			if(1)
				return crudasm_intel_execute__add(context);
			return 2;  /* internal error */
		case crudasm_intel_insn_or:
			if(1)
				return crudasm_intel_execute__or(context);
			return 2;  /* internal error */
		case crudasm_intel_insn_sbb:
			if(1)
				return crudasm_intel_execute__sbb(context);
			return 2;  /* internal error */
		case crudasm_intel_insn_and:
			if(1)
				return crudasm_intel_execute__and(context);
			return 2;  /* internal error */
		case crudasm_intel_insn_sub:
			if(1)
				return crudasm_intel_execute__sub(context);
			return 2;  /* internal error */
		case crudasm_intel_insn_xor:
			if(1)
				return crudasm_intel_execute__xor(context);
			return 2;  /* internal error */
		case crudasm_intel_insn_cmp:
			if(1)
				return crudasm_intel_execute__cmp(context);
			return 2;  /* internal error */
		case crudasm_intel_insn_rol:
			if(1)
				return crudasm_intel_execute__rol(context);
			return 2;  /* internal error */
		case crudasm_intel_insn_ror:
			if(1)
				return crudasm_intel_execute__ror(context);
			return 2;  /* internal error */
		case crudasm_intel_insn_rcl:
			if(1 && osz == 16)
				return crudasm_intel_execute_o16__rcl(context);
			if(1 && osz == 32)
				return crudasm_intel_execute_o32__rcl(context);
			if(1 && osz == 64)
				return crudasm_intel_execute_o64__rcl(context);
			return 2;  /* internal error */
		case crudasm_intel_insn_rcr:
			if(1 && osz == 16)
				return crudasm_intel_execute_o16__rcr(context);
			if(1 && osz == 32)
				return crudasm_intel_execute_o32__rcr(context);
			if(1 && osz == 64)
				return crudasm_intel_execute_o64__rcr(context);
			return 2;  /* internal error */
		case crudasm_intel_insn_shl:
			if(1 && osz == 16)
				return crudasm_intel_execute_o16__shl(context);
			if(1 && osz == 32)
				return crudasm_intel_execute_o32__shl(context);
			if(1 && osz == 64)
				return crudasm_intel_execute_o64__shl(context);
			return 2;  /* internal error */
		case crudasm_intel_insn_shr:
			if(1 && osz == 16)
				return crudasm_intel_execute_o16__shr(context);
			if(1 && osz == 32)
				return crudasm_intel_execute_o32__shr(context);
			if(1 && osz == 64)
				return crudasm_intel_execute_o64__shr(context);
			return 2;  /* internal error */
		case crudasm_intel_insn__sal:
			if(1 && osz == 16)
				return crudasm_intel_execute_o16___sal(context);
			if(1 && osz == 32)
				return crudasm_intel_execute_o32___sal(context);
			if(1 && osz == 64)
				return crudasm_intel_execute_o64___sal(context);
			return 2;  /* internal error */
		case crudasm_intel_insn_sar:
			if(1 && osz == 16)
				return crudasm_intel_execute_o16__sar(context);
			if(1 && osz == 32)
				return crudasm_intel_execute_o32__sar(context);
			if(1 && osz == 64)
				return crudasm_intel_execute_o64__sar(context);
			return 2;  /* internal error */
		case crudasm_intel_insn_mov:
			if(1)
				return crudasm_intel_execute__mov(context);
			return 2;  /* internal error */
		case crudasm_intel_insn__ins:
			if(1 && asz == 16)
				return crudasm_intel_execute_a16___ins(context);
			if(1 && asz == 32)
				return crudasm_intel_execute_a32___ins(context);
			if(1 && asz == 64)
				return crudasm_intel_execute_a64___ins(context);
			return 2;  /* internal error */
		case crudasm_intel_insn__outs:
			if(1 && asz == 16)
				return crudasm_intel_execute_a16___outs(context);
			if(1 && asz == 32)
				return crudasm_intel_execute_a32___outs(context);
			if(1 && asz == 64)
				return crudasm_intel_execute_a64___outs(context);
			return 2;  /* internal error */
		case crudasm_intel_insn_les:
			if(1 && osz == 16)
				return crudasm_intel_execute_o16__les(context);
			if(1 && osz == 32)
				return crudasm_intel_execute_o32__les(context);
			if(1 && osz == 64)
				return crudasm_intel_execute_o64__les(context);
			return 2;  /* internal error */
		case crudasm_intel_insn_lds:
			if(1 && osz == 16)
				return crudasm_intel_execute_o16__lds(context);
			if(1 && osz == 32)
				return crudasm_intel_execute_o32__lds(context);
			if(1 && osz == 64)
				return crudasm_intel_execute_o64__lds(context);
			return 2;  /* internal error */
		case crudasm_intel_insn_lss:
			if(1 && osz == 16)
				return crudasm_intel_execute_o16__lss(context);
			if(1 && osz == 32)
				return crudasm_intel_execute_o32__lss(context);
			if(1 && osz == 64)
				return crudasm_intel_execute_o64__lss(context);
			return 2;  /* internal error */
		case crudasm_intel_insn_lfs:
			if(1 && osz == 16)
				return crudasm_intel_execute_o16__lfs(context);
			if(1 && osz == 32)
				return crudasm_intel_execute_o32__lfs(context);
			if(1 && osz == 64)
				return crudasm_intel_execute_o64__lfs(context);
			return 2;  /* internal error */
		case crudasm_intel_insn_lgs:
			if(1 && osz == 16)
				return crudasm_intel_execute_o16__lgs(context);
			if(1 && osz == 32)
				return crudasm_intel_execute_o32__lgs(context);
			if(1 && osz == 64)
				return crudasm_intel_execute_o64__lgs(context);
			return 2;  /* internal error */
		case crudasm_intel_insn_movzx:
			if(1)
				return crudasm_intel_execute__movzx(context);
			return 2;  /* internal error */
		case crudasm_intel_insn_movsx:
			if(1)
				return crudasm_intel_execute__movsx(context);
			return 2;  /* internal error */
		case crudasm_intel_insn_enter:
			if(1 && ssz == 16)
				return crudasm_intel_execute_s16__enter(context);
			if(1 && ssz == 32)
				return crudasm_intel_execute_s32__enter(context);
			if(1 && ssz == 64)
				return crudasm_intel_execute_s64__enter(context);
			return 2;  /* internal error */
		case crudasm_intel_insn_leave:
			if(1 && ssz == 16)
				return crudasm_intel_execute_s16__leave(context);
			if(1 && ssz == 32)
				return crudasm_intel_execute_s32__leave(context);
			if(1 && ssz == 64)
				return crudasm_intel_execute_s64__leave(context);
			return 2;  /* internal error */
		case crudasm_intel_insn_in:
			if(1)
				return crudasm_intel_execute__in(context);
			return 2;  /* internal error */
		case crudasm_intel_insn_out:
			if(1)
				return crudasm_intel_execute__out(context);
			return 2;  /* internal error */
		case crudasm_intel_insn__pushsr:
			if(1)
				return crudasm_intel_execute___pushsr(context);
			return 2;  /* internal error */
		case crudasm_intel_insn__popsr:
			if(1)
				return crudasm_intel_execute___popsr(context);
			return 2;  /* internal error */
		case crudasm_intel_insn__push:
			if(1)
				return crudasm_intel_execute___push(context);
			return 2;  /* internal error */
		case crudasm_intel_insn__pop:
			if(1)
				return crudasm_intel_execute___pop(context);
			return 2;  /* internal error */
		case crudasm_intel_insn__movsrv:
			if(1)
				return crudasm_intel_execute___movsrv(context);
			return 2;  /* internal error */
		case crudasm_intel_insn__movvsr:
			if(1)
				return crudasm_intel_execute___movvsr(context);
			return 2;  /* internal error */
		case crudasm_intel_insn__pushf:
			if(1 && osz == 16)
				return crudasm_intel_execute_o16___pushf(context);
			if(1 && osz == 32)
				return crudasm_intel_execute_o32___pushf(context);
			if(1 && osz == 64)
				return crudasm_intel_execute_o64___pushf(context);
			return 2;  /* internal error */
		case crudasm_intel_insn__popf:
			if(1 && osz == 16)
				return crudasm_intel_execute_o16___popf(context);
			if(1 && osz == 32)
				return crudasm_intel_execute_o32___popf(context);
			if(1 && osz == 64)
				return crudasm_intel_execute_o64___popf(context);
			return 2;  /* internal error */
		case crudasm_intel_insn__movs:
			if(1 && asz == 16)
				return crudasm_intel_execute_a16___movs(context);
			if(1 && asz == 32)
				return crudasm_intel_execute_a32___movs(context);
			if(1 && asz == 64)
				return crudasm_intel_execute_a64___movs(context);
			return 2;  /* internal error */
		case crudasm_intel_insn__cmps:
			if(1 && asz == 16)
				return crudasm_intel_execute_a16___cmps(context);
			if(1 && asz == 32)
				return crudasm_intel_execute_a32___cmps(context);
			if(1 && asz == 64)
				return crudasm_intel_execute_a64___cmps(context);
			return 2;  /* internal error */
		case crudasm_intel_insn__stos:
			if(1 && asz == 16)
				return crudasm_intel_execute_a16___stos(context);
			if(1 && asz == 32)
				return crudasm_intel_execute_a32___stos(context);
			if(1 && asz == 64)
				return crudasm_intel_execute_a64___stos(context);
			return 2;  /* internal error */
		case crudasm_intel_insn__lods:
			if(1 && asz == 16)
				return crudasm_intel_execute_a16___lods(context);
			if(1 && asz == 32)
				return crudasm_intel_execute_a32___lods(context);
			if(1 && asz == 64)
				return crudasm_intel_execute_a64___lods(context);
			return 2;  /* internal error */
		case crudasm_intel_insn__scas:
			if(1 && asz == 16)
				return crudasm_intel_execute_a16___scas(context);
			if(1 && asz == 32)
				return crudasm_intel_execute_a32___scas(context);
			if(1 && asz == 64)
				return crudasm_intel_execute_a64___scas(context);
			return 2;  /* internal error */
		case crudasm_intel_insn__test:
			if(1)
				return crudasm_intel_execute___test(context);
			return 2;  /* internal error */
		case crudasm_intel_insn__xchg:
			if(1)
				return crudasm_intel_execute___xchg(context);
			return 2;  /* internal error */
		case crudasm_intel_insn_not:
			if(1)
				return crudasm_intel_execute__not(context);
			return 2;  /* internal error */
		case crudasm_intel_insn_neg:
			if(1)
				return crudasm_intel_execute__neg(context);
			return 2;  /* internal error */
		case crudasm_intel_insn_inc:
			if(1)
				return crudasm_intel_execute__inc(context);
			return 2;  /* internal error */
		case crudasm_intel_insn_dec:
			if(1)
				return crudasm_intel_execute__dec(context);
			return 2;  /* internal error */
		case crudasm_intel_insn__xlat:
			if(1)
				return crudasm_intel_execute___xlat(context);
			return 2;  /* internal error */
		case crudasm_intel_insn__nopmb:
			if(1)
				return crudasm_intel_execute___nopmb(context);
			return 2;  /* internal error */
		case crudasm_intel_insn__sxacc:
			if(1 && osz == 16)
				return crudasm_intel_execute_o16___sxacc(context);
			if(1 && osz == 32)
				return crudasm_intel_execute_o32___sxacc(context);
			if(1 && osz == 64)
				return crudasm_intel_execute_o64___sxacc(context);
			return 2;  /* internal error */
		case crudasm_intel_insn__sxdax:
			if(1 && osz == 16)
				return crudasm_intel_execute_o16___sxdax(context);
			if(1 && osz == 32)
				return crudasm_intel_execute_o32___sxdax(context);
			if(1 && osz == 64)
				return crudasm_intel_execute_o64___sxdax(context);
			return 2;  /* internal error */
		case crudasm_intel_insn_sahf:
			if(1)
				return crudasm_intel_execute__sahf(context);
			return 2;  /* internal error */
		case crudasm_intel_insn_lahf:
			if(1)
				return crudasm_intel_execute__lahf(context);
			return 2;  /* internal error */
		case crudasm_intel_insn_cmc:
			if(1)
				return crudasm_intel_execute__cmc(context);
			return 2;  /* internal error */
		case crudasm_intel_insn_clc:
			if(1)
				return crudasm_intel_execute__clc(context);
			return 2;  /* internal error */
		case crudasm_intel_insn_stc:
			if(1)
				return crudasm_intel_execute__stc(context);
			return 2;  /* internal error */
		case crudasm_intel_insn_cld:
			if(1)
				return crudasm_intel_execute__cld(context);
			return 2;  /* internal error */
		case crudasm_intel_insn_std:
			if(1)
				return crudasm_intel_execute__std(context);
			return 2;  /* internal error */
		case crudasm_intel_insn_lea:
			if(1)
				return crudasm_intel_execute__lea(context);
			return 2;  /* internal error */
		case crudasm_intel_insn_into:
			if(1)
				return crudasm_intel_execute__into(context);
			return 2;  /* internal error */
		case crudasm_intel_insn__cmul3:
			if(1)
				return crudasm_intel_execute___cmul3(context);
			return 2;  /* internal error */
		case crudasm_intel_insn__cmul2:
			if(1)
				return crudasm_intel_execute___cmul2(context);
			return 2;  /* internal error */
		case crudasm_intel_insn__imulb:
			if(1)
				return crudasm_intel_execute___imulb(context);
			return 2;  /* internal error */
		case crudasm_intel_insn_imul:
			if(1 && osz == 16)
				return crudasm_intel_execute_o16__imul(context);
			if(1 && osz == 32)
				return crudasm_intel_execute_o32__imul(context);
			if(1 && osz == 64)
				return crudasm_intel_execute_o64__imul(context);
			return 2;  /* internal error */
		case crudasm_intel_insn__mulb:
			if(1)
				return crudasm_intel_execute___mulb(context);
			return 2;  /* internal error */
		case crudasm_intel_insn_mul:
			if(1 && osz == 16)
				return crudasm_intel_execute_o16__mul(context);
			if(1 && osz == 32)
				return crudasm_intel_execute_o32__mul(context);
			if(1 && osz == 64)
				return crudasm_intel_execute_o64__mul(context);
			return 2;  /* internal error */
		case crudasm_intel_insn__divb:
			if(1)
				return crudasm_intel_execute___divb(context);
			return 2;  /* internal error */
		case crudasm_intel_insn_div:
			if(1 && osz == 16)
				return crudasm_intel_execute_o16__div(context);
			if(1 && osz == 32)
				return crudasm_intel_execute_o32__div(context);
			if(1 && osz == 64)
				return crudasm_intel_execute_o64__div(context);
			return 2;  /* internal error */
		case crudasm_intel_insn__idivb:
			if(1)
				return crudasm_intel_execute___idivb(context);
			return 2;  /* internal error */
		case crudasm_intel_insn_idiv:
			if(1 && osz == 16)
				return crudasm_intel_execute_o16__idiv(context);
			if(1 && osz == 32)
				return crudasm_intel_execute_o32__idiv(context);
			if(1 && osz == 64)
				return crudasm_intel_execute_o64__idiv(context);
			return 2;  /* internal error */
		case crudasm_intel_insn__jo:
			if(1)
				return crudasm_intel_execute___jo(context);
			return 2;  /* internal error */
		case crudasm_intel_insn__jno:
			if(1)
				return crudasm_intel_execute___jno(context);
			return 2;  /* internal error */
		case crudasm_intel_insn__jc:
			if(1)
				return crudasm_intel_execute___jc(context);
			return 2;  /* internal error */
		case crudasm_intel_insn__jnc:
			if(1)
				return crudasm_intel_execute___jnc(context);
			return 2;  /* internal error */
		case crudasm_intel_insn__jz:
			if(1)
				return crudasm_intel_execute___jz(context);
			return 2;  /* internal error */
		case crudasm_intel_insn__jnz:
			if(1)
				return crudasm_intel_execute___jnz(context);
			return 2;  /* internal error */
		case crudasm_intel_insn__jbe:
			if(1)
				return crudasm_intel_execute___jbe(context);
			return 2;  /* internal error */
		case crudasm_intel_insn__ja:
			if(1)
				return crudasm_intel_execute___ja(context);
			return 2;  /* internal error */
		case crudasm_intel_insn__js:
			if(1)
				return crudasm_intel_execute___js(context);
			return 2;  /* internal error */
		case crudasm_intel_insn__jns:
			if(1)
				return crudasm_intel_execute___jns(context);
			return 2;  /* internal error */
		case crudasm_intel_insn__jp:
			if(1)
				return crudasm_intel_execute___jp(context);
			return 2;  /* internal error */
		case crudasm_intel_insn__jnp:
			if(1)
				return crudasm_intel_execute___jnp(context);
			return 2;  /* internal error */
		case crudasm_intel_insn__jl:
			if(1)
				return crudasm_intel_execute___jl(context);
			return 2;  /* internal error */
		case crudasm_intel_insn__jge:
			if(1)
				return crudasm_intel_execute___jge(context);
			return 2;  /* internal error */
		case crudasm_intel_insn__jle:
			if(1)
				return crudasm_intel_execute___jle(context);
			return 2;  /* internal error */
		case crudasm_intel_insn__jg:
			if(1)
				return crudasm_intel_execute___jg(context);
			return 2;  /* internal error */
		case crudasm_intel_insn_jmp:
			if(1)
				return crudasm_intel_execute__jmp(context);
			return 2;  /* internal error */
		case crudasm_intel_insn__loop:
			if(1 && asz == 16)
				return crudasm_intel_execute_a16___loop(context);
			if(1 && asz == 32)
				return crudasm_intel_execute_a32___loop(context);
			if(1 && asz == 64)
				return crudasm_intel_execute_a64___loop(context);
			return 2;  /* internal error */
		default:  break;
	}
	return -1;  /* no semantics */
}

/*
internal_compute_1__lognot
internal_compute_1__lsb
internal_compute_1__parity
internal_compute_1__sign
internal_compute_1__sign2
internal_compute_1__zero
internal_compute_1_bitnot
internal_compute_1_neg
internal_compute_1_ofs16
internal_compute_1_ofs32
internal_compute_1_ofs64
internal_compute_1_seg16
internal_compute_1_seg32
internal_compute_1_seg64
internal_compute_2__add_af
internal_compute_2__add_cf
internal_compute_2__add_of
internal_compute_2__eq
internal_compute_2__gtu
internal_compute_2__logor
internal_compute_2__logxor
internal_compute_2__ltu
internal_compute_2__sar_cf
internal_compute_2__shl_cf
internal_compute_2__shr_cf
internal_compute_2__smul_overflow
internal_compute_2__sub_af
internal_compute_2__sub_cf
internal_compute_2__sub_of
internal_compute_2__umul_overflow
internal_compute_2_add
internal_compute_2_bitand
internal_compute_2_bitor
internal_compute_2_bitxor
internal_compute_2_cmul
internal_compute_2_rol
internal_compute_2_ror
internal_compute_2_sdiv
internal_compute_2_shl
internal_compute_2_shrs
internal_compute_2_shru
internal_compute_2_smod
internal_compute_2_smulhi
internal_compute_2_sub
internal_compute_2_udiv
internal_compute_2_umod
internal_compute_2_umulhi
internal_compute_3__adc_af
internal_compute_3__adc_cf
internal_compute_3__adc_of
internal_compute_3__quest
internal_compute_3__rcl_cf
internal_compute_3__rcr_cf
internal_compute_3__sbb_af
internal_compute_3__sbb_cf
internal_compute_3__sbb_of
internal_compute_3_quest
internal_compute_3_rcl
internal_compute_3_rcr
internal_compute_3_sdiv64
internal_compute_3_smod64
internal_compute_3_udiv64
internal_compute_3_umod64
internal_compute_sx
internal_compute_tr
internal_compute_zx
*/

