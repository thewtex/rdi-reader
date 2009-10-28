/**
 * @file vs-convert.cpp
 * @brief command line converter
 * @date 2009-07-23
 */

#include <exception>
#include <fstream>
#include <memory>
using namespace std;

#include "itkExtractImageFilter.h"
#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"

#include "metaCommand.h"

#include "itkVisualSonicsImageIO.h"
#include "itkVisualSonicsImageIOFactory.h"

/**
 * @brief hold the results of command line argument parsing.
 */
struct Args
{
  string in_file;
  string out_file;
  int frame_num;

  Args():
    frame_num(-1)
  {};
};

Args parse_args( int argc, char* argv[] );

int  convert_frame( const Args& args );

int  convert_all( const Args& args );

int main(int argc, char* argv[])
{
  Args args = parse_args( argc, argv );

  // An alternative to the below would be to
  //
  //typedef itk::VisualSonicsImageIO  ImageIOType;
  //ImageIOType::Pointer rfdio = ImageIOType::New();
  //reader->SetImageIO( rfdio );
  //
  // There is also the plugin mechanism, see
  // http://www.itk.org/Wiki/Plugin_IO_mechanisms
  itk::VisualSonicsImageIOFactory::RegisterOneFactory();

  try
    {
    if( args.frame_num > 0 )
      {
      return convert_frame( args );
      }
    else
      {
      return convert_all( args );
      }
    }
  catch ( itk::ExceptionObject& e )
    {
    cerr << "Error: " << e << endl;
    return EXIT_FAILURE;
    }
  catch (ifstream::failure& e)
    {
    cerr << "Error: failure opening/reading file." << endl;
    return EXIT_FAILURE;
    }
  catch (std::exception& e)
    {
    std::cerr << "Error: " << e.what() << std::endl;
    return EXIT_FAILURE;
    }

  return EXIT_FAILURE;
}

Args parse_args( int argc, char* argv[] )
{
  Args args;

  MetaCommand command;

  command.AddField("in_file", "Input filepath.", MetaCommand::STRING, true);

  command.SetOption("out_file", "o", false, "Output filepath.");
  command.SetOptionLongTag("out_file", "output");
  command.AddOptionField("out_file", "out_file", MetaCommand::STRING, true);

  command.SetOption("frame_num", "f", false, "Requested frame number; if not specified, all frames are obtained.  Counts from 1.");
  command.SetOptionLongTag("frame_num", "frame");
  command.AddOptionField("frame_num", "num", MetaCommand::INT, true);

  command.SetDescription("Read an VisualSonics file from the Seimens machine and convert it into an ITK supported image format");

  if( !command.Parse(argc, argv) )
    {
    throw logic_error( "Could not parse command line arguments." );
    }

  // -h
  if( !command.GetOptionWasSet("in_file") )
    throw runtime_error( "Input file was not specified." );

  if(command.GetOptionWasSet("frame_num"))
    args.frame_num = command.GetValueAsInt("frame_num", "num");

  args.in_file = command.GetValueAsString("in_file");

  //set output file name
  if( !command.GetOptionWasSet("out_file") )
    {
    // truncate the in_file extension
    std::string file_base = args.in_file;
    size_t file_base_l = file_base.length();
    if(file_base_l > 4)
      {
      if(!args.in_file.compare(file_base_l - 4, 1, "."))
	file_base = file_base.substr(0, file_base_l - 4);
      }
    args.out_file = file_base + ".mhd";
    }
  else
    args.out_file = command.GetValueAsString("out_file", "out_file");

  return args;
}


int  convert_frame( const Args& args )
{
  typedef signed short  PixelType;
  typedef itk::Image< PixelType, 3 >	InputImageType;
  typedef itk::Image< PixelType, 2 >	OutputImageType;

  typedef itk::ImageFileReader< InputImageType > ReaderType;
  typedef itk::ImageFileWriter< OutputImageType > WriterType;

  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();

  reader->SetFileName( args.in_file.c_str() );
  writer->SetFileName( args.out_file.c_str() );

  typedef itk::ExtractImageFilter< InputImageType, OutputImageType > ExtractFilterType;
  ExtractFilterType::Pointer extractor = ExtractFilterType::New();

  reader->UpdateOutputInformation();
  InputImageType::RegionType in_region =
    reader->GetOutput()->GetLargestPossibleRegion();
  InputImageType::SizeType size = in_region.GetSize();
  if( static_cast<int>( size[2] ) < args.frame_num - 1 ||
      args.frame_num < 0 )
    throw runtime_error("Requested frame number was greater than the frames available.");
  size[2] = 0;
  InputImageType::IndexType index = in_region.GetIndex();
  index[2] = args.frame_num - 1;
  InputImageType::RegionType desired_region;
  desired_region.SetSize( size );
  desired_region.SetIndex( index );
  extractor->SetExtractionRegion( desired_region );

  extractor->SetInput( reader->GetOutput() );
  writer->SetInput( extractor->GetOutput() );

  writer->Update();

  return EXIT_SUCCESS;
}


int  convert_all( const Args& args )
{
  typedef signed short  PixelType;
  typedef itk::Image< PixelType, 3 >	ImageType;

  typedef itk::ImageFileReader< ImageType > ReaderType;
  typedef itk::ImageFileWriter< ImageType > WriterType;

  ReaderType::Pointer reader = ReaderType::New();
  WriterType::Pointer writer = WriterType::New();

  reader->SetFileName( args.in_file.c_str() );
  writer->SetFileName( args.out_file.c_str() );

  writer->SetInput( reader->GetOutput() );

  writer->Update();

  return EXIT_SUCCESS;
}