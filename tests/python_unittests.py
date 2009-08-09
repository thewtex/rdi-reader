#!/usr/bin/env python3.1

import os
import unittest
import sys

project_path = os.path.join(os.path.dirname(__file__), '..')
if len(sys.argv) == 2:
    sys.path.insert(0, project_path)
    sys.path.insert(0,
            os.path.join(project_path, 'source', 'common', 'formats'))
    sys.path.insert(0,
            os.path.join(project_path, 'tests', 'common', 'formats'))
else:
    print("Usage: " + sys.argv[0] + " <test suite>")
    sys.exit(1)


from test_format_evaluator import TestFormatEvaluator
FormatEvaluatorSuite = unittest.TestLoader().loadTestsFromTestCase(TestFormatEvaluator)


unittest.TextTestRunner(verbosity=2)
if __name__ == "__main__":
    unittest.main()
