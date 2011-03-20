#ifndef  __itkMeanAcrossDirection_h
#define  __itkMeanAcrossDirection_h

#include "itkImageToImageFilter.h"
#include "itkRegionOfInterestImageFilter.h"

namespace itk
{
  /** @class MeanAcrossDirection
   * @brief Create a image where each pixel contains a vector of frequency components.
   *
   */

template <class TInputImage, class TOutputImage>
class MeanAcrossDirection :
  public ImageToImageFilter<TInputImage, TOutputImage>
{
public:
  /** Standard class typedefs. */
  typedef MeanAcrossDirection Self;
  typedef ImageToImageFilter<TInputImage, TOutputImage> Superclass;
  typedef SmartPointer<Self>  Pointer;
  typedef SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(MeanAcrossDirection, ImageToImageFilter);

  /**Typedefs from the superclass */
  typedef typename Superclass::InputImageType  InputImageType;
  typedef typename Superclass::OutputImageType OutputImageType;

  itkStaticConstMacro(InputImageDimension, unsigned int, TInputImage::ImageDimension);
  itkStaticConstMacro(OutputImageDimension, unsigned int,
                      TOutputImage::ImageDimension);

  /** The pixel type of the output image will be used in computations.
   * Inherited from the superclass. */
  typedef typename InputImageType::PixelType   InputPixelType;
  typedef typename OutputImageType::PixelType	PixelType;
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
  MeanAcrossDirection();
  virtual ~MeanAcrossDirection(){};
  void PrintSelf(std::ostream& os, Indent indent) const;

  // inherited from ImageSource, step in threaded execution
  virtual void AllocateOutputs();
  virtual void GenerateOutputInformation();
  virtual void GenerateInputRequestedRegion();


  /** Direction in which the filter is to be applied
   * this should be in the range [0,ImageDimension-1]. */
  unsigned int m_Direction;

  typedef RegionOfInterestImageFilter< TInputImage, TInputImage > ROIFilterType;

  typename ROIFilterType::Pointer m_ROIFilter;


private:
  MeanAcrossDirection( const Self& ); // purposely not implemented
  void operator=( const Self& ); // purposely not implemented

};

} // namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkMeanAcrossDirection.txx"
#endif

#endif // inclusion guard


