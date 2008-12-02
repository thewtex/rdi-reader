#ifndef __itkBSC_txx
#define __itkBSC_txx

#include <iostream>
using std::cout;
using std::endl;

#include "itkBSC.h"

#include "itkImageFileReader.h"
#include "itkImageLinearConstIteratorWithIndex.h"
#include "itkImageLinearIteratorWithIndex.h"
#include "itkImageRegionConstIterator.h"
#include "itkVector.h"

namespace itk
{

template <class TInputImage, class TOutputImage >
BSC<TInputImage,TOutputImage>
::BSC()
: m_Direction(1)
{
  //@ todo change to 2 so that the reference can be specified
  this->SetNumberOfRequiredInputs( 1 );

  m_SampleAdaptor = SampleVectorImageToImageType::New();
  m_ReferenceAdaptor = ReferenceVectorImageToImageType::New();

  m_ReferenceReader = ReferenceReaderType::New();
  reference_filename = "phantom10dBfreqs.mhd";
  m_ReferenceReader->SetFileName( reference_filename.c_str() );
  m_ReferenceReader->Update();

  m_ReferenceAdaptor->SetImage( m_ReferenceReader->GetOutput() );
  m_ReferenceAdaptor->Update();
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

  this->m_SampleAdaptor->SetImage( inputPtr );
  this->m_SampleAdaptor->Update();

  const unsigned int num_components = this->m_ReferenceReader->GetOutput()->GetVectorLength();

  outputPtr->FillBuffer( 0.0 );

  typedef itk::ImageRegionConstIterator< SampleVectorImageToImageType > SampleIteratorType;

  typedef itk::ImageRegionIterator< OutputImageType > OutputIteratorType;
  OutputIteratorType output_it( outputPtr, inputPtr->GetRequestedRegion() );

  typename ReferenceVectorImageType::IndexType refIndex;
  typename ReferenceVectorImageType::SizeType refSize;
  refIndex[0] = inputIndex[direction];
  refSize[0] = inputSize[direction];
  typename ReferenceVectorImageType::RegionType refRegion;
  refRegion.SetIndex( refIndex );
  refRegion.SetSize( refSize );
  typedef itk::ImageRegionConstIterator< ReferenceVectorImageToImageType > ReferenceIteratorType;
  ReferenceIteratorType ref_it( this->m_ReferenceAdaptor, refRegion );

  unsigned int i;
  for( i = 0; i < num_components; i++ )
    {
    this->m_SampleAdaptor->SetExtractComponentIndex( i );
    this->m_SampleAdaptor->Update();
    SampleIteratorType sample_it( this->m_SampleAdaptor, inputPtr->GetRequestedRegion() );
    }
}


} // end namespace itk

#endif // inclusion guard
