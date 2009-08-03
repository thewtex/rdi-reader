#include "rdiReader.h"

#include <fstream>
#include <iostream>
#include <string>
#include <stdexcept>
using namespace std;


rdiReader::rdiReader(const char* filepath):
  m_filepath(filepath)
{
}

void rdiReader::parse()
{
  std::ifstream infile(m_filepath.c_str());
  infile.exceptions(ifstream::eofbit|ifstream::failbit|ifstream::badbit);

  std::string line;
  getline(infile, line);
  cout <<  (line == "=== IMAGE INFO ===") << endl;
}
