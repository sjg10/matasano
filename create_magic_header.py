# This file makes the magic_header as described in AUTOGEN_TEXT
AUTOGEN_TEXT="""
// This header is autogenerated by create_magic_header.py
// It contains the necessary definition of setup_function_array
// using the #define NUMBER_OF_CHALLENGE_SETS in matasano.h\n
"""
GUARDS         = "#ifndef MAGIC_H_\n#define MAGIC_H_\n"
MAIN_DEF_INTRO = "#define setup_function_array() "
CLOSING_LINE   = "\n\n#endif /* MAGIC_H_ */\n"

def get_number_of_challenge_sets(matasano_h):
    for line in matasano_h:
        if "#define NUMBER_OF_CHALLENGE_SETS" in line:
            stripped_line = line.rstrip()
            return int(stripped_line[stripped_line.rfind(' '):])

if __name__ == "__main__":
    matasano_h = open("include/matasano.h","r")
    magic_h = open("include/magic.h","w")
    magic_h.write(AUTOGEN_TEXT)
    magic_h.write(GUARDS)
    num_challenge_sets = get_number_of_challenge_sets(matasano_h)
    for i in range(num_challenge_sets):
        magic_h.write("bool challenge_main_set_" + str(i + 1) + "(void);\n")
    magic_h.write(MAIN_DEF_INTRO)
    for i in range(num_challenge_sets):
        magic_h.write("\\\n    challenge_set_run[" + str(i) + "] = challenge_main_set_" + str(i + 1) + ";")
    magic_h.write(CLOSING_LINE)


