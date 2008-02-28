#include "ExtractImage.h"


using namespace visual_sonics::cxx;


ExtractImage::ExtractImage(const bf::path& in_file_path, const bf::path& in_file_name)
{
  its_extract_metadata = new ExtractMetadataBase( in_file_path, in_file_name);

}


ExtractImage::~ExtractImage()
{
  delete its_extract_metadata;
}


