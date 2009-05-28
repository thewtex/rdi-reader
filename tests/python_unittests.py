#!/usr/bin/env python3.0

import os
import unittest
import sys

project_path = ''
if len(sys.argv) == 3:
    project_path = sys.argv[2]
    sys.argv = sys.argv[:-1]
    sys.path.insert(0, project_path)
    sys.path.remove( project_path + '/tests' )
else:
    print("Usage: " + sys.argv[0] + " <test suite> <path to project path>")
    sys.exit(1)


from tests.common.formats.test_format_evaluator import TestFormatEvaluator
FormatEvaluatorSuite = unittest.TestLoader().loadTestsFromTestCase(TestFormatEvaluator)


unittest.TextTestRunner(verbosity=2)
if __name__ == "__main__":
    unittest.main()
