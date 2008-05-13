/*! @file   Interpolation.h
 *  @brief  Abstract base class for an interpolation function
 *  @author Matt McCormick <matt@mmmccormick.com>
 *  @date   2008 January 18
 *
 */

#ifndef INTERPOLATION_H
#define INTERPOLATION_H

#include <vector>


//! ImageDataT is the type of the image we are working with and CoordT is the type of the coordinates
template<class ImageDataT, class CoordT>
class Interpolation
{
  public:

    Interpolation( const std::vector<CoordT>& x_vals,
		   const std::vector<CoordT>& y_vals,
		   const std::vector<ImageDataT>& data,
		   const CoordT& target_x_pos,
		   const CoordT& target_y_pos):
		      its_x_vals(x_vals),
		      its_y_vals(y_vals),
		      its_data(data),
		      its_x_pos(target_x_pos),
		      its_y_pos(target_y_pos)
		{};

    virtual ~Interpolation(){};

    //! perform the interpolation and return the result
    virtual ImageDataT interpolate() = 0;

  protected:
    const std::vector<CoordT> its_x_vals, its_y_vals;
    
    const std::vector<ImageDataT> its_data;

    const CoordT its_x_pos, its_y_pos;

}; 


#endif // inclusion guard

