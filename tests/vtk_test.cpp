#include <exception>
#include <iostream>


#include "vtk/ImageViewer.h"

using namespace visual_sonics::vtk;


int main()
{
  try
  {
    ImageViewer* vi = new ImageViewer("/mnt/dataa/visualsonics/test_base/20071105/3000e-55MHz-Gp10dB");

    vi->view_b_mode();

    delete vi;
  }
  catch ( std::exception& e )
  {
    std::cerr << "There was a std::exception: "  << e.what() << std::endl;
  }

  return 0;
}
