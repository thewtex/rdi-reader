#include <iostream>
#include <string>
#include <vector>

#include "tclap/CmdLine.h"

#include "itkAbsImageFilter.h"
#include "itkAddConstantToImageFilter.h"
#include "itkBSC.h"
#include "itkComplexToModulusImageFilter.h"
#include "itkFrequencyVectorImageFilter.h"
#include "itkHammingWindowImageFilter.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkImportImageFilter.h"
#include "itkLog10ImageFilter.h"
#include "itkMeanAcrossDirection.h"
#include "itkMultiplyByConstantImageFilter.h"
#include "itkRegionOfInterestImageFilter.h"
#include "itkFFTW1DRealToComplexConjugateImageFilter.h"
#include "itkRecursiveGaussianImageFilter.h"
#include "itkResampleImageFilter.h"
//#include "itkSquareImageFilter.h"
#include "itkVector.h"
#include "itkVectorImage.h"
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
    "Input *.rdi that is the first reference phantom data.",
    true,
    "",
    "ref_file_seg1.rdi",
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

  string in_file = "p4.mhd";
  string rdi_filename = "/mnt/datab/research/Research/in_vivo/pat144/visual_sonics/pat144_seg3";
  //string in_file = "phantom10dB.mhd";
  //string rdi_filename = "/mnt/dataa/visualsonics/@VisualSonics2mat/private/linux64/bin/3mmtop_seg2_10dBtgc";

  itk::VisualSonicsImageIOFactory::RegisterOneFactory();
  typedef itk::VisualSonicsSeriesReader< ImageType > ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( rdiFile.getValue().c_str() );
  reader->UpdateOutputInformation();

  typedef itk::RegionOfInterestImageFilter< ImageType, ImageType > ROIType;
  ROIType::Pointer roi_filter = ROIType::New();
  roi_filter->SetInput( reader->GetOutput());

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
  typedef itk::FrequencyVectorImageFilter< ImageType > FrequencyVectorFilter;
  FrequencyVectorFilter::Pointer freq_vect = FrequencyVectorFilter::New();
  freq_vect->SetInput( roi_filter->GetOutput() );
  freq_vect->SetFrequencyExtractStartIndex( 2 );
  const unsigned int extraction_size = 8; // do not forget to change vector_length in itkMeanAcrossDirection.txx !!!!
  freq_vect->SetFrequencyExtractSize( extraction_size );

  //[>************** mean across direction **************<]
  //typedef itk::MeanAcrossDirection< FrequencyVectorFilter::OutputImageType, itk::VectorImage< PixelType, 1 > > MeanAcrossDirectionType;
  //MeanAcrossDirectionType::Pointer mean_across_d = MeanAcrossDirectionType::New();
  //mean_across_d->SetInput( freq_vect->GetOutput() );

  /*************** bsc ***************/
  typedef itk::BSC< FrequencyVectorFilter::OutputImageType, ImageType > BSCFilterType;
  BSCFilterType::Pointer bsc = BSCFilterType::New();
  bsc->SetInput( freq_vect->GetOutput() );

  /*************** resample backscatter ***************/
  typedef itk::ResampleImageFilter< ImageType, ImageType, PixelType> ResampleFilterType;
  ResampleFilterType::Pointer resample = ResampleFilterType::New();

  //typedef itk::BSplineInterpolateImageFunction< ImageType, PixelType, PixelType > ResampleInterpolatorType;
  //ResampleInterpolatorType::Pointer resample_interpolator  = ResampleInterpolatorType::New();
  ////resample->SetOutputDirection( 1 );
  ////resample_interpolator->UseImageDirectionOn();
  //resample->SetInterpolator( resample_interpolator );
  //cout << "spline order: " << resample_interpolator->GetSplineOrder() << endl;
  //cout << "use image direction : " << resample_interpolator->GetUseImageDirection() << endl;

  resample->SetDefaultPixelValue( 0.0 );

  bsc->UpdateOutputInformation();
  ImageType::SpacingType resample_spacing = bsc->GetOutput()->GetSpacing();
  const ImageType::SizeType& bscSize = bsc->GetOutput()->GetLargestPossibleRegion().GetSize();
  resample_spacing[1] = resample_spacing[1] * (bscSize[1] - 1) / (roi_length - 1 );
  resample->SetOutputSpacing( resample_spacing );

  resample->SetOutputOrigin( bsc->GetOutput()->GetOrigin() );

  ImageType::SizeType resample_size = bscSize;
  resample_size[1] = roi_length;
  resample->SetSize( resample_size );
  resample->SetInput( bsc->GetOutput() );

  typedef itk::IntensityWindowingImageFilter< ImageType > WindowingType;
  WindowingType::Pointer intensity_window = WindowingType::New();
  intensity_window->SetInput( resample->GetOutput() );
  intensity_window->SetWindowMinimum( 0.0 );
  intensity_window->SetWindowMaximum(255.0);
  intensity_window->SetOutputMinimum( 0.0000000001 );
  intensity_window->SetOutputMaximum( 255.0 );

  /*************** b mode  ***************/
  // for p144 0 dB gain
  typedef itk::MultiplyByConstantImageFilter< ImageType, PixelType, ImageType > MultiplyFilterType;
  MultiplyFilterType::Pointer multiply = MultiplyFilterType::New();
  multiply->SetConstant( 3.2 );
  multiply->SetInput( roi_filter->GetOutput() );

  typedef itk::AbsImageFilter< ImageType, ImageType > AbsType;
  AbsType::Pointer abs  = AbsType::New();
  //abs->SetInput( roi_filter->GetOutput());
  abs->SetInput( multiply->GetOutput());

  typedef itk::RecursiveGaussianImageFilter< ImageType, ImageType > GaussType;
  GaussType::Pointer smooth = GaussType::New();
  smooth->SetSigma( 3.6*spacing[1] );
  smooth->SetDirection( 1 );
  smooth->SetInput( abs->GetOutput() );
  smooth->SetNormalizeAcrossScale( false );

  typedef itk::AddConstantToImageFilter< ImageType, PixelType, ImageType > AddCType;
  AddCType::Pointer add_c = AddCType::New();
  add_c->SetConstant( 0.000001 );
  add_c->SetInput( smooth->GetOutput() );

  typedef itk::Log10ImageFilter< ImageType, ImageType > LogType;
  LogType::Pointer log = LogType::New();
  log->SetInput( add_c->GetOutput() );
  //log->SetInput( intensity_window->GetOutput() );


  //[>************** scan convert **************<]
  using namespace visual_sonics;
  typedef float CoordT;
  typedef visual_sonics::VisualSonicsTransform< PixelType, PixelType, CoordT  > VSTransformType;
  vector<PixelType> in_image;
  vector<PixelType> out_image;
  unsigned int transform_rows;
  unsigned int transform_cols;
  CoordT delta_x;
  CoordT delta_y;
  vector<CoordT> image_x;
  vector<CoordT> image_y;
  visual_sonics::rdiParser rdi_parser( rdi_filename );
  rdiParserData* rpd = new rdiParserData;
  *rpd = rdi_parser.parse();
  rpd->its_image_acquisition_size = roi_length * 2;

  VSTransformType* vs_transform = new VSTransformType( in_image,
    out_image,
    transform_rows,
    transform_cols,
    delta_x,
    delta_y,
    image_x,
    image_y,
    rpd,
    false);
  vs_transform->set_outside_bounds_value( 0.0 );
  //vs_transform->set_outside_bounds_value( -4.0 );
  vs_transform->set_do_calc_coords( true );

  log->Update();
  ImageType::Pointer transform_target = log->GetOutput();
  //resample->Update();
  //ImageType::Pointer transform_target = resample->GetOutput();
  //intensity_window->Update();
  //ImageType::Pointer transform_target = intensity_window->GetOutput();
  unsigned int cols = size[0] ;
  unsigned int rows = size[1];
  in_image.resize( cols*rows );

  fill_in_image( transform_target, in_image, 0, cols, rows );
  vs_transform->transform();



  /*************** the import image filter  ***************/
  typedef itk::ImportImageFilter< PixelType, Dimension > ImportFilterType;
  ImportFilterType::Pointer import_filter = ImportFilterType::New();
  ImportFilterType::SizeType import_size;
  import_size[0] = transform_cols;
  import_size[1] = transform_rows;
  import_size[2] = size[2];

  ImportFilterType::IndexType import_start;
  start.Fill(0);
  ImportFilterType::RegionType import_region;
  import_region.SetIndex( import_start );
  import_region.SetSize( import_size );
  import_filter->SetRegion( import_region );
  double origin[ Dimension ];
  origin[0] = 0.0;
  origin[1] = 0.0;
  origin[2] = 0.0;
  import_filter->SetOrigin( origin );
  double import_spacing[ Dimension ];
  import_spacing[0] = static_cast<double> ( delta_x );
  import_spacing[1] = static_cast<double> ( delta_y );
  import_spacing[2] = static_cast<double> ( rpd->its_rf_mode_3d_scan_distance / rpd->its_image_frames );
  import_filter->SetSpacing( import_spacing );



  const unsigned int numberOfPixels = import_size[0]*import_size[1]*import_size[2];

  PixelType* out_image_full = new PixelType[numberOfPixels];
  if( ! out_image_full )
    {
    cerr << "memory allocation error." << endl;
    return 1;
    }


  vs_transform->set_do_calc_coords( false );
  for( unsigned int i = 0 ; i < size[2]; i++ )
    {
    fill_in_image( transform_target, in_image, i, cols, rows );
    vs_transform->transform();
    fill_out_image( out_image, out_image_full, i, transform_cols, transform_rows );
    }


  import_filter->SetImportPointer( out_image_full, numberOfPixels, true );


  /*************** writer  ***************/
  typedef itk::ImageFileWriter< ImageType > WriterType;
  //typedef itk::ImageFileWriter< FrequencyVectorFilter::OutputImageType > WriterType;
  //typedef itk::ImageFileWriter< MeanAcrossDirectionType::OutputImageType > WriterType;
  WriterType::Pointer writer = WriterType::New();
  //writer->SetInput( window->GetOutput() );
  //writer->SetInput( complex_to_real->GetOutput() );
  //writer->SetInput( ifft1d_filter->GetOutput() );
  //writer->SetInput( log->GetOutput() );
  writer->SetInput( import_filter->GetOutput() );
  //writer->SetInput( modulus->GetOutput() );
  //writer->SetInput( square->GetOutput() );
  //writer->SetInput( freq_vect->GetOutput() );
  //writer->SetInput( mean_across_d->GetOutput() );
  //writer->SetInput( bsc->GetOutput() );
  //writer->SetInput( resample->GetOutput() );
  writer->SetFileName( "out.mhd" ) ;

  try
    {
    //freq_vect->UpdateOutputInformation();
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

  delete rpd;
  delete vs_transform;

  return 0;
}
