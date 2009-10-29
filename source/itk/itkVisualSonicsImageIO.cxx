#include "itkVisualSonicsImageIO.h"

#include <fstream>

#include "itkMetaDataObject.h"

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
    m_rdiReader = std::auto_ptr< rdiReader >( new rdiReader( this->m_FileName.c_str() ) );
    m_rdi = m_rdiReader->parse_to_rdi_t();
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
    1540.0 / 2.0 / 1.0e6 /
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

  itk::MetaDataDictionary& thisMetaDict = this->GetMetaDataDictionary();

  itk::EncapsulateMetaData< double >( thisMetaDict, "Radius",
    static_cast< double >(
      ( m_rdi->image_parameters().RF_Mode().ActiveProbe().Pivot_Transducer_Face_Dist() +
      m_rdi->image_parameters().RF_Mode().RX().V_Delay_Length() )
      * 0.001
      )
  );


}


void
VisualSonicsImageIO::
Read
( void* buffer )
{

  string rdbFilename = m_FileName.substr( 0, m_FileName.length() - 4 ) + ".rdb" ;
  std::ifstream inputStream( rdbFilename.c_str(), std::ios::in | std::ios::binary );
  if( !inputStream.is_open() )
    itkExceptionMacro( << " File: " << rdbFilename << " could not be opened. " );

  // skip the b mode and saturation scout images
  const unsigned int scoutSamplesPerLine = m_rdi->image_parameters().RF_Mode().RX().AD_Gate_Width();
  const unsigned int scoutNumLines       = m_rdi->image_parameters().RF_Mode().TX().Trig_Tbl_Trigs();
  inputStream.seekg( 2 * scoutSamplesPerLine * scoutNumLines * sizeof(unsigned short),
    std::ios::beg );

  size_t skip_amount = 0;
  if( m_rdi->image_info().Image_Acquisition_Per_Line() > 1 )
    skip_amount = m_Dimensions[0] * sizeof( short ) * m_rdi->image_info().Image_Acquisition_Per_Line();

  inputStream.seekg( skip_amount, std::ios::cur );

  const size_t bytesPerLine = this->m_Dimensions[0]*sizeof(short);
  unsigned int line = 0;
  char*  data_as_char = reinterpret_cast< char* >( buffer );

  for( unsigned int frame = 0; frame < m_Dimensions[2]; frame++ )
    {
    for( line = 0; line < m_Dimensions[1]; line++ )
      {
      inputStream.read( data_as_char, bytesPerLine );
      data_as_char += bytesPerLine;
      inputStream.seekg( skip_amount, std::ios::cur );
      }
    }

  inputStream.close();
}


void
VisualSonicsImageIO::
PrintSelf( std::ostream& os, Indent indent ) const
{
  Superclass::PrintSelf( os, indent );
}

} // end namespace itk
