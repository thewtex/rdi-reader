from namespace_vars import *

##
# @brief determine the XML schema type
class ElementTyper():
    def __init__(self):
        self.line = []

    _element_name_dict = {
            'Image Frames':'positiveInteger',
            'Image Lines':'positiveInteger',
            'Image Acquisition Per Line':'positiveInteger'
            }

    _units_dict = {
            'bytes':'positiveInteger',
            'µs':'float'
            }

    def get_type(self, line):
        self.line = line
        xs_type = 'string'
        if len(line) > 2 and line[2] in ElementTyper._units_dict:
            xs_type = ElementTyper._units_dict[line[2]]
        elif line[0][0] in ElementTyper._element_name_dict:
            xs_type = ElementTyper._element_name_dict[line[0][0]]

        return XS_NS + xs_type

