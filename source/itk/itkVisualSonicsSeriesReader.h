/**
 * @file itkVisualSonicsSeriesReader.h
 * @brief Read in a series of VisualSonics Digital RF volumetric slices and
 * concatenate them.
 * @author Matthew McCormick (thewtex) <matt@mmmccormick.com>
 * @date 2009-10-27
 */

#ifndef __itkVisualSonicsSeriesReader_h
#define __itkVisualSonicsSeriesReader_h

#include "itkImageSeriesReader.h"

namespace itk
{

/** \brief Read in a series of VisualSonics Digital RF slices and
 * concatenate them.
 *
 * The A/D board on the VisualSonics Vevo 770 often does not have enough memory
 * buffer to hold the entire field of view in on acquisition.  Therefore, if
 * multiple overlapping slices are taken, they can be patched together using the
 * metadata in the header file.
 *
 * This class inherits from ImageSeriesReader and its behaviour is similar.
 *
 * Image slices should be in order from closest to the transducer to furthest
 * away.  slices should overlap slightly.
 *
 * \sa ImageSeriesReader
 * \sa VisualSonicsImageIO
 *
 */

template < class TOutputImage >
class ITK_EXPORT VisualSonicsSeriesReader : public ImageSeriesReader< TOutputImage >
{
};

} // end namespace itk

#endif // inclusion guard
