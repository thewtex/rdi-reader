/*! @file  vs-vtk-convert.cpp
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
#include "vtkStructuredGridWriter.h"
#include "vtkStructuredPointsWriter.h"
#include "vtkUnsafeStructuredGridToImage.h"
#include "vtkXMLImageDataWriter.h"
#include "vtkXMLStructuredGridWriter.h"

#include "vtkVisualSonicsReader.h"
//using namespace visual_sonics::vtk;



//! perform the ->Write() action
void Write( vtkAlgorithm* );

int main( int argc, char** argv )
{
  using std::string;
  using std::cerr;
  using std::cout;
  using std::endl;

  /*************** command line parsing ***************/
  typedef vtkmetaio::MetaCommand vtkmc;
  vtkmc command;

  command.SetOption( "target", "t", false, "target aspect of the file to convert to.\n  Options listed include scan converted ImageData, StructuredGrid, and non-scan converted ImageData (data is not interpolated, but geometry is not correct)" );
  command.AddOptionField( "target", "target", vtkmc::STRING, true, "rf-no-scan-convert-image", "\nbmode, \nsaturation, \nrf, \nbmode-no-scan-convert, \nsaturation-no-scan-convert, \nrf-no-scan-convert, \nbmode-no-scan-convert-image, \nsaturation-no-scan-convert-image, \nor rf-no-scan-convert-image\n");

  command.AddField( "in_file", "Prefix of the .rdi/.rdb set of Visual Sonics raw files that should be in the same directory.", vtkmc::STRING, true );
  command.AddField( "out_file", "Output file name.  Extension determines output format.", vtkmc::STRING, true );

  command.SetDescription("command line application to convert VisualSonics Digital RF files with VTK\n\npass one or more *.rdb or *.rdi files as arguments");
  command.SetAuthor("Matt McCormick (thewtex)");

  if( !command.Parse( argc, argv ) || !command.GetOptionWasSet("in_file") )
      return 1;


  /*************** check input file name, truncate if needed ***************/
  string in_file = command.GetValueAsString( "in_file" );
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

  /*************** create writer type based on the out file extension ***************/
  const string target = command.GetValueAsString( "target" );
  vtkAlgorithm* writer;
  const string out_file = command.GetValueAsString( "out_file" );
  size_t out_file_l = out_file.length();
  if( out_file_l > 4 )
    {
    std::string extension = out_file.substr( out_file_l - 4, 4 );
    if( extension == ".vti" || extension == "pvti" )
    {
      if( target == "bmode-no-scan-convert" ||
	  target == "saturation-no-scan-convert" ||
	  target == "rf-no-scan-convert" )
      {
	cerr << "The VTK XML ImageData format only supports raster images, not general structured grids." << endl;
	return 1;
      }
      writer = vtkXMLImageDataWriter::New();
      vtkXMLWriter* writer_t = dynamic_cast<vtkXMLWriter*>( writer );
      writer_t->SetFileName( out_file.c_str() );
    }
    else if( extension == ".vts" || extension == "pvts" )
    {
      if( ! (target == "bmode-no-scan-convert" ||
	  target == "saturation-no-scan-convert" ||
	  target == "rf-no-scan-convert" ))
      {
	cerr << "The VTK XML StructuredGrid only supports general structured grids." << endl;
	return 1;
      }
      writer = vtkXMLStructuredGridWriter::New();
      vtkXMLStructuredGridWriter* writer_t = dynamic_cast<vtkXMLStructuredGridWriter*>( writer );
      writer_t->SetFileName( out_file.c_str() );
    }
    else if( extension == ".mhd" || extension == ".mha" )
    {
      if( target == "bmode-no-scan-convert" ||
	  target == "saturation-no-scan-convert" ||
	  target == "rf-no-scan-convert" )
      {
	cerr << "The MetaImage format only supports raster images, not general structured grids." << endl;
	return 1;
      }
      writer = vtkMetaImageWriter::New();
      vtkMetaImageWriter* writer_t = dynamic_cast<vtkMetaImageWriter*>( writer );
      writer_t->SetFileName( out_file.c_str() );
      writer_t->SetRAWFileName( (out_file.substr(0, out_file_l - 4) + ".raw").c_str() );
      writer_t->SetCompression( false );
    }
    else if( extension == ".vtk" &&
	( target == "bmode-no-scan-convert" ||
	  target == "saturation-no-scan-convert" ||
	  target == "rf-no-scan-convert"))
    {
      writer = vtkStructuredGridWriter::New();
      vtkStructuredGridWriter* writer_t = dynamic_cast<vtkStructuredGridWriter*>( writer );
      writer_t->SetFileName( out_file.c_str() );
      //writer_t->SetFileTypeToASCII();
      //writer_t->SetFileTypeToBinary();
    }
    else if( extension == ".vtk" )
    {
      writer = vtkStructuredPointsWriter::New();
      vtkStructuredPointsWriter* writer_t = dynamic_cast<vtkStructuredPointsWriter*>( writer );
      writer_t->SetFileName( out_file.c_str() );
      //writer_t->SetFileTypeToASCII();
      //writer_t->SetFileTypeToBinary();
    }
    else
    {
      std::cerr << "Unrecognized extension: " << extension << std::endl;
      return 1;
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
   /*************** nike ***************/
  try
  {
    vtk_vs_reader->SetFilePrefix( in_file.c_str() );

    if     ( target == "bmode" )
      {
      //caster->SetOutputScalarTypeToUnsignedShort();
      //caster->SetInputConnection( vtk_vs_reader->GetOutputPort(3) );
      //writer->SetInputConnection( caster->GetOutputPort() );
      writer->SetInputConnection( vtk_vs_reader->GetOutputPort(3) );
      }
    else if( target == "saturation" )
    {
      writer->SetInputConnection( vtk_vs_reader->GetOutputPort(4) );
    }
    else if( target == "rf" )
    {
      writer->SetInputConnection( vtk_vs_reader->GetOutputPort(5) );
    }
    else if( target == "bmode-no-scan-convert" )
    {
      writer->SetInputConnection( vtk_vs_reader->GetOutputPort(0) );
    }
    else if( target == "saturation-no-scan-convert" )
    {
      writer->SetInputConnection( vtk_vs_reader->GetOutputPort(1) );
    }
    else if( target == "rf-no-scan-convert" )
    {
      writer->SetInputConnection( vtk_vs_reader->GetOutputPort(2) );
    }
    else if( target == "bmode-no-scan-convert-image" )
    {
      usgti->SetInputConnection( vtk_vs_reader->GetOutputPort(0) );
      writer->SetInputConnection( usgti->GetOutputPort() );
    }
    else if( target == "saturation-no-scan-convert-image" )
    {
      usgti->SetInputConnection( vtk_vs_reader->GetOutputPort(1) );
      writer->SetInputConnection( usgti->GetOutputPort() );
    }
    else if( target == "rf-no-scan-convert-image" )
    {
      usgti->SetInputConnection( vtk_vs_reader->GetOutputPort(2) );
      writer->SetInputConnection( usgti->GetOutputPort() );
    }
    else
    {
      cerr << "Unsupported target: " << target << endl;
    }

    Write(writer);

  }
  catch ( std::exception& e )
  {
    std::cerr << "There was a std::exception: "  << e.what() << std::endl;
    return 1;
  }

  return 0;

};



void Write( vtkAlgorithm* writer )
{
  vtkXMLWriter* writer_xml = dynamic_cast<vtkXMLWriter*>(writer);
  vtkWriter* writer_vtk = dynamic_cast<vtkWriter*>(writer);
  vtkImageWriter* writer_image = dynamic_cast<vtkImageWriter*>(writer);

  if( writer_xml )
    writer_xml->Write();
  else if( writer_vtk )
    writer_vtk->Write();
  else if( writer_image )
    writer_image->Write();
  else
    std::cerr << "unknown writer type" << endl;
}
