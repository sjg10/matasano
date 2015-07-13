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

#define PASS 0
#define FAIL !PASS

#define NUMBER_OF_CHALLENGE_SETS 1

typedef int bool;

bool (*challenge_set_run[NUMBER_OF_CHALLENGE_SETS])(void);

//TODO: find something nicer, use it within sets as well.
bool challenge_main_set_one(void);
#define setup_function_array() \
		challenge_set_run[0] = challenge_main_set_one;


#endif /* MATASANO_H_ */
