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
#include "itkMetaDataObject.h"
#include "itkVector.h"

namespace itk
{

template <class TInputImage, class TReferenceSpectrum, class TReferenceBSC, class TOutputImage >
BSC<TInputImage,TReferenceSpectrum,TReferenceBSC,TOutputImage>
::BSC()
: m_Direction(0),
  m_SampleRadius( 0.0 )
{
  this->SetNumberOfRequiredInputs( 3 );

  m_ReferenceBSCInterpolator = ReferenceBSCInterpolatorType::New();
  m_ReferenceSpectrumInterpolator = ReferenceSpectrumInterpolaterType::New();
}

template <class TInputImage, class TReferenceSpectrum, class TReferenceBSC, class TOutputImage >
void
BSC<TInputImage,TReferenceSpectrum,TReferenceBSC,TOutputImage>
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os,indent);
  os << indent << "Direction: " << m_Direction << std::endl;
  os << std::endl;
}

template <class TInputImage, class TReferenceSpectrum, class TReferenceBSC, class TOutputImage >
void
BSC<TInputImage,TReferenceSpectrum,TReferenceBSC,TOutputImage>
::SetReferenceSpectrum( const TReferenceSpectrum * refSpec )
{
  this->ProcessObject::SetNthInput( 1, const_cast< TReferenceSpectrum * >( refSpec ));
}

template <class TInputImage, class TReferenceSpectrum, class TReferenceBSC, class TOutputImage >
const TReferenceSpectrum *
BSC<TInputImage,TReferenceSpectrum,TReferenceBSC,TOutputImage>
::GetReferenceSpectrum() const
{
  return const_cast< const TReferenceSpectrum * >( this->ProcessObject::GetInput( 1 ));
}

template <class TInputImage, class TReferenceSpectrum, class TReferenceBSC, class TOutputImage >
void
BSC<TInputImage,TReferenceSpectrum,TReferenceBSC,TOutputImage>
::SetReferenceBSC( const TReferenceBSC * refSpec )
{
  this->ProcessObject::SetNthInput( 2, const_cast< TReferenceBSC * >( refSpec ));
}

template <class TInputImage, class TReferenceSpectrum, class TReferenceBSC, class TOutputImage >
const TReferenceBSC *
BSC<TInputImage,TReferenceSpectrum,TReferenceBSC,TOutputImage>
::GetReferenceBSC() const
{
  return const_cast< const TReferenceBSC * >( this->ProcessObject::GetInput( 2 ));
}

template <class TInputImage, class TReferenceSpectrum, class TReferenceBSC, class TOutputImage >
void
BSC<TInputImage,TReferenceSpectrum,TReferenceBSC,TOutputImage>
::GenerateData()
{

  // get pointers to the input and output
  typename TInputImage::ConstPointer  inputPtr  = this->GetInput();
  typename TReferenceSpectrum::Pointer refSpecPtr = static_cast< ReferenceSpectrumType * >(
    this->ProcessObject::GetInput( 1 ));
  typename TReferenceBSC::Pointer refBSCPtr = static_cast< ReferenceBSCType * >(
    this->ProcessObject::GetInput( 2 ));
  //refSpecPtr->Update();
  typename TOutputImage::Pointer      outputPtr = this->GetOutput();

  if ( !inputPtr || !outputPtr )
    {
    return;
    }

  // allocate output buffer memory
  outputPtr->SetBufferedRegion( outputPtr->GetRequestedRegion() );
  outputPtr->Allocate();

  const unsigned int direction = this->m_Direction;

  const unsigned int num_components = ReferenceSpectrumType::PixelType::Dimension;

  outputPtr->FillBuffer( 0.0 );

  typedef itk::ImageLinearConstIteratorWithIndex< InputImageType > SampleIteratorType;
  SampleIteratorType sample_it( inputPtr, inputPtr->GetRequestedRegion() );
  sample_it.SetDirection( direction );

  typedef itk::ImageLinearIteratorWithIndex< OutputImageType > OutputIteratorType;
  OutputIteratorType output_it( outputPtr, inputPtr->GetRequestedRegion() );
  output_it.SetDirection( direction );

  // @todo fix hard coding
  const PixelType freq_start = 6.5625;
  //const PixelType freq_end = 29.53125;
  const PixelType freq_step = 210.0 / 64.0;
  // assume rayleigh scattering, sigma = A*f^4
  //const PixelType rayleigh_coeff = 1.5625e-6;
  //std::vector< PixelType > bsc_r(num_components);
  //std::vector< PixelType > freqs(num_components);
  unsigned int i;
  //for( i = 0; i < num_components; i++ )
    //{
    //freqs[i] = freq_start + i* freq_step;
    //bsc_r[i] = rayleigh_coeff * ( freqs[i] * freqs[i] * freqs[i] * freqs[i] );
    //}
  //const PixelType freq_range = freqs[ num_components-1 ] - freqs[0] ;
  m_ReferenceBSCInterpolator->SetInputImage( refBSCPtr );
  typename ReferenceBSCInterpolatorType::PointType freqPoint;
  freqPoint[0] = freq_start;
  InputPixelType refBSC;
  typename ReferenceBSCType::IndexType refbscidx;
  for( i = 0; i < num_components; ++i )
    {
    if( m_ReferenceBSCInterpolator->IsInsideBuffer( freqPoint ))
      {
      refbscidx[0] = i;
      refBSC[i] = m_ReferenceBSCInterpolator->Evaluate( freqPoint );
      }
    else
      {
      // do something better here
      //refBSC[i] = 0.0;
      itkExceptionMacro(<< "Requested frequency for the reference is outside that given for the BSC.");
      }
    freqPoint[0] += freq_step;
    }


  InputPixelType sample_p;
  InputPixelType ref_p;
  m_ReferenceSpectrumInterpolator->SetInputImage( refSpecPtr );
  double rSpacing = inputPtr->GetSpacing()[0];
  typename ReferenceSpectrumType::PointType refPoint;

  for( sample_it.GoToBegin(), output_it.GoToBegin(); !sample_it.IsAtEnd(); sample_it.NextLine(), output_it.NextLine() )
    {
    sample_it.GoToBeginOfLine();
    output_it.GoToBeginOfLine();
    while( !sample_it.IsAtEndOfLine() )
	{
	sample_p = sample_it.Get();
        refPoint[0] = m_SampleRadius + rSpacing * sample_it.GetIndex()[0];
	ref_p = m_ReferenceSpectrumInterpolator->Evaluate( refPoint );
        for( i = 0; i < num_components; i++ )
          {
	  output_it.Set( refBSC[i] * sample_p[i] / ref_p[i] + output_it.Get() );
          }
	++sample_it;
	++output_it;
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
