#ifndef __itkFrequencyVectorImageFilter_txx
#define __itkFrequencyVectorImageFilter_txx


#include "itkFrequencyVectorImageFilter.h"
#include "itkImageLinearConstIteratorWithIndex.h"

// for testing
#include <iostream>
using std::cout;
using std::endl;
namespace itk
{

template <class TInputImage >
FrequencyVectorImageFilter<TInputImage>
::FrequencyVectorImageFilter()
: m_Direction(1),
  m_FFTSize(128),
  m_FFTOverlap(0.5),
  m_FrequencyExtractStartIndex(0)
{
  // DC to nyquist
  m_FrequencyExtractSize = m_FFTSize/2 + 1;
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
  cout << " input requested region: " << inputRequestedRegion << endl;

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


  const typename InputImageType::IndexType& inputIndex
    = inputPtr->GetRequestedRegion().GetIndex();
  const typename InputImageType::SizeType& inputSize
    = inputPtr->GetRequestedRegion().GetSize();

  const typename OutputImageType::IndexType& outputStartIndex
    = outputPtr->GetRequestedRegion().GetIndex();
  const typename OutputImageType::SizeType& outputSize
    = outputPtr->GetRequestedRegion().GetSize();

  // sub-regions we will be generating
  InputRegionType requested_subregion;
  typename InputImageType::IndexType subregion_index = inputIndex;
  typename InputImageType::SizeType subregion_size = inputSize;

  unsigned int direction = this->m_Direction;
  unsigned int win_step =
      ( static_cast< unsigned int >( (1.0 - this->m_FFTOverlap) * this->m_FFTSize ) );

  this->m_WindowFilter->SetInput( this->GetInput() );

  subregion_size[direction] = this->m_FFTSize;
  requested_subregion.SetSize( subregion_size );
  requested_subregion.SetIndex( subregion_index );
  typename SquareFilterType::OutputImageType::Pointer subregion_output = this->m_SquareFilter->GetOutput();
  cout << "requested subregion: " << requested_subregion << endl;
  subregion_output->SetRequestedRegion( requested_subregion );


  typedef itk::ImageLinearConstIteratorWithIndex< typename SquareFilterType::OutputImageType >  SubregionIteratorType;

  OutputPixelType outpix;
  typename OutputImageType::IndexType outIndex;

  unsigned int j = 0;
  for( unsigned int i = 0; i < outputSize[direction]; i++ )
    {
    this->m_SquareFilter->Update();

    SubregionIteratorType it( subregion_output, requested_subregion );
    it.SetDirection( direction );
    it.GoToBegin();
    while( !it.IsAtEnd() )
      {
      it.GoToBeginOfLine();
      // skip the frequencies we are not interested in
      for( j = 0; j < this->m_FrequencyExtractStartIndex; j++ )
	{
	++it;
	}
      // copy the frequencies to our output Vector
      for( j = 0; j < this->m_FrequencyExtractSize; j++ )
	{
	outpix[j] = it.Get();
	++it;
	}
      outIndex = it.GetIndex();
      outIndex[direction] = outputStartIndex[direction] + i;
      outputPtr->SetPixel( outIndex, outpix );

      it.NextLine();
      }


    // move to the next subregion
    subregion_index[direction] = subregion_index[direction] + win_step;
    requested_subregion.SetIndex( subregion_index );
    cout << "requested subregion: " << requested_subregion << endl;
    subregion_output->SetRequestedRegion( requested_subregion );
    }
}


} // end namespace itk

#endif // inclusion guard
