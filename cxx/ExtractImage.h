/*! @file  ExtractImage.h
 *  @brief extract VisualSonics Digital RF file's image data
 *
 *  @author Matt McCormick (thewtex) <matt@mmmccormick.com>
 *  @date   2008 February 28
 *
 */

#ifndef EXTRACTIMAGE_H
#define EXTRACTIMAGE_H

#include <vector>

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
      /*! @param in_file_path path to the .rdi/.rdb files
       *  @param in_file_name filename of the .rdi/.rdb files less the extension
       *  @param frames_to_extract 
       */
      ExtractImage(const bf::path& in_file_path, const bf::path& in_file_name, std::vector<unsigned int>&  frames_to_extract);
      ExtractImage(const bf::path& in_file_path, const bf::path& in_file_name);
      virtual ~ExtractImage();

    private:

      virtual void get_b_mode_image();

      virtual void get_saturation_image();

      virtual void get_rf_data_image();

      //! I have no need for these at this point --write them as needed
      ExtractImage( const ExtractImage& );
      ExtractImage&  operator=( const ExtractImage& );
    
    };
  }
}

#endif // EXTRACTIMAGE_H

