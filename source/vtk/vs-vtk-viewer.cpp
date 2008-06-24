/*! @file  vs-vtk-viewer.cpp
 *  @brief command line application to view VisualSonics Digital RF files with VTK
 *
 *  @author Matt McCormick (thewtex) <matt@mmmccormick.com>
 *  @date   2008 June 18
 *
 */

#include <cstring> // strcmp
#include <exception>
#include <iostream>
#include <string>
using namespace std;

#include "vtk/ImageViewer.h"
using namespace visual_sonics::vtk;


int main( int argc, char** argv )
{
  try
  {
    // print help
    string first_arg(argv[1]);
    if( argc == 1  ||  ( first_arg ==  "-h") || first_arg == "--help" )
    {
      cout << "command line application to view VisualSonics Digital RF files with VTK\n\n"
      << "pass one or more *.rdb or *.rdi files as arguments" << std::endl;

      if( argc == 1 )
	return 1;
      else
      {
        return 0;
      }
    }

    for( int i = 1; i < argc; i++ )
    {
      string file = string( argv[i] );
      ImageViewer* vi = new ImageViewer( file );
  
      vi->view_b_mode();
      //vi->view_saturation();

      delete vi;
    }
  }
  catch ( std::exception& e )
  {
    std::cerr << "There was a std::exception: "  << e.what() << std::endl;
    return 1;
  }

  return 0;

};


