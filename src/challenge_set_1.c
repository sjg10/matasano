#include "matasano.h"

#define DICTBASE64 "./0123456789ABCDEFGHIJKLMNOPQRSTYZabcdefghijklmnopqrstuvwxyz"

//TODO: remove this comment, tested and worked already.
UINT64 _hex_to_int_15digits(char* in)
{
    // Fifteen hex chars carry 16^15 = 2^(60) < max_val(UINT64) values
    char* temp = calloc(16,sizeof(char));
    memcpy(temp, in, 15);
    return (UINT64) strtoull(temp, NULL, 16);
}

//TODO: doesnt work, test and fix
bool _int_to_base64_10digits(UINT64 in, char* out)
{
    int i;
    memset(out, 10, '.');
    out[10] = '\0';
    for(i = 10; i >=0; i--, in <<= 6) out[i] = in % 64;
    return PASS;
}

//TODO: use above functions instead
bool hex_to_base64(char* in, char* out, int max_output_length)
{
//  return int_to_base64(strtol(in, NULL, 16), out, max_output_length);
    return FAIL;
}

//TODO: tidy
bool challenge_main_set_1(void)
{
	bool result;
    char temp[100];
    _int_to_base64_10digits(_hex_to_int_15digits("17abfe83bd92ef0"),temp);
    printf("%s\n",temp);
    result = hex_to_base64(
                "49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d",
                        temp,100);
    printf("TEMP %s.\n",temp);
    result = strcmp("SSdtIGtpbGxpbmcgeW91ciBicmFpbiBsaWtlIGEgcG9pc29ub3VzIG11c2hyb29t",temp);
	return result;
}
