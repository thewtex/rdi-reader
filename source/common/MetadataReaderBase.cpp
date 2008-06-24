#include "common/MetadataReaderBase.h"

#include "common/spirit_parse/rdiParser.h"
#include "common/spirit_parse/rdiParserData.h"

#include <string>

visual_sonics::MetadataReaderBase::MetadataReaderBase(const bf::path& in_file_path):
		its_in_file_path( in_file_path )
{
  std::string file_path = in_file_path.native_file_string();
  // check for .rdi or .rdb extension and remove if present
  if ( file_path.find( ".rdi", file_path.size()-4 ) != std::string::npos ||
	    file_path.find( ".rdb", file_path.size()-4 ) != std::string::npos )
  {
	file_path = file_path.substr( 0, file_path.size()-4);
	its_in_file_path =  bf::path( file_path );
  }
  its_rpd = new rdiParserData;
  this->parse_metadata();
}


void visual_sonics::MetadataReaderBase::set_in_file_path( const bf::path& ifp )
{
  std::string file_path = ifp.native_file_string();
  // check for .rdi or .rdb extension and remove if present
  if ( file_path.find( ".rdi", file_path.size()-4 ) != std::string::npos ||
	    file_path.find( ".rdb", file_path.size()-4 ) != std::string::npos )
  {
	file_path = file_path.substr( 0, file_path.size()-4);
	its_in_file_path =  bf::path( file_path );
  }
  else
  {
    its_in_file_path = ifp;
  }
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

