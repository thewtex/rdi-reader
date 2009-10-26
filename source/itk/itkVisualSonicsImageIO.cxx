#include "itkVisualSonicsImageIO.h"

#include <fstream>

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
