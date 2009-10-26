#include "itkVisualSonicsImageIOFactory.h"

#include "itkCreateObjectFunction.h"
#include "itkVisualSonicsImageIO.h"
#include "itkVersion.h"

namespace itk
{

VisualSonicsImageIOFactory::
VisualSonicsImageIOFactory()
{
  this->RegisterOverride("itkImageIOBase",
    "itkVisualSonicsImageIO",
    "VisualSonics Image IO",
    1,
    CreateObjectFunction< VisualSonicsImageIO >::New());
}

VisualSonicsImageIOFactory::
~VisualSonicsImageIOFactory()
{
}

const char*
VisualSonicsImageIOFactory::GetITKSourceVersion(void) const
{
  return ITK_SOURCE_VERSION;
}

const char*
VisualSonicsImageIOFactory::GetDescription() const
{
  return "VisualSonics ImageIO Factory, allows the loading of VisualSonics images into Insight";
}

} // end namespace itk


