/*
 * main.c
 *
 *  Created on: 13 Jul 2015
 *      Author: sam
 */

#include "matasano.h"

int main(void)
{
	int i;
	bool result;

	printf("Beginning matasano tests...\n");
	setup_function_array();

	for(i = 0; i < NUMBER_OF_CHALLENGE_SETS; i++)
	{
		printf("Running set %d:\n", i + 1);
		result = challenge_set_run[i]();
		if (result == FAIL)
		{
			printf("Set %d failed.\n", i + 1);
			exit(FAIL);
		}
		else
		{
			printf("Set %d passed.\n", i + 1);
		}
	}

	printf("Success!.\n");
	exit(PASS);
}
