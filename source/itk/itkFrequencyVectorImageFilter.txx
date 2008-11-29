#ifndef __itkFrequencyVectorImageFilter_txx
#define __itkFrequencyVectorImageFilter_txx


#include "itkFrequencyVectorImageFilter.h"
#include "itkImageLinearIteratorWithIndex.h"

namespace itk
{

template <class TInputImage >
FrequencyVectorImageFilter<TInputImage>
::FrequencyVectorImageFilter()
: m_Direction(1),
  m_FFTSize(128),
  m_FFTOverlap(0.5),
  m_FrequencyExtractStartIndex(0),
  m_FrequencyExtractSize(m_FFTSize)
{
  this->SetNumberOfRequiredInputs( 1 );
}

template <class TInputImage >
void
FrequencyVectorImageFilter<TInputImage>
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os,indent);
  os << indent << "Direction: " << m_Direction << std::endl;
  os << std::endl;
}

template < class TInputImage >
void
FrequencyVectorImageFilter< TInputImage >
::AllocateOutputs()
{
  // override the method in itkImageSource
  OutputImageType* output = this->GetOutput();
  output->SetVectorLength( this->m_FrequencySize );
  this->Superclass::AllocateOutputs();
}

template <class TInputImage >
void
FrequencyVectorImageFilter<TInputImage>
::GenerateData()
{
  this->AllocateOutputs();

  // get pointers to the input and output
  typename TInputImage::ConstPointer  inputPtr  = this->GetInput();
  typename OutputImageType::Pointer      outputPtr = this->GetOutput();

  if ( !inputPtr || !outputPtr )
    {
    return;
    }


  //const typename OutputImageType::SizeType& inputSize
    //= inputPtr->GetRequestedRegion().GetSize();

  //unsigned int window_size = inputSize[this->m_Direction];

  //if( !( window_size == this->m_Window.GetSize() ) )
    //{
    //this->GenerateWindow( window_size );
    //}

  //typedef itk::ImageLinearConstIteratorWithIndex< TInputImage >  InputIteratorType;
  //typedef itk::ImageLinearIteratorWithIndex< TOutputImage >      OutputIteratorType;
  //InputIteratorType inputIt( inputPtr, inputPtr->GetRequestedRegion() );
  //// the output region should be the same as the input region
  //OutputIteratorType outputIt( outputPtr, inputPtr->GetRequestedRegion() );

  //inputIt.SetDirection(this->m_Direction);
  //outputIt.SetDirection(this->m_Direction);

  //unsigned int i;
  //for( inputIt.GoToBegin(), outputIt.GoToBegin(); !inputIt.IsAtEnd();
    //outputIt.NextLine(), inputIt.NextLine() )
    //{
    //inputIt.GoToBeginOfLine();
    //outputIt.GoToBeginOfLine();
    //for( i = 0; i < window_size; i++ )
      //{
      //outputIt.Set( static_cast< PixelType > ( inputIt.Get() * this->m_Window[i] ) );
      //++inputIt;
      //++outputIt;
      //}
    //}
}


}

#endif // inclusion guard
