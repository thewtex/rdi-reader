/*! @file   ReadMethod.h
 *  @brief  define the ReadMethod enum for how samples are extracted from data
 *
 *  @author Matt McCormick (thewtex) <matt@mmmccormick.com>
 *  @date   2008 March 02
 *
 */

#ifndef READMETHOD_H
#define READMETHOD_H


namespace visual_sonics
{
  /**
   * @enum ReadMethod Method for reading the data from the file.
   *
   * Since there can be multiple acquisitions per frame, there
   * are many ways of picking or averaging the acquisitions
   */
  enum ReadMethod {
    file_average,  //!< take the average of all acquisitions that exists in the file
    specific_acquisition //!< get a specific acquisition only (must be specified)
    };

}

#endif // inclusion guard
