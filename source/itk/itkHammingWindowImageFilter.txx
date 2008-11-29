#ifndef __itkHammingWindowImageFilter_txx
#define __itkHammingWindowImageFilter_txx

#include <cmath>

#include "itkHammingWindowImageFilter.h"
#include "itkImageLinearIteratorWithIndex.h"

namespace itk
{

template <class TInputImage, class TOutputImage>
HammingWindowImageFilter<TInputImage,TOutputImage>
::HammingWindowImageFilter()
: m_Direction(1)
  {
  }

template <class TInputImage, class TOutputImage>
void
HammingWindowImageFilter<TInputImage, TOutputImage>
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os,indent);
  os << indent << "Direction: " << m_Direction << std::endl;
  os << std::endl;
}

template <class TInputImage, class TOutputImage>
void
HammingWindowImageFilter<TInputImage,TOutputImage>
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

  const typename TOutputImage::SizeType& inputSize
    = inputPtr->GetRequestedRegion().GetSize();

  unsigned int window_size = inputSize[this->m_Direction];

  if( !( window_size == this->m_Window.GetSize() ) )
    {
    this->GenerateWindow( window_size );
    }

  typedef itk::ImageLinearConstIteratorWithIndex< TInputImage >  InputIteratorType;
  typedef itk::ImageLinearIteratorWithIndex< TOutputImage >      OutputIteratorType;
  InputIteratorType inputIt( inputPtr, inputPtr->GetRequestedRegion() );
  // the output region should be the same as the input region
  OutputIteratorType outputIt( outputPtr, inputPtr->GetRequestedRegion() );

  inputIt.SetDirection(this->m_Direction);
  outputIt.SetDirection(this->m_Direction);

  unsigned int i;
  for( inputIt.GoToBegin(), outputIt.GoToBegin(); !inputIt.IsAtEnd();
    outputIt.NextLine(), inputIt.NextLine() )
    {
    inputIt.GoToBeginOfLine();
    outputIt.GoToBeginOfLine();
    for( i = 0; i < window_size; i++ )
      {
      outputIt.Set( static_cast< PixelType > ( inputIt.Get() * this->m_Window[i] ) );
      ++inputIt;
      ++outputIt;
      }
    }
}

template <class TInputImageType, class TOutputImageType>
void
HammingWindowImageFilter<TInputImageType,TOutputImageType>
::GenerateWindow( const unsigned int window_size )
{
  this->m_Window.SetSize( window_size );
  InputPixelType factor = 2.0 * 3.14159265 / ( window_size - 1 ) ;
  for( unsigned int i = 0; i < window_size; i++ )
    {
    this->m_Window[i] = 0.53836 - 0.46164 * ::cos( factor*i );
    }
}

}

#endif // inclusion guard
