#include "rdiReader.h"

#include <exception>
#include <fstream>
#include <iostream>
#include <string>

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
    cerr << "Could not open the file: " << m_filepath.c_str() << endl;
    throw;
    }

  std::string line;
  getline(infile, line);
  cout << line << endl;

}
