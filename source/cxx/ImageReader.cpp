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
  ImageReaderBase( in_file_path, frames_to_read ),
  its_has_calc_scout_coords( false ),
  its_do_scan_conv( true )
{
  its_b_mode_vs_transform = new VisualSonicsTransform<UInt16, ImageDataOutT, CoordT>( its_b_mode_image,
										      its_b_mode_image_sc,
										      its_scout_sc_rows,
										      its_scout_sc_cols,
										      its_scout_sc_delta_x,
										      its_scout_sc_delta_y,
										      its_scout_image_x,
										      its_scout_image_y,
										      this->get_rpd(),
										      true
										      );

  its_saturation_vs_transform = new VisualSonicsTransform<bool, bool, CoordT>( its_saturation_image,
										      its_saturation_image_sc,
										      its_scout_sc_rows,
										      its_scout_sc_cols,
										      its_scout_sc_delta_x,
										      its_scout_sc_delta_y,
										      its_scout_image_x,
										      its_scout_image_y,
										      this->get_rpd(),
										      true
										      );

  its_rf_vs_transform = new VisualSonicsTransform<Int16, ImageDataOutT, CoordT>( its_rf_image,
										      its_rf_image_sc,
										      its_rf_sc_rows,
										      its_rf_sc_cols,
										      its_rf_sc_delta_x,
										      its_rf_sc_delta_y,
										      its_rf_image_x,
										      its_rf_image_y,
										      this->get_rpd(),
										      false
										      );

  // we read in the first frame so that metadata such as the scan converted rows and columns areo  // initialized and can be probed 
  this->read_saturation_image(); // saturation image and b_mode image have the same geometry
  this->read_rf_image();
  its_frames_to_read_index = its_frames_to_read.begin();

}



template<class ImageDataOutT, class CoordT>
ImageReader<ImageDataOutT,CoordT>::ImageReader(const bf::path& in_file_path, std::vector<unsigned int>&  frames_to_read, ReadMethod read_method, unsigned int specific_acquisition):
  ImageReaderBase( in_file_path, frames_to_read, read_method, specific_acquisition ),
  its_has_calc_scout_coords( false ),
  its_do_scan_conv( true )
{
  its_b_mode_vs_transform = new VisualSonicsTransform<UInt16, ImageDataOutT, CoordT>( its_b_mode_image,
										      its_b_mode_image_sc,
										      its_scout_sc_rows,
										      its_scout_sc_cols,
										      its_scout_sc_delta_x,
										      its_scout_sc_delta_y,
										      its_scout_image_x,
										      its_scout_image_y,
										      this->get_rpd(),
										      true
										      );

  its_saturation_vs_transform = new VisualSonicsTransform<bool, bool, CoordT>( its_saturation_image,
										      its_saturation_image_sc,
										      its_scout_sc_rows,
										      its_scout_sc_cols,
										      its_scout_sc_delta_x,
										      its_scout_sc_delta_y,
										      its_scout_image_x,
										      its_scout_image_y,
										      this->get_rpd(),
										      true
										      );

  its_rf_vs_transform = new VisualSonicsTransform<Int16, ImageDataOutT, CoordT>( its_rf_image,
										      its_rf_image_sc,
										      its_rf_sc_rows,
										      its_rf_sc_cols,
										      its_rf_sc_delta_x,
										      its_rf_sc_delta_y,
										      its_rf_image_x,
										      its_rf_image_y,
										      this->get_rpd(),
										      false
										      );

  // we read in the first frame so that metadata such as the scan converted rows and columns areo  // initialized and can be probed 
  this->read_saturation_image(); // saturation image and b_mode image have the same geometry
  this->read_rf_image();
  its_frames_to_read_index = its_frames_to_read.begin();
}



template<class ImageDataOutT, class CoordT>
ImageReader<ImageDataOutT,CoordT>::ImageReader( const bf::path& in_file_path, ReadMethod read_method, unsigned int specific_acquisition ):
  ImageReaderBase( in_file_path, read_method, specific_acquisition ),
  its_has_calc_scout_coords( false ),
  its_do_scan_conv( true )
{
  its_b_mode_vs_transform = new VisualSonicsTransform<UInt16, ImageDataOutT, CoordT>( its_b_mode_image,
										      its_b_mode_image_sc,
										      its_scout_sc_rows,
										      its_scout_sc_cols,
										      its_scout_sc_delta_x,
										      its_scout_sc_delta_y,
										      its_scout_image_x,
										      its_scout_image_y,
										      this->get_rpd(),
										      true
										      );

  its_saturation_vs_transform = new VisualSonicsTransform<bool, bool, CoordT>( its_saturation_image,
										      its_saturation_image_sc,
										      its_scout_sc_rows,
										      its_scout_sc_cols,
										      its_scout_sc_delta_x,
										      its_scout_sc_delta_y,
										      its_scout_image_x,
										      its_scout_image_y,
										      this->get_rpd(),
										      true
										      );

  its_rf_vs_transform = new VisualSonicsTransform<Int16, ImageDataOutT, CoordT>( its_rf_image,
										      its_rf_image_sc,
										      its_rf_sc_rows,
										      its_rf_sc_cols,
										      its_rf_sc_delta_x,
										      its_rf_sc_delta_y,
										      its_rf_image_x,
										      its_rf_image_y,
										      this->get_rpd(),
										      false
										      );
  
  // we read in the first frame so that metadata such as the scan converted rows and columns areo  // initialized and can be probed 
  this->read_saturation_image(); // saturation image and b_mode image have the same geometry
  this->read_rf_image();
  its_frames_to_read_index = its_frames_to_read.begin();
}



template<class ImageDataOutT, class CoordT>
ImageReader<ImageDataOutT,CoordT>::ImageReader(const bf::path& in_file_path ):
  ImageReaderBase( in_file_path ),
  its_has_calc_scout_coords( false ),
  its_do_scan_conv( true )
{
  its_b_mode_vs_transform = new VisualSonicsTransform<UInt16, ImageDataOutT, CoordT>( its_b_mode_image,
										      its_b_mode_image_sc,
										      its_scout_sc_rows,
										      its_scout_sc_cols,
										      its_scout_sc_delta_x,
										      its_scout_sc_delta_y,
										      its_scout_image_x,
										      its_scout_image_y,
										      this->get_rpd(),
										      true
										      );

  its_saturation_vs_transform = new VisualSonicsTransform<bool, bool, CoordT>( its_saturation_image,
										      its_saturation_image_sc,
										      its_scout_sc_rows,
										      its_scout_sc_cols,
										      its_scout_sc_delta_x,
										      its_scout_sc_delta_y,
										      its_scout_image_x,
										      its_scout_image_y,
										      this->get_rpd(),
										      true
										      );

  its_rf_vs_transform = new VisualSonicsTransform<Int16, ImageDataOutT, CoordT>( its_rf_image,
										      its_rf_image_sc,
										      its_rf_sc_rows,
										      its_rf_sc_cols,
										      its_rf_sc_delta_x,
										      its_rf_sc_delta_y,
										      its_rf_image_x,
										      its_rf_image_y,
										      this->get_rpd(),
										      false
										      );

  // we read in the first frame so that metadata such as the scan converted rows and columns areo  // initialized and can be probed 
  this->read_saturation_image(); // saturation image and b_mode image have the same geometry
  this->read_rf_image();
  its_frames_to_read_index = its_frames_to_read.begin();
}



template<class ImageDataOutT, class CoordT>
ImageReader<ImageDataOutT,CoordT>::~ImageReader()
{
  delete its_b_mode_vs_transform;
  delete its_saturation_vs_transform;
  delete its_rf_vs_transform;
}



template<class ImageDataOutT, class CoordT>
void ImageReader<ImageDataOutT,CoordT>::set_in_file_path( const bf::path& ifp )
{
  this->ImageReaderBase::set_in_file_path( ifp );
  its_has_calc_scout_coords = false;
    bool do_scan_conv_tmp = its_do_scan_conv;
  its_do_scan_conv = true;
  // we read in the first frame so that metadata such as the scan converted rows and columns areo  // initialized and can be probed 
  this->read_saturation_image(); // saturation image and b_mode image have the same geometry
  this->read_rf_image();
  its_frames_to_read_index = its_frames_to_read.begin();
    its_do_scan_conv = do_scan_conv_tmp;
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


  char * u_short_data = new char[sizeof(UInt16)];
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
  // finished extracting data


  if( its_do_scan_conv )
  {
    // prepare for transformation
    its_scout_image_x.resize(samples_per_line * num_lines);
    its_scout_image_y.resize(samples_per_line * num_lines);
  
    its_b_mode_vs_transform->set_outside_bounds_value( its_b_mode_min );
  
    if( !its_has_calc_scout_coords )
    {
      its_b_mode_vs_transform->set_do_calc_coords( true );
      its_has_calc_scout_coords = true;
    }
    else
    {
      its_b_mode_vs_transform->set_do_calc_coords( false );
    }
  
    its_b_mode_vs_transform->transform();
  }

}



template<class ImageDataOutT, class CoordT>
void ImageReader<ImageDataOutT,CoordT>::read_saturation_image()
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
  its_saturation_image.resize( samples_per_line * num_lines );


  rdb_file.seekg( num_lines * samples_per_line * sizeof(UInt16), std::ios::beg);
  char * u_short_data = new char[ sizeof(UInt16) ];
  UInt16 * u_short_data_p = reinterpret_cast< UInt16 *> (u_short_data);
  for( unsigned int i = 0; i < num_lines; i++)
  {
    for( unsigned int j = 0; j < samples_per_line; j++)
    {
      rdb_file.read(u_short_data, 2);
      if( *u_short_data_p == 0 )
      {
        its_saturation_image[ i*samples_per_line + j ] = false;
      }
      else
      {
	its_saturation_image[ i*samples_per_line + j ] = true;
      }
    }
  }


  delete[] u_short_data;
  rdb_file.close();
  // finished extracting data


  if ( its_do_scan_conv )
  {
    // prepare for transformation
    its_scout_image_x.resize(samples_per_line * num_lines);
    its_scout_image_y.resize(samples_per_line * num_lines);
  
    its_saturation_vs_transform->set_outside_bounds_value( 0 );
  
    if( !its_has_calc_scout_coords )
    {
      its_saturation_vs_transform->set_do_calc_coords( true );
      its_has_calc_scout_coords = true;
    }
    else
    {
      its_saturation_vs_transform->set_do_calc_coords( false );
    }
  
    its_saturation_vs_transform->transform();
  }

}



template<class ImageDataOutT, class CoordT>
bool ImageReader<ImageDataOutT,CoordT>::read_rf_image()
{

  if( its_frames_to_read_index == its_frames_to_read.end() )
  {
    its_frames_to_read_index = its_frames_to_read.begin();
    return false;
  }

  std::ifstream rdb_file( its_rdb_file_path.native_file_string().c_str(), std::ios::in | std::ios::binary);
  if (!rdb_file.is_open())
  {
    std::ostringstream err_msg (std::ostringstream::out);
    err_msg << "\nFile: " << its_rdb_file_path.native_file_string() << " couldn't be opened :(\n";
    throw std::ios_base::failure( err_msg.str() );
  }


  const unsigned int samples_per_line = this->its_metadata_reader->its_rpd->its_image_acquisition_size / sizeof( Int16 );
  const unsigned int num_lines = this->its_metadata_reader->its_rpd->its_image_lines;
  its_rf_image.resize( samples_per_line * num_lines );


  //skip the b mode and saturation scout images
  const unsigned int scout_samples_per_line = this->its_metadata_reader->its_rpd->its_rf_mode_rx_ad_gate_width;
  const unsigned int scout_num_lines = this->its_metadata_reader->its_rpd->its_rf_mode_tx_trig_tbl_trigs;
  rdb_file.seekg( 2 * scout_num_lines * scout_samples_per_line * sizeof(UInt16), std::ios::beg);

  //skip previous frames
  rdb_file.seekg(  num_lines * samples_per_line * sizeof(Int16) * (*its_frames_to_read_index - 1), std::ios::cur );

  // file_average and specific_acquisition ReadMethod s
  if( this->its_read_method == file_average || this->its_read_method == specific_acquisition )
  {
    unsigned int skip_amount = samples_per_line * 2 * this->its_metadata_reader->its_rpd->its_image_acquisition_per_line;

    if( this->its_read_method == specific_acquisition )
    {
      rdb_file.seekg( its_specific_acquisition * samples_per_line *2, std::ios::cur);
    }
    else // skip to start on file average line
    {
      rdb_file.seekg( skip_amount, std::ios::cur );
    }
  
    char * short_data = new char[ sizeof( Int16 ) ];
    Int16 * short_data_p = reinterpret_cast< Int16 *> (short_data);
    for( unsigned int i = 0; i < num_lines; i++)
    {
      for( unsigned int j = 0; j < samples_per_line; j++)
      {
        rdb_file.read(short_data, 2);
        its_rf_image[ i*samples_per_line + j] = *short_data_p;
      }
      rdb_file.seekg( skip_amount, std::ios::cur );
    }

    delete[] short_data;
  }


  its_rf_max = *std::max_element( its_rf_image.begin(), its_rf_image.end() );
  its_rf_min = *std::min_element( its_rf_image.begin(), its_rf_image.end() );

  rdb_file.close();
  // finished extracting data


  if( its_do_scan_conv )
  {
    // prepare for transformation
    if( its_frames_to_read_index == its_frames_to_read.begin() )
    {
      its_rf_image_x.resize(samples_per_line * num_lines);
      its_rf_image_y.resize(samples_per_line * num_lines);
    
      its_rf_vs_transform->set_outside_bounds_value( 0 );
  
      its_rf_vs_transform->set_do_calc_coords( true );
    }
    else
    {
      its_rf_vs_transform->set_do_calc_coords( false );
    }
  
    its_rf_vs_transform->transform();
  }

  its_frames_to_read_index++;
  return true;
}



template class ImageReader<double,double>;
template class ImageReader<double,float>;
template class ImageReader<float,float>;
