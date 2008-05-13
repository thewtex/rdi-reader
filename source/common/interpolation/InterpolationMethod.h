/*! @file   InterpolationMethod.h
 *  @brief  define the InterpolationMethod enum for how samples are interpolated during scan conversion
 *
 *  @author Matt McCormick (thewtex) <matt@mmmccormick.com>
 *  @date   2008 May 15
 *
 */

#ifndef READMETHOD_H
#define READMETHOD_H


namespace visual_sonics
{
  /**
   * @enum InterpolationMethod Method for how samples are interpolated during scan conversion
   *
   */
  enum InterpolationMethod {
    NearestNeighborM,  //!< take the average of all acquisitions that exists in the file
    BilinearM //!< get a specific acquisition only (must be specified)
    };

}

#endif // inclusion guard
