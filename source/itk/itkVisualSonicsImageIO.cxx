#include "itkVisualSonicsImageIO.h"

#include <fstream>

#include "rdiReader.h"

namespace itk
{

VisualSonicsImageIO::VisualSonicsImageIO()
{
  this->AddSupportedReadExtension(".rdi");
}

VisualSonicsImageIO::~VisualSonicsImageIO()
{
}

bool
VisualSonicsImageIO::CanReadFile
( const char* FileNameToRead )
{
  // First check the filename extension
  std::string filename(FileNameToRead);

  if ( filename == "" )
    {
    itkDebugMacro(<< "No filename specified.");
    }

  std::string::size_type VisualSonicsPos = filename.rfind(".rdi");
  if ((VisualSonicsPos == std::string::npos)
      || !(VisualSonicsPos == filename.length() - 4))
    {
    itkDebugMacro(<<"The filename extension is not recognized");
    return false;
    }

  // Now check the content
  std::ifstream inputStream;
  inputStream.open( FileNameToRead, std::ios::in | std::ios::binary );
  if( inputStream.fail() )
    {
    return false;
    }

  const static char* imageInfo = "\"=== IMAGE INFO ===\"";
  char fileFirstLine[21];
  fileFirstLine[20] = 0;
  inputStream.read( fileFirstLine, 20*sizeof(char) );
  for( int i=0; i < 20; i++ )
    {
    if( fileFirstLine[i] != imageInfo[i] )
      {
      itkDebugMacro(<<"Unexpected rdi file content.");
      inputStream.close();
      return false;
      }
    }
  inputStream.close();
  return true;
}


void
VisualSonicsImageIO::
ReadImageInformation()
{
  try
    {
    rdiReader rdi_reader( this->m_FileName.c_str() );
    m_rdi = rdi_reader.parse_to_rdi_t();
    }
  catch( const std::exception& e )
    {
    itkExceptionMacro( << e.what() );
    }

  this->SetNumberOfDimensions(3);
  this->m_Dimensions[0] = m_rdi->image_info().Image_Acquisition_Size() / sizeof( signed short );
  this->m_Dimensions[1] = m_rdi->image_info().Image_Lines();
  this->m_Dimensions[2] = m_rdi->image_info().Image_Frames();
  this->m_ComponentType = SHORT;
  this->m_PixelType = SCALAR;
  this->SetSpacing( 0,
    1540.0 / 2.0 / 10.0e6 /
    static_cast< double >( m_rdi->image_parameters().RF_Mode().RfModeSoft().SamplesPerSec() )
  );
  this->SetSpacing( 1,
    static_cast< double >(
      m_rdi->image_parameters().RF_Mode().Scan().Scan_Width() /
      m_rdi->image_parameters().RF_Mode().ActiveProbe().Pivot_Encoder_Dist() /
      m_rdi->image_info().Image_Lines() *
      ( m_rdi->image_parameters().RF_Mode().ActiveProbe().Pivot_Transducer_Face_Dist() + m_rdi->image_parameters().RF_Mode().RX().V_Delay_Length() )
      * 0.001
    )
  );
  this->SetSpacing( 2,
    static_cast< double >( m_rdi->image_parameters().RF_Mode().X_3D().Step_Size() ) * 0.001
  );
}


void
VisualSonicsImageIO::
Read
( void* buffer )
{
}


void
VisualSonicsImageIO::
PrintSelf( std::ostream& os, Indent indent ) const
{
  Superclass::PrintSelf( os, indent );
}

} // end namespace itk
