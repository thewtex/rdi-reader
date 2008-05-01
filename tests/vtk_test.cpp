#include <exception>
#include <iostream>


#include "vtk/ViewImage.h"

using namespace visual_sonics::vtk;


int main()
{
  try
  {
    ViewImage* vi = new ViewImage("/mnt/research/Research/in_vivo/test_base/visual_sonics", "2007-06-08-15-57-05-328");

    vi->view_b_mode();

    delete vi;
  }
  catch ( std::exception& e )
  {
    std::cerr << "There was a std::exception: "  << e.what() << std::endl;
  }

  return 0;
}
