from format_evaluator.namespace_vars import *

import logging
fe_logger = logging.getLogger('format_evaluator')

##
# @brief determine the XML schema type
class ElementTyper():
    def __init__(self):
        self.line = []

    _element_name_dict = {
            'Image_Frames':'positiveInteger',
            'Image_Lines':'positiveInteger',
            'Image_Acquisition_Per_Line':'positiveInteger'
            }

    _units_dict = {
            'bytes':'positiveInteger',
            'fps': 'float',
            'Hz': 'float',
            'MHz': 'float',
            'mm': 'float',
            'µs':'float'
            }

    def get_type(self, line):
        self.line = line
        xs_type = 'string'
        if len(line) > 2 and line[2] in ElementTyper._units_dict:
            xs_type = ElementTyper._units_dict[line[2]]
        elif line[0][0] in ElementTyper._element_name_dict:
            xs_type = ElementTyper._element_name_dict[line[0][0]]

        fe_logger.debug("Type Determined: " + xs_type)

        return XS_NS + xs_type

