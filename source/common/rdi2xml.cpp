
#include <iostream>
#include <string>

#include "rdiReader.h"

using std::cerr;
using std::endl;

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
  catch (std::exception& e)
    {
    cerr << "Error: "  << e.what() << endl;
    return 1;
    }
  return 0;
}

