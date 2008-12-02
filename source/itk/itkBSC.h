#ifndef  __itkBSC_h
#define  __itkBSC_h

#include "itkImageFileReader.h"
#include "itkImageToImageFilter.h"
#include "itkVectorImage.h"

namespace itk
{
  /** @class BSC
   * @brief Create a VectorImage where each pixel contains a vector of frequency components.
   *
   */

template <class TInputImage, class TOutputImage>
class BSC :
  public ImageToImageFilter<TInputImage, TOutputImage>
{
public:
  /** Standard class typedefs. */
  typedef BSC Self;
  typedef ImageToImageFilter<TInputImage, TOutputImage> Superclass;
  typedef SmartPointer<Self>  Pointer;
  typedef SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(BSC, ImageToImageFilter);

  /**Typedefs from the superclass */
  typedef typename Superclass::InputImageType  InputImageType;
  typedef typename Superclass::OutputImageType OutputImageType;

  itkStaticConstMacro(InputImageDimension, unsigned int, TInputImage::ImageDimension);
  itkStaticConstMacro(OutputImageDimension, unsigned int,
                      TOutputImage::ImageDimension);

  /** The pixel type of the output image will be used in computations.
   * Inherited from the superclass. */
  typedef typename InputImageType::PixelType	    InputPixelType;
  typedef typename OutputImageType::PixelType	    PixelType;
  typedef typename Superclass::InputImageRegionType InputRegionType;
  typedef typename Superclass::OutputImageRegionType OutputRegionType;
  /** Some typedef */
  typedef typename InputImageType::SizeType   InputSizeType;
  typedef typename OutputImageType::SizeType  SizeType;
  typedef typename InputImageType::IndexType  InputIndexType;
  typedef typename OutputImageType::IndexType IndexType;


  /** Get the direction in which the filter is to be applied. */
  itkGetMacro(Direction, unsigned int);
  /** Set the direction in which the filter is to be applied. */
  itkSetMacro(Direction, unsigned int);


  /** @todo -- threadify */
  virtual void GenerateData();  // generates output from input
protected:
  BSC();
  virtual ~BSC(){};
  void PrintSelf(std::ostream& os, Indent indent) const;

  /** Direction in which the filter is to be applied
   * this should be in the range [0,ImageDimension-1]. */
  unsigned int m_Direction;

  typedef VectorImage< PixelType, 1 > ReferenceVectorImageReaderInType;
  typedef itk::ImageFileReader< ReferenceVectorImageReaderInType > ReferenceReaderType;
  typedef typename ReferenceReaderType::OutputImageType ReferenceVectorImageType;

  typename ReferenceReaderType::Pointer m_ReferenceReader;
  std::string reference_filename;

private:
  BSC( const Self& ); // purposely not implemented
  void operator=( const Self& ); // purposely not implemented

};

} // namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkBSC.txx"
#endif

#endif // inclusion guard


