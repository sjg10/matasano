#include "matasano.h"
#include <ctype.h>
#define DICTBASE64 "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"
#define BASE64_0   'A'
#define REMOVE_NEWLINE(str) do{ str[strcspn(str, "\r\n")] = 0;} while(0);
//TODO tidy!
#include <unistd.h>
double char_freqs[] = {
0.0651738,0.0124248,0.0217339,0.0349835,0.1041442,0.0197881,0.0158610,0.0492888,0.0558094,0.0009033,0.0050529,0.0331490,0.0202124,0.0564513,0.0596302,0.0137645,0.0008606,0.0497563,0.0515760,0.0729357,0.0225134,0.0082903,0.0171272,0.0013692,0.0145984,0.0007836,0.1918182
};

double get_char_std_freq(char c)
{
    double ret_val;
    if (c == ' ')
    {
        ret_val = char_freqs[26];
    }
    else
    {
        char temp_c = toupper(c);
        if (temp_c <= 'A' || temp_c > 'Z')
        {
            ret_val = 0;
        }
        else
        {
            ret_val = char_freqs[temp_c - 'A'];
        }
    }
    return ret_val;
}

double char_freq_weight(char* in)
{
    char c;
    char* s = in;
    int i;
    double sum = 0;
    double len = (double) strlen(in);
    for ( c = 'A'; c <= 'Z'; c++)
    {
        s = in;
        for (i = 0; s[i]; (toupper(s[i]) == c) ? i++ : *s++);
        sum += pow((i / len) - (get_char_std_freq(c)), 2);
    }
    s = in;
    c = ' ';
    for (i = 0; s[i]; (toupper(s[i]) == c) ? i++ : *s++);
    sum += pow((i / len) - (get_char_std_freq(c)), 2);
    return sqrt(sum);
}

bool hex_to_ascii_str(char* in, char* out)
{
    int i;
    int len = strlen(in);
    char temp[3];
    temp[2] = '\0';
    for (i = 0; i < len; i += 2)
    {
        memcpy(temp, in + i, 2);
        out[i / 2] = (char) strtol(temp, NULL, 16);
//        printf("%s - %02x \n",temp,out[i]);
    }
    out[i/2 - 1] - '\0';
    return PASS;
}


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

// Takes an array of (number_of_inputs) hex strings of same length,
// returns out string of same length of their xor'd value
bool fixed_xor(char** in, int number_of_inputs, char* out)
{
    int i, j;
    UINT64 temp_int;
    int len          = strlen(in[0]);
    char* temp       = (char*) malloc(number_of_inputs * sizeof(char));
    // create temp_out that can include extra zero padding if (len % 16) != 0
    char* temp_out   = (char*) malloc((len + (16 - len % 16) + 1) * sizeof(char));
    char** last_char = (char**) malloc(number_of_inputs * sizeof(char*));
    // We proceed in lots of UINT64s (16 digits) to be efficient.
    for (i = 0; i < len; i += 16) 
    {
        temp_int = 0;
        for (j = 0 ; j < number_of_inputs; j ++)
        {
            last_char[j] = (in[j] + i + MIN(16, len - i + 1));
            temp[j] = *(last_char[j]); *(last_char[j]) = '\0';
            temp_int ^= strtoull(in[j] + i, NULL, 16);
            *(last_char[j]) = temp[j];
        }
        sprintf(temp_out + i, "%016llx", (long long int) temp_int);
    }

    if(len % 16 != 0)
    {
        // Rewrite final block to remove leading zeros
        strcpy(temp_out + i - 16, temp_out + i - (len % 16));
    }
    strcpy(out, temp_out);

    free (temp_out);
    free (temp);
    free (last_char);
    return PASS;
}

//works
bool ascii_str_to_hex(char* in, char* out)
{
    int i;
    int len = strlen(in);
    for(i =0; i < len; i++) sprintf(out + (i*2),"%02x",in[i]);
    return PASS;
}


double decode_xord_hex_string(char* in, char* out)
{
    char c;
    double temp_weight;
    double best_weight = 26;
    int len = strlen(in);
//    printf("LEN: %d\n",len);
    char* key = (char*) malloc((len + 1) * sizeof(char));
    char* key_temp = (char*) malloc(((len / 2) + 1) * sizeof(char));
    char* temp_one = (char*) malloc((len + 1) * sizeof(char));
    char* temp_two = (char*) malloc((len + 1) * sizeof(char));
    key_temp[len / 2] = '\0';
    char* strings[2] = {in, key};
    for ( c = 'A'; c <= 'Z'; c++)
    {
        memset(key_temp, c, len / 2);
        ascii_str_to_hex(key_temp, key);
        fixed_xor(strings, 2, temp_one);
        hex_to_ascii_str(temp_one, temp_two);
        temp_weight = char_freq_weight(temp_two);
        //printf("%c (%f) %s\n", c, temp_weight, temp_two);
        if (temp_weight < best_weight)
        {
        //    printf("NEW\n");
            strcpy(out, temp_two);
            best_weight = temp_weight;
        }
    }
    free (key);
    free (temp_one);
    free (temp_two);
    free (key_temp);
    return best_weight;
}

//TODO: debug, doesn't return english (maybe needs better weight function)
bool decode_xord_hex_str_file(FILE * handle, char* out)
{
    char line[100];
    char temp[100];
    double temp_weight;
    double best_weight = 26;
    while (fgets(line, sizeof(line), handle))
    {
        size_t len = strlen(line);
        if (len && (line[len - 1] != '\n'))
        {
            return FAIL;
        }
        REMOVE_NEWLINE(line);
        temp_weight = decode_xord_hex_string(line, temp);
        printf("(%f) %s\n",temp_weight,temp);
        if (temp_weight < best_weight)
        {
            printf("NEW\n");
            strcpy(out, temp);
            best_weight = temp_weight;
        }
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
    char* xor_strs[2] = {xor_str_one, xor_str_two};
    char xor_str_three[100] = "746865206b696420646f6e277420706c6179";
    fixed_xor(xor_strs, 2, temp);
    result = (PASS == !strcmp(xor_str_three, temp));
    CHECK_CHALLENGE(2, result);

    // Challenge 3:
    char xord_string[100] = "1b37373331363f78151b7f2b783431333d78397828372d363c"
                            "78373e783a393b3736";
    decode_xord_hex_string(xord_string, temp);
    printf("Challenge three result = \"%s\"\n", temp);

    // Challenge 4:
    FILE * file_4;
    if ( (file_4 = fopen("../res/4.txt", "r")) == NULL && ((file_4 = fopen("./res/4.txt", "r")) == NULL))
    {
        printf("Error: File cannot be opened\n");
        return FAIL;
    }
    //TODO: might fail due to running in wrong folder, fix!
    decode_xord_hex_str_file(file_4, temp);
    printf("Challenge four result = \"%s\"\n", temp);
    fclose(file_4);

	return PASS;
}
