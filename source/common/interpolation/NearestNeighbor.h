/*! @file NearestNeighbor.h
 *  @brief Perform approximate nearest neighbor interpolation
 *  @author Matt McCormick <matt@mmmccormick.com>
 *  @date 2008 January 21
 *
 */

#ifndef NEARESTNEIGHBOR_H
#define NEARESTNEIGHBOR_H

#include "Interpolation.h"

template<class ImageDataT = double, class CoordT = double>
class NearestNeighbor: public Interpolation<ImageDataT, CoordT>
{
  public:
    NearestNeighbor(  const std::vector<CoordT>& x_vals,
		      const std::vector<CoordT>& y_vals,
		      const std::vector<ImageDataT>& data,
		      const CoordT& target_x_pos,
		      const CoordT& target_y_pos):
			Interpolation<ImageDataT,CoordT>( x_vals, y_vals, data, target_x_pos, target_y_pos )
		{};

    virtual ~NearestNeighbor(){};

    virtual ImageDataT interpolate();

  private:

    using Interpolation<ImageDataT,CoordT>::its_y_vals;
    using Interpolation<ImageDataT,CoordT>::its_x_pos;
    using Interpolation<ImageDataT,CoordT>::its_y_pos;
    using Interpolation<ImageDataT,CoordT>::its_data;
};

//#include "NearestNeighbor.cpp"

#endif //inclusion guard

