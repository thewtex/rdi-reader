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
  template<class ImageDataInT, class ImageDataOutT, class CoordT> class VisualSonicsTransform;
}


namespace visual_sonics
{
  namespace cxx
  {
    template <class ImageDataOutT, class CoordT>
    class ImageReader: public visual_sonics::ImageReaderBase
    {
    public:

      ImageReader( const bf::path& in_file_path, std::vector<unsigned int>&  frames_to_read);
      ImageReader(const bf::path& in_file_path, std::vector<unsigned int>&  frames_to_read, ReadMethod read_method, unsigned int specific_acquisition = 0);

      ImageReader( const bf::path& in_file_path, ReadMethod read_method, unsigned int specific_acquisition = 0 );
      ImageReader( const bf::path& in_file_path );

      virtual ~ImageReader();


      virtual void read_b_mode_image();
      virtual void read_saturation_image();
      virtual bool read_rf_data_image();



      //! get the actual b_mode_image data
      const std::vector<UInt16>&  get_b_mode_image(){ return its_b_mode_image;}
      //! get b_mode_image x coords
      const std::vector<CoordT>& get_b_mode_image_x(){ return its_b_mode_image_x;}
      //! get b_mode_image y coord
      const std::vector<CoordT>& get_b_mode_image_y(){ return its_b_mode_image_y;}
      //! get b_mode_image data scan converted/transformed
      const std::vector<ImageDataOutT>& get_b_mode_image_sc(){ return its_b_mode_image_sc; }
      //! get b_mode scan converted image rows
      unsigned int get_b_mode_image_sc_rows(){ return its_b_mode_sc_rows; };
      //! get b_mode scan converted image columns
      unsigned int get_b_mode_image_sc_cols(){ return its_b_mode_sc_cols; };

      UInt16 get_b_mode_max(){ return its_b_mode_max; }
      UInt16 get_b_mode_min(){ return its_b_mode_min; }


      //! get the actual saturation image data
      const std::vector<bool>&  get_saturation_image(){ return its_saturation_image;}
      //! get saturation_image x coords
      const std::vector<CoordT>& get_saturation_image_x(){ return its_saturation_image_x;}
      //! get saturation_image y coord
      const std::vector<CoordT>& get_saturation_image_y(){ return its_saturation_image_y;}
      //! get saturation_image data scan converted/transformed
      const std::vector<bool>& get_saturation_image_sc(){ return its_saturation_image_sc; }
      //! get saturation scan converted image rows
      unsigned int get_saturation_image_sc_rows(){ return its_saturation_sc_rows; };
      //! get saturation scan converted image columns
      unsigned int get_saturation_image_sc_cols(){ return its_saturation_sc_cols; };

      UInt16 get_saturation_max(){ return its_saturation_max; }
      UInt16 get_saturation_min(){ return its_saturation_min; }


    protected:
      //! scout window b-mode data.  data is stored sequentially by sample in a line, then by line
      std::vector<UInt16> its_b_mode_image;
      //! scout window b-mode data.  data is stored sequentially by sample in a line, then by line, scan converted
      std::vector<ImageDataOutT> its_b_mode_image_sc;
      //! number of rows in the b_mode scan converted image
      unsigned int its_b_mode_sc_rows;
      //! number of columns in the b_mode scan converted image
      unsigned int its_b_mode_sc_cols;
      //! scout window b-mode data.  x coordinates
      std::vector<CoordT> its_b_mode_image_x;
      //! scout window b-mode data.  y coordinates
      std::vector<CoordT> its_b_mode_image_y;
      //! max value in the b mode image
      UInt16 its_b_mode_max;
      //! min value in the b mode image
      UInt16 its_b_mode_min;


      //! scout window b-mode data.  data is stored sequentially by sample in a line, then by line
      std::vector<bool> its_saturation_image;
      //! scout window b-mode data.  data is stored sequentially by sample in a line, then by line, scan converted
      std::vector<bool> its_saturation_image_sc;
      //! number of rows in the saturation scan converted image
      unsigned int its_saturation_sc_rows;
      //! number of columns in the saturation scan converted image
      unsigned int its_saturation_sc_cols;
      //! scout window b-mode data.  x coordinates
      std::vector<CoordT> its_saturation_image_x;
      //! scout window b-mode data.  y coordinates
      std::vector<CoordT> its_saturation_image_y;
      //! max value in the b mode image
      UInt16 its_saturation_max;
      //! min value in the b mode image
      UInt16 its_saturation_min;


      //! rf data.  data is stored sequentially by sample in a line, then by line
      std::vector<Int16>  its_rf_data_image;


      //! performs scan conversion, etc
      VisualSonicsTransform<UInt16, ImageDataOutT, CoordT>* its_b_mode_vs_transform;
      //! performs scan conversion, etc
      VisualSonicsTransform<bool, bool, CoordT>* its_saturation_vs_transform;

    private:

      //! I have no need for these at this point --write them as needed
      ImageReader( const ImageReader& );
      ImageReader&  operator=( const ImageReader& );

    };
  }
}

#endif // CXX_IMAGEREADER_H

