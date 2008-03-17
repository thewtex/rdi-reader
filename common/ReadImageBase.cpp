#include "ReadImageBase.h"



#include <vector>
#include <stdexcept> // out_of_range, ios_base::failure


#include <boost/filesystem/convenience.hpp>
namespace bf = boost::filesystem;


#include "ReadMetadataBase.h"


using namespace visual_sonics;



ReadImageBase::ReadImageBase( const bf::path& in_file_path, const bf::path& in_file_name, std::vector<unsigned int>& frames_to_read ): 
      its_frames_to_read( frames_to_read ),
      its_frames_to_read_ind( 0 ),
      its_frame_status( true ),
      its_read_method ( file_average )
{
  this->create_its_read_metadata( in_file_path, in_file_name );
  its_rdb_file_path = its_read_metadata->its_in_file_path / (its_read_metadata->its_in_file_name.leaf() + ".rdb");
  this->check_if_frames_valid();
}



ReadImageBase::ReadImageBase( const bf::path& in_file_path, const bf::path& in_file_name, std::vector<unsigned int>& frames_to_read, ReadMethod read_method , unsigned int specific_acquisition): 
      its_frames_to_read( frames_to_read ),
      its_frames_to_read_ind( 0 ),
      its_frame_status( true ),
      its_read_method(read_method), 
      its_specific_acquisition( specific_acquisition )
{
  this->create_its_read_metadata( in_file_path, in_file_name );
  its_rdb_file_path = its_read_metadata->its_in_file_path / (its_read_metadata->its_in_file_name.leaf() + ".rdb");
  this->check_if_frames_valid();
}



ReadImageBase::ReadImageBase( const bf::path& in_file_path, const bf::path& in_file_name, ReadMethod read_method, unsigned int specific_acquisition ):
      its_frames_to_read_ind( 0 ),
      its_frame_status( true ),
      its_read_method( read_method ), 
      its_specific_acquisition( specific_acquisition )
{
  this->create_its_read_metadata( in_file_path, in_file_name );
  its_rdb_file_path = its_read_metadata->its_in_file_path / (its_read_metadata->its_in_file_name.leaf() + ".rdb");
  this->read_all_frames();
}



ReadImageBase::ReadImageBase( const bf::path& in_file_path, const bf::path& in_file_name ):
      its_frames_to_read_ind( 0 ),
      its_frame_status( true ),
      its_read_method ( file_average )
{
  this->create_its_read_metadata( in_file_path, in_file_name );
  its_rdb_file_path = its_read_metadata->its_in_file_path / (its_read_metadata->its_in_file_name.leaf() + ".rdb");
  this->read_all_frames();
}



ReadImageBase::~ReadImageBase()
{
  delete its_read_metadata;
}



void ReadImageBase::create_its_read_metadata(const bf::path& in_file_path, const bf::path& in_file_name)
{
  its_read_metadata = new ReadMetadataBase( in_file_path, in_file_name);
}



void ReadImageBase::check_if_frames_valid()
{
  // check if specified frames are valid
  unsigned int max_frame = its_read_metadata->its_rpd.its_image_frames;
  for( std::vector<unsigned int>::const_iterator it = its_frames_to_read.begin(); it < its_frames_to_read.end(); it++)
  {
    if( *it < 1 || *it > max_frame )
    {
      throw std::out_of_range(" specified frame is out of range ");
    }
  }
}



void ReadImageBase::read_all_frames()
{
  // default is to read all frames
  unsigned int max_frame = its_read_metadata->its_rpd.its_image_frames;
  its_frames_to_read.resize( max_frame );
  for( unsigned int i = 0; i < max_frame; i++ )
  {
    its_frames_to_read[i] = i+1;
  }
}
