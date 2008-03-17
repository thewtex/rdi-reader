/*! @file   ReadImageBase.h
 *  @brief  base class for reading VisualSonics Digital RF file's image data
 *
 *  @author Matt McCormick (thewtex) <matt@mmmccormick.com>
 *  @date   2008 February 25
 *
 */

#ifndef	READIMAGEBASE_H
#define READIMAGEBASE_H

#include <vector>


#include <boost/filesystem/convenience.hpp>
namespace bf = boost::filesystem;



namespace visual_sonics
{
  class ReadMetadataBase;

  class ReadImageBase
  {
  public:
    ReadImageBase( std::vector<unsigned int>& frames_to_read ): its_frames_to_read( frames_to_read ) {};
    ReadImageBase(){};

    virtual ~ReadImageBase(){};

  protected:
    //! this MUST be assigned in child classes
    ReadMetadataBase* its_read_metadata;

    bf::path its_rdb_file_path;


    virtual void read_b_mode_image() = 0;
    virtual void read_saturation_image() = 0;
    virtual void read_rf_data_image() = 0;

    std::vector<unsigned int>	its_frames_to_read;

  private:
    //! I have no need for these at this point --write them as needed, carefully
    ReadImageBase( const ReadImageBase& );
    ReadImageBase&  operator=( const ReadImageBase& );
  };
}

#endif // READIMAGEBASE_H
