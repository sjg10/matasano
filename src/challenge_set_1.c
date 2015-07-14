#include "matasano.h"

#define DICTBASE64 "./0123456789ABCDEFGHIJKLMNOPQRSTYZabcdefghijklmnopqrstuvwxyz"


//TODO Tidy, fails, debug.
bool int_to_base64(int in, char* out, int max_output_length)
{
    char* dict = DICTBASE64;
    int temp = in;
    int digits = 0;
    int i;
    for (digits = 0; temp > 0; digits ++) temp /= 64;
    if (digits > max_output_length) return FAIL;
    out[digits] = '\0';
    if (!in)
    {
        out[0] = dict[0];
        return FAIL;
    }
    temp = in;
    for (i = digits - 1; i >= 0; i--)
    {
        out[i] = dict[temp % 64];
        temp /= 64;
    }
    return PASS;
}

//TODO fails, check
bool hex_to_base64(char* in, char* out, int max_output_length)
{
    return int_to_base64(strtol(in, NULL, 16), out, max_output_length);
}

//TODO: tidy
bool challenge_main_set_1(void)
{
	bool result;
    char temp[100];
    result = hex_to_base64(
                "49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d",
                        temp,100);
    printf("TEMP %s.\n",temp);
    result = strcmp("SSdtIGtpbGxpbmcgeW91ciBicmFpbiBsaWtlIGEgcG9pc29ub3VzIG11c2hyb29t",temp);
	return result;
}
