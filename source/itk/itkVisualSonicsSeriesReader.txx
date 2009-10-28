#ifndef __itkVisualSonicsSeriesReader_txx
#define __itkVisualSonicsSeriesReader_txx

#include "itkVisualSonicsSeriesReader.h"

#include "itkVisualSonicsImageIO.h"

namespace itk
{


template <class TOutputImage>
VisualSonicsSeriesReader< TOutputImage >
::VisualSonicsSeriesReader()
{
  this->SetImageIO( VisualSonicsImageIO::New() );
}


template <class TOutputImage>
void
VisualSonicsSeriesReader< TOutputImage >
::GenerateOutputInformation(void)
{
}


template <class TOutputImage>
void
VisualSonicsSeriesReader< TOutputImage >
::EnlargeOutputRequestedRegion( DataObject* output )
{
}


template <class TOutputImage>
void
VisualSonicsSeriesReader< TOutputImage >
::PrintSelf( std::ostream& os, Indent indent ) const
{
}


template <class TOutputImage>
void
VisualSonicsSeriesReader< TOutputImage >
::GenerateData()
{
}


} // end namespace itk

#endif // inclusion guard
