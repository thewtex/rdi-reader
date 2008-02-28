/*! @file  ExtractImage.h
 *  @brief extract VisualSonics Digital RF file's image data
 *
 *  @author Matt McCormick (thewtex) <matt@mmmccormick.com>
 *  @date   2008 February 28
 *
 */

#ifndef EXTRACTIMAGE_H
#define EXTRACTIMAGE_H

#include <boost/filesystem/convenience.hpp>
namespace bf = boost::filesystem;


#include "ExtractImageBase.h"


namespace visual_sonics
{
  namespace cxx
  {
    class ExtractImage: public visual_sonics::ExtractImageBase
    {
    public:
      ExtractImage(const bf::path& in_file_path, const bf::path& in_file_name);
      virtual ~ExtractImage();

    private:

      virtual void get_b_mode_image();

      virtual void get_saturation_image();

      virtual void get_rf_data_image();

      virtual void get_frames_to_extract();
    
    };
  }
}

#endif // EXTRACTIMAGE_H

