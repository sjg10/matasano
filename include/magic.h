
// This header is autogenerated by create_magic_header.py
// It contains the necessary definition of setup_function_array
// using the #define NUMBER_OF_CHALLENGE_SETS in matasano.h

#ifndef MAGIC_H_
#define MAGIC_H_
bool challenge_main_set_1(void);
#define setup_function_array() \
    challenge_set_run[0] = challenge_main_set_1;

#endif /* MAGIC_H_ */
