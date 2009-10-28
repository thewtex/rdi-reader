#ifndef __itkVisualSonicsSeriesReader_txx
#define __itkVisualSonicsSeriesReader_txx

#include "itkVisualSonicsSeriesReader.h"

#include "itkVisualSonicsImageIO.h"

namespace itk
{

template <class TOutputImage>
VisualSonicsSeriesReader<TOutputImage>
::VisualSonicsSeriesReader()
{
  this->SetImageIO( VisualSonicsImageIO::New() );
}

} // end namespace itk

#endif // inclusion guard
