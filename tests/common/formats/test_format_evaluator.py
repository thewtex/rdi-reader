#!/usr/bin/env python3.1

import unittest
import os
import sys

from format_evaluator import *

# get path to source dir
fe_path = os.path.dirname(sys.modules['format_evaluator'].__file__)
source_path = os.path.join(fe_path, '..', '..', '..')

class TestFormatEvaluator(unittest.TestCase):

    def testmain(self):
        self.assertRaises(IOError, main, 'a_nonexistent_filepath')
        self.assertEqual(main(os.path.join(source_path, 'data_for_tests', 'machine', 'visualsonics', 'vevo770', 'rmv710b', 'pat143_seg3.rdi')),\
                None )
