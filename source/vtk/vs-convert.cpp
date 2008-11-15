/*! @file  vs-convert.cpp
 *  @brief command line application to VisualSonics Digital RF files to various other formats with VTK
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

#include "vtkSmartPointer.h"
#include "vtkStructuredGridWriter.h"

#include "vtkVisualSonicsReader.h"
//using namespace visual_sonics::vtk;




int main( int argc, char** argv )
{

  // command line parsing
  typedef vtkmetaio::MetaCommand vtkmc;
  vtkmc command;

  command.SetOption( "target", "t", false, "target aspect of the file to display" );
  command.AddOptionField( "target", "target", vtkmc::STRING, true, "rf-bmode-volume", "bmode-scout, saturation-scout, rf-bmode-surface, or rf-bmode-volume");

  command.AddField( "in_file", "Prefix of the .rdi/.rdb set of Visual Sonics raw files that should be in the same directory.", vtkmc::STRING, true );
  command.AddField( "out_file", "Output file name.  Extension determines output format.", vtkmc::STRING, true );

  command.SetDescription("command line application to convert VisualSonics Digital RF files with VTK\n\npass one or more *.rdb or *.rdi files as arguments");
  command.SetAuthor("Matt McCormick (thewtex)");

  if( !command.Parse( argc, argv ) || !command.GetOptionWasSet("in_file") )
      return 1;


  // check input file name, truncate if needed
  std::string in_file = command.GetValueAsString( "in_file" );
  size_t in_file_l = in_file.length();
  if( in_file_l > 3 )
    {
    if( !in_file.compare( in_file_l - 3, 3, ".rd" ) ) // as often occurs with tab completion
      in_file = in_file.substr( 0, in_file_l - 3 );

    else if( !in_file.compare( in_file_l - 4, 4, ".rdi" ) ||
      !in_file.compare( in_file_l - 4, 4, ".rdb" ) )
      {
      in_file = in_file.substr( 0, in_file_l - 4 );
      }
    }


  vtkSmartPointer<vtkVisualSonicsReader> vtk_vs_reader = vtkSmartPointer<vtkVisualSonicsReader>::New();
  vtkSmartPointer<vtkStructuredGridWriter> sgw = vtkSmartPointer<vtkStructuredGridWriter>::New();
  sgw->SetInputConnection( vtk_vs_reader->GetOutputPort(0) );
  sgw->SetFileTypeToASCII();

  // nike
  try
  {
    vtk_vs_reader->SetFilePrefix( in_file.c_str() );
    sgw->SetFileName( (in_file + ".vtk").c_str() );
    sgw->Update();

    std::string target = command.GetValueAsString( "target" );
    //if     ( target == "rf-bmode-volume" )
      //vi->view_rf();
    //else if( target == "rf-bmode-surface");
      ////vi->view_rf_surface();
    //else if( target == "bmode-scout" )
      //vi->view_b_mode();
    //else if( target == "saturation-scout" )
      //vi->view_saturation();

  }
  catch ( std::exception& e )
  {
    std::cerr << "There was a std::exception: "  << e.what() << std::endl;
    return 1;
  }

  return 0;

};

