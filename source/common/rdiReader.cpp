#include "rdiReader.h"

#include <fstream>
#include <iostream>
#include <string>
#include <stdexcept>
using namespace std;

#include <xercesc/dom/DOM.hpp>
#include <xercesc/util/PlatformUtils.hpp>
using namespace xercesc;

#include <xsd/cxx/xml/dom/auto-ptr.hxx>

#include "common/formats/rdi.hxx"
#include "common/XStr.h"

#define X(str) XStr(str).unicodeForm()

/**
 * @brief maximum number of chars that will need to be transcoded
 */
const static unsigned int transcode_max_chars = 512;

rdiReader::rdiReader(const char* filepath):
  m_filepath(filepath)
{
  xercesc::XMLPlatformUtils::Initialize();
}

rdiReader::~rdiReader()
{
  xercesc::XMLPlatformUtils::Terminate();
}

void rdiReader::parse()
{
  const XMLCh ls[] = {chLatin_L, chLatin_S, chNull};
  DOMImplementation* impl = DOMImplementationRegistry::getDOMImplementation(ls);
  ::xml_schema::dom::auto_ptr< ::xercesc::DOMDocument> domdoc (impl->createDocument(
      X("http://mmmccormick.com/visualsonics"),
      X("rdi"),
      0));

  DOMElement* root_elem = domdoc->getDocumentElement();

  std::ifstream infile(m_filepath.c_str());
  infile.exceptions(ifstream::eofbit|ifstream::failbit|ifstream::badbit);

  std::string line;
  getline(infile, line);
  cout <<  (line == "=== IMAGE INFO ===") << endl;

  ::std::auto_ptr< ::image_info_t> rdi (
    image_info(domdoc,
      xml_schema::flags::keep_dom | xml_schema::flags::own_dom)
  );

}
