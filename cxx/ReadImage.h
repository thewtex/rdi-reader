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

#include "sized_ints.h" // Int16, UInt16



namespace visual_sonics
{
  namespace cxx
  {
    class ReadImage: public visual_sonics::ReadImageBase
    {
    public:

      ReadImage( const bf::path& in_file_path, const bf::path& in_file_name, std::vector<unsigned int>&  frames_to_read);
      ReadImage(const bf::path& in_file_path, const bf::path& in_file_name, std::vector<unsigned int>&  frames_to_read, ReadMethod read_method, unsigned int specific_acquisition = 0); 

      ReadImage( const bf::path& in_file_path, const bf::path& in_file_name, ReadMethod read_method, unsigned int specific_acquisition = 0 );
      ReadImage( const bf::path& in_file_path, const bf::path& in_file_name);

      virtual ~ReadImage(){};


      virtual void read_b_mode_image();
      virtual void read_saturation_image();
      virtual bool read_rf_data_image();


    protected:
      //! scout window b-mode data.  data is stored sequentially by sample in a line, then by line
      std::vector<UInt16> its_b_mode_image;
      //! saturation data.  data is stored sequentially by sample in a line, then by line
      std::vector<bool>   its_saturation_image;
      //! rf data.  data is stored sequentially by sample in a line, then by line
      std::vector<Int16>  its_rf_data_image;


    private:

      //! I have no need for these at this point --write them as needed
      ReadImage( const ReadImage& );
      ReadImage&  operator=( const ReadImage& );
    
    };
  }
}

#endif // READIMAGE_H

