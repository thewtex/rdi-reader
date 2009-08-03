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

#include "common/XStr.h"
#include "common/formats/rdi.hxx"

rdiReader::rdiReader(const char* filepath):
  m_filepath(filepath)
{
}

void rdiReader::parse()
{
  xercesc::XMLPlatformUtils::Initialize();

  ::xml_schema::dom::auto_ptr< ::xercesc::DOMDocument> domdoc;

  std::ifstream infile(m_filepath.c_str());
  infile.exceptions(ifstream::eofbit|ifstream::failbit|ifstream::badbit);

  std::string line;
  getline(infile, line);
  cout <<  (line == "=== IMAGE INFO ===") << endl;

  ::std::auto_ptr< ::image_info_t> rdi (
    image_info(domdoc,
      xml_schema::flags::keep_dom | xml_schema::flags::own_dom)
  );

  xercesc::XMLPlatformUtils::Terminate();
}
