/*
 * matasano.h
 *
 *  Created on: 13 Jul 2015
 *      Author: sam
 */

#ifndef MATASANO_H_
#define MATASANO_H_
#define __STDC_FORMAT_MACROS
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


typedef uint64_t UINT64;
typedef uint32_t UINT32;
#define PASS 0
#define FAIL !PASS
#define MIN(a,b) ( (a < b) ? (a) : (b) )
#define MAX(a,b) ( (a > b) ? (a) : (b) )

// Update on new challenge set to create new magic.h
#define NUMBER_OF_CHALLENGE_SETS 1

#define CHECK_CHALLENGE(no,result)  if (result == PASS)                       \
                                    {                                         \
                                        printf("Challenge %d passed.\n", no); \
                                    }                                         \
                                    else                                      \
                                    {                                         \
                                        printf("Challenge %d failed.\n", no); \
                                        return FAIL;                          \
                                    }
                                        

typedef int bool;

// This provides the function declerations needed for the array
// And the setup_function_array to set up the array
bool (*challenge_set_run[NUMBER_OF_CHALLENGE_SETS])(void);
#include "magic.h"


#endif /* MATASANO_H_ */
