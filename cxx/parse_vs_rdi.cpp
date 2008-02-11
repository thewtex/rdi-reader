#include "parse_vs_rdi.h"

#include <iostream>
#include <string>

#include "rdiParser.h"
#include "rdiParserData.h"

rdiParserData parse_vs_rdi(const std::string& filename)
{
  try
  {

    rdiParser rdi_parser( filename );
    
    return rdi_parser.parse();

  }
  catch ( std::exception& e )
  {
    std::cerr << "There was a std::exception: "  << e.what() << std::endl;
  }

  rdiParserData noworky;
  return noworky;

}

