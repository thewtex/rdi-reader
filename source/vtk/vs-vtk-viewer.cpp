/*! @file  vs-vtk-viewer.cpp
 *  @brief command line application to view VisualSonics Digital RF files with VTK
 *
 *  @author Matt McCormick (thewtex) <matt@mmmccormick.com>
 *  @date   2008 June 18
 *
 */

#include <exception>
#include <iostream>
using namespace std;

#include "vtk/ImageViewer.h"
using namespace visual_sonics::vtk;


int main( int argc, char** argv )
{
  try
  {
    ImageViewer* vi = new ImageViewer("/mnt/research/Research/in_vivo/test_base/visual_sonics/2007-06-08-15-57-05-328");

    //vi->view_b_mode();
    vi->view_saturation();

    delete vi;
  }
  catch ( std::exception& e )
  {
    std::cerr << "There was a std::exception: "  << e.what() << std::endl;
    return 1;
  }

  return 0;

};


