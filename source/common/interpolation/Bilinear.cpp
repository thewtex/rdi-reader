/*! @file Bilinear.cpp
 *  @brief implement Bilinear.h
 *  @author Matt McCormick <matt@mmmccormick.com>
 *  @date 2008 January 21
 *
 */

#include "Bilinear.h"

#ifndef NDEBUG
#include <stdexcept>
#endif

#include <iostream>
using namespace std;
template<class ImageDataT, class CoordT>
ImageDataT Bilinear<ImageDataT, CoordT>::interpolate()
{
  // this is only approximate Bilinear interpolation because the points are not in an exactly rectangular
  // coordinate system
  // for proper bilinear interpolation, we would need to the 4x4 linear system implied by
  // f(x,y) = a x + b y + c xy + d
  // and the four points


  // we'll interpolate the x values first to take advantage of the anisotropy of the sampling
  ImageDataT interp_x_bot = 1.0/(this->its_x_vals[rb] - this->its_x_vals[lb])*( this->its_data[lb] *( this->its_x_vals[rb] - this->its_x_pos) + this->its_data[rb]*( this->its_x_pos - this->its_x_vals[lb] ) );

  ImageDataT interp_x_top = 1.0/(this->its_x_vals[rt] - this->its_x_vals[lt])*( this->its_data[lt] *( this->its_x_vals[rt] - this->its_x_pos) + this->its_data[rt]*( this->its_x_pos - this->its_x_vals[lt] ) );

#ifndef NDEBUG
  if( !( (this->its_x_pos > this->its_x_vals[lb] || this->its_x_pos > this->its_x_vals[lt]) && (this->its_x_pos < this->its_x_vals[rb] || this->its_x_pos < this->its_x_vals[rt]) ) && !( (this->its_y_pos < this->its_y_vals[lb] || this->its_y_pos < this->its_y_vals[rb]) && ( this->its_y_pos > this->its_y_vals[lt] || this->its_y_pos > this->its_y_vals[rt])) )
  {
    throw std::out_of_range( " interpolation cell not chosen correctly " );
  }
#endif
  // if it is closer to the left, use the y values on the left side, else use the y values on the right side
  ImageDataT interp;
  if ( (this->its_x_pos - this->its_x_vals[lb])/(this->its_x_vals[rb] - this->its_x_vals[lb]) < 0.5 )
  {
    interp = 1.0/(this->its_y_vals[lt] - this->its_y_vals[lb])*( interp_x_bot*(this->its_y_vals[lt] - this->its_y_pos ) + interp_x_top*( this->its_y_pos - this->its_y_vals[lb] ) );
  }
  else
  {
    interp = 1.0/(this->its_y_vals[rt] - this->its_y_vals[rb])*( interp_x_bot*(this->its_y_vals[rt] - this->its_y_pos ) + interp_x_top*( this->its_y_pos - this->its_y_vals[rb] ) );
  }

  return interp;

}

template class Bilinear<double,double>;
template class Bilinear<double,float>;
template class Bilinear<float,float>;
