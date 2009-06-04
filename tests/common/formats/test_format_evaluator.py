#!/usr/bin/env python3.0

import unittest

from source.common.formats.format_evaluator import *

class TestFormatEvaluator(unittest.TestCase):

    def testmain(self):
        self.assertRaises(IOError, main, 'a_nonexistent_filepath')
        self.assertEqual(main('test_data/machine/visualsonics/vevo770/rmv710b/pat143_seg3.rdi'),\
                None )
