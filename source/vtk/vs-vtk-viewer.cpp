/*! @file  vs-vtk-viewer.cpp
 *  @brief command line application to view VisualSonics Digital RF files with VTK
 *
 *  @author Matt McCormick (thewtex) <matt@mmmccormick.com>
 *  @date   2008 June 18
 *
 */

#include <exception>
#include <iostream>
#include <map>
#include <string>
using namespace std;

#include "vtkmetaio/metaCommand.h"

#include "vtk/ImageViewer.h"
using namespace visual_sonics::vtk;



int main( int argc, char** argv )
{

  // command line parsing
  typedef vtkmetaio::MetaCommand vtkmc;
  vtkmc command;
  initialize_target_map();

  command.SetOption( "target", "t", false, "target aspect of the file to display" );
  command.AddOptionField( "target", "target", vtkmc::STRING, true, "rf-bmode-volume", "bmode-scout, saturation-scout, rf-bmode-surface, or rf-bmode-volume");

  command.AddField( "in_file", "infile filename", vtkmc::STRING, true );

  command.SetDescription("command line application to view VisualSonics Digital RF files with VTK\n\npass one or more *.rdb or *.rdi files as arguments");
  command.SetAuthor("Matt McCormick (thewtex)");

  if( !command.Parse( argc, argv ) )
      return 1;


  try
  {

    for( int i = 1; i < argc; i++ )
    {
      string file = string( argv[i] );
      ImageViewer* vi = new ImageViewer( file );

      //vi->view_b_mode();
      vi->view_saturation();
      //vi->view_rf();

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

