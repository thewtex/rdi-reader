#ifndef __itkFFTW1DComplexConjugateToRealImageFilter_h
#define __itkFFTW1DComplexConjugateToRealImageFilter_h

#include "itkFFTComplexConjugateToRealImageFilter.h"

//
// FFTWCommon defines proxy classes based on data types
#include "itkFFTWCommon.h"


namespace itk
{

template <typename TPixel,unsigned int Dimension>
class FFTW1DComplexConjugateToRealImageFilter :
    public FFTComplexConjugateToRealImageFilter<TPixel,Dimension>
{
public:
  typedef FFTW1DComplexConjugateToRealImageFilter Self;
  typedef FFTComplexConjugateToRealImageFilter<TPixel,Dimension> Superclass;
  typedef SmartPointer<Self> Pointer;
  typedef SmartPointer<const Self> ConstPointer;
  //
  // the proxy type is a wrapper for the fftw API
  // since the proxy is only defined over double and float,
  // trying to use any other pixel type is inoperative, as
  // is trying to use double if only the float FFTW1D version is
  // configured in, or float if only double is configured.
  //
  typedef typename fftw::Proxy<TPixel> FFTW1DProxyType;

  /** Standard class typedefs.*/
  typedef typename Superclass::TInputImageType TInputImageType;
  typedef typename Superclass::TOutputImageType TOutputImageType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(FFTW1DComplexConjugateToRealImageFilter,
               FFTComplexConjugateToRealImageFilter);

  //! @todo put these in an abstract base class so that a vnl fft 1D can derive too
  /** Get the direction in which the filter is to be applied. */
  itkGetMacro(Direction, unsigned int);

  /** Set the direction in which the filter is to be applied. */
  itkSetMacro(Direction, unsigned int);

  /** Image type typedef support. */
  typedef TInputImageType ImageType;
  typedef typename ImageType::SizeType ImageSizeType;

  //
  // these should be defined in every FFT filter class
  virtual void GenerateData();  // generates output from input
  virtual bool FullMatrix();
protected:
  FFTW1DComplexConjugateToRealImageFilter() : m_PlanComputed(false),
                                            m_LastImageSize(0),
                                            m_InputBuffer(0),
                                            m_OutputBuffer(0),
					    m_Direction(0)
  {
  }
  virtual ~FFTW1DComplexConjugateToRealImageFilter(){
    if(m_PlanComputed)
      {
      FFTW1DProxyType::DestroyPlan(m_Plan);
      delete [] m_InputBuffer;
      delete [] m_OutputBuffer;
      }
  }
  virtual void GenerateOutputInformation(); // figure out allocation for output image
  virtual void GenerateInputRequestedRegion();
  virtual void EnlargeOutputRequestedRegion(DataObject *output);

private:
  FFTW1DComplexConjugateToRealImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
  bool m_PlanComputed;
  typename FFTW1DProxyType::PlanType m_Plan;
  unsigned int m_LastImageSize;
  // local storage needed to keep fftw from scribbling on
  typename FFTW1DProxyType::ComplexType *m_InputBuffer;
  TPixel *m_OutputBuffer;

  /** Direction in which the filter is to be applied
   * this should be in the range [0,ImageDimension-1]. */
  unsigned int m_Direction;
};


} // namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkFFTW1DComplexConjugateToRealImageFilter.txx"
#endif

#endif //__itkFFTW1DComplexConjugateToRealImageFilter_h
