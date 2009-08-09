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

/// for debugging
#include <ostream>

#include <xercesc/dom/DOM.hpp>
#include <xercesc/util/XMLUniDefs.hpp>

#include <xsd/cxx/xml/string.hxx>
#include <xsd/cxx/xml/dom/auto-ptr.hxx>
#include <xsd/cxx/xml/dom/serialization-source.hxx>
#include <xsd/cxx/xml/dom/bits/error-handler-proxy.hxx>

#include <xsd/cxx/tree/exceptions.hxx>
#include <xsd/cxx/tree/error-handler.hxx>

/// end for debugging

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
  const XMLCh units[] = {chLatin_u, chLatin_n, chLatin_i, chLatin_t, chLatin_s, chNull};

  std::ifstream infile(m_filepath.c_str());
  infile.exceptions(ifstream::eofbit|ifstream::failbit|ifstream::badbit);

  std::string line;

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

  getline(infile, line);
  while(!(line == "\"=== IMAGE PARAMETERS ===\"\r"))
    getline(infile, line);

  DOMElement* image_data = domdoc->createElement(X("image_data"));
  root_elem->appendChild(image_data);
  //// for debugging
      namespace xml = xsd::cxx::xml;
    namespace tree = xsd::cxx::tree;

      tree::error_handler<char> eh;
        xml::dom::bits::error_handler_proxy<char> ehp (eh);

	ofstream os("current.xml");
	string encoding("UTF-8");
	  xml::dom::ostream_format_target oft (os);

	   // Create a DOMSerializer.
  //
  xml::dom::auto_ptr<DOMLSSerializer> writer (
    impl->createLSSerializer ());

  DOMConfiguration* conf (writer->getDomConfig ());

  // Set error handler.
  //
  conf->setParameter (XMLUni::fgDOMErrorHandler, &ehp);

  // Set some generally nice features.
  //
  conf->setParameter (XMLUni::fgDOMWRTDiscardDefaultContent, true);
  conf->setParameter (XMLUni::fgDOMWRTFormatPrettyPrint, true);

  xml::dom::auto_ptr<DOMLSOutput> out (impl->createLSOutput ());
  out->setEncoding (xml::string (encoding).c_str ());
  out->setByteStream (&oft);

writer->write(root_elem, out.get() );
eh.throw_if_failed<tree::serialization<char> > ();
  ///// end for debugging

  ::std::auto_ptr< ::rdi_t> rdi_i (
    rdi(domdoc,
      xml_schema::flags::keep_dom | xml_schema::flags::own_dom)
  );

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
    elements[element_index] = m_subline.substr(previous_separator_pos, next_separator_pos - previous_separator_pos);
    previous_separator_pos = next_separator_pos + 1;
    next_separator_pos = m_subline.find_first_of('/', previous_separator_pos);
    element_index++;
    }
  elements[element_index] = m_subline.substr(previous_separator_pos);

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
