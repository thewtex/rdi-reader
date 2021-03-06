#include <iostream>
#include <string>
#include <vector>

#include "tclap/CmdLine.h"

#include "itkArchetypeSeriesFileNames.h"
#include "itkBSC.h"
#include "itkComplexToModulusImageFilter.h"
#include "itkFrequencyVectorImageFilter.h"
#include "itkHammingWindowImageFilter.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkLog10ImageFilter.h"
#include "itkMeanAcrossDirection.h"
#include "itkMultiplyByConstantImageFilter.h"
#include "itkRegionOfInterestImageFilter.h"
#include "itkResampleRThetaToCartesianImageFilter.h"
#include "itkFFTW1DRealToComplexConjugateImageFilter.h"
#include "itkRecursiveGaussianImageFilter.h"
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
typedef itk::Image< PixelType, 1 > ReferenceBSCImageType;
typedef itk::Image< unsigned char, Dimension > OutputImageType;

int main(int argc, char ** argv )
{
  const unsigned int window_length = 128;
  //const unsigned int roi_length = 2048; // necessary?
  typedef itk::Vector< PixelType, window_length > VectorType;

  TCLAP::CmdLine commandLine( "Calculates an integrated absolute backscatter coefficient image from a set of VisualSonics RF volumes." );

  TCLAP::UnlabeledValueArg< std::string > rdiFile( "rdiFile",
    "Input *.rdi that is the first segment in the to-be concatenated volume.",
    true,
    "",
    "file_seg1.rdi",
    commandLine );

  TCLAP::UnlabeledValueArg< std::string > refFile( "refFile",
    "Input mean spectrum file that is the first reference phantom data made with 'phantom-spectrum'.",
    true,
    "",
    "ref_spec.mha",
    commandLine );

  TCLAP::UnlabeledValueArg< std::string > refBSC( "refBSC",
    "'Image' with the reference phantom BSC versus frequency.'",
    true,
    "",
    "ref_bsc.mha",
    commandLine );

  TCLAP::UnlabeledValueArg< std::string > bscFile( "bscFile",
    "Output filename tot place the bsc image.",
    true,
    "",
    "ibsc.mha",
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

  // Reference phantom spectrum reader
  typedef itk::ImageFileReader< ExtractedMeanImageType > ReferenceReaderType;
  ReferenceReaderType::Pointer refReader = ReferenceReaderType::New();
  refReader->SetFileName( refFile.getValue() );

  // Reference phantom bsc reader
  typedef itk::ImageFileReader< ReferenceBSCImageType > ReferenceBSCReaderType;
  ReferenceBSCReaderType::Pointer refBSCReader = ReferenceBSCReaderType::New();
  refBSCReader->SetFileName( refBSC.getValue() );

  typedef itk::RegionOfInterestImageFilter< ImageType, ImageType > ROIType;
  ROIType::Pointer roi_filter = ROIType::New();
  roi_filter->SetInput( reader->GetOutput() );

  ImageType::RegionType in_region = reader->GetOutput()->GetLargestPossibleRegion();
  ImageType::SizeType size = in_region.GetSize();
  ImageType::IndexType start = in_region.GetIndex();
  ImageType::SpacingType spacing = reader->GetOutput()->GetSpacing();
  //size[1] = window_length;
  //size[1] = roi_length;
  //size[2] = 4;
  //start[2] = 43;
  //start[2] = 0;
  ImageType::RegionType desired_region;
  desired_region.SetSize( size );
  desired_region.SetIndex( start );
  roi_filter->SetRegionOfInterest( desired_region );

  /*************** window ***************/
  //typedef itk::HammingWindowImageFilter< ImageType, ImageType > WindowType;
  //WindowType::Pointer window = WindowType::New();
  //window->SetDirection(0);
  //window->SetInput( roi_filter->GetOutput() );

  /*************** fft ***************/
  //typedef itk::FFTW1DRealToComplexConjugateImageFilter< PixelType, Dimension > FFT1DFilterType;
  //FFT1DFilterType::Pointer fft1d_filter = FFT1DFilterType::New();
  //fft1d_filter->SetInput( window->GetOutput() );
  //fft1d_filter->SetDirection(0);
  //typedef FFT1DFilterType::OutputImageType ComplexType;

  //typedef itk::ComplexToModulusImageFilter< ComplexType, ImageType > ModulusFilter;
  //ModulusFilter::Pointer modulus = ModulusFilter::New();
  //modulus->SetInput( fft1d_filter->GetOutput() );

  //typedef itk::SquareImageFilter < ImageType, ImageType > SquareFilter;
  //SquareFilter::Pointer square = SquareFilter::New();
  //square->SetInput( modulus->GetOutput() );

  /*************** frequency vector ***************/
  typedef itk::FrequencyVectorImageFilter< ImageType, ExtractedImageType > FrequencyVectorFilter;
  FrequencyVectorFilter::Pointer freq_vect = FrequencyVectorFilter::New();
  freq_vect->SetInput( roi_filter->GetOutput() );
  freq_vect->SetFrequencyExtractStartIndex( 2 );

  /*************** bsc ***************/
  typedef itk::BSC< FrequencyVectorFilter::OutputImageType, ExtractedMeanImageType, ReferenceBSCImageType, ImageType > BSCFilterType;
  BSCFilterType::Pointer bsc = BSCFilterType::New();
  bsc->SetInput( freq_vect->GetOutput() );
  bsc->SetReferenceSpectrum( refReader->GetOutput() );
  bsc->SetReferenceBSC( refBSCReader->GetOutput() );
  const itk::MetaDataDictionary* dict = reader->GetMetaDataDictionaryArray()->at(0);
  typedef const itk::MetaDataObject< double >* MetaDoubleType;
  MetaDoubleType r = dynamic_cast< MetaDoubleType >( (*dict)["Radius"] );
  double radius;
  if( r != NULL )
    {
    radius = r->GetMetaDataObjectValue();
    }
  else
    {
    std::cerr << "could not get radius metadata dictionary value!" << std::endl;
    return 1;
    }
  bsc->SetSampleRadius( radius );

  typedef itk::IntensityWindowingImageFilter< ImageType > WindowingType;
  WindowingType::Pointer intensity_window = WindowingType::New();
  intensity_window->SetInput( bsc->GetOutput() );
  intensity_window->SetWindowMinimum( 0.0 );
  intensity_window->SetWindowMaximum( 255.0 );
  intensity_window->SetOutputMinimum( 0.0000000001 );
  intensity_window->SetOutputMaximum( 255.0 );

  typedef itk::Log10ImageFilter< ImageType, ImageType > LogType;
  LogType::Pointer log = LogType::New();
  log->SetInput( intensity_window->GetOutput() );

  // scan convert
  typedef itk::ResampleRThetaToCartesianImageFilter< ImageType, ImageType, float > ScanConvertType;
  ScanConvertType::Pointer scanConvert = ScanConvertType::New();
  log->GetOutput()->SetMetaDataDictionary( *dict );
  scanConvert->SetInput( log->GetOutput() );
  scanConvert->SetDefaultPixelValue( -5.0 );

  typedef itk::IntensityWindowingImageFilter< ImageType, OutputImageType > OutputWindowingType;
  OutputWindowingType::Pointer output_intensity_window = OutputWindowingType::New();
  output_intensity_window->SetInput( scanConvert->GetOutput() );
  output_intensity_window->SetWindowMinimum( -3.5 );
  output_intensity_window->SetWindowMaximum( 0.5 );
  //intensity_window->SetOutputMinimum( 0.0000000001 );
  //intensity_window->SetOutputMaximum( 255.0 );

  /*************** writer  ***************/
  //typedef itk::ImageFileWriter< ImageType > WriterType;
  typedef itk::ImageFileWriter< OutputImageType > WriterType;
  //typedef itk::ImageFileWriter< FrequencyVectorFilter::OutputImageType > WriterType;
  //typedef itk::ImageFileWriter< MeanAcrossDirectionType::OutputImageType > WriterType;
  WriterType::Pointer writer = WriterType::New();
  //writer->SetInput( scanConvert->GetOutput() );
  writer->SetInput( output_intensity_window->GetOutput() );
  //writer->SetInput( resample->GetOutput() );
  writer->SetFileName( bscFile.getValue() ) ;

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

  return 0;
}
