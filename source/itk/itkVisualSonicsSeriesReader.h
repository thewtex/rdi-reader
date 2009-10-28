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
public:
  /** Standard class typedefs. */
  typedef VisualSonicsSeriesReader	    Self;
  typedef ImageSeriesReader< TOutputImage > Superclass;
  typedef SmartPointer< Self >		    Pointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(VisualSonicsSeriesReader, ImageSeriesReader);

  /** The size of the output image. */
  typedef typename TOutputImage::SizeType   SizeType;

  SizeType size;
  /** The index of the output image. */
  typedef typename TOutputImage::IndexType  IndexType;

  /** The region of the output image. */
  typedef typename TOutputImage::RegionType  ImageRegionType;

  /** The pixel type of the output image. */
  typedef typename TOutputImage::PixelType OutputImagePixelType;

   //[>* The pixel type of the output image. <]
  //typedef Superclass::DictionaryType                   DictionaryType;
  //typedef Superclass::DictionaryRawPointer             DictionaryRawPointer;
  //typedef Superclass::DictionaryArrayType              DictionaryArrayType;
  //typedef Superclass::DictionaryArrayRawPointer        DictionaryArrayRawPointer;

  //typedef Superclass::FileNamesContainer               FileNamesContainer;

protected:
  VisualSonicsSeriesReader() {};
  ~VisualSonicsSeriesReader() {};


};

} // end namespace itk

#endif // inclusion guard
