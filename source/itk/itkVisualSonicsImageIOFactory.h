#ifndef __itkVisualSonicsImageIOFactory_h
#define __itkVisualSonicsImageIOFactory_h

#ifdef _MSC_VER
#pragma warning ( disable : 4786 )
#endif

#include "itkObjectFactoryBase.h"
#include "itkImageIOBase.h"

namespace itk
{
/** \class VisualSonicsImageIOFactory
   * \brief Create instances of VisualSonicsImageIO objects using an object factory.
   */
class ITK_EXPORT VisualSonicsImageIOFactory : public ObjectFactoryBase
{
public:
  /** Standard class typedefs. */
  typedef VisualSonicsImageIOFactory	   Self;
  typedef ObjectFactoryBase        Superclass;
  typedef SmartPointer<Self>       Pointer;
  typedef SmartPointer<const Self> ConstPointer;

  /** Class methods used to interface with the registered factories. */
  virtual const char* GetITKSourceVersion(void) const;
  virtual const char* GetDescription(void) const;

  /** Method for class instantiation. */
  itkFactorylessNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(VisualSonicsImageIOFactory, ObjectFactoryBase);

  /** Register one factory of this type  */
  static void RegisterOneFactory(void)
    {
    VisualSonicsImageIOFactory::Pointer metaFactory = VisualSonicsImageIOFactory::New();
    ObjectFactoryBase::RegisterFactory(metaFactory);
    }

protected:
  VisualSonicsImageIOFactory();
  ~VisualSonicsImageIOFactory();

private:
  VisualSonicsImageIOFactory(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

};


} // end namespace itk

#endif
