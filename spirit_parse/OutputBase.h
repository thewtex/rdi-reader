/*! @file   Output.h
 *  @brief  base class for extracting the image data output from VisualSonics .rdb files
 *
 *  @author Matt McCormick (thewtex) <matt@mmmccormick.com>
 *  @date   2008 February 25
 *
 */

virtual get_frames
get_b_mode_image
get_saturation_image
get_rf_data_image(frame)

  InputBase

  std::vector b_mode_image
  std::vector saturation_image
  std::vector rf_data_image
  Std::vector its_image_frames
