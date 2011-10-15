// out_semantics.h  (note: this is an automatically generated file - do not edit!)
// Copyright (C) 2008 Willow Schlanger

tnode x86_tcode_nodes[] = {

// _cmovcc
{nt_literal, ns_void, {0, 0, 0}},	// 0
{nt_arg, ns_void, {0, no_void|0x80000000, no_void|0x80000000}},	// 1
{nt_literal, ns_void, {2, 0, 0}},	// 2
{nt_arg, ns_void, {2, no_void|0x80000000, no_void|0x80000000}},	// 3
{nt__x86_cc, ns_void, {3, no_void|0x80000000, no_void|0x80000000}},	// 4
{nt_literal, ns_void, {1, 0, 0}},	// 5
{nt_arg, ns_void, {5, no_void|0x80000000, no_void|0x80000000}},	// 6
{nt_literal, ns_void, {0, 0, 0}},	// 7
{nt_arg, ns_void, {7, no_void|0x80000000, no_void|0x80000000}},	// 8
{nt_quest, ns_void, {4, 6, 8}},	// 9
{nt_asgn, ns_void, {1, 9, no_void|0x80000000}},	// 10

// _setcc
{nt_literal, ns_void, {0, 0, 0}},	// 11
{nt_arg, ns_void, {11, no_void|0x80000000, no_void|0x80000000}},	// 12
{nt_literal, ns_void, {1, 0, 0}},	// 13
{nt_arg, ns_void, {13, no_void|0x80000000, no_void|0x80000000}},	// 14
{nt__x86_cc, ns_void, {14, no_void|0x80000000, no_void|0x80000000}},	// 15
{nt_zx, ns_byte, {15, no_void|0x80000000, no_void|0x80000000}},	// 16
{nt_asgn, ns_void, {12, 16, no_void|0x80000000}},	// 17

// _nopmb
{nt_asgn, ns_void, {no_void|0x80000000, no_void|0x80000000, no_void|0x80000000}},	// 18

// _sxacc
{nt_sx, ns_osz, {no_x86_acc_lo|0x80000000, no_void|0x80000000, no_void|0x80000000}},	// 19
{nt_asgn, ns_void, {no_x86_acc|0x80000000, 19, no_void|0x80000000}},	// 20

// _sxdax
{nt_sx, ns_osz_times_2, {no_x86_acc|0x80000000, no_void|0x80000000, no_void|0x80000000}},	// 21
{nt_asgn, ns_void, {no_x86_dax|0x80000000, 21, no_void|0x80000000}},	// 22

// cmc
{nt_not, ns_void, {no_x86_cf|0x80000000, no_void|0x80000000, no_void|0x80000000}},	// 23
{nt_asgn, ns_void, {no_x86_cf|0x80000000, 23, no_void|0x80000000}},	// 24

// clc
{nt_literal, ns_void, {0, 0, 0}},	// 25
{nt_asgn, ns_void, {no_x86_cf|0x80000000, 25, no_void|0x80000000}},	// 26

// stc
{nt_literal, ns_void, {1, 0, 0}},	// 27
{nt_asgn, ns_void, {no_x86_cf|0x80000000, 27, no_void|0x80000000}},	// 28

// cld
{nt_literal, ns_void, {0, 0, 0}},	// 29
{nt_asgn, ns_void, {no_x86_df|0x80000000, 29, no_void|0x80000000}},	// 30

// std
{nt_literal, ns_void, {1, 0, 0}},	// 31
{nt_asgn, ns_void, {no_x86_cf|0x80000000, 31, no_void|0x80000000}},	// 32

// lea
{nt_literal, ns_void, {0, 0, 0}},	// 33
{nt_arg, ns_void, {33, no_void|0x80000000, no_void|0x80000000}},	// 34
{nt_literal, ns_void, {1, 0, 0}},	// 35
{nt_arg, ns_void, {35, no_void|0x80000000, no_void|0x80000000}},	// 36
{nt_asgn, ns_void, {34, 36, no_void|0x80000000}},	// 37

// _cmul3
{nt_literal, ns_void, {0, 0, 0}},	// 38
{nt_arg, ns_void, {38, no_void|0x80000000, no_void|0x80000000}},	// 39
{nt_literal, ns_void, {1, 0, 0}},	// 40
{nt_arg, ns_void, {40, no_void|0x80000000, no_void|0x80000000}},	// 41
{nt_literal, ns_void, {2, 0, 0}},	// 42
{nt_arg, ns_void, {42, no_void|0x80000000, no_void|0x80000000}},	// 43
{nt_cmul, ns_void, {41, 43, no_void|0x80000000}},	// 44
{nt_asgn, ns_void, {39, 44, no_void|0x80000000}},	// 45
{nt_asgn, ns_void, {no_x86_sf|0x80000000, no_undefined|0x80000000, no_void|0x80000000}},	// 46
{nt_asgn, ns_void, {no_x86_zf|0x80000000, no_undefined|0x80000000, no_void|0x80000000}},	// 47
{nt_asgn, ns_void, {no_x86_af|0x80000000, no_undefined|0x80000000, no_void|0x80000000}},	// 48
{nt_asgn, ns_void, {no_x86_pf|0x80000000, no_undefined|0x80000000, no_void|0x80000000}},	// 49
{nt_literal, ns_void, {1, 0, 0}},	// 50
{nt_arg, ns_void, {50, no_void|0x80000000, no_void|0x80000000}},	// 51
{nt_literal, ns_void, {2, 0, 0}},	// 52
{nt_arg, ns_void, {52, no_void|0x80000000, no_void|0x80000000}},	// 53
{nt_smul_overflow, ns_void, {51, 53, no_void|0x80000000}},	// 54
{nt_asgn, ns_void, {no_x86_cf|0x80000000, 54, no_void|0x80000000}},	// 55
{nt_literal, ns_void, {1, 0, 0}},	// 56
{nt_arg, ns_void, {56, no_void|0x80000000, no_void|0x80000000}},	// 57
{nt_literal, ns_void, {2, 0, 0}},	// 58
{nt_arg, ns_void, {58, no_void|0x80000000, no_void|0x80000000}},	// 59
{nt_smul_overflow, ns_void, {57, 59, no_void|0x80000000}},	// 60
{nt_asgn, ns_void, {no_x86_of|0x80000000, 60, no_void|0x80000000}},	// 61

// _cmul2
{nt_literal, ns_void, {0, 0, 0}},	// 62
{nt_arg, ns_void, {62, no_void|0x80000000, no_void|0x80000000}},	// 63
{nt_literal, ns_void, {0, 0, 0}},	// 64
{nt_arg, ns_void, {64, no_void|0x80000000, no_void|0x80000000}},	// 65
{nt_literal, ns_void, {1, 0, 0}},	// 66
{nt_arg, ns_void, {66, no_void|0x80000000, no_void|0x80000000}},	// 67
{nt_cmul, ns_void, {65, 67, no_void|0x80000000}},	// 68
{nt_asgn, ns_void, {63, 68, no_void|0x80000000}},	// 69
{nt_asgn, ns_void, {no_x86_sf|0x80000000, no_undefined|0x80000000, no_void|0x80000000}},	// 70
{nt_asgn, ns_void, {no_x86_zf|0x80000000, no_undefined|0x80000000, no_void|0x80000000}},	// 71
{nt_asgn, ns_void, {no_x86_af|0x80000000, no_undefined|0x80000000, no_void|0x80000000}},	// 72
{nt_asgn, ns_void, {no_x86_pf|0x80000000, no_undefined|0x80000000, no_void|0x80000000}},	// 73
{nt_literal, ns_void, {0, 0, 0}},	// 74
{nt_arg, ns_void, {74, no_void|0x80000000, no_void|0x80000000}},	// 75
{nt_literal, ns_void, {1, 0, 0}},	// 76
{nt_arg, ns_void, {76, no_void|0x80000000, no_void|0x80000000}},	// 77
{nt_smul_overflow, ns_void, {75, 77, no_void|0x80000000}},	// 78
{nt_asgn, ns_void, {no_x86_cf|0x80000000, 78, no_void|0x80000000}},	// 79
{nt_literal, ns_void, {0, 0, 0}},	// 80
{nt_arg, ns_void, {80, no_void|0x80000000, no_void|0x80000000}},	// 81
{nt_literal, ns_void, {1, 0, 0}},	// 82
{nt_arg, ns_void, {82, no_void|0x80000000, no_void|0x80000000}},	// 83
{nt_smul_overflow, ns_void, {81, 83, no_void|0x80000000}},	// 84
{nt_asgn, ns_void, {no_x86_of|0x80000000, 84, no_void|0x80000000}},	// 85

// _imulb
{nt_literal, ns_void, {0, 0, 0}},	// 86
{nt_arg, ns_void, {86, no_void|0x80000000, no_void|0x80000000}},	// 87
{nt_smul, ns_word, {no_x86_al|0x80000000, 87, no_void|0x80000000}},	// 88
{nt_asgn, ns_void, {no_x86_ax|0x80000000, 88, no_void|0x80000000}},	// 89
{nt_asgn, ns_void, {no_x86_sf|0x80000000, no_undefined|0x80000000, no_void|0x80000000}},	// 90
{nt_asgn, ns_void, {no_x86_zf|0x80000000, no_undefined|0x80000000, no_void|0x80000000}},	// 91
{nt_asgn, ns_void, {no_x86_af|0x80000000, no_undefined|0x80000000, no_void|0x80000000}},	// 92
{nt_asgn, ns_void, {no_x86_pf|0x80000000, no_undefined|0x80000000, no_void|0x80000000}},	// 93
{nt_literal, ns_void, {0, 0, 0}},	// 94
{nt_arg, ns_void, {94, no_void|0x80000000, no_void|0x80000000}},	// 95
{nt_smul_overflow, ns_void, {no_x86_al|0x80000000, 95, no_void|0x80000000}},	// 96
{nt_asgn, ns_void, {no_x86_cf|0x80000000, 96, no_void|0x80000000}},	// 97
{nt_literal, ns_void, {0, 0, 0}},	// 98
{nt_arg, ns_void, {98, no_void|0x80000000, no_void|0x80000000}},	// 99
{nt_smul_overflow, ns_void, {no_x86_al|0x80000000, 99, no_void|0x80000000}},	// 100
{nt_asgn, ns_void, {no_x86_of|0x80000000, 100, no_void|0x80000000}},	// 101

// imul
{nt_literal, ns_void, {0, 0, 0}},	// 102
{nt_arg, ns_void, {102, no_void|0x80000000, no_void|0x80000000}},	// 103
{nt_smul, ns_osz_times_2, {no_x86_acc|0x80000000, 103, no_void|0x80000000}},	// 104
{nt_asgn, ns_void, {no_x86_dax|0x80000000, 104, no_void|0x80000000}},	// 105
{nt_asgn, ns_void, {no_x86_sf|0x80000000, no_undefined|0x80000000, no_void|0x80000000}},	// 106
{nt_asgn, ns_void, {no_x86_zf|0x80000000, no_undefined|0x80000000, no_void|0x80000000}},	// 107
{nt_asgn, ns_void, {no_x86_af|0x80000000, no_undefined|0x80000000, no_void|0x80000000}},	// 108
{nt_asgn, ns_void, {no_x86_pf|0x80000000, no_undefined|0x80000000, no_void|0x80000000}},	// 109
{nt_literal, ns_void, {0, 0, 0}},	// 110
{nt_arg, ns_void, {110, no_void|0x80000000, no_void|0x80000000}},	// 111
{nt_smul_overflow, ns_void, {no_x86_acc|0x80000000, 111, no_void|0x80000000}},	// 112
{nt_asgn, ns_void, {no_x86_cf|0x80000000, 112, no_void|0x80000000}},	// 113
{nt_literal, ns_void, {0, 0, 0}},	// 114
{nt_arg, ns_void, {114, no_void|0x80000000, no_void|0x80000000}},	// 115
{nt_smul_overflow, ns_void, {no_x86_acc|0x80000000, 115, no_void|0x80000000}},	// 116
{nt_asgn, ns_void, {no_x86_of|0x80000000, 116, no_void|0x80000000}},	// 117

// _mulb
{nt_literal, ns_void, {0, 0, 0}},	// 118
{nt_arg, ns_void, {118, no_void|0x80000000, no_void|0x80000000}},	// 119
{nt_umul, ns_word, {no_x86_al|0x80000000, 119, no_void|0x80000000}},	// 120
{nt_asgn, ns_void, {no_x86_ax|0x80000000, 120, no_void|0x80000000}},	// 121
{nt_asgn, ns_void, {no_x86_sf|0x80000000, no_undefined|0x80000000, no_void|0x80000000}},	// 122
{nt_asgn, ns_void, {no_x86_zf|0x80000000, no_undefined|0x80000000, no_void|0x80000000}},	// 123
{nt_asgn, ns_void, {no_x86_af|0x80000000, no_undefined|0x80000000, no_void|0x80000000}},	// 124
{nt_asgn, ns_void, {no_x86_pf|0x80000000, no_undefined|0x80000000, no_void|0x80000000}},	// 125
{nt_literal, ns_void, {0, 0, 0}},	// 126
{nt_arg, ns_void, {126, no_void|0x80000000, no_void|0x80000000}},	// 127
{nt_umul_overflow, ns_void, {no_x86_al|0x80000000, 127, no_void|0x80000000}},	// 128
{nt_asgn, ns_void, {no_x86_cf|0x80000000, 128, no_void|0x80000000}},	// 129
{nt_literal, ns_void, {0, 0, 0}},	// 130
{nt_arg, ns_void, {130, no_void|0x80000000, no_void|0x80000000}},	// 131
{nt_umul_overflow, ns_void, {no_x86_al|0x80000000, 131, no_void|0x80000000}},	// 132
{nt_asgn, ns_void, {no_x86_of|0x80000000, 132, no_void|0x80000000}},	// 133

// mul
{nt_literal, ns_void, {0, 0, 0}},	// 134
{nt_arg, ns_void, {134, no_void|0x80000000, no_void|0x80000000}},	// 135
{nt_umul, ns_osz_times_2, {no_x86_acc|0x80000000, 135, no_void|0x80000000}},	// 136
{nt_asgn, ns_void, {no_x86_dax|0x80000000, 136, no_void|0x80000000}},	// 137
{nt_asgn, ns_void, {no_x86_sf|0x80000000, no_undefined|0x80000000, no_void|0x80000000}},	// 138
{nt_asgn, ns_void, {no_x86_zf|0x80000000, no_undefined|0x80000000, no_void|0x80000000}},	// 139
{nt_asgn, ns_void, {no_x86_af|0x80000000, no_undefined|0x80000000, no_void|0x80000000}},	// 140
{nt_asgn, ns_void, {no_x86_pf|0x80000000, no_undefined|0x80000000, no_void|0x80000000}},	// 141
{nt_literal, ns_void, {0, 0, 0}},	// 142
{nt_arg, ns_void, {142, no_void|0x80000000, no_void|0x80000000}},	// 143
{nt_umul_overflow, ns_void, {no_x86_acc|0x80000000, 143, no_void|0x80000000}},	// 144
{nt_asgn, ns_void, {no_x86_cf|0x80000000, 144, no_void|0x80000000}},	// 145
{nt_literal, ns_void, {0, 0, 0}},	// 146
{nt_arg, ns_void, {146, no_void|0x80000000, no_void|0x80000000}},	// 147
{nt_umul_overflow, ns_void, {no_x86_acc|0x80000000, 147, no_void|0x80000000}},	// 148
{nt_asgn, ns_void, {no_x86_of|0x80000000, 148, no_void|0x80000000}},	// 149

// _divb
{nt_literal, ns_void, {0, 0, 0}},	// 150
{nt_tmp, ns_void, {150, no_void|0x80000000, no_void|0x80000000}},	// 151
{nt_literal, ns_void, {0, 0, 0}},	// 152
{nt_arg, ns_void, {152, no_void|0x80000000, no_void|0x80000000}},	// 153
{nt_udiv, ns_byte, {no_x86_ax|0x80000000, 153, no_void|0x80000000}},	// 154
{nt_asgn, ns_void, {151, 154, no_void|0x80000000}},	// 155
{nt_literal, ns_void, {1, 0, 0}},	// 156
{nt_tmp, ns_void, {156, no_void|0x80000000, no_void|0x80000000}},	// 157
{nt_literal, ns_void, {0, 0, 0}},	// 158
{nt_arg, ns_void, {158, no_void|0x80000000, no_void|0x80000000}},	// 159
{nt_umod, ns_byte, {no_x86_ax|0x80000000, 159, no_void|0x80000000}},	// 160
{nt_asgn, ns_void, {157, 160, no_void|0x80000000}},	// 161
{nt_literal, ns_void, {0, 0, 0}},	// 162
{nt_tmp, ns_void, {162, no_void|0x80000000, no_void|0x80000000}},	// 163
{nt_asgn, ns_void, {no_x86_al|0x80000000, 163, no_void|0x80000000}},	// 164
{nt_literal, ns_void, {1, 0, 0}},	// 165
{nt_tmp, ns_void, {165, no_void|0x80000000, no_void|0x80000000}},	// 166
{nt_asgn, ns_void, {no_x86_ah|0x80000000, 166, no_void|0x80000000}},	// 167
{nt_asgn, ns_void, {no_x86_of|0x80000000, no_undefined|0x80000000, no_void|0x80000000}},	// 168
{nt_asgn, ns_void, {no_x86_sf|0x80000000, no_undefined|0x80000000, no_void|0x80000000}},	// 169
{nt_asgn, ns_void, {no_x86_zf|0x80000000, no_undefined|0x80000000, no_void|0x80000000}},	// 170
{nt_asgn, ns_void, {no_x86_af|0x80000000, no_undefined|0x80000000, no_void|0x80000000}},	// 171
{nt_asgn, ns_void, {no_x86_pf|0x80000000, no_undefined|0x80000000, no_void|0x80000000}},	// 172
{nt_asgn, ns_void, {no_x86_cf|0x80000000, no_undefined|0x80000000, no_void|0x80000000}},	// 173

// div
{nt_literal, ns_void, {0, 0, 0}},	// 174
{nt_tmp, ns_void, {174, no_void|0x80000000, no_void|0x80000000}},	// 175
{nt_literal, ns_void, {0, 0, 0}},	// 176
{nt_arg, ns_void, {176, no_void|0x80000000, no_void|0x80000000}},	// 177
{nt_udiv, ns_osz, {no_x86_dax|0x80000000, 177, no_void|0x80000000}},	// 178
{nt_asgn, ns_void, {175, 178, no_void|0x80000000}},	// 179
{nt_literal, ns_void, {1, 0, 0}},	// 180
{nt_tmp, ns_void, {180, no_void|0x80000000, no_void|0x80000000}},	// 181
{nt_literal, ns_void, {0, 0, 0}},	// 182
{nt_arg, ns_void, {182, no_void|0x80000000, no_void|0x80000000}},	// 183
{nt_umod, ns_osz, {no_x86_dax|0x80000000, 183, no_void|0x80000000}},	// 184
{nt_asgn, ns_void, {181, 184, no_void|0x80000000}},	// 185
{nt_literal, ns_void, {0, 0, 0}},	// 186
{nt_tmp, ns_void, {186, no_void|0x80000000, no_void|0x80000000}},	// 187
{nt_asgn, ns_void, {no_x86_acc|0x80000000, 187, no_void|0x80000000}},	// 188
{nt_literal, ns_void, {1, 0, 0}},	// 189
{nt_tmp, ns_void, {189, no_void|0x80000000, no_void|0x80000000}},	// 190
{nt_asgn, ns_void, {no_x86_dat|0x80000000, 190, no_void|0x80000000}},	// 191
{nt_asgn, ns_void, {no_x86_of|0x80000000, no_undefined|0x80000000, no_void|0x80000000}},	// 192
{nt_asgn, ns_void, {no_x86_sf|0x80000000, no_undefined|0x80000000, no_void|0x80000000}},	// 193
{nt_asgn, ns_void, {no_x86_zf|0x80000000, no_undefined|0x80000000, no_void|0x80000000}},	// 194
{nt_asgn, ns_void, {no_x86_af|0x80000000, no_undefined|0x80000000, no_void|0x80000000}},	// 195
{nt_asgn, ns_void, {no_x86_pf|0x80000000, no_undefined|0x80000000, no_void|0x80000000}},	// 196
{nt_asgn, ns_void, {no_x86_cf|0x80000000, no_undefined|0x80000000, no_void|0x80000000}},	// 197

// _idivb
{nt_literal, ns_void, {0, 0, 0}},	// 198
{nt_tmp, ns_void, {198, no_void|0x80000000, no_void|0x80000000}},	// 199
{nt_literal, ns_void, {0, 0, 0}},	// 200
{nt_arg, ns_void, {200, no_void|0x80000000, no_void|0x80000000}},	// 201
{nt_sdiv, ns_byte, {no_x86_ax|0x80000000, 201, no_void|0x80000000}},	// 202
{nt_asgn, ns_void, {199, 202, no_void|0x80000000}},	// 203
{nt_literal, ns_void, {1, 0, 0}},	// 204
{nt_tmp, ns_void, {204, no_void|0x80000000, no_void|0x80000000}},	// 205
{nt_literal, ns_void, {0, 0, 0}},	// 206
{nt_arg, ns_void, {206, no_void|0x80000000, no_void|0x80000000}},	// 207
{nt_smod, ns_byte, {no_x86_ax|0x80000000, 207, no_void|0x80000000}},	// 208
{nt_asgn, ns_void, {205, 208, no_void|0x80000000}},	// 209
{nt_literal, ns_void, {0, 0, 0}},	// 210
{nt_tmp, ns_void, {210, no_void|0x80000000, no_void|0x80000000}},	// 211
{nt_asgn, ns_void, {no_x86_al|0x80000000, 211, no_void|0x80000000}},	// 212
{nt_literal, ns_void, {1, 0, 0}},	// 213
{nt_tmp, ns_void, {213, no_void|0x80000000, no_void|0x80000000}},	// 214
{nt_asgn, ns_void, {no_x86_ah|0x80000000, 214, no_void|0x80000000}},	// 215
{nt_asgn, ns_void, {no_x86_of|0x80000000, no_undefined|0x80000000, no_void|0x80000000}},	// 216
{nt_asgn, ns_void, {no_x86_sf|0x80000000, no_undefined|0x80000000, no_void|0x80000000}},	// 217
{nt_asgn, ns_void, {no_x86_zf|0x80000000, no_undefined|0x80000000, no_void|0x80000000}},	// 218
{nt_asgn, ns_void, {no_x86_af|0x80000000, no_undefined|0x80000000, no_void|0x80000000}},	// 219
{nt_asgn, ns_void, {no_x86_pf|0x80000000, no_undefined|0x80000000, no_void|0x80000000}},	// 220
{nt_asgn, ns_void, {no_x86_cf|0x80000000, no_undefined|0x80000000, no_void|0x80000000}},	// 221

// idiv
{nt_literal, ns_void, {0, 0, 0}},	// 222
{nt_tmp, ns_void, {222, no_void|0x80000000, no_void|0x80000000}},	// 223
{nt_literal, ns_void, {0, 0, 0}},	// 224
{nt_arg, ns_void, {224, no_void|0x80000000, no_void|0x80000000}},	// 225
{nt_sdiv, ns_osz, {no_x86_dax|0x80000000, 225, no_void|0x80000000}},	// 226
{nt_asgn, ns_void, {223, 226, no_void|0x80000000}},	// 227
{nt_literal, ns_void, {1, 0, 0}},	// 228
{nt_tmp, ns_void, {228, no_void|0x80000000, no_void|0x80000000}},	// 229
{nt_literal, ns_void, {0, 0, 0}},	// 230
{nt_arg, ns_void, {230, no_void|0x80000000, no_void|0x80000000}},	// 231
{nt_smod, ns_osz, {no_x86_dax|0x80000000, 231, no_void|0x80000000}},	// 232
{nt_asgn, ns_void, {229, 232, no_void|0x80000000}},	// 233
{nt_literal, ns_void, {0, 0, 0}},	// 234
{nt_tmp, ns_void, {234, no_void|0x80000000, no_void|0x80000000}},	// 235
{nt_asgn, ns_void, {no_x86_acc|0x80000000, 235, no_void|0x80000000}},	// 236
{nt_literal, ns_void, {1, 0, 0}},	// 237
{nt_tmp, ns_void, {237, no_void|0x80000000, no_void|0x80000000}},	// 238
{nt_asgn, ns_void, {no_x86_dat|0x80000000, 238, no_void|0x80000000}},	// 239
{nt_asgn, ns_void, {no_x86_of|0x80000000, no_undefined|0x80000000, no_void|0x80000000}},	// 240
{nt_asgn, ns_void, {no_x86_sf|0x80000000, no_undefined|0x80000000, no_void|0x80000000}},	// 241
{nt_asgn, ns_void, {no_x86_zf|0x80000000, no_undefined|0x80000000, no_void|0x80000000}},	// 242
{nt_asgn, ns_void, {no_x86_af|0x80000000, no_undefined|0x80000000, no_void|0x80000000}},	// 243
{nt_asgn, ns_void, {no_x86_pf|0x80000000, no_undefined|0x80000000, no_void|0x80000000}},	// 244
{nt_asgn, ns_void, {no_x86_cf|0x80000000, no_undefined|0x80000000, no_void|0x80000000}},	// 245

// test
{nt_literal, ns_void, {0, 0, 0}},	// 246
{nt_asgn, ns_void, {no_x86_of|0x80000000, 246, no_void|0x80000000}},	// 247
{nt_asgn, ns_void, {no_x86_af|0x80000000, no_undefined|0x80000000, no_void|0x80000000}},	// 248
{nt_literal, ns_void, {0, 0, 0}},	// 249
{nt_asgn, ns_void, {no_x86_cf|0x80000000, 249, no_void|0x80000000}},	// 250
{nt_literal, ns_void, {0, 0, 0}},	// 251
{nt_tmp, ns_void, {251, no_void|0x80000000, no_void|0x80000000}},	// 252
{nt_literal, ns_void, {0, 0, 0}},	// 253
{nt_arg, ns_void, {253, no_void|0x80000000, no_void|0x80000000}},	// 254
{nt_literal, ns_void, {1, 0, 0}},	// 255
{nt_arg, ns_void, {255, no_void|0x80000000, no_void|0x80000000}},	// 256
{nt_bitand, ns_void, {254, 256, no_void|0x80000000}},	// 257
{nt_asgn, ns_void, {252, 257, no_void|0x80000000}},	// 258
{nt_literal, ns_void, {0, 0, 0}},	// 259
{nt_tmp, ns_void, {259, no_void|0x80000000, no_void|0x80000000}},	// 260
{nt_sign, ns_void, {260, no_void|0x80000000, no_void|0x80000000}},	// 261
{nt_asgn, ns_void, {no_x86_sf|0x80000000, 261, no_void|0x80000000}},	// 262
{nt_literal, ns_void, {0, 0, 0}},	// 263
{nt_tmp, ns_void, {263, no_void|0x80000000, no_void|0x80000000}},	// 264
{nt_is_zero, ns_void, {264, no_void|0x80000000, no_void|0x80000000}},	// 265
{nt_asgn, ns_void, {no_x86_zf|0x80000000, 265, no_void|0x80000000}},	// 266
{nt_literal, ns_void, {0, 0, 0}},	// 267
{nt_tmp, ns_void, {267, no_void|0x80000000, no_void|0x80000000}},	// 268
{nt_trunc, ns_byte, {268, no_void|0x80000000, no_void|0x80000000}},	// 269
{nt__x86_parity, ns_void, {269, no_void|0x80000000, no_void|0x80000000}},	// 270
{nt_asgn, ns_void, {no_x86_pf|0x80000000, 270, no_void|0x80000000}},	// 271

// _xchg
{nt_literal, ns_void, {0, 0, 0}},	// 272
{nt_tmp, ns_void, {272, no_void|0x80000000, no_void|0x80000000}},	// 273
{nt_literal, ns_void, {0, 0, 0}},	// 274
{nt_arg, ns_void, {274, no_void|0x80000000, no_void|0x80000000}},	// 275
{nt_asgn, ns_void, {273, 275, no_void|0x80000000}},	// 276
{nt_literal, ns_void, {0, 0, 0}},	// 277
{nt_arg, ns_void, {277, no_void|0x80000000, no_void|0x80000000}},	// 278
{nt_literal, ns_void, {1, 0, 0}},	// 279
{nt_arg, ns_void, {279, no_void|0x80000000, no_void|0x80000000}},	// 280
{nt_asgn, ns_void, {278, 280, no_void|0x80000000}},	// 281
{nt_literal, ns_void, {1, 0, 0}},	// 282
{nt_arg, ns_void, {282, no_void|0x80000000, no_void|0x80000000}},	// 283
{nt_literal, ns_void, {0, 0, 0}},	// 284
{nt_tmp, ns_void, {284, no_void|0x80000000, no_void|0x80000000}},	// 285
{nt_asgn, ns_void, {283, 285, no_void|0x80000000}},	// 286

// mov
{nt_literal, ns_void, {0, 0, 0}},	// 287
{nt_arg, ns_void, {287, no_void|0x80000000, no_void|0x80000000}},	// 288
{nt_literal, ns_void, {1, 0, 0}},	// 289
{nt_arg, ns_void, {289, no_void|0x80000000, no_void|0x80000000}},	// 290
{nt_asgn, ns_void, {288, 290, no_void|0x80000000}},	// 291

// not
{nt_literal, ns_void, {0, 0, 0}},	// 292
{nt_arg, ns_void, {292, no_void|0x80000000, no_void|0x80000000}},	// 293
{nt_literal, ns_void, {0, 0, 0}},	// 294
{nt_arg, ns_void, {294, no_void|0x80000000, no_void|0x80000000}},	// 295
{nt_bitnot, ns_void, {295, no_void|0x80000000, no_void|0x80000000}},	// 296
{nt_asgn, ns_void, {293, 296, no_void|0x80000000}},	// 297

// neg
{nt_literal, ns_void, {0, 0, 0}},	// 298
{nt_literal, ns_void, {0, 0, 0}},	// 299
{nt_arg, ns_void, {299, no_void|0x80000000, no_void|0x80000000}},	// 300
{nt__x86_sub_of, ns_void, {298, 300, no_void|0x80000000}},	// 301
{nt_asgn, ns_void, {no_x86_of|0x80000000, 301, no_void|0x80000000}},	// 302
{nt_literal, ns_void, {0, 0, 0}},	// 303
{nt_literal, ns_void, {0, 0, 0}},	// 304
{nt_arg, ns_void, {304, no_void|0x80000000, no_void|0x80000000}},	// 305
{nt_trunc, ns_byte, {305, no_void|0x80000000, no_void|0x80000000}},	// 306
{nt__x86_sub_af, ns_void, {303, 306, no_void|0x80000000}},	// 307
{nt_asgn, ns_void, {no_x86_af|0x80000000, 307, no_void|0x80000000}},	// 308
{nt_literal, ns_void, {0, 0, 0}},	// 309
{nt_literal, ns_void, {0, 0, 0}},	// 310
{nt_arg, ns_void, {310, no_void|0x80000000, no_void|0x80000000}},	// 311
{nt__x86_sub_cf, ns_void, {309, 311, no_void|0x80000000}},	// 312
{nt_asgn, ns_void, {no_x86_cf|0x80000000, 312, no_void|0x80000000}},	// 313
{nt_literal, ns_void, {0, 0, 0}},	// 314
{nt_arg, ns_void, {314, no_void|0x80000000, no_void|0x80000000}},	// 315
{nt_literal, ns_void, {0, 0, 0}},	// 316
{nt_arg, ns_void, {316, no_void|0x80000000, no_void|0x80000000}},	// 317
{nt_neg, ns_void, {317, no_void|0x80000000, no_void|0x80000000}},	// 318
{nt_asgn, ns_void, {315, 318, no_void|0x80000000}},	// 319
{nt_literal, ns_void, {0, 0, 0}},	// 320
{nt_arg, ns_void, {320, no_void|0x80000000, no_void|0x80000000}},	// 321
{nt_sign, ns_void, {321, no_void|0x80000000, no_void|0x80000000}},	// 322
{nt_asgn, ns_void, {no_x86_sf|0x80000000, 322, no_void|0x80000000}},	// 323
{nt_literal, ns_void, {0, 0, 0}},	// 324
{nt_arg, ns_void, {324, no_void|0x80000000, no_void|0x80000000}},	// 325
{nt_is_zero, ns_void, {325, no_void|0x80000000, no_void|0x80000000}},	// 326
{nt_asgn, ns_void, {no_x86_zf|0x80000000, 326, no_void|0x80000000}},	// 327
{nt_literal, ns_void, {0, 0, 0}},	// 328
{nt_arg, ns_void, {328, no_void|0x80000000, no_void|0x80000000}},	// 329
{nt_trunc, ns_byte, {329, no_void|0x80000000, no_void|0x80000000}},	// 330
{nt__x86_parity, ns_void, {330, no_void|0x80000000, no_void|0x80000000}},	// 331
{nt_asgn, ns_void, {no_x86_pf|0x80000000, 331, no_void|0x80000000}},	// 332

// add
{nt_literal, ns_void, {0, 0, 0}},	// 333
{nt_arg, ns_void, {333, no_void|0x80000000, no_void|0x80000000}},	// 334
{nt_literal, ns_void, {1, 0, 0}},	// 335
{nt_arg, ns_void, {335, no_void|0x80000000, no_void|0x80000000}},	// 336
{nt__x86_add_of, ns_void, {334, 336, no_void|0x80000000}},	// 337
{nt_asgn, ns_void, {no_x86_of|0x80000000, 337, no_void|0x80000000}},	// 338
{nt_literal, ns_void, {0, 0, 0}},	// 339
{nt_arg, ns_void, {339, no_void|0x80000000, no_void|0x80000000}},	// 340
{nt_trunc, ns_byte, {340, no_void|0x80000000, no_void|0x80000000}},	// 341
{nt_literal, ns_void, {1, 0, 0}},	// 342
{nt_arg, ns_void, {342, no_void|0x80000000, no_void|0x80000000}},	// 343
{nt_trunc, ns_byte, {343, no_void|0x80000000, no_void|0x80000000}},	// 344
{nt__x86_add_af, ns_void, {341, 344, no_void|0x80000000}},	// 345
{nt_asgn, ns_void, {no_x86_af|0x80000000, 345, no_void|0x80000000}},	// 346
{nt_literal, ns_void, {0, 0, 0}},	// 347
{nt_arg, ns_void, {347, no_void|0x80000000, no_void|0x80000000}},	// 348
{nt_literal, ns_void, {1, 0, 0}},	// 349
{nt_arg, ns_void, {349, no_void|0x80000000, no_void|0x80000000}},	// 350
{nt__x86_add_cf, ns_void, {348, 350, no_void|0x80000000}},	// 351
{nt_asgn, ns_void, {no_x86_cf|0x80000000, 351, no_void|0x80000000}},	// 352
{nt_literal, ns_void, {0, 0, 0}},	// 353
{nt_arg, ns_void, {353, no_void|0x80000000, no_void|0x80000000}},	// 354
{nt_literal, ns_void, {0, 0, 0}},	// 355
{nt_arg, ns_void, {355, no_void|0x80000000, no_void|0x80000000}},	// 356
{nt_literal, ns_void, {1, 0, 0}},	// 357
{nt_arg, ns_void, {357, no_void|0x80000000, no_void|0x80000000}},	// 358
{nt_add, ns_void, {356, 358, no_void|0x80000000}},	// 359
{nt_asgn, ns_void, {354, 359, no_void|0x80000000}},	// 360
{nt_literal, ns_void, {0, 0, 0}},	// 361
{nt_arg, ns_void, {361, no_void|0x80000000, no_void|0x80000000}},	// 362
{nt_sign, ns_void, {362, no_void|0x80000000, no_void|0x80000000}},	// 363
{nt_asgn, ns_void, {no_x86_sf|0x80000000, 363, no_void|0x80000000}},	// 364
{nt_literal, ns_void, {0, 0, 0}},	// 365
{nt_arg, ns_void, {365, no_void|0x80000000, no_void|0x80000000}},	// 366
{nt_is_zero, ns_void, {366, no_void|0x80000000, no_void|0x80000000}},	// 367
{nt_asgn, ns_void, {no_x86_zf|0x80000000, 367, no_void|0x80000000}},	// 368
{nt_literal, ns_void, {0, 0, 0}},	// 369
{nt_arg, ns_void, {369, no_void|0x80000000, no_void|0x80000000}},	// 370
{nt_trunc, ns_byte, {370, no_void|0x80000000, no_void|0x80000000}},	// 371
{nt__x86_parity, ns_void, {371, no_void|0x80000000, no_void|0x80000000}},	// 372
{nt_asgn, ns_void, {no_x86_pf|0x80000000, 372, no_void|0x80000000}},	// 373

// or
{nt_literal, ns_void, {0, 0, 0}},	// 374
{nt_asgn, ns_void, {no_x86_of|0x80000000, 374, no_void|0x80000000}},	// 375
{nt_asgn, ns_void, {no_x86_af|0x80000000, no_undefined|0x80000000, no_void|0x80000000}},	// 376
{nt_literal, ns_void, {0, 0, 0}},	// 377
{nt_asgn, ns_void, {no_x86_cf|0x80000000, 377, no_void|0x80000000}},	// 378
{nt_literal, ns_void, {0, 0, 0}},	// 379
{nt_arg, ns_void, {379, no_void|0x80000000, no_void|0x80000000}},	// 380
{nt_literal, ns_void, {0, 0, 0}},	// 381
{nt_arg, ns_void, {381, no_void|0x80000000, no_void|0x80000000}},	// 382
{nt_literal, ns_void, {1, 0, 0}},	// 383
{nt_arg, ns_void, {383, no_void|0x80000000, no_void|0x80000000}},	// 384
{nt_bitor, ns_void, {382, 384, no_void|0x80000000}},	// 385
{nt_asgn, ns_void, {380, 385, no_void|0x80000000}},	// 386
{nt_literal, ns_void, {0, 0, 0}},	// 387
{nt_arg, ns_void, {387, no_void|0x80000000, no_void|0x80000000}},	// 388
{nt_sign, ns_void, {388, no_void|0x80000000, no_void|0x80000000}},	// 389
{nt_asgn, ns_void, {no_x86_sf|0x80000000, 389, no_void|0x80000000}},	// 390
{nt_literal, ns_void, {0, 0, 0}},	// 391
{nt_arg, ns_void, {391, no_void|0x80000000, no_void|0x80000000}},	// 392
{nt_is_zero, ns_void, {392, no_void|0x80000000, no_void|0x80000000}},	// 393
{nt_asgn, ns_void, {no_x86_zf|0x80000000, 393, no_void|0x80000000}},	// 394
{nt_literal, ns_void, {0, 0, 0}},	// 395
{nt_arg, ns_void, {395, no_void|0x80000000, no_void|0x80000000}},	// 396
{nt_trunc, ns_byte, {396, no_void|0x80000000, no_void|0x80000000}},	// 397
{nt__x86_parity, ns_void, {397, no_void|0x80000000, no_void|0x80000000}},	// 398
{nt_asgn, ns_void, {no_x86_pf|0x80000000, 398, no_void|0x80000000}},	// 399

// adc
{nt_literal, ns_void, {0, 0, 0}},	// 400
{nt_tmp, ns_void, {400, no_void|0x80000000, no_void|0x80000000}},	// 401
{nt_asgn, ns_void, {401, no_x86_cf|0x80000000, no_void|0x80000000}},	// 402
{nt_literal, ns_void, {0, 0, 0}},	// 403
{nt_arg, ns_void, {403, no_void|0x80000000, no_void|0x80000000}},	// 404
{nt_literal, ns_void, {1, 0, 0}},	// 405
{nt_arg, ns_void, {405, no_void|0x80000000, no_void|0x80000000}},	// 406
{nt__x86_adc_of, ns_void, {404, 406, no_void|0x80000000}},	// 407
{nt_asgn, ns_void, {no_x86_of|0x80000000, 407, no_x86_cf|0x80000000}},	// 408
{nt_literal, ns_void, {0, 0, 0}},	// 409
{nt_arg, ns_void, {409, no_void|0x80000000, no_void|0x80000000}},	// 410
{nt_trunc, ns_byte, {410, no_void|0x80000000, no_void|0x80000000}},	// 411
{nt_literal, ns_void, {1, 0, 0}},	// 412
{nt_arg, ns_void, {412, no_void|0x80000000, no_void|0x80000000}},	// 413
{nt_trunc, ns_byte, {413, no_void|0x80000000, no_void|0x80000000}},	// 414
{nt__x86_adc_af, ns_void, {411, 414, no_void|0x80000000}},	// 415
{nt_asgn, ns_void, {no_x86_af|0x80000000, 415, no_x86_cf|0x80000000}},	// 416
{nt_literal, ns_void, {0, 0, 0}},	// 417
{nt_arg, ns_void, {417, no_void|0x80000000, no_void|0x80000000}},	// 418
{nt_literal, ns_void, {1, 0, 0}},	// 419
{nt_arg, ns_void, {419, no_void|0x80000000, no_void|0x80000000}},	// 420
{nt__x86_adc_cf, ns_void, {418, 420, no_void|0x80000000}},	// 421
{nt_asgn, ns_void, {no_x86_cf|0x80000000, 421, no_x86_cf|0x80000000}},	// 422
{nt_literal, ns_void, {0, 0, 0}},	// 423
{nt_arg, ns_void, {423, no_void|0x80000000, no_void|0x80000000}},	// 424
{nt_literal, ns_void, {0, 0, 0}},	// 425
{nt_arg, ns_void, {425, no_void|0x80000000, no_void|0x80000000}},	// 426
{nt_literal, ns_void, {1, 0, 0}},	// 427
{nt_arg, ns_void, {427, no_void|0x80000000, no_void|0x80000000}},	// 428
{nt_add, ns_void, {426, 428, no_void|0x80000000}},	// 429
{nt_literal, ns_void, {0, 0, 0}},	// 430
{nt_tmp, ns_void, {430, no_void|0x80000000, no_void|0x80000000}},	// 431
{nt_zx, ns_argsize_0, {431, no_void|0x80000000, no_void|0x80000000}},	// 432
{nt_add, ns_void, {429, 432, no_void|0x80000000}},	// 433
{nt_asgn, ns_void, {424, 433, no_void|0x80000000}},	// 434
{nt_literal, ns_void, {0, 0, 0}},	// 435
{nt_arg, ns_void, {435, no_void|0x80000000, no_void|0x80000000}},	// 436
{nt_sign, ns_void, {436, no_void|0x80000000, no_void|0x80000000}},	// 437
{nt_asgn, ns_void, {no_x86_sf|0x80000000, 437, no_void|0x80000000}},	// 438
{nt_literal, ns_void, {0, 0, 0}},	// 439
{nt_arg, ns_void, {439, no_void|0x80000000, no_void|0x80000000}},	// 440
{nt_is_zero, ns_void, {440, no_void|0x80000000, no_void|0x80000000}},	// 441
{nt_asgn, ns_void, {no_x86_zf|0x80000000, 441, no_void|0x80000000}},	// 442
{nt_literal, ns_void, {0, 0, 0}},	// 443
{nt_arg, ns_void, {443, no_void|0x80000000, no_void|0x80000000}},	// 444
{nt_trunc, ns_byte, {444, no_void|0x80000000, no_void|0x80000000}},	// 445
{nt__x86_parity, ns_void, {445, no_void|0x80000000, no_void|0x80000000}},	// 446
{nt_asgn, ns_void, {no_x86_pf|0x80000000, 446, no_void|0x80000000}},	// 447

// sbb
{nt_literal, ns_void, {0, 0, 0}},	// 448
{nt_tmp, ns_void, {448, no_void|0x80000000, no_void|0x80000000}},	// 449
{nt_asgn, ns_void, {449, no_x86_cf|0x80000000, no_void|0x80000000}},	// 450
{nt_literal, ns_void, {0, 0, 0}},	// 451
{nt_arg, ns_void, {451, no_void|0x80000000, no_void|0x80000000}},	// 452
{nt_literal, ns_void, {1, 0, 0}},	// 453
{nt_arg, ns_void, {453, no_void|0x80000000, no_void|0x80000000}},	// 454
{nt__x86_sbb_of, ns_void, {452, 454, no_void|0x80000000}},	// 455
{nt_asgn, ns_void, {no_x86_of|0x80000000, 455, no_x86_cf|0x80000000}},	// 456
{nt_literal, ns_void, {0, 0, 0}},	// 457
{nt_arg, ns_void, {457, no_void|0x80000000, no_void|0x80000000}},	// 458
{nt_trunc, ns_byte, {458, no_void|0x80000000, no_void|0x80000000}},	// 459
{nt_literal, ns_void, {1, 0, 0}},	// 460
{nt_arg, ns_void, {460, no_void|0x80000000, no_void|0x80000000}},	// 461
{nt_trunc, ns_byte, {461, no_void|0x80000000, no_void|0x80000000}},	// 462
{nt__x86_sbb_af, ns_void, {459, 462, no_void|0x80000000}},	// 463
{nt_asgn, ns_void, {no_x86_af|0x80000000, 463, no_x86_cf|0x80000000}},	// 464
{nt_literal, ns_void, {0, 0, 0}},	// 465
{nt_arg, ns_void, {465, no_void|0x80000000, no_void|0x80000000}},	// 466
{nt_literal, ns_void, {1, 0, 0}},	// 467
{nt_arg, ns_void, {467, no_void|0x80000000, no_void|0x80000000}},	// 468
{nt__x86_sbb_cf, ns_void, {466, 468, no_void|0x80000000}},	// 469
{nt_asgn, ns_void, {no_x86_cf|0x80000000, 469, no_x86_cf|0x80000000}},	// 470
{nt_literal, ns_void, {0, 0, 0}},	// 471
{nt_arg, ns_void, {471, no_void|0x80000000, no_void|0x80000000}},	// 472
{nt_literal, ns_void, {0, 0, 0}},	// 473
{nt_arg, ns_void, {473, no_void|0x80000000, no_void|0x80000000}},	// 474
{nt_literal, ns_void, {1, 0, 0}},	// 475
{nt_arg, ns_void, {475, no_void|0x80000000, no_void|0x80000000}},	// 476
{nt_sub, ns_void, {474, 476, no_void|0x80000000}},	// 477
{nt_literal, ns_void, {0, 0, 0}},	// 478
{nt_tmp, ns_void, {478, no_void|0x80000000, no_void|0x80000000}},	// 479
{nt_zx, ns_argsize_0, {479, no_void|0x80000000, no_void|0x80000000}},	// 480
{nt_sub, ns_void, {477, 480, no_void|0x80000000}},	// 481
{nt_asgn, ns_void, {472, 481, no_void|0x80000000}},	// 482
{nt_literal, ns_void, {0, 0, 0}},	// 483
{nt_arg, ns_void, {483, no_void|0x80000000, no_void|0x80000000}},	// 484
{nt_sign, ns_void, {484, no_void|0x80000000, no_void|0x80000000}},	// 485
{nt_asgn, ns_void, {no_x86_sf|0x80000000, 485, no_void|0x80000000}},	// 486
{nt_literal, ns_void, {0, 0, 0}},	// 487
{nt_arg, ns_void, {487, no_void|0x80000000, no_void|0x80000000}},	// 488
{nt_is_zero, ns_void, {488, no_void|0x80000000, no_void|0x80000000}},	// 489
{nt_asgn, ns_void, {no_x86_zf|0x80000000, 489, no_void|0x80000000}},	// 490
{nt_literal, ns_void, {0, 0, 0}},	// 491
{nt_arg, ns_void, {491, no_void|0x80000000, no_void|0x80000000}},	// 492
{nt_trunc, ns_byte, {492, no_void|0x80000000, no_void|0x80000000}},	// 493
{nt__x86_parity, ns_void, {493, no_void|0x80000000, no_void|0x80000000}},	// 494
{nt_asgn, ns_void, {no_x86_pf|0x80000000, 494, no_void|0x80000000}},	// 495

// and
{nt_literal, ns_void, {0, 0, 0}},	// 496
{nt_asgn, ns_void, {no_x86_of|0x80000000, 496, no_void|0x80000000}},	// 497
{nt_asgn, ns_void, {no_x86_af|0x80000000, no_undefined|0x80000000, no_void|0x80000000}},	// 498
{nt_literal, ns_void, {0, 0, 0}},	// 499
{nt_asgn, ns_void, {no_x86_cf|0x80000000, 499, no_void|0x80000000}},	// 500
{nt_literal, ns_void, {0, 0, 0}},	// 501
{nt_arg, ns_void, {501, no_void|0x80000000, no_void|0x80000000}},	// 502
{nt_literal, ns_void, {0, 0, 0}},	// 503
{nt_arg, ns_void, {503, no_void|0x80000000, no_void|0x80000000}},	// 504
{nt_literal, ns_void, {1, 0, 0}},	// 505
{nt_arg, ns_void, {505, no_void|0x80000000, no_void|0x80000000}},	// 506
{nt_bitand, ns_void, {504, 506, no_void|0x80000000}},	// 507
{nt_asgn, ns_void, {502, 507, no_void|0x80000000}},	// 508
{nt_literal, ns_void, {0, 0, 0}},	// 509
{nt_arg, ns_void, {509, no_void|0x80000000, no_void|0x80000000}},	// 510
{nt_sign, ns_void, {510, no_void|0x80000000, no_void|0x80000000}},	// 511
{nt_asgn, ns_void, {no_x86_sf|0x80000000, 511, no_void|0x80000000}},	// 512
{nt_literal, ns_void, {0, 0, 0}},	// 513
{nt_arg, ns_void, {513, no_void|0x80000000, no_void|0x80000000}},	// 514
{nt_is_zero, ns_void, {514, no_void|0x80000000, no_void|0x80000000}},	// 515
{nt_asgn, ns_void, {no_x86_zf|0x80000000, 515, no_void|0x80000000}},	// 516
{nt_literal, ns_void, {0, 0, 0}},	// 517
{nt_arg, ns_void, {517, no_void|0x80000000, no_void|0x80000000}},	// 518
{nt_trunc, ns_byte, {518, no_void|0x80000000, no_void|0x80000000}},	// 519
{nt__x86_parity, ns_void, {519, no_void|0x80000000, no_void|0x80000000}},	// 520
{nt_asgn, ns_void, {no_x86_pf|0x80000000, 520, no_void|0x80000000}},	// 521

// sub
{nt_literal, ns_void, {0, 0, 0}},	// 522
{nt_arg, ns_void, {522, no_void|0x80000000, no_void|0x80000000}},	// 523
{nt_literal, ns_void, {1, 0, 0}},	// 524
{nt_arg, ns_void, {524, no_void|0x80000000, no_void|0x80000000}},	// 525
{nt__x86_sub_of, ns_void, {523, 525, no_void|0x80000000}},	// 526
{nt_asgn, ns_void, {no_x86_of|0x80000000, 526, no_void|0x80000000}},	// 527
{nt_literal, ns_void, {0, 0, 0}},	// 528
{nt_arg, ns_void, {528, no_void|0x80000000, no_void|0x80000000}},	// 529
{nt_trunc, ns_byte, {529, no_void|0x80000000, no_void|0x80000000}},	// 530
{nt_literal, ns_void, {1, 0, 0}},	// 531
{nt_arg, ns_void, {531, no_void|0x80000000, no_void|0x80000000}},	// 532
{nt_trunc, ns_byte, {532, no_void|0x80000000, no_void|0x80000000}},	// 533
{nt__x86_sub_af, ns_void, {530, 533, no_void|0x80000000}},	// 534
{nt_asgn, ns_void, {no_x86_af|0x80000000, 534, no_void|0x80000000}},	// 535
{nt_literal, ns_void, {0, 0, 0}},	// 536
{nt_arg, ns_void, {536, no_void|0x80000000, no_void|0x80000000}},	// 537
{nt_literal, ns_void, {1, 0, 0}},	// 538
{nt_arg, ns_void, {538, no_void|0x80000000, no_void|0x80000000}},	// 539
{nt__x86_sub_cf, ns_void, {537, 539, no_void|0x80000000}},	// 540
{nt_asgn, ns_void, {no_x86_cf|0x80000000, 540, no_void|0x80000000}},	// 541
{nt_literal, ns_void, {0, 0, 0}},	// 542
{nt_arg, ns_void, {542, no_void|0x80000000, no_void|0x80000000}},	// 543
{nt_literal, ns_void, {0, 0, 0}},	// 544
{nt_arg, ns_void, {544, no_void|0x80000000, no_void|0x80000000}},	// 545
{nt_literal, ns_void, {1, 0, 0}},	// 546
{nt_arg, ns_void, {546, no_void|0x80000000, no_void|0x80000000}},	// 547
{nt_sub, ns_void, {545, 547, no_void|0x80000000}},	// 548
{nt_asgn, ns_void, {543, 548, no_void|0x80000000}},	// 549
{nt_literal, ns_void, {0, 0, 0}},	// 550
{nt_arg, ns_void, {550, no_void|0x80000000, no_void|0x80000000}},	// 551
{nt_sign, ns_void, {551, no_void|0x80000000, no_void|0x80000000}},	// 552
{nt_asgn, ns_void, {no_x86_sf|0x80000000, 552, no_void|0x80000000}},	// 553
{nt_literal, ns_void, {0, 0, 0}},	// 554
{nt_arg, ns_void, {554, no_void|0x80000000, no_void|0x80000000}},	// 555
{nt_is_zero, ns_void, {555, no_void|0x80000000, no_void|0x80000000}},	// 556
{nt_asgn, ns_void, {no_x86_zf|0x80000000, 556, no_void|0x80000000}},	// 557
{nt_literal, ns_void, {0, 0, 0}},	// 558
{nt_arg, ns_void, {558, no_void|0x80000000, no_void|0x80000000}},	// 559
{nt_trunc, ns_byte, {559, no_void|0x80000000, no_void|0x80000000}},	// 560
{nt__x86_parity, ns_void, {560, no_void|0x80000000, no_void|0x80000000}},	// 561
{nt_asgn, ns_void, {no_x86_pf|0x80000000, 561, no_void|0x80000000}},	// 562

// xor
{nt_literal, ns_void, {0, 0, 0}},	// 563
{nt_asgn, ns_void, {no_x86_of|0x80000000, 563, no_void|0x80000000}},	// 564
{nt_asgn, ns_void, {no_x86_af|0x80000000, no_undefined|0x80000000, no_void|0x80000000}},	// 565
{nt_literal, ns_void, {0, 0, 0}},	// 566
{nt_asgn, ns_void, {no_x86_cf|0x80000000, 566, no_void|0x80000000}},	// 567
{nt_literal, ns_void, {0, 0, 0}},	// 568
{nt_arg, ns_void, {568, no_void|0x80000000, no_void|0x80000000}},	// 569
{nt_literal, ns_void, {0, 0, 0}},	// 570
{nt_arg, ns_void, {570, no_void|0x80000000, no_void|0x80000000}},	// 571
{nt_literal, ns_void, {1, 0, 0}},	// 572
{nt_arg, ns_void, {572, no_void|0x80000000, no_void|0x80000000}},	// 573
{nt_bitxor, ns_void, {571, 573, no_void|0x80000000}},	// 574
{nt_asgn, ns_void, {569, 574, no_void|0x80000000}},	// 575
{nt_literal, ns_void, {0, 0, 0}},	// 576
{nt_arg, ns_void, {576, no_void|0x80000000, no_void|0x80000000}},	// 577
{nt_sign, ns_void, {577, no_void|0x80000000, no_void|0x80000000}},	// 578
{nt_asgn, ns_void, {no_x86_sf|0x80000000, 578, no_void|0x80000000}},	// 579
{nt_literal, ns_void, {0, 0, 0}},	// 580
{nt_arg, ns_void, {580, no_void|0x80000000, no_void|0x80000000}},	// 581
{nt_is_zero, ns_void, {581, no_void|0x80000000, no_void|0x80000000}},	// 582
{nt_asgn, ns_void, {no_x86_zf|0x80000000, 582, no_void|0x80000000}},	// 583
{nt_literal, ns_void, {0, 0, 0}},	// 584
{nt_arg, ns_void, {584, no_void|0x80000000, no_void|0x80000000}},	// 585
{nt_trunc, ns_byte, {585, no_void|0x80000000, no_void|0x80000000}},	// 586
{nt__x86_parity, ns_void, {586, no_void|0x80000000, no_void|0x80000000}},	// 587
{nt_asgn, ns_void, {no_x86_pf|0x80000000, 587, no_void|0x80000000}},	// 588

// cmp
{nt_literal, ns_void, {0, 0, 0}},	// 589
{nt_arg, ns_void, {589, no_void|0x80000000, no_void|0x80000000}},	// 590
{nt_literal, ns_void, {1, 0, 0}},	// 591
{nt_arg, ns_void, {591, no_void|0x80000000, no_void|0x80000000}},	// 592
{nt__x86_sub_of, ns_void, {590, 592, no_void|0x80000000}},	// 593
{nt_asgn, ns_void, {no_x86_of|0x80000000, 593, no_void|0x80000000}},	// 594
{nt_literal, ns_void, {0, 0, 0}},	// 595
{nt_arg, ns_void, {595, no_void|0x80000000, no_void|0x80000000}},	// 596
{nt_trunc, ns_byte, {596, no_void|0x80000000, no_void|0x80000000}},	// 597
{nt_literal, ns_void, {1, 0, 0}},	// 598
{nt_arg, ns_void, {598, no_void|0x80000000, no_void|0x80000000}},	// 599
{nt_trunc, ns_byte, {599, no_void|0x80000000, no_void|0x80000000}},	// 600
{nt__x86_sub_af, ns_void, {597, 600, no_void|0x80000000}},	// 601
{nt_asgn, ns_void, {no_x86_af|0x80000000, 601, no_void|0x80000000}},	// 602
{nt_literal, ns_void, {0, 0, 0}},	// 603
{nt_arg, ns_void, {603, no_void|0x80000000, no_void|0x80000000}},	// 604
{nt_literal, ns_void, {1, 0, 0}},	// 605
{nt_arg, ns_void, {605, no_void|0x80000000, no_void|0x80000000}},	// 606
{nt__x86_sub_cf, ns_void, {604, 606, no_void|0x80000000}},	// 607
{nt_asgn, ns_void, {no_x86_cf|0x80000000, 607, no_void|0x80000000}},	// 608
{nt_literal, ns_void, {0, 0, 0}},	// 609
{nt_tmp, ns_void, {609, no_void|0x80000000, no_void|0x80000000}},	// 610
{nt_literal, ns_void, {0, 0, 0}},	// 611
{nt_arg, ns_void, {611, no_void|0x80000000, no_void|0x80000000}},	// 612
{nt_literal, ns_void, {1, 0, 0}},	// 613
{nt_arg, ns_void, {613, no_void|0x80000000, no_void|0x80000000}},	// 614
{nt_sub, ns_void, {612, 614, no_void|0x80000000}},	// 615
{nt_asgn, ns_void, {610, 615, no_void|0x80000000}},	// 616
{nt_literal, ns_void, {0, 0, 0}},	// 617
{nt_tmp, ns_void, {617, no_void|0x80000000, no_void|0x80000000}},	// 618
{nt_sign, ns_void, {618, no_void|0x80000000, no_void|0x80000000}},	// 619
{nt_asgn, ns_void, {no_x86_sf|0x80000000, 619, no_void|0x80000000}},	// 620
{nt_literal, ns_void, {0, 0, 0}},	// 621
{nt_tmp, ns_void, {621, no_void|0x80000000, no_void|0x80000000}},	// 622
{nt_is_zero, ns_void, {622, no_void|0x80000000, no_void|0x80000000}},	// 623
{nt_asgn, ns_void, {no_x86_zf|0x80000000, 623, no_void|0x80000000}},	// 624
{nt_literal, ns_void, {0, 0, 0}},	// 625
{nt_tmp, ns_void, {625, no_void|0x80000000, no_void|0x80000000}},	// 626
{nt_trunc, ns_byte, {626, no_void|0x80000000, no_void|0x80000000}},	// 627
{nt__x86_parity, ns_void, {627, no_void|0x80000000, no_void|0x80000000}},	// 628
{nt_asgn, ns_void, {no_x86_pf|0x80000000, 628, no_void|0x80000000}},	// 629

// rol
{nt_literal, ns_void, {0, 0, 0}},	// 630
{nt_arg, ns_void, {630, no_void|0x80000000, no_void|0x80000000}},	// 631
{nt_literal, ns_void, {1, 0, 0}},	// 632
{nt_arg, ns_void, {632, no_void|0x80000000, no_void|0x80000000}},	// 633
{nt__x86_rol_of, ns_void, {631, 633, no_void|0x80000000}},	// 634
{nt_asgn, ns_void, {no_x86_of|0x80000000, 634, no_void|0x80000000}},	// 635
{nt_literal, ns_void, {0, 0, 0}},	// 636
{nt_arg, ns_void, {636, no_void|0x80000000, no_void|0x80000000}},	// 637
{nt_literal, ns_void, {1, 0, 0}},	// 638
{nt_arg, ns_void, {638, no_void|0x80000000, no_void|0x80000000}},	// 639
{nt__x86_rol_af, ns_void, {637, 639, no_void|0x80000000}},	// 640
{nt_asgn, ns_void, {no_x86_af|0x80000000, 640, no_void|0x80000000}},	// 641
{nt_literal, ns_void, {0, 0, 0}},	// 642
{nt_arg, ns_void, {642, no_void|0x80000000, no_void|0x80000000}},	// 643
{nt_literal, ns_void, {1, 0, 0}},	// 644
{nt_arg, ns_void, {644, no_void|0x80000000, no_void|0x80000000}},	// 645
{nt__x86_rol_cf, ns_void, {643, 645, no_void|0x80000000}},	// 646
{nt_asgn, ns_void, {no_x86_cf|0x80000000, 646, no_void|0x80000000}},	// 647
{nt_literal, ns_void, {0, 0, 0}},	// 648
{nt_arg, ns_void, {648, no_void|0x80000000, no_void|0x80000000}},	// 649
{nt_literal, ns_void, {1, 0, 0}},	// 650
{nt_arg, ns_void, {650, no_void|0x80000000, no_void|0x80000000}},	// 651
{nt__x86_rol_sf, ns_void, {649, 651, no_void|0x80000000}},	// 652
{nt_asgn, ns_void, {no_x86_sf|0x80000000, 652, no_void|0x80000000}},	// 653
{nt_literal, ns_void, {0, 0, 0}},	// 654
{nt_arg, ns_void, {654, no_void|0x80000000, no_void|0x80000000}},	// 655
{nt_literal, ns_void, {1, 0, 0}},	// 656
{nt_arg, ns_void, {656, no_void|0x80000000, no_void|0x80000000}},	// 657
{nt__x86_rol_zf, ns_void, {655, 657, no_void|0x80000000}},	// 658
{nt_asgn, ns_void, {no_x86_zf|0x80000000, 658, no_void|0x80000000}},	// 659
{nt_literal, ns_void, {0, 0, 0}},	// 660
{nt_arg, ns_void, {660, no_void|0x80000000, no_void|0x80000000}},	// 661
{nt_literal, ns_void, {1, 0, 0}},	// 662
{nt_arg, ns_void, {662, no_void|0x80000000, no_void|0x80000000}},	// 663
{nt__x86_rol_pf, ns_void, {661, 663, no_void|0x80000000}},	// 664
{nt_asgn, ns_void, {no_x86_pf|0x80000000, 664, no_void|0x80000000}},	// 665
{nt_literal, ns_void, {0, 0, 0}},	// 666
{nt_arg, ns_void, {666, no_void|0x80000000, no_void|0x80000000}},	// 667
{nt_literal, ns_void, {0, 0, 0}},	// 668
{nt_arg, ns_void, {668, no_void|0x80000000, no_void|0x80000000}},	// 669
{nt_literal, ns_void, {1, 0, 0}},	// 670
{nt_arg, ns_void, {670, no_void|0x80000000, no_void|0x80000000}},	// 671
{nt__x86_rol, ns_void, {669, 671, no_void|0x80000000}},	// 672
{nt_asgn, ns_void, {667, 672, no_void|0x80000000}},	// 673

// ror
{nt_literal, ns_void, {0, 0, 0}},	// 674
{nt_arg, ns_void, {674, no_void|0x80000000, no_void|0x80000000}},	// 675
{nt_literal, ns_void, {1, 0, 0}},	// 676
{nt_arg, ns_void, {676, no_void|0x80000000, no_void|0x80000000}},	// 677
{nt__x86_ror_of, ns_void, {675, 677, no_void|0x80000000}},	// 678
{nt_asgn, ns_void, {no_x86_of|0x80000000, 678, no_void|0x80000000}},	// 679
{nt_literal, ns_void, {0, 0, 0}},	// 680
{nt_arg, ns_void, {680, no_void|0x80000000, no_void|0x80000000}},	// 681
{nt_literal, ns_void, {1, 0, 0}},	// 682
{nt_arg, ns_void, {682, no_void|0x80000000, no_void|0x80000000}},	// 683
{nt__x86_ror_af, ns_void, {681, 683, no_void|0x80000000}},	// 684
{nt_asgn, ns_void, {no_x86_af|0x80000000, 684, no_void|0x80000000}},	// 685
{nt_literal, ns_void, {0, 0, 0}},	// 686
{nt_arg, ns_void, {686, no_void|0x80000000, no_void|0x80000000}},	// 687
{nt_literal, ns_void, {1, 0, 0}},	// 688
{nt_arg, ns_void, {688, no_void|0x80000000, no_void|0x80000000}},	// 689
{nt__x86_ror_cf, ns_void, {687, 689, no_void|0x80000000}},	// 690
{nt_asgn, ns_void, {no_x86_cf|0x80000000, 690, no_void|0x80000000}},	// 691
{nt_literal, ns_void, {0, 0, 0}},	// 692
{nt_arg, ns_void, {692, no_void|0x80000000, no_void|0x80000000}},	// 693
{nt_literal, ns_void, {1, 0, 0}},	// 694
{nt_arg, ns_void, {694, no_void|0x80000000, no_void|0x80000000}},	// 695
{nt__x86_ror_sf, ns_void, {693, 695, no_void|0x80000000}},	// 696
{nt_asgn, ns_void, {no_x86_sf|0x80000000, 696, no_void|0x80000000}},	// 697
{nt_literal, ns_void, {0, 0, 0}},	// 698
{nt_arg, ns_void, {698, no_void|0x80000000, no_void|0x80000000}},	// 699
{nt_literal, ns_void, {1, 0, 0}},	// 700
{nt_arg, ns_void, {700, no_void|0x80000000, no_void|0x80000000}},	// 701
{nt__x86_ror_zf, ns_void, {699, 701, no_void|0x80000000}},	// 702
{nt_asgn, ns_void, {no_x86_zf|0x80000000, 702, no_void|0x80000000}},	// 703
{nt_literal, ns_void, {0, 0, 0}},	// 704
{nt_arg, ns_void, {704, no_void|0x80000000, no_void|0x80000000}},	// 705
{nt_literal, ns_void, {1, 0, 0}},	// 706
{nt_arg, ns_void, {706, no_void|0x80000000, no_void|0x80000000}},	// 707
{nt__x86_ror_pf, ns_void, {705, 707, no_void|0x80000000}},	// 708
{nt_asgn, ns_void, {no_x86_pf|0x80000000, 708, no_void|0x80000000}},	// 709
{nt_literal, ns_void, {0, 0, 0}},	// 710
{nt_arg, ns_void, {710, no_void|0x80000000, no_void|0x80000000}},	// 711
{nt_literal, ns_void, {0, 0, 0}},	// 712
{nt_arg, ns_void, {712, no_void|0x80000000, no_void|0x80000000}},	// 713
{nt_literal, ns_void, {1, 0, 0}},	// 714
{nt_arg, ns_void, {714, no_void|0x80000000, no_void|0x80000000}},	// 715
{nt__x86_ror, ns_void, {713, 715, no_void|0x80000000}},	// 716
{nt_asgn, ns_void, {711, 716, no_void|0x80000000}},	// 717

// rcl
{nt_literal, ns_void, {0, 0, 0}},	// 718
{nt_tmp, ns_void, {718, no_void|0x80000000, no_void|0x80000000}},	// 719
{nt_asgn, ns_void, {719, no_x86_cf|0x80000000, no_void|0x80000000}},	// 720
{nt_literal, ns_void, {0, 0, 0}},	// 721
{nt_arg, ns_void, {721, no_void|0x80000000, no_void|0x80000000}},	// 722
{nt_literal, ns_void, {1, 0, 0}},	// 723
{nt_arg, ns_void, {723, no_void|0x80000000, no_void|0x80000000}},	// 724
{nt_literal, ns_void, {0, 0, 0}},	// 725
{nt_tmp, ns_void, {725, no_void|0x80000000, no_void|0x80000000}},	// 726
{nt__x86_rcl_of, ns_void, {722, 724, 726}},	// 727
{nt_asgn, ns_void, {no_x86_of|0x80000000, 727, no_void|0x80000000}},	// 728
{nt_literal, ns_void, {0, 0, 0}},	// 729
{nt_arg, ns_void, {729, no_void|0x80000000, no_void|0x80000000}},	// 730
{nt_literal, ns_void, {1, 0, 0}},	// 731
{nt_arg, ns_void, {731, no_void|0x80000000, no_void|0x80000000}},	// 732
{nt_literal, ns_void, {0, 0, 0}},	// 733
{nt_tmp, ns_void, {733, no_void|0x80000000, no_void|0x80000000}},	// 734
{nt__x86_rcl_af, ns_void, {730, 732, 734}},	// 735
{nt_asgn, ns_void, {no_x86_af|0x80000000, 735, no_void|0x80000000}},	// 736
{nt_literal, ns_void, {0, 0, 0}},	// 737
{nt_arg, ns_void, {737, no_void|0x80000000, no_void|0x80000000}},	// 738
{nt_literal, ns_void, {1, 0, 0}},	// 739
{nt_arg, ns_void, {739, no_void|0x80000000, no_void|0x80000000}},	// 740
{nt_literal, ns_void, {0, 0, 0}},	// 741
{nt_tmp, ns_void, {741, no_void|0x80000000, no_void|0x80000000}},	// 742
{nt__x86_rcl_cf, ns_void, {738, 740, 742}},	// 743
{nt_asgn, ns_void, {no_x86_cf|0x80000000, 743, no_void|0x80000000}},	// 744
{nt_literal, ns_void, {0, 0, 0}},	// 745
{nt_arg, ns_void, {745, no_void|0x80000000, no_void|0x80000000}},	// 746
{nt_literal, ns_void, {1, 0, 0}},	// 747
{nt_arg, ns_void, {747, no_void|0x80000000, no_void|0x80000000}},	// 748
{nt_literal, ns_void, {0, 0, 0}},	// 749
{nt_tmp, ns_void, {749, no_void|0x80000000, no_void|0x80000000}},	// 750
{nt__x86_rcl_sf, ns_void, {746, 748, 750}},	// 751
{nt_asgn, ns_void, {no_x86_sf|0x80000000, 751, no_void|0x80000000}},	// 752
{nt_literal, ns_void, {0, 0, 0}},	// 753
{nt_arg, ns_void, {753, no_void|0x80000000, no_void|0x80000000}},	// 754
{nt_literal, ns_void, {1, 0, 0}},	// 755
{nt_arg, ns_void, {755, no_void|0x80000000, no_void|0x80000000}},	// 756
{nt_literal, ns_void, {0, 0, 0}},	// 757
{nt_tmp, ns_void, {757, no_void|0x80000000, no_void|0x80000000}},	// 758
{nt__x86_rcl_zf, ns_void, {754, 756, 758}},	// 759
{nt_asgn, ns_void, {no_x86_zf|0x80000000, 759, no_void|0x80000000}},	// 760
{nt_literal, ns_void, {0, 0, 0}},	// 761
{nt_arg, ns_void, {761, no_void|0x80000000, no_void|0x80000000}},	// 762
{nt_literal, ns_void, {1, 0, 0}},	// 763
{nt_arg, ns_void, {763, no_void|0x80000000, no_void|0x80000000}},	// 764
{nt_literal, ns_void, {0, 0, 0}},	// 765
{nt_tmp, ns_void, {765, no_void|0x80000000, no_void|0x80000000}},	// 766
{nt__x86_rcl_pf, ns_void, {762, 764, 766}},	// 767
{nt_asgn, ns_void, {no_x86_pf|0x80000000, 767, no_void|0x80000000}},	// 768
{nt_literal, ns_void, {0, 0, 0}},	// 769
{nt_arg, ns_void, {769, no_void|0x80000000, no_void|0x80000000}},	// 770
{nt_literal, ns_void, {0, 0, 0}},	// 771
{nt_arg, ns_void, {771, no_void|0x80000000, no_void|0x80000000}},	// 772
{nt_literal, ns_void, {1, 0, 0}},	// 773
{nt_arg, ns_void, {773, no_void|0x80000000, no_void|0x80000000}},	// 774
{nt_literal, ns_void, {0, 0, 0}},	// 775
{nt_tmp, ns_void, {775, no_void|0x80000000, no_void|0x80000000}},	// 776
{nt__x86_rcl, ns_void, {772, 774, 776}},	// 777
{nt_asgn, ns_void, {770, 777, no_void|0x80000000}},	// 778

// rcr
{nt_literal, ns_void, {0, 0, 0}},	// 779
{nt_tmp, ns_void, {779, no_void|0x80000000, no_void|0x80000000}},	// 780
{nt_asgn, ns_void, {780, no_x86_cf|0x80000000, no_void|0x80000000}},	// 781
{nt_literal, ns_void, {0, 0, 0}},	// 782
{nt_arg, ns_void, {782, no_void|0x80000000, no_void|0x80000000}},	// 783
{nt_literal, ns_void, {1, 0, 0}},	// 784
{nt_arg, ns_void, {784, no_void|0x80000000, no_void|0x80000000}},	// 785
{nt_literal, ns_void, {0, 0, 0}},	// 786
{nt_tmp, ns_void, {786, no_void|0x80000000, no_void|0x80000000}},	// 787
{nt__x86_rcr_of, ns_void, {783, 785, 787}},	// 788
{nt_asgn, ns_void, {no_x86_of|0x80000000, 788, no_void|0x80000000}},	// 789
{nt_literal, ns_void, {0, 0, 0}},	// 790
{nt_arg, ns_void, {790, no_void|0x80000000, no_void|0x80000000}},	// 791
{nt_literal, ns_void, {1, 0, 0}},	// 792
{nt_arg, ns_void, {792, no_void|0x80000000, no_void|0x80000000}},	// 793
{nt_literal, ns_void, {0, 0, 0}},	// 794
{nt_tmp, ns_void, {794, no_void|0x80000000, no_void|0x80000000}},	// 795
{nt__x86_rcr_af, ns_void, {791, 793, 795}},	// 796
{nt_asgn, ns_void, {no_x86_af|0x80000000, 796, no_void|0x80000000}},	// 797
{nt_literal, ns_void, {0, 0, 0}},	// 798
{nt_arg, ns_void, {798, no_void|0x80000000, no_void|0x80000000}},	// 799
{nt_literal, ns_void, {1, 0, 0}},	// 800
{nt_arg, ns_void, {800, no_void|0x80000000, no_void|0x80000000}},	// 801
{nt_literal, ns_void, {0, 0, 0}},	// 802
{nt_tmp, ns_void, {802, no_void|0x80000000, no_void|0x80000000}},	// 803
{nt__x86_rcr_cf, ns_void, {799, 801, 803}},	// 804
{nt_asgn, ns_void, {no_x86_cf|0x80000000, 804, no_void|0x80000000}},	// 805
{nt_literal, ns_void, {0, 0, 0}},	// 806
{nt_arg, ns_void, {806, no_void|0x80000000, no_void|0x80000000}},	// 807
{nt_literal, ns_void, {1, 0, 0}},	// 808
{nt_arg, ns_void, {808, no_void|0x80000000, no_void|0x80000000}},	// 809
{nt_literal, ns_void, {0, 0, 0}},	// 810
{nt_tmp, ns_void, {810, no_void|0x80000000, no_void|0x80000000}},	// 811
{nt__x86_rcr_sf, ns_void, {807, 809, 811}},	// 812
{nt_asgn, ns_void, {no_x86_sf|0x80000000, 812, no_void|0x80000000}},	// 813
{nt_literal, ns_void, {0, 0, 0}},	// 814
{nt_arg, ns_void, {814, no_void|0x80000000, no_void|0x80000000}},	// 815
{nt_literal, ns_void, {1, 0, 0}},	// 816
{nt_arg, ns_void, {816, no_void|0x80000000, no_void|0x80000000}},	// 817
{nt_literal, ns_void, {0, 0, 0}},	// 818
{nt_tmp, ns_void, {818, no_void|0x80000000, no_void|0x80000000}},	// 819
{nt__x86_rcr_zf, ns_void, {815, 817, 819}},	// 820
{nt_asgn, ns_void, {no_x86_zf|0x80000000, 820, no_void|0x80000000}},	// 821
{nt_literal, ns_void, {0, 0, 0}},	// 822
{nt_arg, ns_void, {822, no_void|0x80000000, no_void|0x80000000}},	// 823
{nt_literal, ns_void, {1, 0, 0}},	// 824
{nt_arg, ns_void, {824, no_void|0x80000000, no_void|0x80000000}},	// 825
{nt_literal, ns_void, {0, 0, 0}},	// 826
{nt_tmp, ns_void, {826, no_void|0x80000000, no_void|0x80000000}},	// 827
{nt__x86_rcr_pf, ns_void, {823, 825, 827}},	// 828
{nt_asgn, ns_void, {no_x86_pf|0x80000000, 828, no_void|0x80000000}},	// 829
{nt_literal, ns_void, {0, 0, 0}},	// 830
{nt_arg, ns_void, {830, no_void|0x80000000, no_void|0x80000000}},	// 831
{nt_literal, ns_void, {0, 0, 0}},	// 832
{nt_arg, ns_void, {832, no_void|0x80000000, no_void|0x80000000}},	// 833
{nt_literal, ns_void, {1, 0, 0}},	// 834
{nt_arg, ns_void, {834, no_void|0x80000000, no_void|0x80000000}},	// 835
{nt_literal, ns_void, {0, 0, 0}},	// 836
{nt_tmp, ns_void, {836, no_void|0x80000000, no_void|0x80000000}},	// 837
{nt__x86_rcr, ns_void, {833, 835, 837}},	// 838
{nt_asgn, ns_void, {831, 838, no_void|0x80000000}},	// 839

// shl
{nt_literal, ns_void, {0, 0, 0}},	// 840
{nt_arg, ns_void, {840, no_void|0x80000000, no_void|0x80000000}},	// 841
{nt_literal, ns_void, {1, 0, 0}},	// 842
{nt_arg, ns_void, {842, no_void|0x80000000, no_void|0x80000000}},	// 843
{nt__x86_shl_of, ns_void, {841, 843, no_void|0x80000000}},	// 844
{nt_asgn, ns_void, {no_x86_of|0x80000000, 844, no_void|0x80000000}},	// 845
{nt_literal, ns_void, {0, 0, 0}},	// 846
{nt_arg, ns_void, {846, no_void|0x80000000, no_void|0x80000000}},	// 847
{nt_literal, ns_void, {1, 0, 0}},	// 848
{nt_arg, ns_void, {848, no_void|0x80000000, no_void|0x80000000}},	// 849
{nt__x86_shl_af, ns_void, {847, 849, no_void|0x80000000}},	// 850
{nt_asgn, ns_void, {no_x86_af|0x80000000, 850, no_void|0x80000000}},	// 851
{nt_literal, ns_void, {0, 0, 0}},	// 852
{nt_arg, ns_void, {852, no_void|0x80000000, no_void|0x80000000}},	// 853
{nt_literal, ns_void, {1, 0, 0}},	// 854
{nt_arg, ns_void, {854, no_void|0x80000000, no_void|0x80000000}},	// 855
{nt__x86_shl_cf, ns_void, {853, 855, no_void|0x80000000}},	// 856
{nt_asgn, ns_void, {no_x86_cf|0x80000000, 856, no_void|0x80000000}},	// 857
{nt_literal, ns_void, {0, 0, 0}},	// 858
{nt_arg, ns_void, {858, no_void|0x80000000, no_void|0x80000000}},	// 859
{nt_literal, ns_void, {1, 0, 0}},	// 860
{nt_arg, ns_void, {860, no_void|0x80000000, no_void|0x80000000}},	// 861
{nt__x86_shl_sf, ns_void, {859, 861, no_void|0x80000000}},	// 862
{nt_asgn, ns_void, {no_x86_sf|0x80000000, 862, no_void|0x80000000}},	// 863
{nt_literal, ns_void, {0, 0, 0}},	// 864
{nt_arg, ns_void, {864, no_void|0x80000000, no_void|0x80000000}},	// 865
{nt_literal, ns_void, {1, 0, 0}},	// 866
{nt_arg, ns_void, {866, no_void|0x80000000, no_void|0x80000000}},	// 867
{nt__x86_shl_zf, ns_void, {865, 867, no_void|0x80000000}},	// 868
{nt_asgn, ns_void, {no_x86_zf|0x80000000, 868, no_void|0x80000000}},	// 869
{nt_literal, ns_void, {0, 0, 0}},	// 870
{nt_arg, ns_void, {870, no_void|0x80000000, no_void|0x80000000}},	// 871
{nt_literal, ns_void, {1, 0, 0}},	// 872
{nt_arg, ns_void, {872, no_void|0x80000000, no_void|0x80000000}},	// 873
{nt__x86_shl_pf, ns_void, {871, 873, no_void|0x80000000}},	// 874
{nt_asgn, ns_void, {no_x86_pf|0x80000000, 874, no_void|0x80000000}},	// 875
{nt_literal, ns_void, {0, 0, 0}},	// 876
{nt_arg, ns_void, {876, no_void|0x80000000, no_void|0x80000000}},	// 877
{nt_literal, ns_void, {0, 0, 0}},	// 878
{nt_arg, ns_void, {878, no_void|0x80000000, no_void|0x80000000}},	// 879
{nt_literal, ns_void, {1, 0, 0}},	// 880
{nt_arg, ns_void, {880, no_void|0x80000000, no_void|0x80000000}},	// 881
{nt__x86_shl, ns_void, {879, 881, no_void|0x80000000}},	// 882
{nt_asgn, ns_void, {877, 882, no_void|0x80000000}},	// 883

// shr
{nt_literal, ns_void, {0, 0, 0}},	// 884
{nt_arg, ns_void, {884, no_void|0x80000000, no_void|0x80000000}},	// 885
{nt_literal, ns_void, {1, 0, 0}},	// 886
{nt_arg, ns_void, {886, no_void|0x80000000, no_void|0x80000000}},	// 887
{nt__x86_ushr_of, ns_void, {885, 887, no_void|0x80000000}},	// 888
{nt_asgn, ns_void, {no_x86_of|0x80000000, 888, no_void|0x80000000}},	// 889
{nt_literal, ns_void, {0, 0, 0}},	// 890
{nt_arg, ns_void, {890, no_void|0x80000000, no_void|0x80000000}},	// 891
{nt_literal, ns_void, {1, 0, 0}},	// 892
{nt_arg, ns_void, {892, no_void|0x80000000, no_void|0x80000000}},	// 893
{nt__x86_ushr_af, ns_void, {891, 893, no_void|0x80000000}},	// 894
{nt_asgn, ns_void, {no_x86_af|0x80000000, 894, no_void|0x80000000}},	// 895
{nt_literal, ns_void, {0, 0, 0}},	// 896
{nt_arg, ns_void, {896, no_void|0x80000000, no_void|0x80000000}},	// 897
{nt_literal, ns_void, {1, 0, 0}},	// 898
{nt_arg, ns_void, {898, no_void|0x80000000, no_void|0x80000000}},	// 899
{nt__x86_ushr_cf, ns_void, {897, 899, no_void|0x80000000}},	// 900
{nt_asgn, ns_void, {no_x86_cf|0x80000000, 900, no_void|0x80000000}},	// 901
{nt_literal, ns_void, {0, 0, 0}},	// 902
{nt_arg, ns_void, {902, no_void|0x80000000, no_void|0x80000000}},	// 903
{nt_literal, ns_void, {1, 0, 0}},	// 904
{nt_arg, ns_void, {904, no_void|0x80000000, no_void|0x80000000}},	// 905
{nt__x86_ushr_sf, ns_void, {903, 905, no_void|0x80000000}},	// 906
{nt_asgn, ns_void, {no_x86_sf|0x80000000, 906, no_void|0x80000000}},	// 907
{nt_literal, ns_void, {0, 0, 0}},	// 908
{nt_arg, ns_void, {908, no_void|0x80000000, no_void|0x80000000}},	// 909
{nt_literal, ns_void, {1, 0, 0}},	// 910
{nt_arg, ns_void, {910, no_void|0x80000000, no_void|0x80000000}},	// 911
{nt__x86_ushr_zf, ns_void, {909, 911, no_void|0x80000000}},	// 912
{nt_asgn, ns_void, {no_x86_zf|0x80000000, 912, no_void|0x80000000}},	// 913
{nt_literal, ns_void, {0, 0, 0}},	// 914
{nt_arg, ns_void, {914, no_void|0x80000000, no_void|0x80000000}},	// 915
{nt_literal, ns_void, {1, 0, 0}},	// 916
{nt_arg, ns_void, {916, no_void|0x80000000, no_void|0x80000000}},	// 917
{nt__x86_ushr_pf, ns_void, {915, 917, no_void|0x80000000}},	// 918
{nt_asgn, ns_void, {no_x86_pf|0x80000000, 918, no_void|0x80000000}},	// 919
{nt_literal, ns_void, {0, 0, 0}},	// 920
{nt_arg, ns_void, {920, no_void|0x80000000, no_void|0x80000000}},	// 921
{nt_literal, ns_void, {0, 0, 0}},	// 922
{nt_arg, ns_void, {922, no_void|0x80000000, no_void|0x80000000}},	// 923
{nt_literal, ns_void, {1, 0, 0}},	// 924
{nt_arg, ns_void, {924, no_void|0x80000000, no_void|0x80000000}},	// 925
{nt__x86_ushr, ns_void, {923, 925, no_void|0x80000000}},	// 926
{nt_asgn, ns_void, {921, 926, no_void|0x80000000}},	// 927

// _sal
{nt_literal, ns_void, {0, 0, 0}},	// 928
{nt_arg, ns_void, {928, no_void|0x80000000, no_void|0x80000000}},	// 929
{nt_literal, ns_void, {1, 0, 0}},	// 930
{nt_arg, ns_void, {930, no_void|0x80000000, no_void|0x80000000}},	// 931
{nt__x86_shl_of, ns_void, {929, 931, no_void|0x80000000}},	// 932
{nt_asgn, ns_void, {no_x86_of|0x80000000, 932, no_void|0x80000000}},	// 933
{nt_literal, ns_void, {0, 0, 0}},	// 934
{nt_arg, ns_void, {934, no_void|0x80000000, no_void|0x80000000}},	// 935
{nt_literal, ns_void, {1, 0, 0}},	// 936
{nt_arg, ns_void, {936, no_void|0x80000000, no_void|0x80000000}},	// 937
{nt__x86_shl_af, ns_void, {935, 937, no_void|0x80000000}},	// 938
{nt_asgn, ns_void, {no_x86_af|0x80000000, 938, no_void|0x80000000}},	// 939
{nt_literal, ns_void, {0, 0, 0}},	// 940
{nt_arg, ns_void, {940, no_void|0x80000000, no_void|0x80000000}},	// 941
{nt_literal, ns_void, {1, 0, 0}},	// 942
{nt_arg, ns_void, {942, no_void|0x80000000, no_void|0x80000000}},	// 943
{nt__x86_shl_cf, ns_void, {941, 943, no_void|0x80000000}},	// 944
{nt_asgn, ns_void, {no_x86_cf|0x80000000, 944, no_void|0x80000000}},	// 945
{nt_literal, ns_void, {0, 0, 0}},	// 946
{nt_arg, ns_void, {946, no_void|0x80000000, no_void|0x80000000}},	// 947
{nt_literal, ns_void, {1, 0, 0}},	// 948
{nt_arg, ns_void, {948, no_void|0x80000000, no_void|0x80000000}},	// 949
{nt__x86_shl_sf, ns_void, {947, 949, no_void|0x80000000}},	// 950
{nt_asgn, ns_void, {no_x86_sf|0x80000000, 950, no_void|0x80000000}},	// 951
{nt_literal, ns_void, {0, 0, 0}},	// 952
{nt_arg, ns_void, {952, no_void|0x80000000, no_void|0x80000000}},	// 953
{nt_literal, ns_void, {1, 0, 0}},	// 954
{nt_arg, ns_void, {954, no_void|0x80000000, no_void|0x80000000}},	// 955
{nt__x86_shl_zf, ns_void, {953, 955, no_void|0x80000000}},	// 956
{nt_asgn, ns_void, {no_x86_zf|0x80000000, 956, no_void|0x80000000}},	// 957
{nt_literal, ns_void, {0, 0, 0}},	// 958
{nt_arg, ns_void, {958, no_void|0x80000000, no_void|0x80000000}},	// 959
{nt_literal, ns_void, {1, 0, 0}},	// 960
{nt_arg, ns_void, {960, no_void|0x80000000, no_void|0x80000000}},	// 961
{nt__x86_shl_pf, ns_void, {959, 961, no_void|0x80000000}},	// 962
{nt_asgn, ns_void, {no_x86_pf|0x80000000, 962, no_void|0x80000000}},	// 963
{nt_literal, ns_void, {0, 0, 0}},	// 964
{nt_arg, ns_void, {964, no_void|0x80000000, no_void|0x80000000}},	// 965
{nt_literal, ns_void, {0, 0, 0}},	// 966
{nt_arg, ns_void, {966, no_void|0x80000000, no_void|0x80000000}},	// 967
{nt_literal, ns_void, {1, 0, 0}},	// 968
{nt_arg, ns_void, {968, no_void|0x80000000, no_void|0x80000000}},	// 969
{nt__x86_shl, ns_void, {967, 969, no_void|0x80000000}},	// 970
{nt_asgn, ns_void, {965, 970, no_void|0x80000000}},	// 971

// sar
{nt_literal, ns_void, {0, 0, 0}},	// 972
{nt_arg, ns_void, {972, no_void|0x80000000, no_void|0x80000000}},	// 973
{nt_literal, ns_void, {1, 0, 0}},	// 974
{nt_arg, ns_void, {974, no_void|0x80000000, no_void|0x80000000}},	// 975
{nt__x86_sshr_of, ns_void, {973, 975, no_void|0x80000000}},	// 976
{nt_asgn, ns_void, {no_x86_of|0x80000000, 976, no_void|0x80000000}},	// 977
{nt_literal, ns_void, {0, 0, 0}},	// 978
{nt_arg, ns_void, {978, no_void|0x80000000, no_void|0x80000000}},	// 979
{nt_literal, ns_void, {1, 0, 0}},	// 980
{nt_arg, ns_void, {980, no_void|0x80000000, no_void|0x80000000}},	// 981
{nt__x86_sshr_af, ns_void, {979, 981, no_void|0x80000000}},	// 982
{nt_asgn, ns_void, {no_x86_af|0x80000000, 982, no_void|0x80000000}},	// 983
{nt_literal, ns_void, {0, 0, 0}},	// 984
{nt_arg, ns_void, {984, no_void|0x80000000, no_void|0x80000000}},	// 985
{nt_literal, ns_void, {1, 0, 0}},	// 986
{nt_arg, ns_void, {986, no_void|0x80000000, no_void|0x80000000}},	// 987
{nt__x86_sshr_cf, ns_void, {985, 987, no_void|0x80000000}},	// 988
{nt_asgn, ns_void, {no_x86_cf|0x80000000, 988, no_void|0x80000000}},	// 989
{nt_literal, ns_void, {0, 0, 0}},	// 990
{nt_arg, ns_void, {990, no_void|0x80000000, no_void|0x80000000}},	// 991
{nt_literal, ns_void, {1, 0, 0}},	// 992
{nt_arg, ns_void, {992, no_void|0x80000000, no_void|0x80000000}},	// 993
{nt__x86_sshr_sf, ns_void, {991, 993, no_void|0x80000000}},	// 994
{nt_asgn, ns_void, {no_x86_sf|0x80000000, 994, no_void|0x80000000}},	// 995
{nt_literal, ns_void, {0, 0, 0}},	// 996
{nt_arg, ns_void, {996, no_void|0x80000000, no_void|0x80000000}},	// 997
{nt_literal, ns_void, {1, 0, 0}},	// 998
{nt_arg, ns_void, {998, no_void|0x80000000, no_void|0x80000000}},	// 999
{nt__x86_sshr_zf, ns_void, {997, 999, no_void|0x80000000}},	// 1000
{nt_asgn, ns_void, {no_x86_zf|0x80000000, 1000, no_void|0x80000000}},	// 1001
{nt_literal, ns_void, {0, 0, 0}},	// 1002
{nt_arg, ns_void, {1002, no_void|0x80000000, no_void|0x80000000}},	// 1003
{nt_literal, ns_void, {1, 0, 0}},	// 1004
{nt_arg, ns_void, {1004, no_void|0x80000000, no_void|0x80000000}},	// 1005
{nt__x86_sshr_pf, ns_void, {1003, 1005, no_void|0x80000000}},	// 1006
{nt_asgn, ns_void, {no_x86_pf|0x80000000, 1006, no_void|0x80000000}},	// 1007
{nt_literal, ns_void, {0, 0, 0}},	// 1008
{nt_arg, ns_void, {1008, no_void|0x80000000, no_void|0x80000000}},	// 1009
{nt_literal, ns_void, {0, 0, 0}},	// 1010
{nt_arg, ns_void, {1010, no_void|0x80000000, no_void|0x80000000}},	// 1011
{nt_literal, ns_void, {1, 0, 0}},	// 1012
{nt_arg, ns_void, {1012, no_void|0x80000000, no_void|0x80000000}},	// 1013
{nt__x86_sshr, ns_void, {1011, 1013, no_void|0x80000000}},	// 1014
{nt_asgn, ns_void, {1009, 1014, no_void|0x80000000}},	// 1015

// inc
{nt_literal, ns_void, {0, 0, 0}},	// 1016
{nt_arg, ns_void, {1016, no_void|0x80000000, no_void|0x80000000}},	// 1017
{nt_literal, ns_void, {1, 0, 0}},	// 1018
{nt__x86_add_of, ns_void, {1017, 1018, no_void|0x80000000}},	// 1019
{nt_asgn, ns_void, {no_x86_of|0x80000000, 1019, no_void|0x80000000}},	// 1020
{nt_literal, ns_void, {0, 0, 0}},	// 1021
{nt_arg, ns_void, {1021, no_void|0x80000000, no_void|0x80000000}},	// 1022
{nt_trunc, ns_byte, {1022, no_void|0x80000000, no_void|0x80000000}},	// 1023
{nt_literal, ns_void, {1, 0, 0}},	// 1024
{nt__x86_add_af, ns_void, {1023, 1024, no_void|0x80000000}},	// 1025
{nt_asgn, ns_void, {no_x86_af|0x80000000, 1025, no_void|0x80000000}},	// 1026
{nt_literal, ns_void, {0, 0, 0}},	// 1027
{nt_arg, ns_void, {1027, no_void|0x80000000, no_void|0x80000000}},	// 1028
{nt_literal, ns_void, {0, 0, 0}},	// 1029
{nt_arg, ns_void, {1029, no_void|0x80000000, no_void|0x80000000}},	// 1030
{nt_literal, ns_void, {1, 0, 0}},	// 1031
{nt_add, ns_void, {1030, 1031, no_void|0x80000000}},	// 1032
{nt_asgn, ns_void, {1028, 1032, no_void|0x80000000}},	// 1033
{nt_literal, ns_void, {0, 0, 0}},	// 1034
{nt_arg, ns_void, {1034, no_void|0x80000000, no_void|0x80000000}},	// 1035
{nt_sign, ns_void, {1035, no_void|0x80000000, no_void|0x80000000}},	// 1036
{nt_asgn, ns_void, {no_x86_sf|0x80000000, 1036, no_void|0x80000000}},	// 1037
{nt_literal, ns_void, {0, 0, 0}},	// 1038
{nt_arg, ns_void, {1038, no_void|0x80000000, no_void|0x80000000}},	// 1039
{nt_is_zero, ns_void, {1039, no_void|0x80000000, no_void|0x80000000}},	// 1040
{nt_asgn, ns_void, {no_x86_zf|0x80000000, 1040, no_void|0x80000000}},	// 1041
{nt_literal, ns_void, {0, 0, 0}},	// 1042
{nt_arg, ns_void, {1042, no_void|0x80000000, no_void|0x80000000}},	// 1043
{nt_trunc, ns_byte, {1043, no_void|0x80000000, no_void|0x80000000}},	// 1044
{nt__x86_parity, ns_void, {1044, no_void|0x80000000, no_void|0x80000000}},	// 1045
{nt_asgn, ns_void, {no_x86_pf|0x80000000, 1045, no_void|0x80000000}},	// 1046

// dec
{nt_literal, ns_void, {0, 0, 0}},	// 1047
{nt_arg, ns_void, {1047, no_void|0x80000000, no_void|0x80000000}},	// 1048
{nt_literal, ns_void, {1, 0, 0}},	// 1049
{nt__x86_sub_of, ns_void, {1048, 1049, no_void|0x80000000}},	// 1050
{nt_asgn, ns_void, {no_x86_of|0x80000000, 1050, no_void|0x80000000}},	// 1051
{nt_literal, ns_void, {0, 0, 0}},	// 1052
{nt_arg, ns_void, {1052, no_void|0x80000000, no_void|0x80000000}},	// 1053
{nt_trunc, ns_byte, {1053, no_void|0x80000000, no_void|0x80000000}},	// 1054
{nt_literal, ns_void, {1, 0, 0}},	// 1055
{nt__x86_sub_af, ns_void, {1054, 1055, no_void|0x80000000}},	// 1056
{nt_asgn, ns_void, {no_x86_af|0x80000000, 1056, no_void|0x80000000}},	// 1057
{nt_literal, ns_void, {0, 0, 0}},	// 1058
{nt_arg, ns_void, {1058, no_void|0x80000000, no_void|0x80000000}},	// 1059
{nt_literal, ns_void, {0, 0, 0}},	// 1060
{nt_arg, ns_void, {1060, no_void|0x80000000, no_void|0x80000000}},	// 1061
{nt_literal, ns_void, {1, 0, 0}},	// 1062
{nt_sub, ns_void, {1061, 1062, no_void|0x80000000}},	// 1063
{nt_asgn, ns_void, {1059, 1063, no_void|0x80000000}},	// 1064
{nt_literal, ns_void, {0, 0, 0}},	// 1065
{nt_arg, ns_void, {1065, no_void|0x80000000, no_void|0x80000000}},	// 1066
{nt_sign, ns_void, {1066, no_void|0x80000000, no_void|0x80000000}},	// 1067
{nt_asgn, ns_void, {no_x86_sf|0x80000000, 1067, no_void|0x80000000}},	// 1068
{nt_literal, ns_void, {0, 0, 0}},	// 1069
{nt_arg, ns_void, {1069, no_void|0x80000000, no_void|0x80000000}},	// 1070
{nt_is_zero, ns_void, {1070, no_void|0x80000000, no_void|0x80000000}},	// 1071
{nt_asgn, ns_void, {no_x86_zf|0x80000000, 1071, no_void|0x80000000}},	// 1072
{nt_literal, ns_void, {0, 0, 0}},	// 1073
{nt_arg, ns_void, {1073, no_void|0x80000000, no_void|0x80000000}},	// 1074
{nt_trunc, ns_byte, {1074, no_void|0x80000000, no_void|0x80000000}},	// 1075
{nt__x86_parity, ns_void, {1075, no_void|0x80000000, no_void|0x80000000}},	// 1076
{nt_asgn, ns_void, {no_x86_pf|0x80000000, 1076, no_void|0x80000000}},	// 1077

// _push
{nt_literal, ns_void, {0, 0, 0}},	// 1078
{nt_stack, ns_osz, {1078, no_void|0x80000000, no_void|0x80000000}},	// 1079
{nt_literal, ns_void, {0, 0, 0}},	// 1080
{nt_arg, ns_void, {1080, no_void|0x80000000, no_void|0x80000000}},	// 1081
{nt_asgn, ns_void, {1079, 1081, no_void|0x80000000}},	// 1082

// _pop
{nt_literal, ns_void, {0, 0, 0}},	// 1083
{nt_arg, ns_void, {1083, no_void|0x80000000, no_void|0x80000000}},	// 1084
{nt_literal, ns_void, {0, 0, 0}},	// 1085
{nt_stack, ns_osz, {1085, no_void|0x80000000, no_void|0x80000000}},	// 1086
{nt_asgn, ns_void, {1084, 1086, no_void|0x80000000}},	// 1087

// movzx
{nt_literal, ns_void, {0, 0, 0}},	// 1088
{nt_arg, ns_void, {1088, no_void|0x80000000, no_void|0x80000000}},	// 1089
{nt_literal, ns_void, {1, 0, 0}},	// 1090
{nt_arg, ns_void, {1090, no_void|0x80000000, no_void|0x80000000}},	// 1091
{nt_zx, ns_argsize_0, {1091, no_void|0x80000000, no_void|0x80000000}},	// 1092
{nt_asgn, ns_void, {1089, 1092, no_void|0x80000000}},	// 1093

// movsx
{nt_literal, ns_void, {0, 0, 0}},	// 1094
{nt_arg, ns_void, {1094, no_void|0x80000000, no_void|0x80000000}},	// 1095
{nt_literal, ns_void, {1, 0, 0}},	// 1096
{nt_arg, ns_void, {1096, no_void|0x80000000, no_void|0x80000000}},	// 1097
{nt_sx, ns_argsize_0, {1097, no_void|0x80000000, no_void|0x80000000}},	// 1098
{nt_asgn, ns_void, {1095, 1098, no_void|0x80000000}},	// 1099

// shld
{nt_literal, ns_void, {0, 0, 0}},	// 1100
{nt_arg, ns_void, {1100, no_void|0x80000000, no_void|0x80000000}},	// 1101
{nt_literal, ns_void, {0, 0, 0}},	// 1102
{nt_arg, ns_void, {1102, no_void|0x80000000, no_void|0x80000000}},	// 1103
{nt_literal, ns_void, {1, 0, 0}},	// 1104
{nt_arg, ns_void, {1104, no_void|0x80000000, no_void|0x80000000}},	// 1105
{nt_literal, ns_void, {2, 0, 0}},	// 1106
{nt_arg, ns_void, {1106, no_void|0x80000000, no_void|0x80000000}},	// 1107
{nt__x86_shld, ns_void, {1103, 1105, 1107}},	// 1108
{nt_asgn, ns_void, {1101, 1108, no_void|0x80000000}},	// 1109
{nt_literal, ns_void, {0, 0, 0}},	// 1110
{nt_arg, ns_void, {1110, no_void|0x80000000, no_void|0x80000000}},	// 1111
{nt_literal, ns_void, {1, 0, 0}},	// 1112
{nt_arg, ns_void, {1112, no_void|0x80000000, no_void|0x80000000}},	// 1113
{nt_literal, ns_void, {2, 0, 0}},	// 1114
{nt_arg, ns_void, {1114, no_void|0x80000000, no_void|0x80000000}},	// 1115
{nt__x86_shld_of, ns_void, {1111, 1113, 1115}},	// 1116
{nt_asgn, ns_void, {no_x86_of|0x80000000, 1116, no_void|0x80000000}},	// 1117
{nt_literal, ns_void, {0, 0, 0}},	// 1118
{nt_arg, ns_void, {1118, no_void|0x80000000, no_void|0x80000000}},	// 1119
{nt_literal, ns_void, {1, 0, 0}},	// 1120
{nt_arg, ns_void, {1120, no_void|0x80000000, no_void|0x80000000}},	// 1121
{nt_literal, ns_void, {2, 0, 0}},	// 1122
{nt_arg, ns_void, {1122, no_void|0x80000000, no_void|0x80000000}},	// 1123
{nt__x86_shld_sf, ns_void, {1119, 1121, 1123}},	// 1124
{nt_asgn, ns_void, {no_x86_sf|0x80000000, 1124, no_void|0x80000000}},	// 1125
{nt_literal, ns_void, {0, 0, 0}},	// 1126
{nt_arg, ns_void, {1126, no_void|0x80000000, no_void|0x80000000}},	// 1127
{nt_literal, ns_void, {1, 0, 0}},	// 1128
{nt_arg, ns_void, {1128, no_void|0x80000000, no_void|0x80000000}},	// 1129
{nt_literal, ns_void, {2, 0, 0}},	// 1130
{nt_arg, ns_void, {1130, no_void|0x80000000, no_void|0x80000000}},	// 1131
{nt__x86_shld_zf, ns_void, {1127, 1129, 1131}},	// 1132
{nt_asgn, ns_void, {no_x86_zf|0x80000000, 1132, no_void|0x80000000}},	// 1133
{nt_literal, ns_void, {0, 0, 0}},	// 1134
{nt_arg, ns_void, {1134, no_void|0x80000000, no_void|0x80000000}},	// 1135
{nt_literal, ns_void, {1, 0, 0}},	// 1136
{nt_arg, ns_void, {1136, no_void|0x80000000, no_void|0x80000000}},	// 1137
{nt_literal, ns_void, {2, 0, 0}},	// 1138
{nt_arg, ns_void, {1138, no_void|0x80000000, no_void|0x80000000}},	// 1139
{nt__x86_shld_af, ns_void, {1135, 1137, 1139}},	// 1140
{nt_asgn, ns_void, {no_x86_af|0x80000000, 1140, no_void|0x80000000}},	// 1141
{nt_literal, ns_void, {0, 0, 0}},	// 1142
{nt_arg, ns_void, {1142, no_void|0x80000000, no_void|0x80000000}},	// 1143
{nt_literal, ns_void, {1, 0, 0}},	// 1144
{nt_arg, ns_void, {1144, no_void|0x80000000, no_void|0x80000000}},	// 1145
{nt_literal, ns_void, {2, 0, 0}},	// 1146
{nt_arg, ns_void, {1146, no_void|0x80000000, no_void|0x80000000}},	// 1147
{nt__x86_shld_pf, ns_void, {1143, 1145, 1147}},	// 1148
{nt_asgn, ns_void, {no_x86_pf|0x80000000, 1148, no_void|0x80000000}},	// 1149
{nt_literal, ns_void, {0, 0, 0}},	// 1150
{nt_arg, ns_void, {1150, no_void|0x80000000, no_void|0x80000000}},	// 1151
{nt_literal, ns_void, {1, 0, 0}},	// 1152
{nt_arg, ns_void, {1152, no_void|0x80000000, no_void|0x80000000}},	// 1153
{nt_literal, ns_void, {2, 0, 0}},	// 1154
{nt_arg, ns_void, {1154, no_void|0x80000000, no_void|0x80000000}},	// 1155
{nt__x86_shld_cf, ns_void, {1151, 1153, 1155}},	// 1156
{nt_asgn, ns_void, {no_x86_cf|0x80000000, 1156, no_void|0x80000000}},	// 1157

// shrd
{nt_literal, ns_void, {0, 0, 0}},	// 1158
{nt_arg, ns_void, {1158, no_void|0x80000000, no_void|0x80000000}},	// 1159
{nt_literal, ns_void, {0, 0, 0}},	// 1160
{nt_arg, ns_void, {1160, no_void|0x80000000, no_void|0x80000000}},	// 1161
{nt_literal, ns_void, {1, 0, 0}},	// 1162
{nt_arg, ns_void, {1162, no_void|0x80000000, no_void|0x80000000}},	// 1163
{nt_literal, ns_void, {2, 0, 0}},	// 1164
{nt_arg, ns_void, {1164, no_void|0x80000000, no_void|0x80000000}},	// 1165
{nt__x86_shrd, ns_void, {1161, 1163, 1165}},	// 1166
{nt_asgn, ns_void, {1159, 1166, no_void|0x80000000}},	// 1167
{nt_literal, ns_void, {0, 0, 0}},	// 1168
{nt_arg, ns_void, {1168, no_void|0x80000000, no_void|0x80000000}},	// 1169
{nt_literal, ns_void, {1, 0, 0}},	// 1170
{nt_arg, ns_void, {1170, no_void|0x80000000, no_void|0x80000000}},	// 1171
{nt_literal, ns_void, {2, 0, 0}},	// 1172
{nt_arg, ns_void, {1172, no_void|0x80000000, no_void|0x80000000}},	// 1173
{nt__x86_shrd_of, ns_void, {1169, 1171, 1173}},	// 1174
{nt_asgn, ns_void, {no_x86_of|0x80000000, 1174, no_void|0x80000000}},	// 1175
{nt_literal, ns_void, {0, 0, 0}},	// 1176
{nt_arg, ns_void, {1176, no_void|0x80000000, no_void|0x80000000}},	// 1177
{nt_literal, ns_void, {1, 0, 0}},	// 1178
{nt_arg, ns_void, {1178, no_void|0x80000000, no_void|0x80000000}},	// 1179
{nt_literal, ns_void, {2, 0, 0}},	// 1180
{nt_arg, ns_void, {1180, no_void|0x80000000, no_void|0x80000000}},	// 1181
{nt__x86_shrd_sf, ns_void, {1177, 1179, 1181}},	// 1182
{nt_asgn, ns_void, {no_x86_sf|0x80000000, 1182, no_void|0x80000000}},	// 1183
{nt_literal, ns_void, {0, 0, 0}},	// 1184
{nt_arg, ns_void, {1184, no_void|0x80000000, no_void|0x80000000}},	// 1185
{nt_literal, ns_void, {1, 0, 0}},	// 1186
{nt_arg, ns_void, {1186, no_void|0x80000000, no_void|0x80000000}},	// 1187
{nt_literal, ns_void, {2, 0, 0}},	// 1188
{nt_arg, ns_void, {1188, no_void|0x80000000, no_void|0x80000000}},	// 1189
{nt__x86_shrd_zf, ns_void, {1185, 1187, 1189}},	// 1190
{nt_asgn, ns_void, {no_x86_zf|0x80000000, 1190, no_void|0x80000000}},	// 1191
{nt_literal, ns_void, {0, 0, 0}},	// 1192
{nt_arg, ns_void, {1192, no_void|0x80000000, no_void|0x80000000}},	// 1193
{nt_literal, ns_void, {1, 0, 0}},	// 1194
{nt_arg, ns_void, {1194, no_void|0x80000000, no_void|0x80000000}},	// 1195
{nt_literal, ns_void, {2, 0, 0}},	// 1196
{nt_arg, ns_void, {1196, no_void|0x80000000, no_void|0x80000000}},	// 1197
{nt__x86_shrd_af, ns_void, {1193, 1195, 1197}},	// 1198
{nt_asgn, ns_void, {no_x86_af|0x80000000, 1198, no_void|0x80000000}},	// 1199
{nt_literal, ns_void, {0, 0, 0}},	// 1200
{nt_arg, ns_void, {1200, no_void|0x80000000, no_void|0x80000000}},	// 1201
{nt_literal, ns_void, {1, 0, 0}},	// 1202
{nt_arg, ns_void, {1202, no_void|0x80000000, no_void|0x80000000}},	// 1203
{nt_literal, ns_void, {2, 0, 0}},	// 1204
{nt_arg, ns_void, {1204, no_void|0x80000000, no_void|0x80000000}},	// 1205
{nt__x86_shrd_pf, ns_void, {1201, 1203, 1205}},	// 1206
{nt_asgn, ns_void, {no_x86_pf|0x80000000, 1206, no_void|0x80000000}},	// 1207
{nt_literal, ns_void, {0, 0, 0}},	// 1208
{nt_arg, ns_void, {1208, no_void|0x80000000, no_void|0x80000000}},	// 1209
{nt_literal, ns_void, {1, 0, 0}},	// 1210
{nt_arg, ns_void, {1210, no_void|0x80000000, no_void|0x80000000}},	// 1211
{nt_literal, ns_void, {2, 0, 0}},	// 1212
{nt_arg, ns_void, {1212, no_void|0x80000000, no_void|0x80000000}},	// 1213
{nt__x86_shrd_cf, ns_void, {1209, 1211, 1213}},	// 1214
{nt_asgn, ns_void, {no_x86_cf|0x80000000, 1214, no_void|0x80000000}},	// 1215

// leave
{nt_asgn, ns_void, {no_x86_xsp|0x80000000, no_x86_xbp|0x80000000, no_void|0x80000000}},	// 1216
{nt_literal, ns_void, {0, 0, 0}},	// 1217
{nt_stack, ns_osz, {1217, no_void|0x80000000, no_void|0x80000000}},	// 1218
{nt_asgn, ns_void, {no_x86_xbp|0x80000000, 1218, no_void|0x80000000}},	// 1219

// end of table
{nt_void}
};

U4 sem_insn__cmovcc[] = {
	11,	// total size
	0,	// start index
	0xa,
	0xffffffff	// end
};

U4 sem_insn__setcc[] = {
	7,	// total size
	11,	// start index
	0x11,
	0xffffffff	// end
};

U4 sem_insn__nopmb[] = {
	1,	// total size
	18,	// start index
	0x12,
	0xffffffff	// end
};

U4 sem_insn__sxacc[] = {
	2,	// total size
	19,	// start index
	0x14,
	0xffffffff	// end
};

U4 sem_insn__sxdax[] = {
	2,	// total size
	21,	// start index
	0x16,
	0xffffffff	// end
};

U4 sem_insn_cmc[] = {
	2,	// total size
	23,	// start index
	0x18,
	0xffffffff	// end
};

U4 sem_insn_clc[] = {
	2,	// total size
	25,	// start index
	0x1a,
	0xffffffff	// end
};

U4 sem_insn_stc[] = {
	2,	// total size
	27,	// start index
	0x1c,
	0xffffffff	// end
};

U4 sem_insn_cld[] = {
	2,	// total size
	29,	// start index
	0x1e,
	0xffffffff	// end
};

U4 sem_insn_std[] = {
	2,	// total size
	31,	// start index
	0x20,
	0xffffffff	// end
};

U4 sem_insn_lea[] = {
	5,	// total size
	33,	// start index
	0x25,
	0xffffffff	// end
};

U4 sem_insn__cmul3[] = {
	24,	// total size
	38,	// start index
	0x2d,
	0x2e,
	0x2f,
	0x30,
	0x31,
	0x37,
	0x3d,
	0xffffffff	// end
};

U4 sem_insn__cmul2[] = {
	24,	// total size
	62,	// start index
	0x45,
	0x46,
	0x47,
	0x48,
	0x49,
	0x4f,
	0x55,
	0xffffffff	// end
};

U4 sem_insn__imulb[] = {
	16,	// total size
	86,	// start index
	0x59,
	0x5a,
	0x5b,
	0x5c,
	0x5d,
	0x61,
	0x65,
	0xffffffff	// end
};

U4 sem_insn_imul[] = {
	16,	// total size
	102,	// start index
	0x69,
	0x6a,
	0x6b,
	0x6c,
	0x6d,
	0x71,
	0x75,
	0xffffffff	// end
};

U4 sem_insn__mulb[] = {
	16,	// total size
	118,	// start index
	0x79,
	0x7a,
	0x7b,
	0x7c,
	0x7d,
	0x81,
	0x85,
	0xffffffff	// end
};

U4 sem_insn_mul[] = {
	16,	// total size
	134,	// start index
	0x89,
	0x8a,
	0x8b,
	0x8c,
	0x8d,
	0x91,
	0x95,
	0xffffffff	// end
};

U4 sem_insn__divb[] = {
	24,	// total size
	150,	// start index
	0x9b,
	0xa1,
	0xa4,
	0xa7,
	0xa8,
	0xa9,
	0xaa,
	0xab,
	0xac,
	0xad,
	0xffffffff	// end
};

U4 sem_insn_div[] = {
	24,	// total size
	174,	// start index
	0xb3,
	0xb9,
	0xbc,
	0xbf,
	0xc0,
	0xc1,
	0xc2,
	0xc3,
	0xc4,
	0xc5,
	0xffffffff	// end
};

U4 sem_insn__idivb[] = {
	24,	// total size
	198,	// start index
	0xcb,
	0xd1,
	0xd4,
	0xd7,
	0xd8,
	0xd9,
	0xda,
	0xdb,
	0xdc,
	0xdd,
	0xffffffff	// end
};

U4 sem_insn_idiv[] = {
	24,	// total size
	222,	// start index
	0xe3,
	0xe9,
	0xec,
	0xef,
	0xf0,
	0xf1,
	0xf2,
	0xf3,
	0xf4,
	0xf5,
	0xffffffff	// end
};

U4 sem_insn_test[] = {
	26,	// total size
	246,	// start index
	0xf7,
	0xf8,
	0xfa,
	0x102,
	0x106,
	0x10a,
	0x10f,
	0xffffffff	// end
};

U4 sem_insn__xchg[] = {
	15,	// total size
	272,	// start index
	0x114,
	0x119,
	0x11e,
	0xffffffff	// end
};

U4 sem_insn_mov[] = {
	5,	// total size
	287,	// start index
	0x123,
	0xffffffff	// end
};

U4 sem_insn_not[] = {
	6,	// total size
	292,	// start index
	0x129,
	0xffffffff	// end
};

U4 sem_insn_neg[] = {
	35,	// total size
	298,	// start index
	0x12e,
	0x134,
	0x139,
	0x13f,
	0x143,
	0x147,
	0x14c,
	0xffffffff	// end
};

U4 sem_insn_add[] = {
	41,	// total size
	333,	// start index
	0x152,
	0x15a,
	0x160,
	0x168,
	0x16c,
	0x170,
	0x175,
	0xffffffff	// end
};

U4 sem_insn_or[] = {
	26,	// total size
	374,	// start index
	0x177,
	0x178,
	0x17a,
	0x182,
	0x186,
	0x18a,
	0x18f,
	0xffffffff	// end
};

U4 sem_insn_adc[] = {
	48,	// total size
	400,	// start index
	0x192,
	0x198,
	0x1a0,
	0x1a6,
	0x1b2,
	0x1b6,
	0x1ba,
	0x1bf,
	0xffffffff	// end
};

U4 sem_insn_sbb[] = {
	48,	// total size
	448,	// start index
	0x1c2,
	0x1c8,
	0x1d0,
	0x1d6,
	0x1e2,
	0x1e6,
	0x1ea,
	0x1ef,
	0xffffffff	// end
};

U4 sem_insn_and[] = {
	26,	// total size
	496,	// start index
	0x1f1,
	0x1f2,
	0x1f4,
	0x1fc,
	0x200,
	0x204,
	0x209,
	0xffffffff	// end
};

U4 sem_insn_sub[] = {
	41,	// total size
	522,	// start index
	0x20f,
	0x217,
	0x21d,
	0x225,
	0x229,
	0x22d,
	0x232,
	0xffffffff	// end
};

U4 sem_insn_xor[] = {
	26,	// total size
	563,	// start index
	0x234,
	0x235,
	0x237,
	0x23f,
	0x243,
	0x247,
	0x24c,
	0xffffffff	// end
};

U4 sem_insn_cmp[] = {
	41,	// total size
	589,	// start index
	0x252,
	0x25a,
	0x260,
	0x268,
	0x26c,
	0x270,
	0x275,
	0xffffffff	// end
};

U4 sem_insn_rol[] = {
	44,	// total size
	630,	// start index
	0x27b,
	0x281,
	0x287,
	0x28d,
	0x293,
	0x299,
	0x2a1,
	0xffffffff	// end
};

U4 sem_insn_ror[] = {
	44,	// total size
	674,	// start index
	0x2a7,
	0x2ad,
	0x2b3,
	0x2b9,
	0x2bf,
	0x2c5,
	0x2cd,
	0xffffffff	// end
};

U4 sem_insn_rcl[] = {
	61,	// total size
	718,	// start index
	0x2d0,
	0x2d8,
	0x2e0,
	0x2e8,
	0x2f0,
	0x2f8,
	0x300,
	0x30a,
	0xffffffff	// end
};

U4 sem_insn_rcr[] = {
	61,	// total size
	779,	// start index
	0x30d,
	0x315,
	0x31d,
	0x325,
	0x32d,
	0x335,
	0x33d,
	0x347,
	0xffffffff	// end
};

U4 sem_insn_shl[] = {
	44,	// total size
	840,	// start index
	0x34d,
	0x353,
	0x359,
	0x35f,
	0x365,
	0x36b,
	0x373,
	0xffffffff	// end
};

U4 sem_insn_shr[] = {
	44,	// total size
	884,	// start index
	0x379,
	0x37f,
	0x385,
	0x38b,
	0x391,
	0x397,
	0x39f,
	0xffffffff	// end
};

U4 sem_insn__sal[] = {
	44,	// total size
	928,	// start index
	0x3a5,
	0x3ab,
	0x3b1,
	0x3b7,
	0x3bd,
	0x3c3,
	0x3cb,
	0xffffffff	// end
};

U4 sem_insn_sar[] = {
	44,	// total size
	972,	// start index
	0x3d1,
	0x3d7,
	0x3dd,
	0x3e3,
	0x3e9,
	0x3ef,
	0x3f7,
	0xffffffff	// end
};

U4 sem_insn_inc[] = {
	31,	// total size
	1016,	// start index
	0x3fc,
	0x402,
	0x409,
	0x40d,
	0x411,
	0x416,
	0xffffffff	// end
};

U4 sem_insn_dec[] = {
	31,	// total size
	1047,	// start index
	0x41b,
	0x421,
	0x428,
	0x42c,
	0x430,
	0x435,
	0xffffffff	// end
};

U4 sem_insn__push[] = {
	5,	// total size
	1078,	// start index
	0x43a,
	0xffffffff	// end
};

U4 sem_insn__pop[] = {
	5,	// total size
	1083,	// start index
	0x43f,
	0xffffffff	// end
};

U4 sem_insn_movzx[] = {
	6,	// total size
	1088,	// start index
	0x445,
	0xffffffff	// end
};

U4 sem_insn_movsx[] = {
	6,	// total size
	1094,	// start index
	0x44b,
	0xffffffff	// end
};

U4 sem_insn_shld[] = {
	58,	// total size
	1100,	// start index
	0x455,
	0x45d,
	0x465,
	0x46d,
	0x475,
	0x47d,
	0x485,
	0xffffffff	// end
};

U4 sem_insn_shrd[] = {
	58,	// total size
	1158,	// start index
	0x48f,
	0x497,
	0x49f,
	0x4a7,
	0x4af,
	0x4b7,
	0x4bf,
	0xffffffff	// end
};

U4 sem_insn_leave[] = {
	4,	// total size
	1216,	// start index
	0x4c0,
	0x4c3,
	0xffffffff	// end
};

tcode_element x86_tcode_table[] =
{
	{ sem_insn__cmovcc, 0, 3},	// _cmovcc
	{ 0, 0, 0},	// emms
	{ 0, 0, 1},	// ldmxcsr
	{ 0, 0, 1},	// fxrstor
	{ 0, 0, 1},	// fxsave
	{ 0, 0, 0},	// fxtract
	{ sem_insn__setcc, 0, 2},	// _setcc
	{ sem_insn__nopmb, 0, 1},	// _nopmb
	{ 0, 0, 0},	// _usalc
	{ 0, 0, 0},	// _uint1
	{ 0, 0, 1},	// _aad
	{ 0, 0, 1},	// _aam
	{ 0, 0, 0},	// das
	{ 0, 0, 0},	// daa
	{ 0, 0, 0},	// aaa
	{ 0, 0, 0},	// aas
	{ sem_insn__sxacc, 0, 0},	// _sxacc
	{ sem_insn__sxdax, 0, 0},	// _sxdax
	{ 0, 0, 0},	// sahf
	{ 0, 0, 0},	// lahf
	{ sem_insn_cmc, 0, 0},	// cmc
	{ sem_insn_clc, 0, 0},	// clc
	{ sem_insn_stc, 0, 0},	// stc
	{ sem_insn_cld, 0, 0},	// cld
	{ sem_insn_std, 0, 0},	// std
	{ sem_insn_lea, 0, 2},	// lea
	{ 0, 0, 2},	// bound
	{ 0, 0, 0},	// into
	{ sem_insn__cmul3, 0, 3},	// _cmul3
	{ sem_insn__cmul2, 0, 2},	// _cmul2
	{ sem_insn__imulb, 0, 1},	// _imulb
	{ sem_insn_imul, 0, 1},	// imul
	{ sem_insn__mulb, 0, 1},	// _mulb
	{ sem_insn_mul, 0, 1},	// mul
	{ sem_insn__divb, 2, 1},	// _divb
	{ sem_insn_div, 2, 1},	// div
	{ sem_insn__idivb, 2, 1},	// _idivb
	{ sem_insn_idiv, 2, 1},	// idiv
	{ sem_insn_test, 1, 2},	// test
	{ sem_insn__xchg, 1, 2},	// _xchg
	{ sem_insn_mov, 0, 2},	// mov
	{ sem_insn_not, 0, 1},	// not
	{ sem_insn_neg, 0, 1},	// neg
	{ sem_insn_add, 0, 2},	// add
	{ sem_insn_or, 0, 2},	// or
	{ sem_insn_adc, 1, 2},	// adc
	{ sem_insn_sbb, 1, 2},	// sbb
	{ sem_insn_and, 0, 2},	// and
	{ sem_insn_sub, 0, 2},	// sub
	{ sem_insn_xor, 0, 2},	// xor
	{ sem_insn_cmp, 1, 2},	// cmp
	{ sem_insn_rol, 0, 2},	// rol
	{ sem_insn_ror, 0, 2},	// ror
	{ sem_insn_rcl, 1, 2},	// rcl
	{ sem_insn_rcr, 1, 2},	// rcr
	{ sem_insn_shl, 0, 2},	// shl
	{ sem_insn_shr, 0, 2},	// shr
	{ sem_insn__sal, 0, 2},	// _sal
	{ sem_insn_sar, 0, 2},	// sar
	{ sem_insn_inc, 0, 1},	// inc
	{ sem_insn_dec, 0, 1},	// dec
	{ 0, 0, 1},	// _xlat
	{ 0, 0, 2},	// _ins
	{ 0, 0, 2},	// _outs
	{ 0, 0, 2},	// _movs
	{ 0, 0, 2},	// _cmps
	{ 0, 0, 2},	// _stos
	{ 0, 0, 2},	// _lods
	{ 0, 0, 2},	// _scas
	{ 0, 0, 1},	// _pushsr
	{ 0, 0, 1},	// _popsr
	{ sem_insn__push, 0, 1},	// _push
	{ sem_insn__pop, 0, 1},	// _pop
	{ 0, 0, 2},	// _movsrv
	{ 0, 0, 2},	// _movvsr
	{ 0, 0, 0},	// _pusha
	{ 0, 0, 0},	// _popa
	{ 0, 0, 0},	// _pushf
	{ 0, 0, 0},	// _popf
	{ 0, 0, 2},	// _jcc
	{ 0, 0, 1},	// call
	{ 0, 0, 1},	// _calli
	{ 0, 0, 2},	// _callfd
	{ 0, 0, 1},	// _callfi
	{ 0, 0, 1},	// jmp
	{ 0, 0, 1},	// _jmpi
	{ 0, 0, 2},	// _jmpfd
	{ 0, 0, 1},	// _jmpfi
	{ 0, 0, 1},	// _retnum
	{ 0, 0, 1},	// _ret
	{ 0, 0, 1},	// _retfnum
	{ 0, 0, 1},	// _retf
	{ 0, 0, 0},	// _int3
	{ 0, 0, 1},	// int
	{ 0, 0, 0},	// _iret
	{ 0, 0, 2},	// _loopnz
	{ 0, 0, 2},	// _loopz
	{ 0, 0, 2},	// _loop
	{ 0, 0, 2},	// _jrcxz
	{ 0, 0, 0},	// hlt
	{ 0, 0, 0},	// cli
	{ 0, 0, 0},	// sti
	{ 0, 0, 2},	// in
	{ 0, 0, 2},	// out
	{ 0, 0, 0},	// wait
	{ 0, 0, 2},	// bsf
	{ 0, 0, 2},	// bsr
	{ 0, 0, 2},	// bt
	{ 0, 0, 2},	// bts
	{ 0, 0, 2},	// btr
	{ 0, 0, 2},	// btc
	{ 0, 0, 1},	// bswap
	{ 0, 0, 2},	// cmpxchg
	{ sem_insn_movzx, 0, 2},	// movzx
	{ sem_insn_movsx, 0, 2},	// movsx
	{ 0, 0, 2},	// _arpl_movsxd
	{ sem_insn_shld, 0, 3},	// shld
	{ sem_insn_shrd, 0, 3},	// shrd
	{ 0, 0, 1},	// _cmpxchgxb
	{ 0, 0, 1},	// vmxon
	{ 0, 0, 1},	// vmclear
	{ 0, 0, 0},	// vmcall
	{ 0, 0, 0},	// vmlaunch
	{ 0, 0, 0},	// vmresume
	{ 0, 0, 0},	// vmxoff
	{ 0, 0, 1},	// vmptrld
	{ 0, 0, 1},	// vmptrst
	{ 0, 0, 2},	// vmread
	{ 0, 0, 2},	// vmwrite
	{ 0, 0, 2},	// xadd
	{ 0, 0, 2},	// les
	{ 0, 0, 2},	// lds
	{ 0, 0, 2},	// lss
	{ 0, 0, 2},	// lfs
	{ 0, 0, 2},	// lgs
	{ 0, 0, 2},	// enter
	{ sem_insn_leave, 0, 0},	// leave
	{ 0, 0, 1},	// sidt
	{ 0, 0, 1},	// lgdt
	{ 0, 0, 1},	// lidt
	{ 0, 0, 1},	// sldt
	{ 0, 0, 1},	// smsw
	{ 0, 0, 1},	// lmsw
	{ 0, 0, 1},	// str
	{ 0, 0, 1},	// verr
	{ 0, 0, 1},	// verw
	{ 0, 0, 0},	// wbinvd
	{ 0, 0, 0},	// wrmsr
	{ 0, 0, 0},	// rdmsr
	{ 0, 0, 0},	// rdpmc
	{ 0, 0, 0},	// rdtsc
	{ 0, 0, 0},	// clts
	{ 0, 0, 0},	// cpuid
	{ 0, 0, 0},	// invd
	{ 0, 0, 1},	// invlpg
	{ 0, 0, 2},	// lar
	{ 0, 0, 1},	// lldt
	{ 0, 0, 2},	// lsl
	{ 0, 0, 1},	// ltr
	{ 0, 0, 2},	// _movcr
	{ 0, 0, 2},	// _movdr
	{ 0, 0, 1},	// sgdt
	{ 0, 0, 0},	// rsm
	{ 0, 0, 0},	// ud2
	{ 0, 0, 0},	// sysenter
	{ 0, 0, 0},	// sysexit
	{0, 0, 0}	// end of table
};

