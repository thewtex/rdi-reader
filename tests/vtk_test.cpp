#include "vtk/ViewImage.h"

using namespace visual_sonics::vtk;


int main()
{

  ViewImage* vi = new ViewImage("/mnt/dataa/visualsonics/test_base/visual_sonics", "2007-06-08-15-57-05-328");

  vi->view_b_mode();

  delete vi;

  return 0;
}
