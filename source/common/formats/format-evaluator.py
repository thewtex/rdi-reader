#!/usr/bin/env python3.1
#
##
# @file fe.py
# @brief Determine the file format given an example .rdi file.
# @author Matthew McCormick (thewtex)
# @version
# @date 2009-05-21

# Public Domain

import sys
from optparse import OptionParser
import os

import logging
logging.basicConfig(level = logging.CRITICAL)
fe_logger = logging.getLogger('format_evaluator')

# for working from within the source
script_path = os.path.dirname(sys.modules[__name__].__file__)
sys.path.insert(0, script_path)

from format_evaluator.format_evaluator import FormatEvaluator

##
# @brief run the format evaluator
#
# @param rdi_filepath path to the example .rdi file
#
# @return
def main(rdi_filepath):
    with open(rdi_filepath, 'r', encoding='latin_1') as rdi_file:
        fe = FormatEvaluator(rdi_file, script_path)
        fe.run()

usage = "Usage: " + sys.argv[0] + " <sample-file.rdi>"
if __name__ == "__main__":
    parser = OptionParser(usage=usage)
    parser.add_option("-v", "--verbose", action="store_true", dest="verbose",
            default=False,
            help = "Print DEBUG message to stdout, default=%default")
    (options, args) = parser.parse_args()
    if options.verbose:
        fe_logger.setLevel(logging.DEBUG)
    if len(args) != 1:
        parser.error("Please specify sample rdi file.")
    else:
        main(args[0])
