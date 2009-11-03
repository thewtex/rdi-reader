/**
 * @file vs-convert-b-mode.cxx
 * @brief Test VisualSonics conversion by making a B-Mode.
 * @author Matthew McCormick (thewtex) <matt@mmmccormick.com>
 * @date 2009-10-26
 */

#include <iostream>
using namespace std;

#include "itkArchetypeSeriesFileNames.h"
#include "itkImage.h"
#include "itkImageFileWriter.h"
#include "itkSimpleFilterWatcher.h"

#include "itkBModeImageFilter.h"

#include "itkVisualSonicsImageIO.h"
#include "itkVisualSonicsImageIOFactory.h"

#include "itkVisualSonicsSeriesReader.h"

int main( int argc, char* argv[] )
{
  // usage: vs-convert-b-mode-series input-archetype.rdi output.mhd
  if( argc != 3 )
    return EXIT_FAILURE;

  itk::VisualSonicsImageIOFactory::RegisterOneFactory();

  typedef signed short InputPixelType;
  typedef unsigned short OutputPixelType;
  typedef itk::Image< InputPixelType, 3 > InputImageType;
  typedef itk::Image< OutputPixelType, 3 > OutputImageType;

  typedef itk::VisualSonicsSeriesReader< InputImageType > ReaderType;
  typedef itk::BModeImageFilter< InputImageType, OutputImageType > BModeType;
  typedef itk::ImageFileWriter< OutputImageType > WriterType;

  ReaderType::Pointer reader = ReaderType::New();
  BModeType::Pointer  bmode  = BModeType::New();
  WriterType::Pointer writer = WriterType::New();

  typedef itk::ArchetypeSeriesFileNames NameGeneratorType;
  NameGeneratorType::Pointer nameGenerator = NameGeneratorType::New();
  nameGenerator->SetArchetype( argv[1] );

  reader->SetFileNames( nameGenerator->GetFileNames() );

  writer->SetFileName( argv[2] );
  writer->SetNumberOfStreamDivisions( 20 );

  bmode->SetInput( reader->GetOutput() );
  writer->SetInput( bmode->GetOutput() );

  itk::SimpleFilterWatcher watcher(writer, "stream writing");

  try
    {
    writer->Update();
    }
  catch ( itk::ExceptionObject& e )
    {
    cerr << "Error: " << e << endl;
    return EXIT_FAILURE;
    }
  catch (std::exception& e)
    {
    std::cerr << "Error: " << e.what() << std::endl;
    return EXIT_FAILURE;
    }
  return EXIT_SUCCESS;
}
