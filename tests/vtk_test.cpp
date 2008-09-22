#include <exception>
#include <iostream>


#include "vtk/ImageViewer.h"

using namespace visual_sonics::vtk;


int main()
{
  try
  {
    //ImageViewer* vi = new ImageViewer("/mnt/research/Research/in_vivo/test_base/visual_sonics/2007-06-08-15-57-05-328");
    ImageViewer* vi = new ImageViewer("/tmp/2007-06-08-15-57-05-328");
    //ImageViewer* vi = new ImageViewer("/mnt/dataa/visualsonics/test_base/visual_sonics/2007-06-08-15-57-05-328");

    vi->view_b_mode();
    //vi->view_saturation();
    //vi->view_rf();

    delete vi;
  }
  catch ( std::exception& e )
  {
    std::cerr << "There was a std::exception: "  << e.what() << std::endl;
    return 1;
  }

  return 0;
}
