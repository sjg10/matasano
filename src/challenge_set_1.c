#include "matasano.h"

#define DICTBASE64 "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"
#define BASE64_0   'A'
// TODO: For now functions DO NOT do input checks.

// (Private) Takes a <=15 char hex string and returns UINT64 of its value
UINT64 _hex_to_int_15digits(char* in)
{
    // Fifteen hex chars carry 16^15 = 2^(60) < max_val(UINT64) values
    char* temp = calloc(16,sizeof(char));
    memcpy(temp, in, 15);
    return (UINT64) strtoull(temp, NULL, 16);
}

// (Private) Take a UINT64 and translate to a base64 string (<=10 digits)
bool _int_to_base64_10digits(UINT64 in, char* out)
{
    int i, j;
    char* dict = DICTBASE64;
    memset(out, BASE64_0, 10);
    out[10] = '\0';

    for(i = 9; i >= 0; i--, in >>= 6) out[i] = dict[in % 64];

    //remove leading zeros:
    for (i = 0; out[i] == BASE64_0; i++);
    for (j = 0; j < 10 && out[i + j - 1] != '\0'; j++) out[j] = out[i + j];
    return PASS;
}

// Takes a hex string, returns out string in base 64
// of length <= max_output_length
bool hex_to_base64(char* in, char* out, int max_output_length)
{
    int i;
    int len = strlen(in);
    out[0]='\0';
    // Grab all but the digits in lots of 15
    for (i = 0; i < len; i+= 15) 
    {
        _int_to_base64_10digits(_hex_to_int_15digits(in + i),
                                out + ((i/15) * 10));
    }
    return PASS;
}

bool fixed_xor(char* in1, char* in2, char* out)
{
    int i;
    int len = strlen(in1);
    char temp;
    char* last_char;
    //TODO this just takes in1 as int 15 dig at a time, need to xor them!
    //TODO last guy needs to be shifted to the left!
    for (i = 0; i < len; i += 15) 
    {
        last_char = (in1 + i + MIN(16, len - i + 1));
        temp = *last_char; *last_char = '\0';
        printf("%016llX\n",strtoull(in1 + i, NULL, 16)); sprintf(out + i, "%016llX",strtoull(in1 + i,NULL,16));
        *last_char = temp;
    }
    return PASS;
}

bool challenge_main_set_1(void)
{
	bool result;
    char temp[100];

    // Challenge 1.
    char hex_string[100]      = "49276d206b696c6c696e6720796f757220627261696e20"
                                "6c696b65206120706f69736f6e6f7573206d757368726f6f6d";
    char base64_string[100]   = "SSdtIGtpbGxpbmcgeW91ciBicmFpbiBsaWtlIGEgcG9pc29"
                                "ub3VzIG11c2hyb29t";
    hex_to_base64(hex_string, temp, 100);
    result = (PASS == !strcmp(base64_string, temp));
    CHECK_CHALLENGE(1, result);

    // Challenge 2:
    char xor_str_one[100]   = "1c0111001f010100061a024b53535009181c";
    char xor_str_two[100]   = "686974207468652062756c6c277320657965";
    char xor_str_three[100] = "746865206b696420646f6e277420706c6179";
    fixed_xor(xor_str_one, xor_str_two, temp);
    result = (PASS == !strcmp(xor_str_three, temp);
    printf("%s\n",temp);
    CHECK_CHALLENGE(2, result);

	return PASS;
}
