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

void rdiReader::parse()
{
  const XMLCh ls[] = {chLatin_L, chLatin_S, chNull};
  DOMImplementation* impl = DOMImplementationRegistry::getDOMImplementation(ls);
  ::xml_schema::dom::auto_ptr< ::xercesc::DOMDocument> domdoc (impl->createDocument(
      0,
      X("rdi"),
      0));

  DOMElement* root_elem = domdoc->getDocumentElement();

  std::ifstream infile(m_filepath.c_str());
  infile.exceptions(ifstream::eofbit|ifstream::failbit|ifstream::badbit);

  std::string line;
  std::vector<std::string> parsed_v;

  getline(infile, line);
  if(line == "=== IMAGE INFO ===")
    throw(xml_schema::expected_element("image_info", ""));

  DOMElement* image_info = domdoc->createElement(X("image_info"));
  root_elem->appendChild(image_info);

  m_line_parser.parse_line(infile);

  DOMElement* study_name = domdoc->createElement(X(m_line_parser.elements[0].c_str()));
  image_info->appendChild(study_name);

  ::std::auto_ptr< ::rdi_t> rdi_i (
    rdi(domdoc,
      xml_schema::flags::keep_dom | xml_schema::flags::own_dom)
  );

}


void rdiReader::LineParser::parse_line(istream& f)
{
  //skip first "
  f.seekg(1, ios::cur);
  getline(f, m_line, '"');

  this->elements.resize(count(m_line.begin(), m_line.end(), '/')+1);
  replace(m_line.begin(), m_line.end(), ' ', '_');
  size_t previous_separator_pos = 0;
  size_t next_separator_pos = m_line.find_first_of('/');
  size_t element_index=0;
  while(next_separator_pos != string::npos)
    {
    elements[element_index] = m_line.substr(previous_separator_pos, next_separator_pos - previous_separator_pos);
    previous_separator_pos = next_separator_pos + 1;
    next_separator_pos = m_line.find_first_of('/', previous_separator_pos);
    element_index++;
    }
  elements[element_index] = m_line.substr(previous_separator_pos);
}
