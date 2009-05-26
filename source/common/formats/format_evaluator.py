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

#with open('../../../test_data/machine/visualsonics/vevo770/rmv710b/pat143_seg3.rdi', 'r' ) as file:
with open('test_data/machine/visualsonics/vevo770/rmv710b/pat143_seg3.rdi', 'r' ) as file:

    #a = 5

    print("hello world")

def myfun():
    print("hey you")

myfun()

usage = "Usage: " + sys.argv[0] + " <sample-file.rdi>"
if __name__ == "__main__":
    if len( sys.argv ) != 2:
        print(usage)
        sys.exit(1)
