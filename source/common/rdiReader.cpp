#include "rdiReader.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <stdexcept>
using namespace std;

#include <xercesc/dom/DOM.hpp>
#include <xercesc/util/PlatformUtils.hpp>
using namespace xercesc;

#include <xsd/cxx/xml/dom/auto-ptr.hxx>
#include <xsd/cxx/tree/exceptions.hxx>

#include "common/formats/rdi.hxx"
#include "common/ToXMLCh.h"
#define X(str) m_transcoder->transcode(str)
// alternate, as long as local encoding is correct, as I understand it
//#include "common/XStr.h"
//#define X(str) XStr(str).unicodeForm()


/**
 * @brief maximum number of chars that will need to be transcoded
 */
const static unsigned int transcode_max_chars = 512;

rdiReader::rdiReader(const char* filepath):
  m_filepath(filepath)
{
  xercesc::XMLPlatformUtils::Initialize();

  m_transcoder = new ToXMLCh();
}

rdiReader::~rdiReader()
{
  delete m_transcoder;

  xercesc::XMLPlatformUtils::Terminate();
}

auto_ptr<rdi_t> rdiReader::parse()
{
  const XMLCh ls[] = {chLatin_L, chLatin_S, chNull};
  DOMImplementation* impl = DOMImplementationRegistry::getDOMImplementation(ls);
  ::xml_schema::dom::auto_ptr< ::xercesc::DOMDocument> domdoc (impl->createDocument(
      0,
      X("rdi"),
      0));

  DOMElement* root_elem = domdoc->getDocumentElement();
  const XMLCh units[] = {chLatin_u, chLatin_n, chLatin_i, chLatin_t, chLatin_s, chNull};

  std::ifstream infile(m_filepath.c_str());
  infile.exceptions(ifstream::eofbit|ifstream::failbit|ifstream::badbit);

  std::string line;

  // @todo split these sections of code up into functions
  // parse IMAGE INFO SECTION
  getline(infile, line);
  if(line == "=== IMAGE INFO ===")
    throw(xml_schema::expected_element("image_info", ""));

  DOMElement* image_info = domdoc->createElement(X("image_info"));
  root_elem->appendChild(image_info);

  DOMElement* child;
  DOMText* text;
  //skip first "
  infile.seekg(1, ios::cur);
  while(m_line_parser.parse_line(infile))
    {
    child = domdoc->createElement(X(m_line_parser.elements[0].c_str()));
    image_info->appendChild(child);
    text = domdoc->createTextNode(X(m_line_parser.content.c_str()));
    child->appendChild(text);
    if(m_line_parser.units.size() > 0)
      child->setAttribute(units, X(m_line_parser.units.c_str()));
    }

  // parse IMAGE DATA SECTION
  getline(infile, line);
  while(!(line == "\"=== IMAGE PARAMETERS ===\"\r"))
    getline(infile, line);

  DOMElement* image_data = domdoc->createElement(X("image_data"));
  root_elem->appendChild(image_data);

  // parse IMAGE PARAMETERS SECTION
  DOMElement* image_parameters = domdoc->createElement(X("image_parameters"));
  root_elem->appendChild(image_parameters);
  try
    {
    //skip first "
    infile.seekg(1, ios::cur);
    size_t node_depth = 0;
    DOMElement* current_element;
    DOMElement* new_child;
    DOMNodeList* child_elements_with_tag;
    while(m_line_parser.parse_line(infile))
      {
      current_element = image_parameters;
      for(node_depth = 0; node_depth < m_line_parser.elements.size() - 1; ++node_depth)
	{
	child_elements_with_tag =
	  current_element->getElementsByTagName(X(m_line_parser.elements[node_depth].c_str()));
	if(child_elements_with_tag->getLength() == 0)
	  {
	  new_child = domdoc->createElement(X(m_line_parser.elements[node_depth].c_str()));
	  current_element->appendChild(new_child);
	  current_element = new_child;
	  }
	else
	  {
	  current_element = dynamic_cast<DOMElement*>(child_elements_with_tag->item(0));
	  }
	}
      new_child = domdoc->createElement(X(m_line_parser.elements[node_depth].c_str()));
      current_element->appendChild(new_child);
      text = domdoc->createTextNode(X(m_line_parser.content.c_str()));
      new_child->appendChild(text);
      if(m_line_parser.units.size() > 0)
	new_child->setAttribute(units, X(m_line_parser.units.c_str()));
      } // end parsing and handling a line
    }
  catch(const ifstream::failure& e)
    {
    if(!infile.eof())
      throw e;
    }

  ::std::auto_ptr< ::rdi_t> rdi_i (
    rdi(domdoc,
      xml_schema::flags::keep_dom | xml_schema::flags::own_dom)
  );

  return rdi_i;
}


string rdiReader::LineParser::make_element_name_valid(const string& element_name)
{
  if(element_name.substr(0,1).find_first_of("0123456789-") == 0)
    return string("X_") + element_name;
  return element_name;
}


bool rdiReader::LineParser::parse_line(istream& f)
{
  units.clear();

  getline(f, m_subline, '"');

  this->elements.resize(count(m_subline.begin(), m_subline.end(), '/')+1);
  replace(m_subline.begin(), m_subline.end(), ' ', '_');
  size_t previous_separator_pos = 0;
  size_t next_separator_pos = m_subline.find_first_of('/');
  size_t element_index=0;
  while(next_separator_pos != string::npos)
    {
    elements[element_index] = make_element_name_valid(m_subline.substr(previous_separator_pos, next_separator_pos - previous_separator_pos));
    previous_separator_pos = next_separator_pos + 1;
    next_separator_pos = m_subline.find_first_of('/', previous_separator_pos);
    element_index++;
    }
  elements[element_index] = make_element_name_valid(m_subline.substr(previous_separator_pos));

  getline(f, m_subline, '"');
  if(!(m_subline == ","))
    {
    return false;
    }

  getline(f, content, '"');

  getline(f, m_subline, '"');
  if(m_subline == ",")
    {
    getline(f, units, '"');
    // carriage return, newline and starting "
    f.seekg(3, ios::cur);
    }

  return true;
}
