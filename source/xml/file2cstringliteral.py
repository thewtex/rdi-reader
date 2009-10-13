#!/usr/bin/env python3.1

# transform an input file to an c/c++ string literal
# usage: <input file> <output file>

import sys

with open(sys.argv[1], 'r') as input:
    with open(sys.argv[2], 'w') as output:
        output.write('"')
        for line in input:
            output.write(line[:-1] + '\\\n')
        output.write('";\n')
