/**
 * @file vs-convert.cpp
 * @brief command line converter
 * @date 2009-07-23
 */

#include <exception>
#include <fstream>
#include <memory>
using namespace std;

#include "itkArchetypeSeriesFileNames.h"
#include "itkExtractImageFilter.h"
#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkImageIOBase.h"
#include "itkImageIOFactory.h"

#include "metaCommand.h"

#include "itkVisualSonicsImageIO.h"
#include "itkVisualSonicsImageIOFactory.h"
#include "itkVisualSonicsSeriesReader.h"

/**
 * @brief hold the results of command line argument parsing.
 */
struct Args
{
  string in_file;
  string out_file;
  int frame_num;
  double start_of_aquisition_speed_of_sound;
  double acquisition_speed_of_sound;
  double speed_of_sound;

  Args():
    frame_num(-1),
    start_of_aquisition_speed_of_sound( -1. ),
    acquisition_speed_of_sound( -1. ),
    speed_of_sound( -1. )
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

  command.SetOption( "start_of_aquisition_speed_of_sound", "s", false, "Set the assumed speed of sound from the start of the transducer face to the start of data aquisition. [m/s]" );
  command.SetOptionLongTag( "start_of_aquisition_speed_of_sound", "soa-sos" );
  command.AddOptionField( "start_of_aquisition_speed_of_sound", "start_of_aquisition_speed_of_sound", MetaCommand::FLOAT, true );

  command.SetOption( "acquisition_speed_of_sound", "a", false, "Set the assumed speed of sound in the medium. [m/s]" );
  command.SetOptionLongTag( "acquisition_speed_of_sound", "a-sos" );
  command.AddOptionField( "acquisition_speed_of_sound", "acquisition_speed_of_sound", MetaCommand::FLOAT, true );

  command.SetOption( "speed_of_sound", "c", false, "Set the assumed speed of sound.  This sets the StartOfAquisitionSpeedOfSound and the AcquisitionSpeedOfSound." );
  command.SetOptionLongTag( "speed_of_sound", "sos" );
  command.AddOptionField( "speed_of_sound", "speed_of_sound", MetaCommand::FLOAT, true );

  command.SetDescription("Read an VisualSonics Raw Data file and convert it into an ITK supported image format");

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
    args.out_file = file_base + ".nrrd";
    }
  else
    args.out_file = command.GetValueAsString("out_file", "out_file");

  if( command.GetOptionWasSet( "start_of_aquisition_speed_of_sound" ) )
    args.start_of_aquisition_speed_of_sound = command.GetValueAsFloat( "start_of_aquisition_speed_of_sound", "start_of_aquisition_speed_of_sound" );

  if( command.GetOptionWasSet( "acquisition_speed_of_sound" ) )
    args.acquisition_speed_of_sound = command.GetValueAsFloat( "acquisition_speed_of_sound", "acquisition_speed_of_sound" );

  if( command.GetOptionWasSet( "speed_of_sound" ) )
    args.speed_of_sound = command.GetValueAsFloat( "speed_of_sound", "speed_of_sound" );


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

  typedef itk::VisualSonicsSeriesReader< InputImageType > VisualSonicsReaderType;
  VisualSonicsReaderType::Pointer vsReader = VisualSonicsReaderType::New();
  if( vsReader->GetImageIO()->CanReadFile( args.in_file.c_str() ) )
    {
    typedef itk::ArchetypeSeriesFileNames NameGeneratorType;
    NameGeneratorType::Pointer nameGenerator = NameGeneratorType::New();
    nameGenerator->SetArchetype( args.in_file.c_str() );
    vsReader->SetFileNames( nameGenerator->GetFileNames() );
    itk::VisualSonicsImageIO * vs_image_io = dynamic_cast< itk::VisualSonicsImageIO * >( vsReader->GetImageIO() );
    if ( args.speed_of_sound > 0. )
      vs_image_io->SetSpeedOfSound( args.speed_of_sound );
    if ( args.start_of_aquisition_speed_of_sound > 0. )
      vs_image_io->SetStartOfAquisitionSpeedOfSound( args.start_of_aquisition_speed_of_sound );
    if ( args.acquisition_speed_of_sound > 0. )
      vs_image_io->SetAcquisitionSpeedOfSound( args.acquisition_speed_of_sound );
    extractor->SetInput( vsReader->GetOutput() );
    }
  else
    {
    extractor->SetInput( reader->GetOutput() );
    }

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

  typedef itk::VisualSonicsSeriesReader< ImageType > VisualSonicsReaderType;
  VisualSonicsReaderType::Pointer vsReader = VisualSonicsReaderType::New();
  bool isVisualSonicsFile = vsReader->GetImageIO()->CanReadFile( args.in_file.c_str() ); 
  if( isVisualSonicsFile )
    {
    typedef itk::ArchetypeSeriesFileNames NameGeneratorType;
    NameGeneratorType::Pointer nameGenerator = NameGeneratorType::New();
    nameGenerator->SetArchetype( args.in_file.c_str() );
    vsReader->SetFileNames( nameGenerator->GetFileNames() );
    itk::VisualSonicsImageIO * vs_image_io = dynamic_cast< itk::VisualSonicsImageIO * >( vsReader->GetImageIO() );
    if ( args.speed_of_sound > 0. )
      vs_image_io->SetSpeedOfSound( args.speed_of_sound );
    if ( args.start_of_aquisition_speed_of_sound > 0. )
      vs_image_io->SetStartOfAquisitionSpeedOfSound( args.start_of_aquisition_speed_of_sound );
    if ( args.acquisition_speed_of_sound > 0. )
      vs_image_io->SetAcquisitionSpeedOfSound( args.acquisition_speed_of_sound );
    writer->SetInput( vsReader->GetOutput() );
    vsReader->UpdateOutputInformation();
    writer->UseInputMetaDataDictionaryOff();
    itk::ImageIOBase::Pointer imageIO = itk::ImageIOFactory::CreateImageIO( args.out_file.c_str(),
      itk::ImageIOFactory::WriteMode );  
    imageIO->SetMetaDataDictionary( vsReader->GetImageIO()->GetMetaDataDictionary() );

    writer->SetImageIO( imageIO );
    }
  else
    {
    writer->SetInput( reader->GetOutput() );
    }

  writer->Update();

  return EXIT_SUCCESS;
}
