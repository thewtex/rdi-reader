#include "rdiReader.h"

#include <fstream>
#include <iostream>
#include <string>
#include <stdexcept>
using namespace std;

#include <xercesc/util/PlatformUtils.hpp>

rdiReader::rdiReader(const char* filepath):
  m_filepath(filepath)
{
}

void rdiReader::parse()
{
  xercesc::XMLPlatformUtils::Initialize();

  std::ifstream infile(m_filepath.c_str());
  infile.exceptions(ifstream::eofbit|ifstream::failbit|ifstream::badbit);

  std::string line;
  getline(infile, line);
  cout <<  (line == "=== IMAGE INFO ===") << endl;

  xercesc::XMLPlatformUtils::Terminate();
}
