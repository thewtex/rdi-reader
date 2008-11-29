#ifndef  __itkHammingWindowImageFilter_h
#define  __itkHammingWindowImageFilter_h

#include "itkImageToImageFilter.h"
#include "itkVariableLengthVector.h"

namespace itk
{
  /** @class HammingWindowImageFilter
   * @brief Apply a Hamming Window (raised cosine) to one dimension of the image.
   *
   */

template < class TInputImage, class TOutputImage >
class HammingWindowImageFilter :
  public ImageToImageFilter<TInputImage, TOutputImage>
{
public:
  /** Standard class typedefs. */
  typedef HammingWindowImageFilter Self;
  typedef ImageToImageFilter<TInputImage,TOutputImage> Superclass;
  typedef SmartPointer<Self>  Pointer;
  typedef SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(HammingWindowImageFilter, ImageToImageFilter);

  /**Typedefs from the superclass */
  typedef typename Superclass::InputImageType  InputImageType;
  typedef typename Superclass::OutputImageType OutputImageType;

  /** Dimensionality of input and output data is assumed to be the same.
   * It is inherited from the superclass. */
  itkStaticConstMacro(ImageDimension, unsigned int, TInputImage::ImageDimension);
  itkStaticConstMacro(OutputImageDimension, unsigned int,
                      TOutputImage::ImageDimension);

  /** The pixel type of the output image will be used in computations.
   * Inherited from the superclass. */
  typedef typename OutputImageType::PixelType  PixelType;
  typedef typename InputImageType::PixelType   InputPixelType;
  typedef typename OutputImageType::RegionType OutputImageRegionType;
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
  HammingWindowImageFilter();
  virtual ~HammingWindowImageFilter(){};
  void PrintSelf(std::ostream& os, Indent indent) const;


  typedef typename itk::VariableLengthVector< InputPixelType > WindowType;
  WindowType m_Window;
  virtual void GenerateWindow( const unsigned int window_size );


  /** Direction in which the filter is to be applied
   * this should be in the range [0,ImageDimension-1]. */
  unsigned int m_Direction;

private:
  HammingWindowImageFilter( const Self& ); // purposely not implemented
  void operator=( const Self& ); // purposely not implemented

};

} // namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkHammingWindowImageFilter.txx"
#endif

#endif // inclusion guard


