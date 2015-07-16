#include "matasano.h"

#define DICTBASE64 "./0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"

// This works
UINT64 _hex_to_int_15digits(char* in)
{
    // Fifteen hex chars carry 16^15 = 2^(60) < max_val(UINT64) values
    char* temp = calloc(16,sizeof(char));
    memcpy(temp, in, 15);
    return (UINT64) strtoull(temp, NULL, 16);
}

//TODO: This may only work for the dictionary given. Work it out.
bool _int_to_base64_10digits(UINT64 in, char* out)
{
    int i, j;
    char* dict = DICTBASE64;
    memset(out, '.', 10);
    out[10] = '\0';

    for(i = 9; i >= 0; i--, in >>= 6) {out[i] = dict[in % 64]; printf("%d, %c\n",in % 64, out[i]);}
    //remove leading zeros:
    for (i = 0; out[i] == '.'; i++);
    for (j = 0; j < 10 && out[i + j - 1] != '\0'; j++) out[j] = out[i + j];
    printf("%s\n",out);
    return PASS;
}

//Given its calls this works.
bool hex_to_base64(char* in, char* out, int max_output_length)
{
    int i;
    int len = strlen(in);
    out[0]='\0';
    // Grab all but the digits in lots of 15
    for (i = 0; i < len; i+= 15) 
    {
        _int_to_base64_10digits(_hex_to_int_15digits(in + i), out + ((i/15) * 10));
        printf("%d, %d, %s\n",i,((i/15) * 10),out);
    }
    return PASS;
}

//TODO: tidy
bool challenge_main_set_1(void)
{
	bool result;
    char temp[100];
    char hex_string[100] = "49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d";
    char base64_string[100]   = "SSdtIGtpbGxpbmcgeW91ciBicmFpbiBsaWtlIGEgcG9pc29ub3VzIG11c2hyb29t";
    result = hex_to_base64(hex_string, temp, 100);
    printf("OUT: (len %d, and len %d), %s\n",strlen(temp), strlen(base64_string), temp);
    result =  (PASS == !strcmp(base64_string,temp));
	return result;
}
