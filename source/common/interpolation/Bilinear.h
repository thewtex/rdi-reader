/*! @file Bilinear.h
 *  @brief Perform bilinear interpolation
 *  @author Matt McCormick <matt@mmmccormick.com>
 *  @date 2008 January 18
 *
 */

#ifndef BILINEAR_H
#define BILINEAR_H

#include "Interpolation.h"

template<class ImageDataT = double, class CoordT = double>
class Bilinear: public Interpolation<ImageDataT, CoordT>
{
  public:
    Bilinear(  const std::vector<CoordT>& x_vals,
		      const std::vector<CoordT>& y_vals,
		      const std::vector<ImageDataT>& data,
		      const CoordT& target_x_pos,
		      const CoordT& target_y_pos):
			Interpolation<ImageDataT,CoordT>( x_vals, y_vals, data, target_x_pos, target_y_pos )
		{};

    virtual ~Bilinear(){};

    virtual ImageDataT interpolate(){ return 0.0;};

  private:
    //! vector point locations, lt = left top, etc
    enum locations { lt, lb , rb, rt };
	    
};

// using a template, so...
//#include "Bilinear.cpp"

#endif //inclusion guard

