#include <iostream>
#include <string>
#include <vector>

#include "tclap/CmdLine.h"

#include "itkArchetypeSeriesFileNames.h"
#include "itkChangeInformationImageFilter.h"
#include "itkComplexToModulusImageFilter.h"
#include "itkFrequencyVectorImageFilter.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkImportImageFilter.h"
#include "itkMeanAcrossDirection.h"
#include "itkMultiplyByConstantImageFilter.h"
#include "itkRegionOfInterestImageFilter.h"
#include "itkFFTW1DRealToComplexConjugateImageFilter.h"
#include "itkRecursiveGaussianImageFilter.h"
#include "itkResampleImageFilter.h"
//#include "itkSquareImageFilter.h"
#include "itkVector.h"
#include "itkVisualSonicsImageIOFactory.h"
#include "itkVisualSonicsSeriesReader.h"
#include "itkIntensityWindowingImageFilter.h"

using std::string;
using std::cout;
using std::endl;
using std::cerr;
using std::vector;

// also change itkMeanAcrossDirection.txx itk::Vector< aoeuaoue !!!
typedef float PixelType;
const unsigned int Dimension = 3;
typedef itk::Image< PixelType, Dimension > ImageType;
const unsigned int window_length = 128;
const unsigned int extraction_size = 8; // do not forget to change vector_length in itkMeanAcrossDirection.txx !!!!
//const unsigned int roi_length = 2048; // necessary?
typedef itk::Vector< PixelType, extraction_size > ExtractedVectorType;
typedef itk::Image< ExtractedVectorType, Dimension > ExtractedImageType;
typedef itk::Image< ExtractedVectorType, 1 > ExtractedMeanImageType;

int main(int argc, char ** argv )
{

  TCLAP::CmdLine commandLine( "Calculates the mean reflector spectrum of phantom data for use with the reference phantom method." );

  TCLAP::UnlabeledValueArg< std::string > rdiFile( "rdiFile",
    "Input *.rdi that is the first segment in the to-be concatenated volume.",
    true,
    "",
    "file_seg1.rdi",
    commandLine );

  TCLAP::UnlabeledValueArg< std::string > meanSpectrumFile( "meanSpectrumFile",
    "Output filename to place the mean spectrum image.",
    true,
    "",
    "refSpectrum.mha",
    commandLine );

  try
    {
    commandLine.parse( argc, argv );
    }
  catch( const TCLAP::ArgException &e )
    {
    std::cerr << "Error: " << e.error() << " for arg " << e.argId() << std::endl;
    return 1;
    }

  itk::VisualSonicsImageIOFactory::RegisterOneFactory();
  typedef itk::VisualSonicsSeriesReader< ImageType > ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  typedef itk::ArchetypeSeriesFileNames NameGeneratorType;
  NameGeneratorType::Pointer nameGenerator = NameGeneratorType::New();
  nameGenerator->SetArchetype( rdiFile.getValue().c_str() );
  reader->SetFileNames( nameGenerator->GetFileNames() );
  reader->UpdateOutputInformation();

  //typedef itk::RegionOfInterestImageFilter< ImageType, ImageType > ROIType;
  //ROIType::Pointer roi_filter = ROIType::New();
  //roi_filter->SetInput( reader->GetOutput());

  //ImageType::RegionType in_region = reader->GetOutput()->GetLargestPossibleRegion();

  /*************** frequency vector ***************/
  typedef itk::FrequencyVectorImageFilter< ImageType, ExtractedImageType > FrequencyVectorFilter;
  FrequencyVectorFilter::Pointer freq_vect = FrequencyVectorFilter::New();
  //freq_vect->SetInput( roi_filter->GetOutput() );
  freq_vect->SetInput( reader->GetOutput() );
  freq_vect->SetFrequencyExtractStartIndex( 2 );

  //[>************** mean across direction **************<]
  typedef itk::MeanAcrossDirection< FrequencyVectorFilter::OutputImageType, ExtractedMeanImageType  > MeanAcrossDirectionType;
  MeanAcrossDirectionType::Pointer mean_across_d = MeanAcrossDirectionType::New();
  mean_across_d->SetInput( freq_vect->GetOutput() );

  // Hack to add in the radius information.
  typedef itk::ChangeInformationImageFilter< MeanAcrossDirectionType::OutputImageType > OriginSetterType;
  OriginSetterType::Pointer originSetter = OriginSetterType::New();
  originSetter->SetInput( mean_across_d->GetOutput() );
  const itk::MetaDataDictionary* dict = reader->GetMetaDataDictionaryArray()->at(0);
  typedef const itk::MetaDataObject< double >* MetaDoubleType;
  MetaDoubleType r = dynamic_cast< MetaDoubleType >( (*dict)["Radius"] );
  ExtractedMeanImageType::PointType origin;
  if( r != NULL )
    {
    origin[0] = r->GetMetaDataObjectValue();
    }
  else
    {
    std::cerr << "could not get radius metadata dictionary value!" << std::endl;
    return 1;
    }
  originSetter->ChangeOriginOn();
  originSetter->SetOutputOrigin( origin );

  /*************** writer  ***************/
  typedef itk::ImageFileWriter< MeanAcrossDirectionType::OutputImageType > WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetInput( originSetter->GetOutput() );
  writer->SetFileName( meanSpectrumFile.getValue() );

  try
    {
    writer->Update();
    }
  catch( itk::ExceptionObject & err )
    {
    cerr << "Exception Object caught! " << endl;
    cerr << err << endl;
    return 1;
    }

  //typedef MeanAcrossDirectionType::OutputImageType::PixelType OutFreqType;
  //OutFreqType outfreq;
  //outfreq.SetSize( extraction_size );
  //MeanAcrossDirectionType::OutputImageType::IndexType outfreqIndex;
  //outfreqIndex[0] = 0;
  //cout << "results: " << endl;
  //cout << mean_across_d->GetOutput()->GetPixel( outfreqIndex ) << endl;
  //cout << " at the end: " << endl;
  //outfreqIndex[0] = 30;
  //cout << mean_across_d->GetOutput()->GetPixel( outfreqIndex ) << endl;
  // sometimes the last three numbers are sane -- sometimes not -- WTF?

  return 0;
}
