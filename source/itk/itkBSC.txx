#ifndef __itkBSC_txx
#define __itkBSC_txx

#include <iostream>
using std::cout;
using std::endl;

#include "itkBSC.h"

#include "itkImageFileReader.h"
#include "itkImageLinearConstIteratorWithIndex.h"
#include "itkImageLinearIteratorWithIndex.h"
#include "itkImageRegionIterator.h"
#include "itkVector.h"

namespace itk
{

template <class TInputImage, class TOutputImage >
BSC<TInputImage,TOutputImage>
::BSC()
: m_Direction(1)
{
  // @todo change to 2 so that the reference can be specified
  this->SetNumberOfRequiredInputs( 1 );

  m_ReferenceReader = ReferenceReaderType::New();
  reference_filename = "phantom10dBfreqs.mhd";
  m_ReferenceReader->SetFileName( reference_filename.c_str() );
  m_ReferenceReader->Update();
}

template <class TInputImage, class TOutputImage >
void
BSC<TInputImage,TOutputImage>
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os,indent);
  os << indent << "Direction: " << m_Direction << std::endl;
  os << std::endl;
}


template <class TInputImage, class TOutputImage >
void
BSC<TInputImage,TOutputImage>
::GenerateData()
{

  // get pointers to the input and output
  typename TInputImage::ConstPointer  inputPtr  = this->GetInput();
  typename TOutputImage::Pointer      outputPtr = this->GetOutput();

  if ( !inputPtr || !outputPtr )
    {
    return;
    }

  // allocate output buffer memory
  outputPtr->SetBufferedRegion( outputPtr->GetRequestedRegion() );
  outputPtr->Allocate();

  const typename InputImageType::IndexType& inputIndex
    = inputPtr->GetRequestedRegion().GetIndex();
  const typename InputImageType::SizeType& inputSize
    = inputPtr->GetRequestedRegion().GetSize();

  const unsigned int direction = this->m_Direction;

  const unsigned int num_components = this->m_ReferenceReader->GetOutput()->GetVectorLength();

  outputPtr->FillBuffer( 0.0 );

  typedef itk::ImageLinearConstIteratorWithIndex< InputImageType > SampleIteratorType;
  SampleIteratorType sample_it( inputPtr, inputPtr->GetRequestedRegion() );
  sample_it.SetDirection( direction );

  typedef itk::ImageLinearIteratorWithIndex< OutputImageType > OutputIteratorType;
  OutputIteratorType output_it( outputPtr, inputPtr->GetRequestedRegion() );
  output_it.SetDirection( direction );

  typename ReferenceVectorImageType::IndexType refIndex;
  typename ReferenceVectorImageType::SizeType refSize;
  refIndex[0] = inputIndex[direction];
  refSize[0] = inputSize[direction];
  typename ReferenceVectorImageType::RegionType refRegion;
  refRegion.SetIndex( refIndex );
  refRegion.SetSize( refSize );
  typedef itk::ImageLinearConstIteratorWithIndex< ReferenceVectorImageType > ReferenceIteratorType;
  ReferenceIteratorType ref_it( this->m_ReferenceReader->GetOutput(), refRegion );
  ref_it.SetDirection(0);

  const PixelType freq_start = 6.5625;
  //const PixelType freq_end = 29.53125;
  const PixelType freq_step = 210.0 / 64.0;
  // assume rayleigh scattering, sigma = A*f^4
  const PixelType rayleigh_coeff = 1.5625e-6;
  std::vector< PixelType > bsc_r(num_components);
  std::vector< PixelType > freqs(num_components);
  unsigned int i;
  for( i = 0; i< num_components; i++ )
    {
    freqs[i] = freq_start + i* freq_step;
    bsc_r[i] = rayleigh_coeff * ( freqs[i] * freqs[i] * freqs[i] * freqs[i] );
    }
  //const PixelType freq_range = freqs[ num_components-1 ] - freqs[0] ;

  const PixelType* sample_p;
  const PixelType* ref_p;

  for( sample_it.GoToBegin(), output_it.GoToBegin(); !sample_it.IsAtEnd(); sample_it.NextLine(), output_it.NextLine() )
    {
    sample_it.GoToBeginOfLine();
    output_it.GoToBeginOfLine();
    ref_it.GoToBeginOfLine();
    while( !sample_it.IsAtEndOfLine() )
	{
	sample_p = sample_it.Get().GetDataPointer();
	ref_p = ref_it.Get().GetDataPointer();
        for( i = 0; i < num_components; i++ )
          {
	  output_it.Set( bsc_r[i] * sample_p[i] / ref_p[i] + output_it.Get() );
          }
	++sample_it;
	++output_it;
	++ref_it;
	}
    }

  itk::ImageRegionIterator< OutputImageType > simple_out_it( outputPtr, outputPtr->GetRequestedRegion() );
  simple_out_it.GoToBegin();
  while( !simple_out_it.IsAtEnd() )
    {
    //simple_out_it.Set( simple_out_it.Get() / freq_range );
    simple_out_it.Set( simple_out_it.Get() / num_components );
    ++simple_out_it;
    }

}


} // end namespace itk

#endif // inclusion guard
