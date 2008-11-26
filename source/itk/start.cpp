#include <iostream>
#include <string>
#include <vector>
#include <cmath>

//#include "itkComplexToImaginaryImageFilter.h"
//#include "itkComplexToRealImageFilter.h"
#include "itkAbsImageFilter.h"
#include "itkAddConstantToImageFilter.h"
#include "itkComplexToModulusImageFilter.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkImportImageFilter.h"
#include "itkLog10ImageFilter.h"
//#include "itkMultiplyByConstantImageFilter.h"
//#include "itkRealAndImaginaryToComplexImageFilter.h"
#include "itkRegionOfInterestImageFilter.h"
//#include "itkFFTW1DComplexConjugateToRealImageFilter.h"
#include "itkFFTW1DRealToComplexConjugateImageFilter.h"
#include "itkRecursiveGaussianImageFilter.h"
#include "itkVector.h"


#include "common/VisualSonicsTransform.h"
#include "common/spirit_parse/rdiParserData.h"
#include "common/spirit_parse/rdiParser.h"
#include "common/MetadataReaderBase.h"
#include "common/sized_ints.h"

using std::string;
using std::cout;
using std::endl;
using std::cerr;
using std::vector;

  typedef float PixelType;
  const unsigned int Dimension = 3;
  typedef itk::Image< PixelType, Dimension > ImageType;

void fill_in_image( const ImageType* target, vector<PixelType>& in_image, const unsigned int & frame, const unsigned int & cols, const unsigned int& rows )
{
  ImageType::IndexType target_index;
  target_index[0] = 0;
  target_index[1] = 0;
  target_index[2] = frame;
  for( unsigned int i = 0; i < cols; i++ )
    {
    for( unsigned int j = 0; j < rows; j++ )
      {
      in_image[ j + i*rows ] = target->GetPixel(target_index) ;
//      in_image[ j*cols + i ] = target->GetPixel(target_index) ;
      target_index[1] += 1;
      }
    target_index[1] = 0;
    target_index[0] += 1;
    }
}

void fill_out_image( const vector< PixelType>& out_image, PixelType* out_image_full, const unsigned int & frame, const unsigned int & cols, const unsigned int& rows )
{
  PixelType* offset = out_image_full + frame*rows*cols;
  for( unsigned int i = 0; i < cols; i++ )
    {
    for( unsigned int j = 0; j < rows; j++ )
      {
      *(offset + i +j*cols ) = out_image[ j + i*rows ];
      }
    }
}

int main(int argc, char ** argv )
{


  typedef itk::ImageFileReader< ImageType > ReaderType;
  const unsigned int window_length = 128;
  typedef itk::Vector< PixelType, window_length > VectorType;

  ReaderType::Pointer reader = ReaderType::New();

  //string in_file = "p3.mhd";
  //string rdi_filename = "/mnt/datab/research/Research/in_vivo/pat143/visual_sonics/pat143_seg3";
  string in_file = "phantom0dB.mhd";
  string rdi_filename = "/mnt/dataa/visualsonics/@VisualSonics2mat/private/linux64/bin/3mmtop_seg2_0dBtgc";
  reader->SetFileName( in_file.c_str() );

  reader->UpdateOutputInformation();

  float pi = 3.14159265;
  VectorType hamming_win;
  for( unsigned int i = 0; i<window_length; i++ )
    {
    hamming_win[i] = 0.53836 - 0.46164 * ::cos(2*pi*i/(window_length-1) ) ;

    }
  cout << hamming_win << endl;
  typedef itk::RegionOfInterestImageFilter< ImageType, ImageType > ROIType;
  ROIType::Pointer roi_filter = ROIType::New();
  roi_filter->SetInput( reader->GetOutput());

  ImageType::RegionType in_region = reader->GetOutput()->GetLargestPossibleRegion();
  ImageType::SizeType size = in_region.GetSize();
  ImageType::IndexType start = in_region.GetIndex();
  ImageType::SpacingType spacing = reader->GetOutput()->GetSpacing();
  size[1] = window_length;
  size[2] = 4;
  start[2] = 43;
  ImageType::RegionType desired_region;
  desired_region.SetSize( size );
  desired_region.SetIndex( start );
  roi_filter->SetRegionOfInterest( desired_region );

  /*************** b mode  ***************/
  typedef itk::AbsImageFilter< ImageType, ImageType > AbsType;
  AbsType::Pointer abs  = AbsType::New();
  abs->SetInput( roi_filter->GetOutput());

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


  /*************** fft ***************/
  typedef itk::FFTW1DRealToComplexConjugateImageFilter< PixelType, Dimension > FFT1DFilterType;
  FFT1DFilterType::Pointer fft1d_filter = FFT1DFilterType::New();
  fft1d_filter->SetInput( roi_filter->GetOutput() );
  fft1d_filter->SetDirection(1);
  typedef FFT1DFilterType::OutputImageType ComplexType;

  //typedef itk::ComplexToRealImageFilter< FFT1DFilterType::OutputImageType,
  //ImageType > ComplexToRealType;
  //ComplexToRealType::Pointer complex_to_real = ComplexToRealType::New();
  //complex_to_real->SetInput( fft1d_filter->GetOutput() );

  //typedef itk::ComplexToImaginaryImageFilter< FFT1DFilterType::OutputImageType,
  //ImageType > ComplexToImaginaryType;
  //ComplexToImaginaryType::Pointer complex_to_imag = ComplexToImaginaryType::New();
  //complex_to_imag->SetInput( fft1d_filter->GetOutput() );

  //typedef itk::MultiplyByConstantImageFilter< ImageType, ImageType > NegateType;
  //NegateType::Pointer negate = NegateType::New();
  //negate->SetInput( complex_to_real->GetOutput() );
  //negate->SetConstant( -1 );

  typedef itk::ComplexToModulusImageFilter< ComplexType, ImageType > ModulusFilter;
  ModulusFilter::Pointer modulus = ModulusFilter::New();
  modulus->SetInput( fft1d_filter->GetOutput() );


  //typedef itk::RealAndImaginaryToComplexImageFilter< PixelType, PixelType, PixelType, Dimension> RealImagToComplexType;
  //RealImagToComplexType::Pointer real_imag_to_complex = RealImagToComplexType::New();
  //real_imag_to_complex->SetInput1( complex_to_imag->GetOutput() );
  //real_imag_to_complex->SetInput2( negate->GetOutput() );


  //typedef itk::FFTW1DComplexConjugateToRealImageFilter< PixelType, Dimension > IFFT1DFilterType;
  //IFFT1DFilterType::Pointer ifft1d_filter = IFFT1DFilterType::New();
  //ifft1d_filter->SetInput( fft1d_filter->GetOutput() );
  //ifft1d_filter->SetDirection(1);
  //

  //typedef itk::ComplexToRealImageFilter< FFT1DFilterType::OutputImageType,
  //ImageType > ComplexToRealType;
  //ComplexToRealType::Pointer complex_to_real = ComplexToRealType::New();
  //complex_to_real->SetInput( fft1d_filter->GetOutput() );
  //

  //[>************** scan convert **************<]
  //using namespace visual_sonics;
  //typedef float CoordT;
  //typedef visual_sonics::VisualSonicsTransform< PixelType, PixelType, CoordT  > VSTransformType;
  //vector<PixelType> in_image;
  //vector<PixelType> out_image;
  //unsigned int transform_rows;
  //unsigned int transform_cols;
  //CoordT delta_x;
  //CoordT delta_y;
  //vector<CoordT> image_x;
  //vector<CoordT> image_y;
  //visual_sonics::rdiParser rdi_parser( rdi_filename );
  //rdiParserData* rpd = new rdiParserData;
  //*rpd = rdi_parser.parse();
  //rpd->its_image_acquisition_size = 2048 * 2;

  //VSTransformType* vs_transform = new VSTransformType( in_image,
    //out_image,
    //transform_rows,
    //transform_cols,
    //delta_x,
    //delta_y,
    //image_x,
    //image_y,
    //rpd,
    //false);
  //vs_transform->set_outside_bounds_value( 0.0 );
  //vs_transform->set_do_calc_coords( true );

  //log->Update();
  //ImageType::Pointer transform_target = log->GetOutput();
  //unsigned int cols = size[0] ;
  //unsigned int rows = size[1];
  //in_image.resize( cols*rows );

  //fill_in_image( transform_target, in_image, 0, cols, rows );
  //vs_transform->transform();



  //[>************** the import image filter  **************<]
  //typedef itk::ImportImageFilter< PixelType, Dimension > ImportFilterType;
  //ImportFilterType::Pointer import_filter = ImportFilterType::New();
  //ImportFilterType::SizeType import_size;
  //import_size[0] = transform_cols;
  //import_size[1] = transform_rows;
  //import_size[2] = size[2];

  //ImportFilterType::IndexType import_start;
  //start.Fill(0);
  //ImportFilterType::RegionType import_region;
  //import_region.SetIndex( import_start );
  //import_region.SetSize( import_size );
  //import_filter->SetRegion( import_region );
  //double origin[ Dimension ];
  //origin[0] = 0.0;
  //origin[1] = 0.0;
  //origin[2] = 0.0;
  //import_filter->SetOrigin( origin );
  //double import_spacing[ Dimension ];
  //import_spacing[0] = static_cast<double> ( delta_x );
  //import_spacing[1] = static_cast<double> ( delta_y );
  //import_spacing[2] = static_cast<double> ( rpd->its_rf_mode_3d_scan_distance / rpd->its_image_frames );
  //import_filter->SetSpacing( import_spacing );



  //const unsigned int numberOfPixels = import_size[0]*import_size[1]*import_size[2];

  //PixelType* out_image_full = new PixelType[numberOfPixels];
  //if( ! out_image_full )
    //{
    //cerr << "memory allocation error." << endl;
    //return 1;
    //}


  //vs_transform->set_do_calc_coords( false );
  //for( unsigned int i = 0 ; i < size[2]; i++ )
    //{
    //fill_in_image( transform_target, in_image, i, cols, rows );
    //vs_transform->transform();
    //fill_out_image( out_image, out_image_full, i, transform_cols, transform_rows );
    //}


  //import_filter->SetImportPointer( out_image_full, numberOfPixels, true );


  /*************** writer  ***************/
  typedef itk::ImageFileWriter< ImageType > WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetInput( roi_filter->GetOutput() );
  //writer->SetInput( complex_to_real->GetOutput() );
  //writer->SetInput( ifft1d_filter->GetOutput() );
  //writer->SetInput( log->GetOutput() );
  //writer->SetInput( import_filter->GetOutput() );
  //writer->SetInput( modulus->GetOutput() );
  writer->SetFileName( "out.mhd" ) ;

  try
    {
    //fft1d_filter->Update();
    writer->Update();
    }
  catch( itk::ExceptionObject & err )
    {
    cerr << "Exception Object caught! " << endl;
    cerr << err << endl;
    return 1;
    }

  //delete rpd;
  //delete vs_transform;

  return 0;
}
