#include "matasano.h"
#include <unistd.h>
#include <ctype.h>
#include "strings_ext.h"

#define REMOVE_NEWLINE(str) do{ str[strcspn(str, "\r\n")] = 0;} while(0);
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

double char_freq_weight(ASCSTR in)
{
    char c;
    ASCSTR s = in;
    int i;
    double sum = 0;
    double len = (double) strlen(in);
    // Check as not good if not printable ascii!
    //for (i = 0; s[i]; i++) {if(!isprint(s[i]) && s[i] != '\n') {len = 0; break;} }; could be cracked out in future?
    if(len > 0)
    {
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
    }
    else
    {
        sum = 1;
    }
    return sqrt(sum);
}

double decode_xord_hexstr(HEXSTR in, ASCSTR out)
{
    char c;
    double temp_weight;
    double best_weight = 26;
    int len = strlen(in);
    HEXSTR temp_hex = (HEXSTR) malloc((len + 1) * sizeof(char));
    ASCSTR temp_asc = (ASCSTR) malloc((len + 1) * sizeof(char));

    for ( c = 1; c != 0; c++)
    {
        xor_hexstr_with_char(in, c, temp_hex);
        hexstr_to_ascstr(temp_hex, temp_asc);
        temp_weight = char_freq_weight(temp_asc);

        //printf("in  = %s\n key = %02x (%f) %s\nout = %s\n\n", in,c, temp_weight, temp_asc, temp_hex);
        if (temp_weight < best_weight)
        {
            strcpy(out, temp_asc);
            best_weight = temp_weight;
        }
    }

    free (temp_hex);
    free (temp_asc);
    return best_weight;
}

bool decode_xord_hexstr_file(FILE * handle, ASCSTR out)
{
    HEXSTR line = (HEXSTR) malloc(100 * sizeof(char));
    ASCSTR temp = (HEXSTR) malloc(100 * sizeof(char));
    double temp_weight;
    double best_weight = 1;
    while (fgets(line, 100, handle))
    {
        REMOVE_NEWLINE(line);
        temp_weight =  decode_xord_hexstr(line, temp);
        if (temp_weight < best_weight)
        {
            strcpy(out, temp);
            best_weight = temp_weight;
        }
    }
    free(line);
    free(temp);
    return PASS;
}

bool challenge_main_set_1(void)
{
	bool result;
    char temp[100];
    
    // Challenge 1.
    HEXSTR hex_string      = "49276d206b696c6c696e6720796f757220627261696e20"
                                "6c696b65206120706f69736f6e6f7573206d757368726f6f6d";
    B64STR base64_string   = "SSdtIGtpbGxpbmcgeW91ciBicmFpbiBsaWtlIGEgcG9pc29"
                                "ub3VzIG11c2hyb29t";
    hexstr_to_b64str(hex_string, temp, 100);
    result = (PASS == !strcmp(base64_string, temp));
    CHECK_CHALLENGE(1, result);
    
    // Challenge 2:
    {
        HEXSTR xor_str_one   = "1c0111001f010100061a024b53535009181c";
        HEXSTR xor_str_two   = "686974207468652062756c6c277320657965";
        HEXSTR xor_strs[2] = {xor_str_one, xor_str_two};
        HEXSTR xor_str_three = "746865206b696420646f6e277420706c6179";
        xor_hexstrs(xor_strs, 2, temp);
        result = (PASS == !strcmp(xor_str_three, temp));
        CHECK_CHALLENGE(2, result);
    }

    // Challenge 3:
    {
        HEXSTR xord_string = "1b37373331363f78151b7f2b783431333d78397828372d363c"
                             "78373e783a393b3736";
        decode_xord_hexstr(xord_string, temp);
        printf("Challenge 3 result = \"%s\"\n", temp);
    }
    
    // Challenge 4:
    {
        FILE * file_4;
        if ( (file_4 = fopen("../res/4.txt", "r")) == NULL && ((file_4 = fopen("./res/4.txt", "r")) == NULL))
        {
            printf("Error: File cannot be opened\n");
            return FAIL;
        }
        decode_xord_hexstr_file(file_4, temp);
        fclose(file_4);
        printf("Challenge 4 result = \"%s\"\n", temp);
    }

    // Challenge 5:
    {
        ASCSTR in_asc = "Burning 'em, if you ain't quick and nimble\n"
                        "I go crazy when I hear a cymbal";
        HEXSTR expected = "0b3637272a2b2e63622c2e69692a23693a2a3c6324"
                          "202d623d63343c2a26226324272765272a282b2f20"
                          "430a652e2c652a3124333a653e2b2027630c692b20"
                          "283165286326302e27282f";
        HEXSTR temp_hex = (HEXSTR) malloc((150) * sizeof(char));
        ascstr_to_hexstr(in_asc, temp_hex);
        xor_hexstr_with_repeatingkey(temp_hex, "ICE", 3, temp);
        free (temp_hex);
        result = (PASS == !strcmp(expected, temp));
        CHECK_CHALLENGE(5, result);
    }



	return PASS;
}
