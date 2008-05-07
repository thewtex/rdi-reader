#include "ImageReaderBase.h"



#include <vector>
#include <stdexcept> // out_of_range, ios_base::failure


#include <boost/filesystem/convenience.hpp>
namespace bf = boost::filesystem;


#include "MetadataReaderBase.h"
#include "rdiParserData.h"


using namespace visual_sonics;



ImageReaderBase::ImageReaderBase( const bf::path& in_file_path, const bf::path& in_file_name, std::vector<unsigned int>& frames_to_read ):
      its_frames_to_read( frames_to_read ),
      its_frames_to_read_ind( 0 ),
      its_frame_status( true ),
      its_read_method ( file_average )
{
  create_its_metadata_reader( in_file_path, in_file_name );
  its_rdb_file_path = its_metadata_reader->its_in_file_path / (its_metadata_reader->its_in_file_name.leaf() + ".rdb");
  check_if_frames_valid();
}



ImageReaderBase::ImageReaderBase( const bf::path& in_file_path, const bf::path& in_file_name, std::vector<unsigned int>& frames_to_read, ReadMethod read_method , unsigned int specific_acquisition):
      its_frames_to_read( frames_to_read ),
      its_frames_to_read_ind( 0 ),
      its_frame_status( true ),
      its_read_method(read_method),
      its_specific_acquisition( specific_acquisition )
{
  create_its_metadata_reader( in_file_path, in_file_name );
  its_rdb_file_path = its_metadata_reader->its_in_file_path / (its_metadata_reader->its_in_file_name.leaf() + ".rdb");
  check_if_frames_valid();
}



ImageReaderBase::ImageReaderBase( const bf::path& in_file_path, const bf::path& in_file_name, ReadMethod read_method, unsigned int specific_acquisition ):
      its_frames_to_read_ind( 0 ),
      its_frame_status( true ),
      its_read_method( read_method ),
      its_specific_acquisition( specific_acquisition )
{
  create_its_metadata_reader( in_file_path, in_file_name );
  its_rdb_file_path = its_metadata_reader->its_in_file_path / (its_metadata_reader->its_in_file_name.leaf() + ".rdb");
  read_all_frames();
}



ImageReaderBase::ImageReaderBase( const bf::path& in_file_path, const bf::path& in_file_name ):
      its_frames_to_read_ind( 0 ),
      its_frame_status( true ),
      its_read_method ( file_average )
{
  create_its_metadata_reader( in_file_path, in_file_name );
  its_rdb_file_path = its_metadata_reader->its_in_file_path / (its_metadata_reader->its_in_file_name.leaf() + ".rdb");
  read_all_frames();
}



ImageReaderBase::~ImageReaderBase()
{
  delete its_metadata_reader;
}



void ImageReaderBase::set_in_file_path( const bf::path& ifp )
{
  this->its_metadata_reader->set_in_file_path(ifp);
}



void ImageReaderBase::set_in_file_name( const bf::path& ifn )
{
  this->its_metadata_reader->set_in_file_name(ifn);
}



const rdiParserData* get_rpd()
{
  return this->its_metadata_reader->its_rpd;
}




void ImageReaderBase::create_its_metadata_reader(const bf::path& in_file_path, const bf::path& in_file_name)
{
  its_metadata_reader = new MetadataReaderBase( in_file_path, in_file_name);
}



void ImageReaderBase::check_if_frames_valid()
{
  // check if specified frames are valid
  unsigned int max_frame = its_metadata_reader->its_rpd->its_image_frames;
  for( std::vector<unsigned int>::const_iterator it = its_frames_to_read.begin(); it < its_frames_to_read.end(); it++)
  {
    if( *it < 1 || *it > max_frame )
    {
      throw std::out_of_range(" specified frame is out of range ");
    }
  }
}



void ImageReaderBase::read_all_frames()
{
  // default is to read all frames
  unsigned int max_frame = its_metadata_reader->its_rpd->its_image_frames;
  its_frames_to_read.resize( max_frame );
  for( unsigned int i = 0; i < max_frame; i++ )
  {
    its_frames_to_read[i] = i+1;
  }
}
