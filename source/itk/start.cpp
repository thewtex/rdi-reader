#include <iostream>
#include <string>

//#include "itkComplexToImaginaryImageFilter.h"
//#include "itkComplexToRealImageFilter.h"
#include "itkAbsImageFilter.h"
#include "itkAddConstantToImageFilter.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkLog10ImageFilter.h"
//#include "itkMultiplyByConstantImageFilter.h"
//#include "itkRealAndImaginaryToComplexImageFilter.h"
#include "itkRegionOfInterestImageFilter.h"
#include "itkFFTW1DComplexConjugateToRealImageFilter.h"
#include "itkFFTW1DRealToComplexConjugateImageFilter.h"
#include "itkRecursiveGaussianImageFilter.h"
//#include "itkComplexToRealImageFilter.h"

using std::string;
using std::cout;
using std::endl;
using std::cerr;

int main(int argc, char ** argv )
{

  typedef float PixelType;
  const unsigned int Dimension = 3;

  typedef itk::Image< PixelType, Dimension > ImageType;

  typedef itk::ImageFileReader< ImageType > ReaderType;

  ReaderType::Pointer reader = ReaderType::New();

  string in_file = "p3.mhd";
  reader->SetFileName( in_file.c_str() );

  reader->UpdateOutputInformation();

  typedef itk::RegionOfInterestImageFilter< ImageType, ImageType > ROIType;
  ROIType::Pointer roi_filter = ROIType::New();
  roi_filter->SetInput( reader->GetOutput());

  ImageType::RegionType in_region = reader->GetOutput()->GetLargestPossibleRegion();
  ImageType::SizeType size = in_region.GetSize();
  ImageType::IndexType start = in_region.GetIndex();
  ImageType::SpacingType spacing = reader->GetOutput()->GetSpacing();
  size[1] = 2048;
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

  /*************** scan convert  ***************/

  /*************** fft ***************/
  //typedef itk::FFTW1DRealToComplexConjugateImageFilter< PixelType, Dimension > FFT1DFilterType;
  //FFT1DFilterType::Pointer fft1d_filter = FFT1DFilterType::New();
  //fft1d_filter->SetInput( roi_filter->GetOutput() );
  //fft1d_filter->SetDirection(1);

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

  //typedef ComplexToModulusImageFilter


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

  /*************** writer  ***************/
  typedef itk::ImageFileWriter< ImageType > WriterType;
  WriterType::Pointer writer = WriterType::New();
  //writer->SetInput( roi_filter->GetOutput() );
  //writer->SetInput( complex_to_real->GetOutput() );
  //writer->SetInput( ifft1d_filter->GetOutput() );
  writer->SetInput( log->GetOutput() );
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

  return 0;
}
