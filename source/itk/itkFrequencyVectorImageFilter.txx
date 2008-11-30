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

  m_WindowFilter = WindowType::New();
  m_FFT1DFilter = FFT1DFilterType::New();
  m_ModulusFilter = ModulusFilterType::New();
  m_SquareFilter = SquareFilterType::New();

  m_FFT1DFilter->SetInput( m_WindowFilter->GetOutput() );
  m_ModulusFilter->SetInput( m_FFT1DFilter->GetOutput() );
  m_SquareFilter->SetInput( m_ModulusFilter->GetOutput() );
}

template <class TInputImage >
void
FrequencyVectorImageFilter<TInputImage>
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os,indent);
  os << indent << "Direction: " << m_Direction << std::endl;
  os << indent << "FFTSize: " << m_FFTSize << std::endl;
  os << indent << "FFTOverlap: " << m_FFTOverlap << std::endl;
  os << indent << "FrequencyExtractStartIndex: " << m_FrequencyExtractStartIndex << std::endl;
  os << indent << "FrequencyExtractSize: " << m_FrequencyExtractSize << std::endl;
  os << std::endl;
}

template < class TInputImage >
void
FrequencyVectorImageFilter< TInputImage >
::GenerateOutputInformation()
{
  this->Superclass::GenerateOutputInformation();

  // get pointers to the input and output
  typename InputImageType::ConstPointer  inputPtr  = this->GetInput();
  typename OutputImageType::Pointer      outputPtr = this->GetOutput();

  if ( !inputPtr || !outputPtr )
    {
    return;
    }

  const typename InputImageType::SizeType&   inputSize
    = inputPtr->GetLargestPossibleRegion().GetSize();
  const typename InputImageType::IndexType&  inputStartIndex
    = inputPtr->GetLargestPossibleRegion().GetIndex();

  typename OutputImageType::SizeType     outputSize = inputSize;
  typename OutputImageType::IndexType    outputStartIndex = inputStartIndex;

  if( inputSize[this->m_Direction] < this->m_FFTSize )
    {
    outputSize[this->m_Direction] = 0;
    }
  else
    {
    // integer division rounds down
    outputSize[this->m_Direction] = 1 +
      (inputSize[this->m_Direction] - this->m_FFTSize) /
      ( static_cast< unsigned int >( (1.0 - this->m_FFTOverlap) * this->m_FFTSize ) );
    }

  /** @todo fix outputStartIndex
   * should be smaller than the inputStartIndex in general
   */
  OutputRegionType outputLargestPossibleRegion;
  outputLargestPossibleRegion.SetSize( outputSize );
  outputLargestPossibleRegion.SetIndex( outputStartIndex );

  outputPtr->SetLargestPossibleRegion( outputLargestPossibleRegion );
}

template < class TInputImage >
void
FrequencyVectorImageFilter< TInputImage >
::GenerateInputRequestedRegion()
{
  this->Superclass::GenerateInputRequestedRegion();

  // get pointers to the inputs
  typename InputImageType::Pointer inputPtr  =
    const_cast<InputImageType *> (this->GetInput());
  typename OutputImageType::Pointer outputPtr = this->GetOutput();

  if ( !inputPtr || !outputPtr )
    {
    return;
    }

  // we need to compute the input requested region (size and start index)
  typedef const typename OutputImageType::SizeType& OutputSizeType;
  OutputSizeType outputRequestedRegionSize =
    outputPtr->GetRequestedRegion().GetSize();
  typedef const typename OutputImageType::IndexType& OutputIndexType;
  OutputIndexType outputRequestedRegionStartIndex =
    outputPtr->GetRequestedRegion().GetIndex();

  typename InputImageType::SizeType  inputRequestedRegionSize = outputRequestedRegionSize;
  typename InputImageType::IndexType inputRequestedRegionStartIndex = outputRequestedRegionStartIndex;


  const unsigned int direction = this->m_Direction;
  unsigned int win_step =
      ( static_cast< unsigned int >( (1.0 - this->m_FFTOverlap) * this->m_FFTSize ) );

  const typename OutputImageType::IndexType& outputLargeIndex =
    outputPtr->GetLargestPossibleRegion().GetIndex();
  const typename InputImageType::IndexType& inputLargeIndex =
    inputPtr->GetLargestPossibleRegion().GetIndex();
  inputRequestedRegionStartIndex[direction] = inputLargeIndex[direction] +
    ( outputRequestedRegionStartIndex[direction] - outputLargeIndex[direction] ) * win_step ;


  if( outputRequestedRegionSize[direction] == 0 )
    {
    inputRequestedRegionSize[direction] = 0;
    }
  else
    {
    inputRequestedRegionSize[direction] = this->m_FFTSize +
      win_step * ( outputRequestedRegionSize[direction] - 1 );
    }

  InputRegionType inputRequestedRegion;
  inputRequestedRegion.SetSize( inputRequestedRegionSize );
  inputRequestedRegion.SetIndex( inputRequestedRegionStartIndex );

  inputPtr->SetRequestedRegion( inputRequestedRegion );
}

template < class TInputImage >
void
FrequencyVectorImageFilter< TInputImage >
::AllocateOutputs()
{
  // override the method in itkImageSource
  OutputImageType* output = this->GetOutput();
  output->SetVectorLength( this->m_FrequencyExtractSize );
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
