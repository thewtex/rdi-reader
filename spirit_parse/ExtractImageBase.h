/*! @file   ExtractImageBase.h
 *  @brief  base class for extracting VisualSonics Digital RF file's image data
 *
 *  @author Matt McCormick (thewtex) <matt@mmmccormick.com>
 *  @date   2008 February 25
 *
 */

#ifndef	EXTRACTIMAGEBASE_H
#define EXTRACTIMAGEBASE_H

//#include "ExtractMetadataBase.h"

#include <vector>

namespace visual_sonics
{
  class ExtractMetadataBase;

  class ExtractImageBase
  {
  public:
    ExtractImageBase(){};

    virtual ~ExtractImageBase(){};

  protected:
    ExtractMetadataBase* its_extract_metadata;

    std::vector<unsigned short> its_b_mode_image;
    virtual void get_b_mode_image() = 0;

    std::vector<bool>		its_saturation_image;
    virtual void get_saturation_image() = 0;

    std::vector<short>		its_rf_data_image;
    virtual void get_rf_data_image() = 0;

    std::vector<unsigned int>	its_frames_to_extract;
    virtual void get_frames_to_extract() = 0;

  };
}

#endif // EXTRACTIMAGEBASE_H
