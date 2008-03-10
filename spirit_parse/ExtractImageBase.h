/*! @file   ExtractImageBase.h
 *  @brief  base class for extracting VisualSonics Digital RF file's image data
 *
 *  @author Matt McCormick (thewtex) <matt@mmmccormick.com>
 *  @date   2008 February 25
 *
 */

#ifndef	EXTRACTIMAGEBASE_H
#define EXTRACTIMAGEBASE_H

#include <vector>


#include <boost/filesystem/convenience.hpp>
namespace bf = boost::filesystem;



namespace visual_sonics
{
  class ExtractMetadataBase;

  class ExtractImageBase
  {
  public:
    ExtractImageBase( std::vector<unsigned int>& frames_to_extract ): its_frames_to_extract( frames_to_extract ) {};
    ExtractImageBase(){};

    virtual ~ExtractImageBase(){};

  protected:
    //! this MUST be assigned in child classes
    ExtractMetadataBase* its_extract_metadata;

    bf::path its_rdb_file_path;

    virtual void get_b_mode_image() = 0;

    virtual void get_saturation_image() = 0;

    virtual void get_rf_data_image() = 0;

    std::vector<unsigned int>	its_frames_to_extract;

  private:
    //! I have no need for these at this point --write them as needed, carefully
    ExtractImageBase( const ExtractImageBase& );
    ExtractImageBase&  operator=( const ExtractImageBase& );
  };
}

#endif // EXTRACTIMAGEBASE_H
