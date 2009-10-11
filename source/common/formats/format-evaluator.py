#!/usr/bin/env python3.1
#
##
# @file fe.py
# @brief Determine the file format given an example .rdi file.
# @author Matthew McCormick (thewtex)
# @version
# @date 2009-05-21

# Public Domain

import re
import sys
from optparse import OptionParser
import os

import logging
logging.basicConfig(level = logging.CRITICAL)
fe_logger = logging.getLogger('format_evaluator')

from lxml import etree


# for working from within the source
module_path = os.path.dirname(sys.modules[__name__].__file__)
sys.path.insert(0, module_path)
from format_evaluator.namespace_vars import *
from format_evaluator import element_types

class UnexpectedContent(Exception):
    def __init__(self, message="Unexpected content for a .rdi file."):
        self.message = message
    def __str__(self):
        return repr(self.message)

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




##
# @brief run the format evaluator
#
# @param rdi_filepath path to the example .rdi file
#
# @return
def main(rdi_filepath):
    with open(rdi_filepath, 'r', encoding='latin_1') as rdi_file:
        rdi_line_parser = RDILineParser(rdi_file)

# create the XML Schema
        rdi_schema = etree.Element(XS + 'schema', \
                nsmap={XS_NS[:-1]: XS_NAMESPACE})


        etyper = element_types.ElementTyper()

# parse IMAGE INFO section
        first_line = rdi_line_parser.get_line()
        if(first_line[0][0] != "===_IMAGE_INFO_==="):
            raise UnexpectedContent
        image_info_t = etree.SubElement(rdi_schema, XS + 'complexType', \
                name='image_info_t')
        image_info_seq = etree.SubElement(image_info_t, XS + 'sequence')

        next_line = rdi_line_parser.get_line()
        while(next_line[0][0] != "===_IMAGE_DATA_==="):
            element_name = next_line[0][0].replace(' ', '_')
            etree.SubElement(image_info_seq, XS + 'element',
                    name = element_name,
                    type = etyper.get_type(next_line))
            next_line = rdi_line_parser.get_line()


# parse IMAGE DATA section
        image_data_t = etree.SubElement(rdi_schema, XS + 'complexType', \
                name='image_data_t')
        next_line = rdi_line_parser.get_raw_line()
        while(next_line != '"=== IMAGE PARAMETERS ==="\n'):
            next_line = rdi_line_parser.get_raw_line()

# parse IMAGE PARAMETERS
        image_parameters_t = etree.SubElement(rdi_schema, XS + 'complexType', \
                name='image_parameters_t')
        image_parameters_seq = etree.SubElement(image_parameters_t, XS + \
                'sequence')
        next_line = rdi_line_parser.get_line()
        count = 0

# for '3D-Volume' and possibly others
        while(next_line != [['']]):
            node_e = image_parameters_seq
            for node in range(len(next_line[0])-1):
                element_name = next_line[0][node]
                next_node_locator = etree.XPath('./' + XS_NS + 'element' \
                        + "[@name='" + element_name + "']",
                        namespaces={XS_NS[:-1]:XS_NAMESPACE})
                if len(next_node_locator(node_e)) == 0:
                    next_node_e = etree.Element(XS + 'element', name=element_name)
                    next_node_ct = etree.Element(XS + 'complexType')
                    next_node_seq = etree.Element(XS + 'sequence')
                    next_node_ct.append(next_node_seq)
                    next_node_e.append(next_node_ct)
                    node_e.append(next_node_e)
                    node_e = next_node_seq
                else:
                    node_e = next_node_locator(node_e)[0]
                    node_e = node_e[0][0]
            element_name = next_line[0][-1]
            if len(next_line) < 3: # no units attribute
                element = etree.Element(XS + 'element',
                        name = element_name,
                        type = etyper.get_type(next_line))
                node_e.append(element)
            else: # units attribute,
                # http://www.w3.org/TR/xmlschema-0/ 2.5.1
                element = etree.Element(XS + 'element',
                        name = element_name)
                element_ct = etree.Element(XS + 'complexType')
                element_sc = etree.Element(XS + 'simpleContent')
                element_ex = etree.Element(XS + 'extension',
                        base = etyper.get_type(next_line) )
                element_attr = etree.Element(XS + 'attribute',
                        name = 'units',
                        type = XS_NS + 'string',
                        use = 'required')
                element_ex.append(element_attr)
                element_sc.append(element_ex)
                element_ct.append(element_sc)
                element.append(element_ct)
                node_e.append(element)
            next_line = rdi_line_parser.get_line()

# create rdi root element and main IMAGE* sub elements
        rdi_t = etree.SubElement(rdi_schema, XS + 'complexType', \
                name = 'rdi_t')
        rdi_seq = etree.SubElement(rdi_t, XS + 'sequence')
        etree.SubElement(rdi_seq, XS + 'element',
                name = 'image_info',
                type = 'image_info_t')
        etree.SubElement(rdi_seq, XS + 'element',
                name = 'image_data',
                type = 'image_data_t')
        etree.SubElement(rdi_seq, XS + 'element',
                name = 'image_parameters',
                type = 'image_parameters_t')
        rdi = etree.SubElement(rdi_schema, XS + 'element',
                name = 'rdi',
                type = 'rdi_t')


        schemaschema = etree.XMLSchema(file=os.path.join(module_path, 'XMLSchema.xsd'))

        tree = etree.ElementTree(rdi_schema)
        tree.write(os.path.join(module_path,"rdi.xsd"), pretty_print=True, xml_declaration=True, encoding="UTF-8")
        schemaschema.assertValid(rdi_schema)


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
