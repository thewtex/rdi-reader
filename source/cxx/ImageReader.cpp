#include "cxx/ImageReader.h"


#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>


#include <boost/filesystem/convenience.hpp>
namespace bf = boost::filesystem;


#include "common/spirit_parse/rdiParserData.h"
#include "common/MetadataReaderBase.h"
#include "common/sized_ints.h"
#include "common/VisualSonicsTransform.h"


using namespace visual_sonics::cxx;


template<class ImageDataOutT, class CoordT>
ImageReader<ImageDataOutT,CoordT>::ImageReader(const bf::path& in_file_path, std::vector<unsigned int>&  frames_to_read):
  ImageReaderBase( in_file_path, frames_to_read )
{
}



template<class ImageDataOutT, class CoordT>
ImageReader<ImageDataOutT,CoordT>::ImageReader(const bf::path& in_file_path, std::vector<unsigned int>&  frames_to_read, ReadMethod read_method, unsigned int specific_acquisition):
  ImageReaderBase( in_file_path, frames_to_read, read_method, specific_acquisition )
{
}



template<class ImageDataOutT, class CoordT>
ImageReader<ImageDataOutT,CoordT>::ImageReader( const bf::path& in_file_path, ReadMethod read_method, unsigned int specific_acquisition ):
  ImageReaderBase( in_file_path, read_method, specific_acquisition )
{
}



template<class ImageDataOutT, class CoordT>
ImageReader<ImageDataOutT,CoordT>::ImageReader(const bf::path& in_file_path ):
  ImageReaderBase( in_file_path )
{
}



template<class ImageDataOutT, class CoordT>
void ImageReader<ImageDataOutT,CoordT>::read_b_mode_image()
{
  std::ifstream rdb_file( its_rdb_file_path.native_file_string().c_str(), std::ios::in | std::ios::binary);
  if (!rdb_file.is_open())
  {
    std::ostringstream err_msg (std::ostringstream::out);
    err_msg << "\nFile: " << its_rdb_file_path.native_file_string() << " couldn't be opened :(\n";
    throw std::ios_base::failure( err_msg.str() );
  }


  const unsigned int samples_per_line = this->its_metadata_reader->its_rpd->its_rf_mode_rx_ad_gate_width;
  const unsigned int num_lines = this->its_metadata_reader->its_rpd->its_rf_mode_tx_trig_tbl_trigs;
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



template<class ImageDataOutT, class CoordT>
void ImageReader<ImageDataOutT,CoordT>::read_saturation_image()
{
}



template<class ImageDataOutT, class CoordT>
bool ImageReader<ImageDataOutT,CoordT>::read_rf_data_image()
{
  return true;
}



template class ImageReader<double,double>;
template class ImageReader<double,float>;
template class ImageReader<float,float>;
