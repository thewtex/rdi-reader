
#include <fstream>
#include <iostream>
#include <string>
using namespace std;

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
  catch (ifstream::failure& e)
    {
    cerr << "Error: failure opening/reading file." << endl;
    return 1;
    }
  catch (std::exception& e)
    {
    cerr << "Error: "  << e.what() << endl;
    return 1;
    }
  return 0;
}

