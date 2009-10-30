/**
 * @file itkVisualSonicsImageIO.h
 * @brief ITK IO source filter object for a VisualSonics raw (.rdi/.rdb)
 * Ultrasound image format.
 * @author Matthew McCormick (thewtex) <matt@mmmccormick.com>
 * @date 2009-10-26
 */

#ifndef __itkVisualSonicsImageIO_h
#define __itkVisualSonicsImageIO_h

#include <memory>

#include "itkImageIOBase.h"

#include "formats/rdi.hxx"
#include "rdiReader.h"

namespace itk
{
  /**
   * @brief ITK IO source filter object for a VisualSonics raw (.rdi/.rdb)
   * Ultrasound image file format.
   *
   * Dimensions are populated as follows:
   *  1 - Beamline samples (axial)
   *  2 - Beamlines (lateral, angular)
   *  3 - Frame
   *
   *  All units are in meters.
   *
   * Populates some itk::MetaDataDictionary objects:
   *
   *  "Radius"
   *	The distance from the center of curvature to the first beamline sample.
   *  "Theta"
   *	An array of the angles for every beamline.  In Radians.
   *
   */
class ITK_EXPORT VisualSonicsImageIO : public ImageIOBase
{
public:
  /** Standard class typedefs. */
  typedef VisualSonicsImageIO	  Self;
  typedef ImageIOBase	  Superclass;
  typedef SmartPointer<Self>  Pointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(VisualSonicsImageIO, Superclass);

  /** Determine the file type. Returns true if this ImageIO can read the
   * file specified. */
  virtual bool CanReadFile(const char*);

  /** Set the spacing and dimension information for the set filename. */
  virtual void ReadImageInformation();

  /** Reads the data from disk into the memory buffer provided. */
  virtual void Read(void* buffer);

  virtual bool CanStreamRead()
    { return true; };

  /** Method for supporting streaming.  Given a requested region, calculate what
   * could be the region that we can read from the file. This is called the
   * streamable region, which will be smaller than the LargestPossibleRegion and
   * greater or equal to the RequestedRegion */
  virtual ImageIORegion
  GenerateStreamableReadRegionFromRequestedRegion( const ImageIORegion & requested ) const;

  /*-------- This part of the interfaces deals with writing data. ----- */

  /** Determine the file type. Returns true if this ImageIO can write the
   * file specified.  A writer is not implemented. */
  virtual bool CanWriteFile(const char* )
    { return false; };

  /** Set the spacing and dimension information for the set filename. */
  virtual void WriteImageInformation(){};

  /** Writes the data to disk from the memory buffer provided. Make sure
   * that the IORegions has been set properly. */
  virtual void Write(const void* buffer){};

  VisualSonicsImageIO();
  virtual ~VisualSonicsImageIO();
  void PrintSelf(std::ostream& os, Indent indent) const;

private:
  VisualSonicsImageIO(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  /** we need this here because of the memory management in Xerces-C */
  std::auto_ptr< rdiReader > m_rdiReader;

protected:
  /**
   * @brief holds the rdi metadata object
   */
  std::auto_ptr< rdi_t > m_rdi;
};
} // end namespace itk

#endif // __itkVisualSonicsImageIO_h
