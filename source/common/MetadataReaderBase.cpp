#include "MetadataReaderBase.h"

#include "rdiParser.h"
#include "rdiParserData.h"


visual_sonics::MetadataReaderBase::MetadataReaderBase(const bf::path& in_file_path):
		its_in_file_path( in_file_path )
{
  its_rpd = new rdiParserData;
  this->parse_metadata();
}


void visual_sonics::MetadataReaderBase::set_in_file_path( const bf::path& ifp )
{
  its_in_file_path = ifp;
  this->parse_metadata();
};

visual_sonics::MetadataReaderBase::~MetadataReaderBase()
{
  delete its_rpd;
}


void visual_sonics::MetadataReaderBase::parse_metadata()
{
  rdiParser rdi_parser( its_in_file_path.native_file_string() );
  *its_rpd = rdi_parser.parse();
}

