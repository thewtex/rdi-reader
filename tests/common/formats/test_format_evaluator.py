#!/usr/bin/env python3.1

import unittest
import os
import sys

from format_evaluator.format_evaluator import FormatEvaluator

# get path to source dir
fe_path = os.path.dirname(sys.modules['format_evaluator'].__file__)
script_path = os.path.join(fe_path, '..')
source_path = os.path.join(fe_path, '..', '..', '..', '..')

# copy for now since we squashed a clean way of doing it by putting a - in the
# filename
def main(rdi_filepath):
    with open(rdi_filepath, 'r', encoding='latin_1') as rdi_file:
        fe = FormatEvaluator(rdi_file, script_path)
        fe.run()


class TestFormatEvaluator(unittest.TestCase):

    def testmain(self):
        self.assertRaises(IOError, main, 'a_nonexistent_filepath')
        self.assertEqual(main(os.path.join(source_path, 'source', 'common', 'formats', 'model_rdi', '710B', '1090-400_seg1.rdi')),\
                None )
