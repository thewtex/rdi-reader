#include <stdexcept> // out_of_range
#include <vector>

#include <boost/filesystem/convenience.hpp>
namespace bf = boost::filesystem;

#include "ExtractImage.h"

#include "ExtractMetadataBase.h"


using namespace visual_sonics::cxx;


ExtractImage::ExtractImage(const bf::path& in_file_path, const bf::path& in_file_name, std::vector<unsigned int>&  frames_to_extract): ExtractImageBase( frames_to_extract )
{
  its_extract_metadata = new ExtractMetadataBase( in_file_path, in_file_name);

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



void ExtractImage::get_b_mode_image()
{
}



void ExtractImage::get_saturation_image()
{
}



void ExtractImage::get_rf_data_image()
{
}



