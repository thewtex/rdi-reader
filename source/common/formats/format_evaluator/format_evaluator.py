import os

from lxml import etree

from format_evaluator import element_types
from format_evaluator.namespace_vars import *
from format_evaluator.rdi_line_parser import RDILineParser
from format_evaluator.unexpected_content_exception import UnexpectedContent

class FormatEvaluator():
    def __init__(self, rdi_file, script_path):
        """ args:
            - rdi_file: the open rdi file object
            - script_path: path of the main script: used for determining the paths
            to some resource files.
            """
        self.rdi_line_parser = RDILineParser(rdi_file)
        self.script_path = script_path

    def run(self):
# create the XML Schema
        rdi_schema = etree.Element(XS + 'schema', \
                nsmap={XS_NS[:-1]: XS_NAMESPACE})


        etyper = element_types.ElementTyper()

# parse IMAGE INFO section
        first_line = self.rdi_line_parser.get_line()
        if(first_line[0][0] != "===_IMAGE_INFO_==="):
            raise UnexpectedContent
        image_info_t = etree.SubElement(rdi_schema, XS + 'complexType', \
                name='image_info_t')
        image_info_seq = etree.SubElement(image_info_t, XS + 'sequence')

        next_line = self.rdi_line_parser.get_line()
        while(next_line[0][0] != "===_IMAGE_DATA_==="):
            element_name = next_line[0][0].replace(' ', '_')
            etree.SubElement(image_info_seq, XS + 'element',
                    name = element_name,
                    type = etyper.get_type(next_line))
            next_line = self.rdi_line_parser.get_line()


# parse IMAGE DATA section
        image_data_t = etree.SubElement(rdi_schema, XS + 'complexType', \
                name='image_data_t')
        next_line = self.rdi_line_parser.get_raw_line()
        while(next_line != '"=== IMAGE PARAMETERS ==="\n'):
            next_line = self.rdi_line_parser.get_raw_line()

# parse IMAGE PARAMETERS
        image_parameters_t = etree.SubElement(rdi_schema, XS + 'complexType', \
                name='image_parameters_t')
        image_parameters_seq = etree.SubElement(image_parameters_t, XS + \
                'sequence')
        next_line = self.rdi_line_parser.get_line()
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
            next_line = self.rdi_line_parser.get_line()

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


        schemaschema = etree.XMLSchema(file=os.path.join(self.script_path, 'XMLSchema.xsd'))

        tree = etree.ElementTree(rdi_schema)
        tree.write(os.path.join(self.script_path, "rdi.xsd"), pretty_print=True, xml_declaration=True, encoding="UTF-8")
        schemaschema.assertValid(rdi_schema)
