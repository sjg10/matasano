#ifndef STRINGS_EXT_H
#define STRINGS_EXT_H

#define BASE64_0 'A' 

typedef char * HEXSTR;
typedef char * ASCSTR;
typedef char * B64STR;

BOOL ascstr_to_hexstr(ASCSTR in, HEXSTR out);
BOOL hexstr_to_b64str(HEXSTR in, B64STR out, int max_output_length);
BOOL b64str_to_hexstr(B64STR in, HEXSTR out, int max_output_length);
BOOL hexstr_to_ascstr(HEXSTR in, ASCSTR out);

BOOL xor_hexstrs(HEXSTR* in, int number_of_inputs, HEXSTR out);
BOOL xor_hexstr_with_repeatingkey(HEXSTR in, ASCSTR key, int key_len, HEXSTR out_hex);

UINT32 ascstr_hamming_distance(ASCSTR in1, ASCSTR in2);
#endif

