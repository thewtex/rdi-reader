/*! @file   VisualSonicsTransform.cpp
 *  @brief  Implementation of VisualSonicsTransform.h
 *  @author Matt McCormick <matt@mmmccormick.com>
 *  @date   2007 December 10
 *
 */

#include "VisualSonicsTransform.h"

#include <algorithm> // lower bound
#include <cassert>
#include <cmath> // sine cosine, find_if
#include <stdexcept>  // out_of_range
#include <vector> 

#ifndef NDEBUG
  #include <iostream>
#endif


#ifdef MATLAB_MEX_FILE
  #include "mex.h"
#endif


#include "Bilinear.h"
#include "NearestNeighbor.h"

#ifdef MATLAB_MEX_FILE 
template <class ImageDataT, class CoordT> 
VisualSonicsTransform<ImageDataT,CoordT>::VisualSonicsTransform( const mxArray* const image,
			    const mxArray* const metadata,
			    const mxArray* const output_roi,
			    const mxArray* const output_size,
			    const int& interpmethod
	):
    its_image_rows( mxGetM( image) ),
    its_image_cols( mxGetN( image ) ),
    its_image( its_image_rows*its_image_cols ),
    its_image_x( its_image_rows*its_image_cols ),
    its_image_y( its_image_rows*its_image_cols ),
    its_encoder_positions( its_image_cols ),
    its_col_cos( its_image_cols ),
    its_col_sin( its_image_cols ),
    its_theta( its_image_cols ),
    its_r( its_image_rows ),
    its_output_roi( 4 ),
    its_transform_rows( 0 ),
    its_transform_cols( 0 ),
    its_transform( 0 ),
    its_has_been_transformed( false ),
    its_interpolation_method( interpmethod )
{
  double* image_data_p = mxGetPr( image );
  for( unsigned int i = 0; i < its_image_cols; i++ )
  {
    for( unsigned int j = 0; j < its_image_rows; j++)
    {
      its_image[j + i*its_image_rows] = static_cast<ImageDataT>(*(image_data_p + i*its_image_rows + j));
    }
  }


  mxArray* image_params_struct = mxGetField( metadata, 0, "image_parameters");
  assert( image_params_struct != NULL );


  mxArray* pivot_to_encoder_mxa = mxGetField( image_params_struct, 0, "RF_Mode__ActiveProbe__Pivot_Encoder_Dist___mm" );
  assert( pivot_to_encoder_mxa != NULL );
  const double* pivot_to_encoder_p = mxGetPr( pivot_to_encoder_mxa );
  assert( pivot_to_encoder_p != NULL );
  its_pivot_to_encoder_dist = static_cast<CoordT>(*pivot_to_encoder_p) ;

  mxArray* shaft_length_mxa = mxGetField( image_params_struct, 0, "RF_Mode__ActiveProbe__Pivot_Transducer_Face_Dist___mm" );
  assert( shaft_length_mxa != NULL );
  double* shaft_length_p = mxGetPr( shaft_length_mxa );
  double shaft_length = *shaft_length_p;

  mxArray* delay_length_mxa = mxGetField( image_params_struct, 0, "RF_Mode__RX__V_Delay_Length___mm" );
  assert ( delay_length_mxa != NULL );
  double delay_length = *mxGetPr( delay_length_mxa );

  its_pivot_to_xdcr_dist = static_cast<CoordT>(shaft_length + delay_length);

  
  mxArray* encoder_dists_mxa = mxGetField( image_params_struct, 0, "RF_Mode__RfModeSoft__V_Lines_Pos___mm" );
  assert( encoder_dists_mxa != NULL );
  double* encoder_dists_p = mxGetPr( encoder_dists_mxa );
  assert( encoder_dists_p != NULL );
  for (unsigned int i = 0; i < its_image_cols; i++ )
  {
    its_encoder_positions[i] = static_cast<CoordT>(*(encoder_dists_p + i));
  }


  mxArray* digi_depth_mxa = mxGetField( image_params_struct, 0, "RF_Mode__RX__V_Digi_Depth_Imaging___mm" );
  double* digi_depth_p = mxGetPr( digi_depth_mxa );
  its_sample_delta = static_cast<CoordT>(*digi_depth_p / its_image_rows);


  double* output_roi_p = mxGetPr( output_roi );
  for ( unsigned int i = 0; i < 4; i++)
  {
    its_output_roi[i] = static_cast<unsigned int>( *(output_roi_p +i) );
  }

  if ( its_output_roi[rows_start] == 0 )
  {
    its_output_roi[rows_start] = 1;
  }
  if ( its_output_roi[rows_end] == 0 )
  {
    its_output_roi[rows_end] = its_image_rows;
  }
  if ( its_output_roi[cols_start] == 0 )
  {
    its_output_roi[cols_start] = 1;
  }
  if ( its_output_roi[cols_end] == 0 )
  {
    its_output_roi[cols_end] = its_image_cols;
  }


  double* output_size_p = mxGetPr( output_size );
  its_transform_rows = static_cast<unsigned int>( *output_size_p  );
  its_transform_cols = static_cast<unsigned int>( *(output_size_p+1)  );

  //! default output image sizes
  //! @todo give a proper default aspect ratio
  if ( its_transform_rows == 0 )
    its_transform_rows = 600;
  if ( its_transform_cols == 0 )
    its_transform_cols = 800;
  
  its_transform.resize( its_transform_rows*its_transform_cols );

}
#endif // MATLAB_MEX_FILE


template <class ImageDataT, class CoordT>
VisualSonicsTransform<ImageDataT,CoordT>::~VisualSonicsTransform()
{
}



template <class ImageDataT, class CoordT>
std::vector<ImageDataT> VisualSonicsTransform<ImageDataT,CoordT>::transform()
{

  for(unsigned int i = 0; i < its_image_cols; i++)
  {
    its_col_cos[i] = std::cos( its_encoder_positions[i] / its_pivot_to_encoder_dist );
    its_col_sin[i] = std::sin( its_encoder_positions[i] / its_pivot_to_encoder_dist );

    its_theta[i] = its_encoder_positions[i] / its_pivot_to_encoder_dist ;
  }

  for(unsigned int i = 0; i < its_image_rows; i++)
    its_r[i] = its_pivot_to_xdcr_dist + i*its_sample_delta;



  for(unsigned int i = 0; i < its_image_cols; i++)
  {
    for(unsigned int j = 0; j < its_image_rows; j++)
    {
      its_image_x[j + i*its_image_rows] = ( its_pivot_to_xdcr_dist + j*its_sample_delta ) * its_col_sin[i];
      its_image_y[j + i*its_image_rows] = ( its_pivot_to_xdcr_dist + j*its_sample_delta ) * its_col_cos[i];
    }
  }

  
  // lower left
  const CoordT x_min = its_image_x[ (its_output_roi[cols_start] -1) * its_image_rows + its_output_roi[rows_end] -1 ] ;
  // lower right
  const CoordT x_max = its_image_x[ (its_output_roi[cols_end  ] -1) * its_image_rows + its_output_roi[rows_end] -1 ] ;

  // minimum of upper left and upper right
  const CoordT y_min_left = its_image_y[ (its_output_roi[cols_start] -1) * its_image_rows + its_output_roi[rows_start] - 1] ;
  const CoordT y_min_right = its_image_y[ (its_output_roi[cols_end ] -1) * its_image_rows + its_output_roi[rows_end  ] - 1] ;
  const CoordT y_min = std::min(y_min_left, y_min_right);

  // maximum of values if image was off y-axis, value on y-axis otherwise
  const CoordT y_max_left = its_image_y[  (its_output_roi[cols_start] -1) * its_image_rows + its_output_roi[rows_start] -1] ;
  const CoordT y_max_right = its_image_y[ (its_output_roi[cols_end  ] -1) * its_image_rows + its_output_roi[rows_end  ] -1] ;
  CoordT y_max;
  if( ( x_min < 0.0 && x_max < 0.0 ) || ( x_min > 0.0 && x_max > 0.0 ) )
    y_max = std::max( y_max_left, y_max_right );
  else
    y_max = its_pivot_to_xdcr_dist + (its_image_rows-1)*its_sample_delta ;


  // distance between points in the transformed image
  const CoordT delta_x = (x_max - x_min)/its_transform_cols;
  const CoordT delta_y = (y_max - y_min)/its_transform_rows;
  

  // perform the transformation

  // start in the top left corner
  CoordT x_pos = x_min;
  CoordT y_pos = y_min;

  // the four points in the original image that surround our target point:
  // lt = left-top 
  // rt = right-top
  // lb = left-bottom 
  // ....
  //! used in referencing the values in the following x_vals, y_vals 
  enum locations { lt, lb, rb, rt };

  std::vector<CoordT> x_vals(4), y_vals(4);
  std::vector<ImageDataT> data(4);

  std::size_t	lt_ind, // left-top index
		lb_ind,
		rb_ind,
		rt_ind;
  
  typename std::vector<CoordT >::const_iterator current_col, // column in the original image where the current_lt resides
				      current_row; // row in the original image where the current_lt resides


  CoordT theta = 0.0;
  CoordT r = 0.0;


  // step through the transformed image and find values column by column
  for( unsigned int i = 0; i < its_transform_cols; i++) // for every transformed image column
  {
    for( unsigned int j = 0; j < its_transform_rows; j++)
    {
      theta = std::atan(x_pos / y_pos); // because of the way the coordinate system is set up, it may be opposite from what we're used to 
      r = std::sqrt( x_pos*x_pos + y_pos*y_pos );

      current_col = std::lower_bound( its_theta.begin(), its_theta.end() , theta);
      current_row = std::lower_bound( its_r.begin()    , its_r.end()     , r    );
      lt_ind = (current_col - its_theta.begin() )*its_image_rows + (current_row - its_r.begin() );
      lb_ind = lt_ind + 1;
      rt_ind = ( (current_col - its_theta.begin()) + 1) * its_image_rows + (current_row - its_r.begin() );
      rb_ind = rt_ind + 1;

      x_vals[lt] = its_image_x[lt_ind];  y_vals[lt] = its_image_y[lt_ind];
      x_vals[lb] = its_image_x[lb_ind];  y_vals[lb] = its_image_y[lb_ind];
      x_vals[rb] = its_image_x[rb_ind];  y_vals[rb] = its_image_y[rb_ind];
      x_vals[rt] = its_image_x[rt_ind];  y_vals[rt] = its_image_y[rt_ind];

      data[lt] = its_image[lt_ind];
      data[lb] = its_image[lb_ind];
      data[rb] = its_image[rb_ind];
      data[rt] = its_image[rt_ind];

      switch (its_interpolation_method)
      {
	case (0):
	  its_interpolation = new NearestNeighbor<ImageDataT,CoordT>( x_vals, y_vals, data, x_pos, y_pos );
	  break;
      }
	  
      its_transform[ i*its_transform_cols + j ] = its_interpolation->interpolate(); 

      delete its_interpolation;

      // check to make sure we are within the bounds,
      //  advance which row we are in
      //  and give the default value if we outside the bounds of the original image
      //try
      //{
        //while ( x_pos < its_image_x.at(current_x_lower_bound) )
	//{
	  //current_x_lower_bound++ ;
	//}
      //}
      //! @note cannot use std::out_of_range because of a visibility issue in gcc 4.[0-1], may be fixed in 4.2
      //! for more info, see http://www.dribin.org/dave/blog/archives/2006/02/10/gcc_exception_bug/
 //     catch ( std::exception ) // for the last column
      //{
	//its_transform[ i*its_transform_rows + j ] = its_outside_bounds_value;
	//continue;
      //}
//      if ( current_x_lower_bound == x_upper_bound_plus_one ) //for the other columns
      //{
	//its_transform[ i*its_transform_rows + j ] = its_outside_bounds_value;
	//continue;
      //}
      
      //lt_x = current_x_lower_bound;
      //lb_x = current_x_lower_bound+1;

      //std::cout << " some text " <<  std::endl;



      
      
     
       y_pos = y_pos + delta_y;
    }
    

    x_pos = x_pos + delta_x;
    y_pos = y_min;

  }



  its_has_been_transformed = true;

  return its_transform;
}


template <class ImageDataT, class CoordT>
std::vector<ImageDataT> VisualSonicsTransform<ImageDataT,CoordT>::get_transformed_image()
{
  if (!its_has_been_transformed)
    return this->transform();

  return its_transform; 
}

template class VisualSonicsTransform<double,double>;
template class VisualSonicsTransform<double,float>;
template class VisualSonicsTransform<float,float>;
