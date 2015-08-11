#include "matasano.h"
#include "strings_ext.h"

// Private Defns
#define DICTBASE64 "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"
#define BASE64_0 'A'
// TODO: tidy!

// TODO: For now functions DO NOT do input checks.
// TODO: Add in some const keywords.

// (Private) Takes a <=15 char hex string and returns UINT64 of its value
UINT64 _hexstr_to_int_15digits(HEXSTR in)
{
    UINT64 retval;
    // Fifteen hex chars carry 16^15 = 2^(60) < max_val(UINT64) values
    char* temp = calloc(16,sizeof(char));
    memcpy(temp, in, 15);
    retval = (UINT64) strtoull(temp, NULL, 16);
    free (temp);
    return retval;
}

// (Private) Take a UINT64 and translate to a base64 string (<=10 digits)
BOOL _int_to_b64str_10digits(UINT64 in, B64STR out)
{
    int i, j;
    char* dict = DICTBASE64;
    memset(out, BASE64_0, 10);
    out[10] = '\0';

    for(i = 9; i >= 0; i--, in >>= 6) out[i] = dict[in % 64];
    
    //remove leading zeros:
    for (i = 0; out[i] == BASE64_0; i++);
    if (i > 0)
    {
        for (j = 0; j < 10 && out[i + j - 1] != '\0'; j++) {out[j] = out[i + j];}
    }
    return PASS;
}


// Public functions:

BOOL ascstr_to_hexstr(ASCSTR in, HEXSTR out)
{
    int i;
    int len = strlen(in);
    for(i =0; i < len; i++) sprintf(out + (i*2),"%02x",in[i]);
    return PASS;
}

BOOL hexstr_to_ascstr(HEXSTR in, ASCSTR out)
{
    int i;
    int len = strlen(in);
    char temp[3];
    temp[2] = '\0';
    for (i = 0; i < len; i += 2)
    {
        memcpy(temp, in + i, 2);
        out[i / 2] = strtoul(temp, NULL, 16);
    }
    out[i / 2] = '\0';

    return PASS;
}


// Takes a hex string, returns out string in base 64
// of length <= max_output_length
BOOL hexstr_to_b64str(HEXSTR in, B64STR out, int max_output_length)
{
    int i;
    int len = strlen(in);
    out[0]='\0';
    // Grab all but the digits in lots of 15
    for (i = 0; i < len; i+= 15) 
    {
        _int_to_b64str_10digits(_hexstr_to_int_15digits(in + i),
                                out + ((i/15) * 10));
    }
    return PASS;
}

// Takes an array of (number_of_inputs) hex strings of same length,
// returns out string of same length of their xor'd value
BOOL xor_hexstrs(HEXSTR* in, int number_of_inputs, HEXSTR out)
{
    int i, j;
    UINT64 temp_int;
    int len = strlen(in[0]);

    // Copy the in because we will edit it!
    HEXSTR* in_safe = (HEXSTR*) malloc(number_of_inputs * sizeof(HEXSTR));
    for (i = 0; i < number_of_inputs; i++)
    {
        in_safe[i] = malloc((len + 1) * sizeof(char));
        strncpy(in_safe[i], in[i], len + 1);
    }

    char* temp        = (char*) malloc(number_of_inputs * sizeof(char));
    
    // create temp_out that can include extra zero padding if (len % 16) != 0
    HEXSTR temp_out   = (HEXSTR) malloc((len + (16 - len % 16) + 1) * sizeof(char));
    HEXSTR temp_out_two   = (HEXSTR) malloc(17 * sizeof(char));

    // backup the chars that we are going to zero to split up our string
    char** last_char  = (char**) malloc(number_of_inputs * sizeof(char*));

    // We proceed in lots of UINT64s (16 digits) to be efficient.
    for (i = 0; i < len; i += 16) 
    {
        temp_int = 0;
        for (j = 0 ; j < number_of_inputs; j ++)
        {
            if(i + 16 < len)
            {
                last_char[j] = (in_safe[j] + i + 16);
                temp[j] = *(last_char[j]);
                *(last_char[j]) = '\0';
            }
            temp_int ^= strtoull(in_safe[j] + i, NULL, 16);
            *(last_char[j]) = temp[j];
        }
            sprintf(temp_out + i, "%016llx", (long long int) temp_int);
    }

    if(len % 16 != 0)
    {
        // Rewrite final block to remove leading zeros
        strcpy(temp_out_two, temp_out + i - (len % 16));
        strcpy(temp_out + i - 16, temp_out_two);
    }
    strcpy(out,temp_out);

    for (i = 0; i < number_of_inputs; i++) free (in_safe[i]);
    free (in_safe);
    free (temp_out);
    free (temp_out_two);
    free (temp);
    free (last_char);
    return PASS;
}

BOOL xor_hexstr_with_repeatingkey(HEXSTR in, ASCSTR key, int key_len, HEXSTR out_hex)
{
    int i;
    int len = strlen(in);
    char* key_hex = (char*) malloc((len + 1) * sizeof(char));
    char* key_asc = (char*) malloc(((len / 2) + 1) * sizeof(char));

    for(i = 0; i < len / 2; i += key_len)
    {
        strncpy(key_asc + i, key, MIN(key_len, (len / 2) - i));
    }
    key_asc[len / 2] = '\0';
    ascstr_to_hexstr(key_asc, key_hex);

    HEXSTR strings[] = {in, key_hex};
    xor_hexstrs(strings, 2, out_hex);
    
    free (key_hex);
    free (key_asc);

    return PASS;
}
