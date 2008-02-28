#include "ExtractMetadataBase.h"

#include "rdiParser.h"


visual_sonics::ExtractMetadataBase::ExtractMetadataBase(const bf::path& in_file_path, 
		      const bf::path& in_file_name):
		its_in_file_path( in_file_path ),
		its_in_file_name( in_file_name )
{
  parse_metadata();
}



void visual_sonics::ExtractMetadataBase::parse_metadata()
{
  rdiParser rdi_parser( (its_in_file_path / its_in_file_name).native_file_string() );
  its_rpd = rdi_parser.parse();
}

