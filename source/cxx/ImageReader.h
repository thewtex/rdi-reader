/*! @file  ImageReader.h
 *  @brief read VisualSonics Digital RF file's image data
 *
 *  @author Matt McCormick (thewtex) <matt@mmmccormick.com>
 *  @date   2008 February 28
 *
 */

#ifndef CXX_IMAGEREADER_H
#define CXX_IMAGEREADER_H

#include <vector>

#include <boost/filesystem/convenience.hpp>
namespace bf = boost::filesystem;


#include "common/ImageReaderBase.h"

#include "common/sized_ints.h" // Int16, UInt16



namespace visual_sonics
{
  namespace cxx
  {
    class ImageReader: public visual_sonics::ImageReaderBase
    {
    public:

      ImageReader( const bf::path& in_file_path, std::vector<unsigned int>&  frames_to_read);
      ImageReader(const bf::path& in_file_path, std::vector<unsigned int>&  frames_to_read, ReadMethod read_method, unsigned int specific_acquisition = 0);

      ImageReader( const bf::path& in_file_path, ReadMethod read_method, unsigned int specific_acquisition = 0 );
      ImageReader( const bf::path& in_file_path );

      virtual ~ImageReader(){};


      virtual void read_b_mode_image();
      virtual void read_saturation_image();
      virtual bool read_rf_data_image();

      UInt16* get_b_mode_image_p(){ return &its_b_mode_image[0];} 


    protected:
      //! scout window b-mode data.  data is stored sequentially by sample in a line, then by line
      std::vector<UInt16> its_b_mode_image;
      //! max value in the b mode image
      UInt16 its_b_mode_max;
      //! min value in the b mode image
      UInt16 its_b_mode_min;
      //! saturation data.  data is stored sequentially by sample in a line, then by line
      std::vector<bool>   its_saturation_image;
      //! rf data.  data is stored sequentially by sample in a line, then by line
      std::vector<Int16>  its_rf_data_image;


    private:

      //! I have no need for these at this point --write them as needed
      ImageReader( const ImageReader& );
      ImageReader&  operator=( const ImageReader& );

    };
  }
}

#endif // CXX_IMAGEREADER_H

