
#include <exception>
#include <iostream>
#include <string>

#include "rdiParser.h"
#include "rdiParserData.h"

int main()
{
  std::cout << "Hello World!" << std::endl;

  try
  {

    std::string filename("/mnt/research/Research/in_vivo/test_base/visual_sonics/2007-06-08-15-57-05-328");
    rdiParser rdi_parser( filename);
    
    rdiParserData rpd = rdi_parser.parse();

    
    std::cout << "Study name: " << rpd.its_study_name << "\nImage lines: " << rpd.its_image_lines << "\ntime: " << rpd.its_acquisition_time << "\nscan width: " << rpd.its_rf_mode_scan_scan_width << std::endl;

  }
  catch ( std::exception& e )
  {
    std::cerr << "There was a std::exception: "  << e.what() << std::endl;
  }

  return 0;
}
