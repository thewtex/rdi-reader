
#include <fstream>
#include <iostream>
#include <string>
using namespace std;

#include <xercesc/util/PlatformUtils.hpp>

#include "rdiReader.h"


int main(int argc, char* argv[])
{
  if(argc != 2)
    {
    cerr << "usage: " << argv[0] << " filename.rdi" << endl;
    return 1;
    }

  try
  {
    rdiReader rdi_reader(argv[1]);

    rdi_reader.parse();
  }
  catch (const ifstream::failure& e)
    {
    cerr << "Error: failure opening/reading file." << endl;
    return 1;
    }
  // xerces-C++
  catch (const xercesc::XMLException& e)
    {
    cerr << "Error: In Xerces-C++ xml processing." << endl;
    return 1;
    }
  catch (const std::exception& e)
    {
    cerr << "Error: "  << e.what() << endl;
    return 1;
    }
  return 0;
}

