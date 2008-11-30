#ifndef  __itkFrequencyVectorImageFilter_h
#define  __itkFrequencyVectorImageFilter_h

#include "itkComplexToModulusImageFilter.h"
#include "itkFFTW1DRealToComplexConjugateImageFilter.h"
#include "itkHammingWindowImageFilter.h"
#include "itkImageToImageFilter.h"
#include "itkSquareImageFilter.h"
#include "itkRegionOfInterestImageFilter.h"
#include "itkVectorImage.h"

namespace itk
{
  /** @class FrequencyVectorImageFilter
   * @brief Create a VectorImage where each pixel contains a vector of frequency components.
   *
   */

template < class TInputImage >
class FrequencyVectorImageFilter :
  public ImageToImageFilter<TInputImage,
    VectorImage< ITK_TYPENAME TInputImage::InternalPixelType,
    itk::GetImageDimension<TInputImage>::ImageDimension> >
{
public:
  /** Standard class typedefs. */
  typedef FrequencyVectorImageFilter Self;
  typedef ImageToImageFilter<TInputImage,
    VectorImage< ITK_TYPENAME TInputImage::InternalPixelType,
    itk::GetImageDimension<TInputImage>::ImageDimension> >
      Superclass;
  typedef SmartPointer<Self>  Pointer;
  typedef SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(FrequencyVectorImageFilter, ImageToImageFilter);

  /**Typedefs from the superclass */
  typedef typename Superclass::InputImageType  InputImageType;
  typedef typename Superclass::OutputImageType OutputImageType;

  itkStaticConstMacro(Dimension, unsigned int, TInputImage::ImageDimension);

  /** The pixel type of the output image will be used in computations.
   * Inherited from the superclass. */
  typedef typename InputImageType::PixelType   InputPixelType;
  typedef typename OutputImageType::PixelType OutputPixelType;
  typedef typename Superclass::InputImageRegionType InputRegionType;
  typedef typename Superclass::OutputImageRegionType OutputRegionType;


  /** Get the direction in which the filter is to be applied. */
  itkGetMacro(Direction, unsigned int);
  /** Set the direction in which the filter is to be applied. */
  itkSetMacro(Direction, unsigned int);

  /** Get the size of the FFT window used to calculate the frequencies. */
  itkGetMacro(FFTSize, unsigned int);
  /** Set the size of the FFT window used to calculate the frequencies. */
  itkSetMacro(FFTSize, unsigned int);

  /** Get the percent overlap between the FFT windows.  Range [0.0-1.0). */
  itkGetMacro(FFTOverlap, float);
  /** Set the percent overlap between the FFT windows.  Range [0.0-1.0). */
  itkSetMacro(FFTOverlap, float);

  /** Get the starting frequency point for extraction. */
  itkGetMacro(FrequencyExtractStartIndex, unsigned int);
  /** Set the starting frequency point for extraction. */
  itkSetMacro(FrequencyExtractStartIndex, unsigned int);

  /** Get the number of frequency points to extract starting from the FrequencyStartIndex. */
  itkGetMacro(FrequencyExtractSize, unsigned int);
  /** Set the number of frequency points to extract starting from the FrequencyStartIndex. */
  itkSetMacro(FrequencyExtractSize, unsigned int);

  /** @todo -- threadify */
  virtual void GenerateData();  // generates output from input
protected:
  FrequencyVectorImageFilter();
  virtual ~FrequencyVectorImageFilter(){};
  void PrintSelf(std::ostream& os, Indent indent) const;

  // inherited from ImageSource, step in threaded execution
  virtual void AllocateOutputs();
  virtual void GenerateOutputInformation();
  virtual void GenerateInputRequestedRegion();


  /** Direction in which the filter is to be applied
   * this should be in the range [0,ImageDimension-1]. */
  unsigned int m_Direction;
  /** Size of the FFT window used to calculate the frequencies. */
  unsigned int m_FFTSize;
  /** Percent overlap between the FFT windows. Range [0.0-1.0)*/
  float m_FFTOverlap;
  /** The frequency point to start extraction from. */
  unsigned int m_FrequencyExtractStartIndex;
  /** The number of frequency points to extract starting from m_FrequencyStartIndex. */
  unsigned int m_FrequencyExtractSize;

  typedef RegionOfInterestImageFilter< TInputImage, TInputImage > ROIFilterType;
  typedef HammingWindowImageFilter< TInputImage, TInputImage > WindowType;
  typedef FFTW1DRealToComplexConjugateImageFilter< InputPixelType, Dimension > FFT1DFilterType;
  typedef typename FFT1DFilterType::OutputImageType ComplexType;
  /** @todo use a filter that does re^2+im^2 instead of (sqrt(re^2+im^2))^2 */
  typedef ComplexToModulusImageFilter< ComplexType, TInputImage > ModulusFilterType;
  typedef SquareImageFilter< TInputImage, TInputImage > SquareFilterType;

  typename ROIFilterType::Pointer m_ROIFilter;
  typename WindowType::Pointer m_WindowFilter;
  typename FFT1DFilterType::Pointer m_FFT1DFilter;
  typename ModulusFilterType::Pointer m_ModulusFilter;
  typename SquareFilterType::Pointer m_SquareFilter;


private:
  FrequencyVectorImageFilter( const Self& ); // purposely not implemented
  void operator=( const Self& ); // purposely not implemented

};

} // namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkFrequencyVectorImageFilter.txx"
#endif

#endif // inclusion guard


