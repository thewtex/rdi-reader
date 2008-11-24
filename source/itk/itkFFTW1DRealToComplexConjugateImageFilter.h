#ifndef __itkFFTW1DRealToComplexConjugateImageFilter_h
#define __itkFFTW1DRealToComplexConjugateImageFilter_h

#include "itkFFTRealToComplexConjugateImageFilter.h"
#include "itkFFTWCommon.h"


namespace itk
{
/** /class FFTW1DRealToComplexConjugateImageFilter
 * /brief only do FFT along one dimension
 *
 * \ingroup
 */

template <class TPixel, unsigned int Dimension = 3>
class ITK_EXPORT FFTW1DRealToComplexConjugateImageFilter :
    public FFTRealToComplexConjugateImageFilter<TPixel,Dimension>
{
public:
  typedef FFTW1DRealToComplexConjugateImageFilter Self;
  typedef FFTRealToComplexConjugateImageFilter<TPixel,Dimension> Superclass;
  typedef SmartPointer<Self> Pointer;
  typedef SmartPointer<const Self> ConstPointer;

  /** Standard class typedefs.*/
  typedef typename Superclass::TInputImageType TInputImageType;
  typedef typename Superclass::TOutputImageType TOutputImageType;

  /**
   * the proxy type is a wrapper for the fftw API
   * since the proxy is only defined over double and float,
   * trying to use any other pixel type is inoperative, as
   * is trying to use double if only the float FFTW1D version is
   * configured in, or float if only double is configured.
   */
  typedef typename fftw::Proxy<TPixel> FFTW1DProxyType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(FFTW1DRealToComplexConjugateImageFilter,
               FFTRealToComplexConjugateImageFilter);

  //! @todo put these in an abstract base class so that a vnl fft 1D can derive too
  /** Get the direction in which the filter is to be applied. */
  itkGetMacro(Direction, unsigned int);

  /** Set the direction in which the filter is to be applied. */
  itkSetMacro(Direction, unsigned int);

  //
  //
  // these should be defined in every FFT filter class
  virtual void GenerateData();  // generates output from input

protected:
  FFTW1DRealToComplexConjugateImageFilter() : m_PlanComputed(false),
                                            m_LastImageSize(0),
                                            m_InputBuffer(0),
                                            m_OutputBuffer(0),
					    m_Direction(0)
  {
  }
  virtual ~FFTW1DRealToComplexConjugateImageFilter()
  {
    if(m_PlanComputed)
      {
      FFTW1DProxyType::DestroyPlan(this->m_Plan);
      delete [] this->m_InputBuffer;
      delete [] this->m_OutputBuffer;
      }
  }

  virtual void GenerateOutputInformation(); // figure out allocation for output image
  virtual void GenerateInputRequestedRegion();
  virtual void EnlargeOutputRequestedRegion(DataObject *output);
  virtual bool FullMatrix();
private:
  FFTW1DRealToComplexConjugateImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
  bool m_PlanComputed;
  typename FFTW1DProxyType::PlanType m_Plan;
  unsigned int m_LastImageSize;
  TPixel *m_InputBuffer;
  typename FFTW1DProxyType::ComplexType *m_OutputBuffer;

  /** Direction in which the filter is to be applied
   * this should be in the range [0,ImageDimension-1]. */
  unsigned int m_Direction;

};
} // namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkFFTW1DRealToComplexConjugateImageFilter.txx"
#endif

#endif //__itkFFTW1DRealToComplexConjugateImageFilter_h
