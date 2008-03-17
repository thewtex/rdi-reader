#include "ReadImage.h"


#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept> // out_of_range, ios_base::failure
#include <vector>


#include <boost/filesystem/convenience.hpp>
namespace bf = boost::filesystem;


#include "ReadMetadataBase.h"

#include "sized_ints.h"


using namespace visual_sonics::cxx;


ReadImage::ReadImage(const bf::path& in_file_path, const bf::path& in_file_name, std::vector<unsigned int>&  frames_to_read): ReadImageBase( frames_to_read )
{
  its_read_metadata = new ReadMetadataBase( in_file_path, in_file_name);

  its_rdb_file_path = its_read_metadata->its_in_file_path / (its_read_metadata->its_in_file_name.leaf() + ".rdb");


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


ReadImage::ReadImage(const bf::path& in_file_path, const bf::path& in_file_name) : ReadImageBase()
{
  its_read_metadata = new ReadMetadataBase( in_file_path, in_file_name);

  its_rdb_file_path = its_read_metadata->its_in_file_path / (its_read_metadata->its_in_file_name.leaf() + ".rdb");


  // default is to read all frames
  unsigned int max_frame = its_read_metadata->its_rpd.its_image_frames;
  its_frames_to_read.resize( max_frame );
  for( unsigned int i = 0; i < max_frame; i++ )
  {
    its_frames_to_read[i] = i+1;
  }

}


ReadImage::~ReadImage()
{
  delete its_read_metadata;
}



void ReadImage::read_b_mode_image()
{
  std::ifstream rdb_file( its_rdb_file_path.native_file_string().c_str(), std::ios::in | std::ios::binary);
  if (!rdb_file.is_open())
  {
    std::ostringstream err_msg (std::ostringstream::out);
    err_msg << "\nFile: " << its_rdb_file_path.native_file_string() << " couldn't be opened :(\n";
    throw std::ios_base::failure( err_msg.str() );
  }
  
  
  const unsigned int samples_per_line = its_read_metadata->its_rpd.its_rf_mode_rx_ad_gate_width;
  const unsigned int num_lines = its_read_metadata->its_rpd.its_rf_mode_tx_trig_tbl_trigs;
  its_b_mode_image.resize( samples_per_line * num_lines );


  char * u_short_data = new char[2];
  UInt16 * u_short_data_p = reinterpret_cast< UInt16 *> (u_short_data);
  for( unsigned int i = 0; i < num_lines; i++)
  {
    for( unsigned int j = 0; j < samples_per_line; j++)
    {
      rdb_file.read(u_short_data, sizeof(unsigned short));
      its_b_mode_image[ i*samples_per_line + j ] = *u_short_data_p;
    }
  }

  

  delete[] u_short_data;
  rdb_file.close();

}



void ReadImage::read_saturation_image()
{
}



bool ReadImage::read_rf_data_image()
{
  return true;
}



