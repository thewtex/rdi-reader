#include "cxx/ReadImage.h"


#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>


#include <boost/filesystem/convenience.hpp>
namespace bf = boost::filesystem;


#include "rdiParserData.h"
#include "ReadMetadataBase.h"
#include "sized_ints.h"


using namespace visual_sonics::cxx;


ReadImage::ReadImage(const bf::path& in_file_path, const bf::path& in_file_name, std::vector<unsigned int>&  frames_to_read): 
  ReadImageBase( in_file_path, in_file_name, frames_to_read )
{
}



ReadImage::ReadImage(const bf::path& in_file_path, const bf::path& in_file_name, std::vector<unsigned int>&  frames_to_read, ReadMethod read_method, unsigned int specific_acquisition): 
  ReadImageBase( in_file_path, in_file_name, frames_to_read, read_method, specific_acquisition )
{
}



ReadImage::ReadImage( const bf::path& in_file_path, const bf::path& in_file_name, ReadMethod read_method, unsigned int specific_acquisition ):
  ReadImageBase( in_file_path, in_file_name, read_method, specific_acquisition )
{
}



ReadImage::ReadImage(const bf::path& in_file_path, const bf::path& in_file_name):
  ReadImageBase( in_file_path, in_file_name )
{
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
  
  
  const unsigned int samples_per_line = this->its_read_metadata->its_rpd->its_rf_mode_rx_ad_gate_width;
  const unsigned int num_lines = this->its_read_metadata->its_rpd->its_rf_mode_tx_trig_tbl_trigs;
  its_b_mode_image.resize( samples_per_line * num_lines );


  char * u_short_data = new char[2];
  UInt16 * u_short_data_p = reinterpret_cast< UInt16 *> (u_short_data);
  for( unsigned int i = 0; i < num_lines; i++)
  {
    for( unsigned int j = 0; j < samples_per_line; j++)
    {
      rdb_file.read(u_short_data, 2);
      its_b_mode_image[ i*samples_per_line + j ] = *u_short_data_p;
    }
  }

  its_b_mode_max = *std::max_element( its_b_mode_image.begin(), its_b_mode_image.end() );
  its_b_mode_min = *std::min_element( its_b_mode_image.begin(), its_b_mode_image.end() );
  

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



