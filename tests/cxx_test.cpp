#include <ctime>
#include <exception>
#include <iostream>
#include <string>

#include "rdiParser.h"
#include "rdiParserData.h"

#include "cxx/parse_vs_rdi.h"

#include "cxx/ReadImage.h"

using namespace visual_sonics;

int main()
{
#ifndef NDEBUG
  std::clock_t start,end;
  start = std::clock();
#endif

  try
  {

    std::string filename("/mnt/dataa/visualsonics/test_base/visual_sonics/2007-06-08-15-57-05-328");

    rdiParser rdi_parser( filename);
    rdiParserData rpd = rdi_parser.parse();

    visual_sonics::cxx::ReadImage ei("/mnt/dataa/visualsonics/test_base/visual_sonics", "2007-06-08-15-57-05-328");
    ei.read_b_mode_image();

    rdiParserData rpd_from_lib = parse_vs_rdi(filename);
  }
  catch ( std::exception& e )
  {
    std::cerr << "There was a std::exception: "  << e.what() << std::endl;
  }


#ifndef NDEBUG
  end = std::clock();
  double dif = static_cast<double>(end-start);
  double time = dif/CLOCKS_PER_SEC;
  std::cout << " the time is : " << time << "\n and end is :" << end <<  "\n and start time is : " << start << std::endl;
  std::cout << "clocks per sec: " << CLOCKS_PER_SEC << std::endl;
#endif

  return 0;
}
