#ifndef __itkMeanAcrossDirection_txx
#define __itkMeanAcrossDirection_txx

#include <iostream>
using std::cout;
using std::endl;

#include "itkMeanAcrossDirection.h"

#include "itkImageRegionConstIterator.h"
#include "itkListSample.h"
#include "itkMeanSampleFilter.h"
#include "itkVector.h"

namespace itk
{

template <class TInputImage, class TOutputImage >
MeanAcrossDirection<TInputImage,TOutputImage>
::MeanAcrossDirection()
: m_Direction(1)
{
  this->SetNumberOfRequiredInputs( 1 );

  m_ROIFilter = ROIFilterType::New();
}

template <class TInputImage, class TOutputImage >
void
MeanAcrossDirection<TInputImage,TOutputImage>
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os,indent);
  os << indent << "Direction: " << m_Direction << std::endl;
  os << std::endl;
}

template <class TInputImage, class TOutputImage >
void
MeanAcrossDirection<TInputImage,TOutputImage>
::GenerateOutputInformation()
{
  //this->Superclass::GenerateOutputInformation();

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

  typename OutputImageType::SizeType     outputSize;
  typename OutputImageType::IndexType    outputStartIndex;

  unsigned int direction = this->m_Direction;

  outputSize[0] = inputSize[direction];
  outputStartIndex[0] = inputStartIndex[direction];

  /** @todo fix outputStartIndex
   * should be smaller than the inputStartIndex in general
   */
  OutputRegionType outputLargestPossibleRegion;
  outputLargestPossibleRegion.SetSize( outputSize );
  outputLargestPossibleRegion.SetIndex( outputStartIndex );

  outputPtr->SetLargestPossibleRegion( outputLargestPossibleRegion );

  // spacing
  const typename InputImageType::SpacingType&  inputSpacing
    = inputPtr->GetSpacing();

  typename OutputImageType::SpacingType outputSpacing;
  outputSpacing[0] = inputSpacing[direction];

  outputPtr->SetSpacing( outputSpacing );
}

template <class TInputImage, class TOutputImage >
void
MeanAcrossDirection<TInputImage,TOutputImage>
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

  typename InputImageType::SizeType  inputRequestedRegionSize = inputPtr->GetLargestPossibleRegion().GetSize();
  typename InputImageType::IndexType inputRequestedRegionStartIndex = inputPtr->GetLargestPossibleRegion().GetIndex();


  const unsigned int direction = this->m_Direction;

  inputRequestedRegionSize[direction] = outputRequestedRegionSize[0];
  inputRequestedRegionStartIndex[direction] = outputRequestedRegionStartIndex[0];

  InputRegionType inputRequestedRegion;
  inputRequestedRegion.SetSize( inputRequestedRegionSize );
  inputRequestedRegion.SetIndex( inputRequestedRegionStartIndex );

  inputPtr->SetRequestedRegion( inputRequestedRegion );
}

template <class TInputImage, class TOutputImage >
void
MeanAcrossDirection<TInputImage,TOutputImage>
::AllocateOutputs()
{
  // override the method in itkImageSource
  OutputImageType* output = this->GetOutput();
  const InputImageType* input = this->GetInput();
  output->SetVectorLength( input->GetVectorLength() );
  this->Superclass::AllocateOutputs();
}

template <class TInputImage, class TOutputImage >
void
MeanAcrossDirection<TInputImage,TOutputImage>
::GenerateData()
{
  this->AllocateOutputs();

  // get pointers to the input and output
  typename TInputImage::ConstPointer  inputPtr  = this->GetInput();
  typename TOutputImage::Pointer      outputPtr = this->GetOutput();

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

  this->m_ROIFilter->SetInput( this->GetInput() );

  subregion_size[direction] = 1;
  requested_subregion.SetSize( subregion_size );
  requested_subregion.SetIndex( subregion_index );
  this->m_ROIFilter->SetRegionOfInterest( requested_subregion );

  typename ROIFilterType::OutputImageType::ConstPointer roi_out = this->m_ROIFilter->GetOutput();

  /** @todo fix me
   * (8 of 23): error: ‘vector_length’ cannot appear in a constant-expression
   */
  //const unsigned int vector_length = inputPtr->GetVectorLength();
  const unsigned int vector_length = 8;
  //PixelType outpix;
  //outpix.SetSize( vector_length );
  PixelType outpixmean( vector_length );
  const typename PixelType::ValueType* outpix ;
  typename OutputImageType::IndexType outIndex;

  //typedef itk::Vector< typename InputImageType::InternalPixelType, vector_length > MeasurementVectorType;
  /** @todo fixme */
  // maybe ITK_TYPENAME ?
  typedef itk::Vector< float, vector_length > MeasurementVectorType;
  MeasurementVectorType mv;
  typedef itk::Statistics::ListSample< MeasurementVectorType > SampleType;
  SampleType::Pointer sample = SampleType::New();

  unsigned int i;
  unsigned int sample_size = 1;
  for( i = 0; i < InputImageDimension; i++ )
    {
    sample_size = sample_size * inputSize[i];
    }
  sample_size = sample_size / inputSize[direction];
  sample->SetMeasurementVectorSize( vector_length );
  sample->Resize( sample_size );

  typedef itk::Statistics::MeanSampleFilter< SampleType > MeanAlgorithmType;
  MeanAlgorithmType::Pointer meanAlgorithm = MeanAlgorithmType::New();
  meanAlgorithm->SetInput( sample );
  MeanAlgorithmType::MeasurementVectorType mean;

  typedef itk::ImageRegionConstIterator< InputImageType > ConstIteratorType;


  unsigned int j = 0;
  unsigned int k = 0;
  for( i = 0; i < outputSize[0]; i++ )
    {
    this->m_ROIFilter->Update();

    ConstIteratorType roi_it( roi_out, roi_out->GetLargestPossibleRegion() );
    for( roi_it.GoToBegin(), j = 0; !roi_it.IsAtEnd() ; ++roi_it, ++j )
      {
      //cout << "outpix typeid: " << typeid(outpix).name() <<endl;
      //cout << "roi_it.Get() typeid: " << typeid( roi_it.Get() ).name() << endl;
      //cout << "roi_it.Get(): " << roi_it.Get() << endl;
      //cout << "roi_it.Get().GetDataPointer(): " << roi_it.Get().GetDataPointer() << endl;
      //outpix = roi_it.Get();
      outpix = roi_it.Get().GetDataPointer();
      for( k = 0; k < vector_length; k++ )
	{
	mv[k] = outpix[k];
	}
      sample->SetMeasurementVector(j, mv);
      }
    meanAlgorithm->Update();
    mean = meanAlgorithm->GetMean();
    for( k = 0; k < vector_length; k++ )
	{
	outpixmean[k] = static_cast<typename OutputImageType::InternalPixelType >(  mean[k] );
	}
    outIndex[0] = outputStartIndex[0] + i;
    //cout << "outpixmean: " << outpixmean << endl;
    outputPtr->SetPixel( outIndex, outpixmean );

    // move to the next subregion
    subregion_index[direction] = subregion_index[direction] + 1;
    requested_subregion.SetIndex( subregion_index );
    this->m_ROIFilter->SetRegionOfInterest( requested_subregion );
    }
}


} // end namespace itk

#endif // inclusion guard
