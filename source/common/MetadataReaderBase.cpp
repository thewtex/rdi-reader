#include "MetadataReaderBase.h"

#include "rdiParser.h"
#include "rdiParserData.h"


visual_sonics::MetadataReaderBase::MetadataReaderBase(const bf::path& in_file_path,
		      const bf::path& in_file_name):
		its_in_file_path( in_file_path ),
		its_in_file_name( in_file_name )
{
  its_rpd = new rdiParserData;
  parse_metadata();
}



visual_sonics::MetadataReaderBase::~MetadataReaderBase()
{
  delete its_rpd;
}


void visual_sonics::MetadataReaderBase::parse_metadata()
{
  rdiParser rdi_parser( (its_in_file_path / its_in_file_name).native_file_string() );
  *its_rpd = rdi_parser.parse();
}

