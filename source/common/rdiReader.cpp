#include "rdiReader.h"

#include <fstream>
#include <iostream>
#include <string>
#include <stdexcept>

using std::cerr;
using std::cout;
using std::endl;


rdiReader::rdiReader(const char* filepath):
  m_filepath(filepath)
{
}

void rdiReader::parse()
{
  std::ifstream infile(m_filepath.c_str());

  if(!infile)
    {
    std::string error_text = "Could not open the file " + m_filepath + '\n';
    throw(std::runtime_error(error_text));
    }

  std::string line;
  getline(infile, line);
  cout << line << endl;

}
