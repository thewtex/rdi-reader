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
import os

#import xml.etree.ElementTree as ET
from lxml import etree


# for working from within the source
module_path = os.path.dirname(sys.modules[__name__].__file__)
sys.path.insert(0, module_path)
from namespace_vars import *
import element_types

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
        self.rdi_file = rdi_file
##
# @brief parse the next line in the file
#
# @return python list of the line contents
    def get_line(self):
        raw_line = self.rdi_file.readline()
        # strip out starting and trailing "
        raw_line = raw_line[1:-1]
        raw_line = raw_line.replace('",','')
        split_line = raw_line.split('"')
# strip out final ''
        return split_line[:-1]

##
# @brief get an unprocessed version of the next line in the file
#
# @return
    def get_raw_line(self):
        return self.rdi_file.readline()


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

        first_line = rdi_line_parser.get_line()
        if(first_line[0] != "=== IMAGE INFO ==="):
            raise UnexpectedContent
        image_info_t = etree.SubElement(rdi_schema, XS + 'complexType', \
                name='image_info_t')
        image_info_seq = etree.SubElement(image_info_t, XS + 'sequence')

        next_line = rdi_line_parser.get_line()
        while(next_line[0] != "=== IMAGE DATA ==="):
            element_name = next_line[0].replace(' ', '_')
            etree.SubElement(image_info_seq, XS + 'element',
                    name = element_name,
                    type = etyper.get_type(next_line))
            next_line = rdi_line_parser.get_line()

        image_data_t = etree.SubElement(rdi_schema, XS + 'complexType', \
                name='image_data_t')
        image_data_seq = etree.SubElement(image_data_t, XS + 'sequence')
        next_line = rdi_line_parser.get_raw_line()
        while(next_line != '"=== IMAGE PARAMETERS ==="\n'):
            next_line = rdi_line_parser.get_raw_line()

        rdi_t = etree.SubElement(rdi_schema, XS + 'complexType', \
                name = 'rdi_t')
        rdi_seq = etree.SubElement(rdi_t, XS + 'sequence')
        etree.SubElement(rdi_seq, XS + 'element',
                name = 'image_info',
                type = 'image_info_t')
        etree.SubElement(rdi_seq, XS + 'element',
                name = 'image_data',
                type = 'image_data_t')
        rdi = etree.SubElement(rdi_schema, XS + 'element',
                name = 'rdi',
                type = 'rdi_t')


        schemaschema = etree.XMLSchema(file=os.path.join(module_path, 'XMLSchema.xsd'))
        schemaschema.assertValid(rdi_schema)

        tree = etree.ElementTree(rdi_schema)
        tree.write(os.path.join(module_path,"rdi.xsd"), pretty_print=True, xml_declaration=True, encoding="UTF-8")

        #XSD_NAMESPACE = "http://www.w3.org/2001/XMLSchema"
        #XSD = "{%s}" % XSD_NAMESPACE
        #helloschema = etree.Element(XSD + "schema", \
                #nsmap={"xs": XSD_NAMESPACE})
        #ct = etree.SubElement(helloschema, XSD + "complexType", name="hello_t")
        #seq = etree.SubElement(ct, XSD + "sequence")
        #elem1 = etree.SubElement(seq, XSD + "element", name="greeting")
        #elem1.set("type", "xs:string")
        #elem2 = etree.SubElement(seq, XSD + "element", name="name")
        #elem2.set( "type", "xs:string")
        #elem2.set("maxOccurs", "unbounded")

        #bigel = etree.SubElement(helloschema, XSD + "element", name="hello", type="hello_t")
        #schematree = etree.ElementTree(helloschema)
        #etree.cleanup_namespaces(helloschema)
        #schematree.write("hello2.xsd", pretty_print=True, xml_declaration=True, encoding="UTF-8")


        #root = objectify.Element("hello", \
                #nsmap={'xsi': 'http://www.w3.org/2001/XMLSchema-instance'})
        #root.set("{http://www.w3.org/2001/XMLSchema-instance}noNamespaceSchemaLocation", "hello.xsd" )
        #greeting = etree.SubElement(root, "greeting")
        #root.greeting = "Hello"
        #name = etree.SubElement(root, "name")
        #name = etree.SubElement(root, "name")
        #name = etree.SubElement(root, "name")
        #name[0] = "sun"
        #name[1] = "mars"
        #name[2] = "world"
        #objectify.deannotate(root)
        #etree.cleanup_namespaces(root)
        #tree = etree.ElementTree(root)
        #tree.write("hello.xml", pretty_print=True, xml_declaration=True, encoding="UTF-8")

        #myschema = etree.XMLSchema(helloschema)
        #myschema.assertValid(root)



        #root = ET.Element('hello')
        #greeting = ET.SubElement(root, "greeting")
        #greeting.text = 'Hello'
        #name1 = ET.SubElement(root, "name")
        #name1.text = 'sun'
        #name2 = ET.SubElement(root, "name")
        #name3 = ET.SubElement(root, "name")
        #name2.text = 'earth'
        #name3.text = 'world'
        #tree = ET.ElementTree(root)
        #tree.write('test.xml')




usage = "Usage: " + sys.argv[0] + " <sample-file.rdi>"
if __name__ == "__main__":
    if len(sys.argv) != 2:
        print(usage)
        sys.exit(1)
    else:
        main(sys.argv[1])
