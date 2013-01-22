/*
  Copyright (C) 2012 Stuffomatic Ltd. <contact@stuff-o-matic.com>

  All rights reserved.
*/
/**
 * \file
 * \brief This file contains all key codes values and must not be included
 *        anywhere but in the input/keyboard.hpp file.
 *
 * Values are here only for readability, but are members of the input::keyboard
 * class.
 */

static const key_code kc_null                 = '\0';
static const key_code kc_start_heading        = 1;
static const key_code kc_start_text           = 2;
static const key_code kc_end_of_text          = 3;
static const key_code kc_end_of_transmission  = 4;
static const key_code kc_enquiry              = 5;
static const key_code kc_acknowledge          = 6;
static const key_code kc_bell                 = 7;
static const key_code kc_backspace            = 8;
static const key_code kc_tab                  = '\t';
static const key_code kc_new_line             = '\n';
static const key_code kc_vertical_tab         = 11;
static const key_code kc_new_page             = 12;
static const key_code kc_carriage_return      = '\r';
static const key_code kc_shift_out            = 14;
static const key_code kc_shift_in             = 15;
static const key_code kc_data_link_escape     = 16;
static const key_code kc_device_control_1     = 17;
static const key_code kc_device_control_2     = 18;
static const key_code kc_pause                = 19;
static const key_code kc_device_control_4     = 20;
static const key_code kc_negative_acknowledge = 21;
static const key_code kc_synchronous_idle     = 22;
static const key_code kc_end_of_block         = 23;
static const key_code kc_cancel               = 24;
static const key_code kc_end_of_medium        = 25;
static const key_code kc_substitute           = 26;
static const key_code kc_escape               = 27;
static const key_code kc_file_separator       = 28;
static const key_code kc_group_separator      = 29;
static const key_code kc_record_separator     = 30;
static const key_code kc_unit_separator       = 31;

static const key_code kc_space            = ' '; // 32
static const key_code kc_exclamation_mark = '!'; // 33
static const key_code kc_double_quotes    = '"'; // 34
static const key_code kc_hash             = '#'; // 35
static const key_code kc_dollar           = '$'; // 36
static const key_code kc_percent          = '%'; // 37
static const key_code kc_ampersand        = '&'; // 38
static const key_code kc_quote            = '\''; // 39

static const key_code kc_left_parenthesis  = '('; // 40
static const key_code kc_right_parenthesis = ')'; // 41

static const key_code kc_asterisk = '*'; // 42
static const key_code kc_plus     = '+'; // 43
static const key_code kc_comma    = ','; // 44
static const key_code kc_minus    = '-'; // 45
static const key_code kc_period   = '.'; // 46
static const key_code kc_slash    = '/'; // 47

static const key_code kc_0 = '0'; // 48
static const key_code kc_1 = '1'; // 49
static const key_code kc_2 = '2'; // 50
static const key_code kc_3 = '3'; // 51
static const key_code kc_4 = '4'; // 52
static const key_code kc_5 = '5'; // 53
static const key_code kc_6 = '6'; // 54
static const key_code kc_7 = '7'; // 55
static const key_code kc_8 = '8'; // 56
static const key_code kc_9 = '9'; // 57

static const key_code kc_colon     = ':'; // 58
static const key_code kc_semicolon = ';'; // 59
static const key_code kc_less      = '<'; // 60
static const key_code kc_equal     = '='; // 61
static const key_code kc_greater   = '>'; // 62
static const key_code kc_question  = '?'; // 63
static const key_code kc_at        = '@'; // 64

static const key_code kc_A = 'A'; // 65
static const key_code kc_B = 'B'; // 66
static const key_code kc_C = 'C'; // 67
static const key_code kc_D = 'D'; // 68
static const key_code kc_E = 'E'; // 69
static const key_code kc_F = 'F'; // 70
static const key_code kc_G = 'G'; // 71
static const key_code kc_H = 'H'; // 72
static const key_code kc_I = 'I'; // 73
static const key_code kc_J = 'J'; // 74
static const key_code kc_K = 'K'; // 75
static const key_code kc_L = 'L'; // 76
static const key_code kc_M = 'M'; // 77
static const key_code kc_N = 'N'; // 78
static const key_code kc_O = 'O'; // 79
static const key_code kc_P = 'P'; // 80
static const key_code kc_Q = 'Q'; // 81
static const key_code kc_R = 'R'; // 82
static const key_code kc_S = 'S'; // 83
static const key_code kc_T = 'T'; // 84
static const key_code kc_U = 'U'; // 85
static const key_code kc_V = 'V'; // 86
static const key_code kc_W = 'W'; // 87
static const key_code kc_X = 'X'; // 88
static const key_code kc_Y = 'Y'; // 89
static const key_code kc_Z = 'Z'; // 90

static const key_code kc_left_bracket  = '['; // 91
static const key_code kc_backslash     = '\\'; // 92
static const key_code kc_right_bracket = ']'; // 93
static const key_code kc_caret         = '^'; // 94
static const key_code kc_underscore    = '_'; // 95
static const key_code kc_backquote     = '`'; // 96

static const key_code kc_a = 'a'; // 97
static const key_code kc_b = 'b'; // 98
static const key_code kc_c = 'c'; // 99
static const key_code kc_d = 'd'; // 100
static const key_code kc_e = 'e'; // 101
static const key_code kc_f = 'f'; // 102
static const key_code kc_g = 'g'; // 103
static const key_code kc_h = 'h'; // 104
static const key_code kc_i = 'i'; // 105
static const key_code kc_j = 'j'; // 106
static const key_code kc_k = 'k'; // 107
static const key_code kc_l = 'l'; // 108
static const key_code kc_m = 'm'; // 109
static const key_code kc_n = 'n'; // 110
static const key_code kc_o = 'o'; // 111
static const key_code kc_p = 'p'; // 112
static const key_code kc_q = 'q'; // 113
static const key_code kc_r = 'r'; // 114
static const key_code kc_s = 's'; // 115
static const key_code kc_t = 't'; // 116
static const key_code kc_u = 'u'; // 117
static const key_code kc_v = 'v'; // 118
static const key_code kc_w = 'w'; // 119
static const key_code kc_x = 'x'; // 120
static const key_code kc_y = 'y'; // 121
static const key_code kc_z = 'z'; // 122

static const key_code kc_left_brace    = '{'; // 123
static const key_code kc_vertical_line = '|'; // 124
static const key_code kc_right_brace   = '}'; // 125
static const key_code kc_tilde         = '~'; // 126
static const key_code kc_delete        = 127; // 127

// undefined until 256
static const key_code kc_128 = 128; // 128
static const key_code kc_129 = 129; // 129
static const key_code kc_130 = 130; // 130
static const key_code kc_131 = 131; // 131
static const key_code kc_132 = 132; // 132
static const key_code kc_133 = 133; // 133
static const key_code kc_134 = 134; // 134
static const key_code kc_135 = 135; // 135
static const key_code kc_136 = 136; // 136
static const key_code kc_137 = 137; // 137
static const key_code kc_138 = 138; // 138
static const key_code kc_139 = 139; // 139
static const key_code kc_140 = 140; // 140
static const key_code kc_141 = 141; // 141
static const key_code kc_142 = 142; // 142
static const key_code kc_143 = 143; // 143
static const key_code kc_144 = 144; // 144
static const key_code kc_145 = 145; // 145
static const key_code kc_146 = 146; // 146
static const key_code kc_147 = 147; // 147
static const key_code kc_148 = 148; // 148
static const key_code kc_149 = 149; // 149
static const key_code kc_150 = 150; // 150
static const key_code kc_151 = 151; // 151
static const key_code kc_152 = 152; // 152
static const key_code kc_153 = 153; // 153
static const key_code kc_154 = 154; // 154
static const key_code kc_155 = 155; // 155
static const key_code kc_156 = 156; // 156
static const key_code kc_157 = 157; // 157
static const key_code kc_158 = 158; // 158
static const key_code kc_159 = 159; // 159
static const key_code kc_160 = 160; // 160
static const key_code kc_161 = 161; // 161
static const key_code kc_162 = 162; // 162
static const key_code kc_163 = 163; // 163
static const key_code kc_164 = 164; // 164
static const key_code kc_165 = 165; // 165
static const key_code kc_166 = 166; // 166
static const key_code kc_167 = 167; // 167
static const key_code kc_168 = 168; // 168
static const key_code kc_169 = 169; // 169
static const key_code kc_170 = 170; // 170
static const key_code kc_171 = 171; // 171
static const key_code kc_172 = 172; // 172
static const key_code kc_173 = 173; // 173
static const key_code kc_174 = 174; // 174
static const key_code kc_175 = 175; // 175
static const key_code kc_176 = 176; // 176
static const key_code kc_177 = 177; // 177
static const key_code kc_178 = 178; // 178
static const key_code kc_179 = 179; // 179
static const key_code kc_180 = 180; // 180
static const key_code kc_181 = 181; // 181
static const key_code kc_182 = 182; // 182
static const key_code kc_183 = 183; // 183
static const key_code kc_184 = 184; // 184
static const key_code kc_185 = 185; // 185
static const key_code kc_186 = 186; // 186
static const key_code kc_187 = 187; // 187
static const key_code kc_188 = 188; // 188
static const key_code kc_189 = 189; // 189
static const key_code kc_190 = 190; // 190
static const key_code kc_191 = 191; // 191
static const key_code kc_192 = 192; // 192
static const key_code kc_193 = 193; // 193
static const key_code kc_194 = 194; // 194
static const key_code kc_195 = 195; // 195
static const key_code kc_196 = 196; // 196
static const key_code kc_197 = 197; // 197
static const key_code kc_198 = 198; // 198
static const key_code kc_199 = 199; // 199
static const key_code kc_200 = 200; // 200
static const key_code kc_201 = 201; // 201
static const key_code kc_202 = 202; // 202
static const key_code kc_203 = 203; // 203
static const key_code kc_204 = 204; // 204
static const key_code kc_205 = 205; // 205
static const key_code kc_206 = 206; // 206
static const key_code kc_207 = 207; // 207
static const key_code kc_208 = 208; // 208
static const key_code kc_209 = 209; // 209
static const key_code kc_210 = 210; // 210
static const key_code kc_211 = 211; // 211
static const key_code kc_212 = 212; // 212
static const key_code kc_213 = 213; // 213
static const key_code kc_214 = 214; // 214
static const key_code kc_215 = 215; // 215
static const key_code kc_216 = 216; // 216
static const key_code kc_217 = 217; // 217
static const key_code kc_218 = 218; // 218
static const key_code kc_219 = 219; // 219
static const key_code kc_220 = 220; // 220
static const key_code kc_221 = 221; // 221
static const key_code kc_222 = 222; // 222
static const key_code kc_223 = 223; // 223
static const key_code kc_224 = 224; // 224
static const key_code kc_225 = 225; // 225
static const key_code kc_226 = 226; // 226
static const key_code kc_227 = 227; // 227
static const key_code kc_228 = 228; // 228
static const key_code kc_229 = 229; // 229
static const key_code kc_230 = 230; // 230
static const key_code kc_231 = 231; // 231
static const key_code kc_232 = 232; // 232
static const key_code kc_233 = 233; // 233
static const key_code kc_234 = 234; // 234
static const key_code kc_235 = 235; // 235
static const key_code kc_236 = 236; // 236
static const key_code kc_237 = 237; // 237
static const key_code kc_238 = 238; // 238
static const key_code kc_239 = 239; // 239
static const key_code kc_240 = 240; // 240
static const key_code kc_241 = 241; // 241
static const key_code kc_242 = 242; // 242
static const key_code kc_243 = 243; // 243
static const key_code kc_244 = 244; // 244
static const key_code kc_245 = 245; // 245
static const key_code kc_246 = 246; // 246
static const key_code kc_247 = 247; // 247
static const key_code kc_248 = 248; // 248
static const key_code kc_249 = 249; // 249
static const key_code kc_250 = 250; // 250
static const key_code kc_251 = 251; // 251
static const key_code kc_252 = 252; // 252
static const key_code kc_253 = 253; // 253
static const key_code kc_254 = 254; // 254
static const key_code kc_255 = 255; // 255

static const key_code kc_keypad_0 = 256;
static const key_code kc_keypad_1 = 257;
static const key_code kc_keypad_2 = 258;
static const key_code kc_keypad_3 = 259;
static const key_code kc_keypad_4 = 260;
static const key_code kc_keypad_5 = 261;
static const key_code kc_keypad_6 = 262;
static const key_code kc_keypad_7 = 263;
static const key_code kc_keypad_8 = 264;
static const key_code kc_keypad_9 = 265;
static const key_code kc_keypad_period   = 266;
static const key_code kc_keypad_divide   = 267;
static const key_code kc_keypad_multiply = 268;
static const key_code kc_keypad_minus    = 269;
static const key_code kc_keypad_plus     = 270;
static const key_code kc_keypad_enter    = 271;
static const key_code kc_keypad_equals   = 272;

static const key_code kc_up    = 273;
static const key_code kc_down  = 274;
static const key_code kc_right = 275;
static const key_code kc_left  = 276;

static const key_code kc_insert    = 277;
static const key_code kc_home      = 278;
static const key_code kc_end       = 279;
static const key_code kc_page_up   = 280;
static const key_code kc_page_down = 281;

static const key_code kc_F1  = 282;
static const key_code kc_F2  = 283;
static const key_code kc_F3  = 284;
static const key_code kc_F4  = 285;
static const key_code kc_F5  = 286;
static const key_code kc_F6  = 287;
static const key_code kc_F7  = 288;
static const key_code kc_F8  = 289;
static const key_code kc_F9  = 290;
static const key_code kc_F10 = 291;
static const key_code kc_F11 = 292;
static const key_code kc_F12 = 293;
static const key_code kc_F13 = 294;
static const key_code kc_F14 = 295;
static const key_code kc_F15 = 296;

static const key_code kc_num_lock    = 297;
static const key_code kc_caps_lock   = 298;
static const key_code kc_scroll_lock = 299;

static const key_code kc_right_shift   = 300;
static const key_code kc_left_shift    = 301;
static const key_code kc_right_control = 302;
static const key_code kc_left_control  = 303;
static const key_code kc_right_alt     = 304;
static const key_code kc_left_alt      = 305;
static const key_code kc_right_super   = 306;
static const key_code kc_left_super    = 307;

static const key_code kc_print_screen = 308;
static const key_code kc_system = 309;
static const key_code kc_break = 310;
static const key_code kc_menu = 311;

/**
 * \brief Code representing an invalid code.
 * \remark Currently the same as c_key_codes_count.
 */
static const key_code kc_not_a_key = 312;

/** \brief Number of valid key codes. */
static const unsigned int c_key_codes_count = 312;

private:
static const key_code kc_range_min = kc_null;
static const key_code kc_range_max = kc_menu;
