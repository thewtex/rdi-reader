#!/usr/bin/env python3.0
#
##
# @file fe.py
# @brief Determine the file format given an example .rdi file.
# @author Matthew McCormick (thewtex)
# @version
# @date 2009-05-21

# Public Domain

import sys

##
# @brief run the format evaluator
#
# @param rdi_filepath path to the example .rdi file
#
# @return
def main(rdi_filepath):
    with open('test_data/machine/visualsonics/vevo770/rmv710b/pat143_seg3.rdi',
            'r' ) as rdi_file:

        #a = 5

        print("hello world")


usage = "Usage: " + sys.argv[0] + " <sample-file.rdi>"
if __name__ == "__main__":
    if len(sys.argv) != 2:
        print(usage)
        sys.exit(1)
    else:
        main(sys.argv[1])
