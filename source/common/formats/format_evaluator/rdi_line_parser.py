import re

import logging
fe_logger = logging.getLogger('format_evaluator')

##
# @brief parse a .rdi file line by line and generate a python list of the
# contents
class RDILineParser:
##
# @brief create the parser
#
# @param rdi_file
#
# @return
    def __init__(self, rdi_file):
        self.digit_re = re.compile('\d')
        self.rdi_file = rdi_file
##
# @brief parse the next line in the file
#
# @return python list of the line contents
    def get_line(self):
        raw_line = self.get_raw_line()
        # strip out starting and trailing " and \r\n
        raw_line = raw_line[1:-2]
        raw_line = raw_line.replace('",','')
        split_line = raw_line.split('"')
        param_hierarchy = split_line[0].split('/')
        for node in range(len(param_hierarchy)):
            param_hierarchy[node] = self.get_element_name(param_hierarchy[node])
        split_line[0] = param_hierarchy
        return split_line

##
# @brief get an unprocessed version of the next line in the file
#
# @return
    def get_raw_line(self):
        raw_line = self.rdi_file.readline()
        fe_logger.debug("Processing: " + raw_line)
        return raw_line

##
# @brief clean rdi so it is a valid XML element name
#
# See http://www.w3schools.com/XML/xml_elements.asp for the naming rules
#
# @param rdi_name
#
# @return valid_element_name
    def get_element_name(self, rdi_name):
        element_name = rdi_name.replace(' ', '_')
        if len(rdi_name) > 0 and self.digit_re.match(rdi_name[0]):
            element_name = 'X_' + element_name
        return element_name

