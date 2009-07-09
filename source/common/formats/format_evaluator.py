#!/usr/bin/env python3.0
#
##
# @file fe.py
# @brief Determine the file format given an example .rdi file.
# @author Matthew McCormick (thewtex)
# @version
# @date 2009-05-21

# Public Domain

import sys

#import xml.etree.ElementTree as ET
from lxml import etree
from lxml import objectify

##
# @brief run the format evaluator
#
# @param rdi_filepath path to the example .rdi file
#
# @return
def main(rdi_filepath):
    with open(rdi_filepath, 'r' ) as rdi_file:

        XSD_NAMESPACE = "http://www.w3.org/2001/XMLSchema"
        XSD = "{%s}" % XSD_NAMESPACE
        helloschema = etree.Element(XSD + "schema", \
                nsmap={"xs": XSD_NAMESPACE})
        ct = etree.SubElement(helloschema, XSD + "complexType", name="hello_t")
        seq = etree.SubElement(ct, XSD + "sequence")
        elem1 = etree.SubElement(seq, XSD + "element", name="greeting")
        elem1.set("type", "xs:string")
        elem2 = etree.SubElement(seq, XSD + "element", name="name")
        elem2.set( "type", "xs:string")
        elem2.set("maxOccurs", "unbounded")

        bigel = etree.SubElement(helloschema, XSD + "element", name="hello", type="hello_t")
        schematree = etree.ElementTree(helloschema)
        etree.cleanup_namespaces(helloschema)
        schematree.write("hello2.xsd", pretty_print=True, xml_declaration=True, encoding="UTF-8")

        schemaschema = etree.XMLSchema(file='XMLSchema.xsd')
        schemaschema.assertValid(helloschema)

        root = objectify.Element("hello", \
                nsmap={'xsi': 'http://www.w3.org/2001/XMLSchema-instance'})
        root.set("{http://www.w3.org/2001/XMLSchema-instance}noNamespaceSchemaLocation", "hello.xsd" )
        greeting = etree.SubElement(root, "greeting")
        root.greeting = "Hello"
        name = etree.SubElement(root, "name")
        name = etree.SubElement(root, "name")
        name = etree.SubElement(root, "name")
        name[0] = "sun"
        name[1] = "mars"
        name[2] = "world"
        objectify.deannotate(root)
        etree.cleanup_namespaces(root)
        tree = etree.ElementTree(root)
        tree.write("hello.xml", pretty_print=True, xml_declaration=True, encoding="UTF-8")

        myschema = etree.XMLSchema(helloschema)
        myschema.assertValid(root)



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
