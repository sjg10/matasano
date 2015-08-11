#ifndef STRINGS_EXT_H
#define STRINGS_EXT_H

#define BASE64_0 'A' 

typedef char * HEXSTR;
typedef char * ASCSTR;
typedef char * B64STR;

bool ascstr_to_hexstr(ASCSTR in, HEXSTR out);
bool hexstr_to_b64str(HEXSTR in, B64STR out, int max_output_length);
bool hexstr_to_ascstr(HEXSTR in, ASCSTR out);

bool xor_hexstrs(HEXSTR* in, int number_of_inputs, HEXSTR out);
bool xor_hexstr_with_repeatingkey(HEXSTR in, ASCSTR key, int key_len, HEXSTR out_hex);

#endif

