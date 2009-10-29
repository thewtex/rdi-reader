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

#include <vector>

#include "itkImageFileReader.h"

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

  /** The spacing type of the output image */
  typedef typename TOutputImage::SpacingType SpacingType;

  /** Prepare the allocation of the output image during the first back
   * propagation of the pipeline. */
  virtual void GenerateOutputInformation(void);

  /** Give the reader a chance to indicate that it will produce more
   * output than it was requested to produce. VisualSonicsSeriesReader cannot
   * currently read a portion of a frame.
   * so the ImageSeriesReader must
   * enlarge the RequestedRegion to the size of the image on disk. */
  virtual void EnlargeOutputRequestedRegion(DataObject *output);

protected:
  VisualSonicsSeriesReader();
  ~VisualSonicsSeriesReader() {};
  void PrintSelf(std::ostream& os, Indent indent) const;

  /** Does the real work. */
  virtual void GenerateData();

  /** Populated with the sub regions of the input file images that will be used
   * to create the output image.  It is a full region of ImageRegionType instead
   * of just a region of the first dimension ( the beamline dimension ) because
   * future inherited classes may further constrain the region if doing more
   * complex sub-image registration. */
  typedef typename std::vector< ImageRegionType >  SubRegionsVectorType;
  SubRegionsVectorType				   m_SubRegionsVector;

  /** Determine the sub regions to use from the input images.
   *
   * Return spacing from the first sub image as a convenience so we do not have
   * to reader in the header twice. */
  virtual SpacingType GenerateSubRegions();

  typedef ImageFileReader<TOutputImage> ReaderType;

private:
  VisualSonicsSeriesReader(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkVisualSonicsSeriesReader.txx"
#endif

#endif // inclusion guard
