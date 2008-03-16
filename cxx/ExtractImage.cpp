#include "ExtractImage.h"


#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept> // out_of_range, ios_base::failure
#include <vector>


#include <boost/filesystem/convenience.hpp>
namespace bf = boost::filesystem;


#include "ExtractMetadataBase.h"


using namespace visual_sonics::cxx;


ExtractImage::ExtractImage(const bf::path& in_file_path, const bf::path& in_file_name, std::vector<unsigned int>&  frames_to_extract): ExtractImageBase( frames_to_extract )
{
  its_extract_metadata = new ExtractMetadataBase( in_file_path, in_file_name);

  its_rdb_file_path = its_extract_metadata->its_in_file_path / (its_extract_metadata->its_in_file_name.leaf() + ".rdb");


  // check if specified frames are valid
  unsigned int max_frame = its_extract_metadata->its_rpd.its_image_frames;
  for( std::vector<unsigned int>::const_iterator it = its_frames_to_extract.begin(); it < its_frames_to_extract.end(); it++)
  {
    if( *it < 1 || *it > max_frame )
    {
      throw std::out_of_range(" specified frame is out of range ");
    }
  }


}


ExtractImage::ExtractImage(const bf::path& in_file_path, const bf::path& in_file_name) : ExtractImageBase()
{
  its_extract_metadata = new ExtractMetadataBase( in_file_path, in_file_name);

  its_rdb_file_path = its_extract_metadata->its_in_file_path / (its_extract_metadata->its_in_file_name.leaf() + ".rdb");


  // default is to extract all frames
  unsigned int max_frame = its_extract_metadata->its_rpd.its_image_frames;
  its_frames_to_extract.resize( max_frame );
  for( unsigned int i = 0; i < max_frame; i++ )
  {
    its_frames_to_extract[i] = i+1;
  }

}


ExtractImage::~ExtractImage()
{
  delete its_extract_metadata;
}



void ExtractImage::extract_b_mode_image()
{
  std::ifstream rdb_file( its_rdb_file_path.native_file_string().c_str(), std::ios::in | std::ios::binary);
  if (!rdb_file.is_open())
  {
    std::ostringstream err_msg (std::ostringstream::out);
    err_msg << "\nFile: " << its_rdb_file_path.native_file_string() << " couldn't be opened :(\n";
    throw std::ios_base::failure( err_msg.str() );
  }
  
  
  const unsigned int samples_per_line = its_extract_metadata->its_rpd.its_rf_mode_rx_ad_gate_width;
  const unsigned int num_lines = its_extract_metadata->its_rpd.its_rf_mode_tx_trig_tbl_trigs;
  its_b_mode_image.resize( samples_per_line * num_lines );


  char * u_short_data = new char[ sizeof(unsigned int) ];
  unsigned short * u_short_data_p = reinterpret_cast< unsigned short *> (u_short_data);
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



void ExtractImage::extract_saturation_image()
{
}



void ExtractImage::extract_rf_data_image()
{
}



