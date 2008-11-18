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

#include "vtkImageCast.h"
#include "vtkMetaImageWriter.h"
#include "vtkSmartPointer.h"
#include "vtkStructuredPointsWriter.h"
#include "vtkUnsafeStructuredGridToImage.h"
#include "vtkXMLImageDataWriter.h"

#include "vtkVisualSonicsReader.h"
//using namespace visual_sonics::vtk;




int main( int argc, char** argv )
{

  // command line parsing
  typedef vtkmetaio::MetaCommand vtkmc;
  vtkmc command;

  command.SetOption( "target", "t", false, "target aspect of the file to display" );
  command.AddOptionField( "target", "target", vtkmc::STRING, true, "rf", "bmode, saturation, rf, bmode-no-scan-convert, saturation-no-scan-convert, or rf-no-scan-convert");

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

  vtkAlgorithm* writer;
  std::string out_file = command.GetValueAsString( "out_file" );
  size_t out_file_l = out_file.length();
  if( out_file_l > 4 )
    {
    std::string extension = out_file.substr( out_file_l - 4, 4 );
    //if( extension == ".tif" || extension == "tiff")
      //{
      //writer = vtkTIFFWriter::New();
      //vtkTIFFWriter* writer_t = dynamic_cast<vtkTIFFWriter*>( writer );
      //writer_t->SetCompressionToPackBits();
      //}
    if( extension == ".vti" || extension == "pvti" )
      {
      writer = vtkXMLImageDataWriter::New();
      }
    else if( extension == ".mhd" || extension == ".mha" )
      {
      writer = vtkMetaImageWriter::New();
      }
    else if( extension == ".vtk" )
      {
      writer = vtkStructuredPointsWriter::New();
      }
    else
      {
      std::cerr << "Unrecognized extension: " << extension << std::endl;
      return 1;
      }


    if( extension == ".vti" || extension == "pvti" )
      {
      vtkXMLWriter* writer_t = dynamic_cast<vtkXMLWriter*>( writer );
      writer_t->SetFileName( out_file.c_str() );
      }
    else if( extension == ".vtk" )
      {
      vtkStructuredPointsWriter* writer_t = dynamic_cast<vtkStructuredPointsWriter*>( writer );
      writer_t->SetFileName( out_file.c_str() );
      //writer_t->SetFileTypeToASCII();
      //writer_t->SetFileTypeToBinary();
      }
    else if( extension == ".mha" || extension == ".mhd" )
      {
      vtkMetaImageWriter* writer_t = dynamic_cast<vtkMetaImageWriter*>( writer );
      writer_t->SetFileName( out_file.c_str() );
      writer_t->SetRAWFileName( (out_file.substr(0, out_file_l - 4) + ".raw").c_str() );
      }
    else
      {
      vtkImageWriter* writer_t = dynamic_cast<vtkImageWriter*>( writer );
      writer_t->SetFileName( out_file.c_str() );
      }
    }
  else
    {
    std::cerr << "Filename + extension not found! in " << out_file << std::endl;
    return 1;
    }


  vtkSmartPointer<vtkVisualSonicsReader> vtk_vs_reader = vtkSmartPointer<vtkVisualSonicsReader>::New();
  vtkSmartPointer<vtkUnsafeStructuredGridToImage> usgti = vtkSmartPointer<vtkUnsafeStructuredGridToImage>::New();
  //vtkSmartPointer<vtkImageCast> caster = vtkSmartPointer<vtkImageCast>::New();
  //caster->ClampOverflowOn();
  // nike
  try
  {
    vtk_vs_reader->SetFilePrefix( in_file.c_str() );

    std::string target = command.GetValueAsString( "target" );
    if     ( target == "bmode" )
      {
      //caster->SetOutputScalarTypeToUnsignedShort();
      //caster->SetInputConnection( vtk_vs_reader->GetOutputPort(3) );
      //writer->SetInputConnection( caster->GetOutputPort() );
      writer->SetInputConnection( vtk_vs_reader->GetOutputPort(3) );
      }
    ////else if( target == "rf-bmode-surface");
      //////vi->view_rf_surface();
    ////else if( target == "bmode-scout" )
      ////vi->view_b_mode();
    ////else if( target == "saturation-scout" )
      ////vi->view_saturation();

    writer->Update();

  }
  catch ( std::exception& e )
  {
    std::cerr << "There was a std::exception: "  << e.what() << std::endl;
    return 1;
  }

  return 0;

};

