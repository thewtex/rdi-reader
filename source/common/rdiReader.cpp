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

#include "formats/rdi.hxx"
#include "ToXMLCh.h"
#define X(str) m_transcoder->transcode(str)
// alternate, as long as local encoding is correct, as I understand it
//#include "common/XStr.h"
//#define X(str) XStr(str).unicodeForm()

const static XMLCh LS[] = {chLatin_L, chLatin_S, chNull};
const static XMLCh UNITS[] = {chLatin_u, chLatin_n, chLatin_i, chLatin_t, chLatin_s, chNull};

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

::xml_schema::dom::auto_ptr< ::xercesc::DOMDocument> rdiReader::parse_to_DOMDocument()
{
  try
    {
    DOMImplementation* impl = DOMImplementationRegistry::getDOMImplementation(LS);
    ::xml_schema::dom::auto_ptr< ::xercesc::DOMDocument> domdoc (impl->createDocument(
	0,
	X("rdi"),
	0));

    std::ifstream infile(m_filepath.c_str());
    infile.exceptions(ifstream::eofbit|ifstream::failbit|ifstream::badbit);

    domdoc = parse_IMAGE_INFO_section(infile, domdoc);
    domdoc = parse_IMAGE_DATA_section(infile, domdoc);
    domdoc = parse_IMAGE_PARAMETERS_section(infile, domdoc);

    return domdoc;
    }
  catch (const ifstream::failure& e)
    {
    throw std::runtime_error("failure opening/reading file.");
    }
  // xerces-C++
  catch (const xercesc::XMLException& e)
    {
    char* msg = xercesc::XMLString::transcode(e.getMessage());
    std::string msg_str = std::string(msg);
    xercesc::XMLString::release(&msg);
    throw std::logic_error(string("In Xerces-C++ xml processing: ") + msg_str);
    }
  catch (const xercesc::DOMException& e)
    {

    std::ostringstream code_strm;
    code_strm << e.code;
    throw std::runtime_error(
      std::string("In Xerces-C++ DOM processing, code: ") + code_strm.str() +
      std::string("\nSee http://xerces.apache.org/xerces-c/apiDocs-3/classDOMException.html for an explanation.")
      );
    }
  catch (const xsd::cxx::tree::expected_element<char>& e)
    {
    throw std::logic_error(
      e.what() +
      string(": '") + e.name() +
      string("' in namespace: ") + e.namespace_()
      );
    }

}


auto_ptr<rdi_t> rdiReader::parse_to_rdi_t()
{
    ::xml_schema::dom::auto_ptr< ::xercesc::DOMDocument> domdoc = this->parse_to_DOMDocument();
    try
      {
      std::auto_ptr< ::rdi_t> rdi_i (
	rdi(domdoc,
	  xml_schema::flags::keep_dom | xml_schema::flags::own_dom) 
	);
      return rdi_i;
      }
    catch (const xsd::cxx::tree::expected_element<char>& e)
      {
      throw std::logic_error(
	e.what() +
	string(": '") + e.name() +
	string("' in namespace: ") + e.namespace_()
	);
      }
}


::xml_schema::dom::auto_ptr< ::xercesc::DOMDocument>
rdiReader::parse_IMAGE_INFO_section(std::ifstream& infile,
  ::xml_schema::dom::auto_ptr< ::xercesc::DOMDocument> domdoc)
{
  std::string line;
  getline(infile, line);
  if(line == "=== IMAGE INFO ===")
    throw(xml_schema::expected_element("image_info", ""));

  DOMElement* image_info = domdoc->createElement(X("image_info"));
  //root_elem->appendChild(image_info);
  domdoc->getDocumentElement()->appendChild(image_info);

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
      child->setAttribute(UNITS, X(m_line_parser.units.c_str()));
    }

  return domdoc;
}


::xml_schema::dom::auto_ptr< ::xercesc::DOMDocument>
rdiReader::parse_IMAGE_DATA_section(std::ifstream& infile,
  ::xml_schema::dom::auto_ptr< ::xercesc::DOMDocument> domdoc)
{
  string line;
  getline(infile, line);
  while(!(line == "\"=== IMAGE PARAMETERS ===\"\r"))
    getline(infile, line);

  DOMElement* image_data = domdoc->createElement(X("image_data"));
  domdoc->getDocumentElement()->appendChild(image_data);
  return domdoc;
}


::xml_schema::dom::auto_ptr< ::xercesc::DOMDocument>
rdiReader::parse_IMAGE_PARAMETERS_section(std::ifstream& infile,
  ::xml_schema::dom::auto_ptr< ::xercesc::DOMDocument> domdoc)
{
  std::string line;
  DOMText* text;
  DOMElement* image_parameters = domdoc->createElement(X("image_parameters"));
  domdoc->getDocumentElement()->appendChild(image_parameters);
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
        {
	new_child->setAttribute(UNITS, X(m_line_parser.units.c_str()));
        }
      } // end parsing and handling a line
    }
  catch(const ifstream::failure& e)
    {
    if(!infile.eof())
      {
      throw e;
      }
    }
  return domdoc;
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

  try
    {
    getline(f, m_subline, '"');
    }
  catch(const ifstream::failure& e)
    {
    f.seekg(3, ios::cur);
    f.clear();
    return true;
    }
  if(m_subline == ",")
    {
    getline(f, units, '"');
    // carriage return, newline and starting "
    f.seekg(3, ios::cur);
    }

  return true;
}
