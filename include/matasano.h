/*
 * matasano.h
 *
 *  Created on: 13 Jul 2015
 *      Author: sam
 */

#ifndef MATASANO_H_
#define MATASANO_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PASS 0
#define FAIL !PASS

// Update on new challenge set to create new magic.h
#define NUMBER_OF_CHALLENGE_SETS 1

typedef int bool;

// This provides the function declerations needed for the array
// And the setup_function_array to set up the array
bool (*challenge_set_run[NUMBER_OF_CHALLENGE_SETS])(void);
#include "magic.h"


#endif /* MATASANO_H_ */
