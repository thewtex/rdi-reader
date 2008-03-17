/*! @file  ReadImage.h
 *  @brief read VisualSonics Digital RF file's image data
 *
 *  @author Matt McCormick (thewtex) <matt@mmmccormick.com>
 *  @date   2008 February 28
 *
 */

#ifndef READIMAGE_H
#define READIMAGE_H

#include <vector>

#include <boost/filesystem/convenience.hpp>
namespace bf = boost::filesystem;


#include "ReadImageBase.h"


namespace visual_sonics
{
  namespace cxx
  {
    class ReadImage: public visual_sonics::ReadImageBase
    {
    public:
      /*! @param in_file_path path to the .rdi/.rdb files
       *  @param in_file_name filename of the .rdi/.rdb files less the extension
       *  @param frames_to_read 
       */
      ReadImage(const bf::path& in_file_path, const bf::path& in_file_name, std::vector<unsigned int>&  frames_to_read);
      ReadImage(const bf::path& in_file_path, const bf::path& in_file_name);
      virtual ~ReadImage();

      virtual void read_b_mode_image();
      virtual void read_saturation_image();
      virtual void read_rf_data_image();

    private:

      std::vector<unsigned short> its_b_mode_image;
      std::vector<bool>	          its_saturation_image;
      std::vector<short>          its_rf_data_image;


      //! I have no need for these at this point --write them as needed
      ReadImage( const ReadImage& );
      ReadImage&  operator=( const ReadImage& );
    
    };
  }
}

#endif // READIMAGE_H

