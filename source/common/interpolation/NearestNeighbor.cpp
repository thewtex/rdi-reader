/*! @file NearestNeighbor.cpp
 *  @brief implement NearestNeighbor.h
 *  @author Matt McCormick <matt@mmmccormick.com>
 *  @date 2008 January 23
 *
 */

#include "NearestNeighbor.h"

#include <cmath> // fabs()

template<class ImageDataT, class CoordT>
ImageDataT NearestNeighbor<ImageDataT, CoordT>::interpolate()
{

  // location of the (approximate) nearest neighbor
  int loc = 0;
  CoordT min_distance = std::fabs(this->its_x_vals[loc] - this->its_x_pos) + std::fabs(this->its_y_vals[loc] - this->its_y_pos);
  CoordT temp_distance;

  for( int loc_it = 1; loc_it < 4; ++loc_it)
  {
    temp_distance = std::fabs(this->its_x_vals[loc_it] - this->its_x_pos) + std::fabs(this->its_y_vals[loc_it] - this->its_y_pos) ;
    if( temp_distance < min_distance )
    {
      loc = loc_it;
      min_distance = temp_distance;
    }
  }

  return this->its_data[loc];

}


template class NearestNeighbor<double,double>;
template class NearestNeighbor<double,float >;
template class NearestNeighbor<float,float >;
template class NearestNeighbor<bool, float>;
template class NearestNeighbor<bool, double>;

