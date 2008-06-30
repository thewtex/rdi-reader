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


#include "common/interpolation/Bilinear.h"
#include "common/interpolation/NearestNeighbor.h"
#include "common/spirit_parse/rdiParserData.h"

using namespace visual_sonics;

#ifdef MATLAB_MEX_FILE
template <class ImageDataInT, class ImageDataOutT, class CoordT>
VisualSonicsTransform<ImageDataInT, ImageDataOutT, CoordT>::VisualSonicsTransform( const mxArray* const image,
			    const mxArray* const metadata,
			    const mxArray* const output_roi,
			    const mxArray* const output_size,
			    const visual_sonics::InterpolationMethod& interpmethod = BilinearM
			    ):
    its_image_rows( mxGetM( image) ),
    its_image_cols( mxGetN( image ) ),
    its_encoder_positions( its_image_cols ),
    its_col_cos( its_image_cols ),
    its_col_sin( its_image_cols ),
    its_theta( its_image_cols ),
    its_r( its_image_rows ),
    its_default_transform_rows( 600 ),
    its_transform_rows( 0 ),
    its_transform_cols( 0 ),
    its_transform( 0 ),
    its_interpolation_method( interpmethod )
    its_delete_output_roi( true )
{




  double* image_data_p = mxGetPr( image );
  its_image = std::vector<ImageDataInT>( its_image_rows*its_image_cols );
  std::vector<ImageDataInT>* its_image_non_c = const_cast< std::vector<ImageDataInT>* > ( its_image );
  for( unsigned int i = 0; i < its_image_cols; i++ )
  {
    for( unsigned int j = 0; j < its_image_rows; j++)
    {
      (*its_image_non_c)[j + i*its_image_rows] = static_cast<ImageDataInT>(*(image_data_p + i*its_image_rows + j));
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


  its_output_roi = new unsigned int[4];
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


}
#endif // MATLAB_MEX_FILE



template <class ImageDataInT, class ImageDataOutT, class CoordT>
VisualSonicsTransform<ImageDataInT, ImageDataOutT, CoordT>::VisualSonicsTransform(
			    const std::vector<ImageDataInT>& image,
			    std::vector<ImageDataOutT> & transform,
			    unsigned int& transform_rows,
			    unsigned int& transform_cols,
			    std::vector<CoordT> & image_x,
			    std::vector<CoordT> & image_y,
			    const rdiParserData * const rpd,
			    const bool & is_scout,
			    const visual_sonics::InterpolationMethod& interpmethod,
			    const unsigned int * const output_roi ,
			    const unsigned int * const output_size
			    ):
    its_is_scout( is_scout ),
    its_image( image ),
    its_image_x( image_x ),
    its_image_y( image_y ),
    its_default_transform_rows( 512 ),
    its_transform_rows( transform_rows ),
    its_transform_cols( transform_cols ),
    its_transform( transform ),
    its_interpolation_method( interpmethod ),
    its_outside_bounds_value( 0 ),
    its_delete_output_roi( false )
{


  if (its_is_scout)
  {
    its_image_rows = rpd->its_rf_mode_rx_ad_gate_width;
    its_image_cols = rpd->its_rf_mode_tx_trig_tbl_trigs;
  }
  else
  {
    its_image_rows = rpd->its_image_acquisition_size / 2 ; // sizeof( UInt16 ) = 2
    its_image_cols = rpd->its_image_lines ;
  }

  its_encoder_positions.resize( its_image_cols );
  its_col_cos.resize( its_image_cols );
  its_col_sin.resize( its_image_cols );
  its_theta.resize( its_image_cols );
  its_r.resize( its_image_rows );

  its_pivot_to_encoder_dist = static_cast<CoordT>(rpd->its_rf_mode_activeprobe_pivot_encoder_dist) ;

  its_pivot_to_xdcr_dist = static_cast<CoordT>(rpd->its_rf_mode_activeprobe_pivot_transducer_face_dist + rpd->its_rf_mode_rx_v_delay_length);


  if (its_is_scout)
  {
    CoordT encoder_start = static_cast<CoordT>( rpd->its_rf_mode_rfmodesoft_v_lines_pos_vec[0]);
    CoordT encoder_end   = static_cast<CoordT>( rpd->its_rf_mode_rfmodesoft_v_lines_pos_vec[rpd->its_image_lines - 1 ]);
    CoordT encoder_inc   = (encoder_end - encoder_start) / (its_image_cols - 1);
    CoordT cur_encoder_pos = encoder_start;
    unsigned int index = 0;
    while( index < its_image_cols )
    {
      its_encoder_positions[index] = cur_encoder_pos;
      index++;
      cur_encoder_pos = cur_encoder_pos + encoder_inc;
    }
  }
  else // rf data
  {
    for (unsigned int i = 0; i < its_image_cols; i++ )
    {
      its_encoder_positions[i] = static_cast<CoordT>(rpd->its_rf_mode_rfmodesoft_v_lines_pos_vec[i]);
    }
  }


  its_sample_delta = static_cast<CoordT>(rpd->its_rf_mode_rx_v_digi_depth_imaging / its_image_rows);


  if (!output_roi)
  {
    its_delete_output_roi = true;
    its_output_roi = new unsigned int[4];
    for( int i = 0; i<4; i++)
      its_output_roi[i] = 0;
  }
  else
  {
    its_output_roi = const_cast< unsigned int *>(output_roi);
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


  its_transform_rows = 0;
  its_transform_cols = 0;
  if (output_size)
  {
    its_transform_rows = output_size[0];
    its_transform_cols = output_size[1];
  }
}




template <class ImageDataInT, class ImageDataOutT, class CoordT>
VisualSonicsTransform<ImageDataInT, ImageDataOutT, CoordT>::~VisualSonicsTransform()
{
  if (its_delete_output_roi )
  {
    delete[] its_output_roi;
  }
}



template <class ImageDataInT, class ImageDataOutT, class CoordT>
void VisualSonicsTransform<ImageDataInT, ImageDataOutT, CoordT>::calc_coords()
{

  for(unsigned int i = 0; i < its_image_cols; i++)
  {
    its_col_cos[i] = std::cos( its_encoder_positions[i] / its_pivot_to_encoder_dist );
    its_col_sin[i] = std::sin( its_encoder_positions[i] / its_pivot_to_encoder_dist );

    its_theta[i] = its_encoder_positions[i] / its_pivot_to_encoder_dist ;
  }

  for(unsigned int i = 0; i < its_image_rows; i++)
  {
    its_r[i] = its_pivot_to_xdcr_dist + i*its_sample_delta;
  }


  its_image_x.resize( its_image_cols * its_image_rows );
  its_image_y.resize( its_image_cols * its_image_rows );

  for(unsigned int i = 0; i < its_image_cols; i++)
  {
    for(unsigned int j = 0; j < its_image_rows; j++)
    {
      its_image_x[j + i*its_image_rows] = its_r[j] * its_col_sin[i];
      its_image_y[j + i*its_image_rows] = its_r[j] * its_col_cos[i];
    }
  }
}




template <class ImageDataInT, class ImageDataOutT, class CoordT>
void VisualSonicsTransform<ImageDataInT, ImageDataOutT, CoordT>::transform()
{

  this->calc_coords();


  // find extrema of the image
  // x minimum of upper left and lower left
  const CoordT x_min_top = its_image_x[ (its_output_roi[cols_start] -1) * its_image_rows + its_output_roi[rows_start] -1 ] ;
  const CoordT x_min_bot = its_image_x[ (its_output_roi[cols_start] -1) * its_image_rows + its_output_roi[rows_end] -1 ] ;
  const CoordT x_min = std::min( x_min_top, x_min_bot );
  // maximum of the upper right and lower right
  const CoordT x_max_top = its_image_x[ (its_output_roi[cols_end  ] -1) * its_image_rows + its_output_roi[rows_start] -1 ] ;
  const CoordT x_max_bot = its_image_x[ (its_output_roi[cols_end  ] -1) * its_image_rows + its_output_roi[rows_end] -1 ] ;
  const CoordT x_max = std::max( x_max_top, x_max_bot );
  // minimum of upper left and upper right
  const CoordT y_min_left = its_image_y[ (its_output_roi[cols_start] -1) * its_image_rows + its_output_roi[rows_start] - 1] ;
  const CoordT y_min_right = its_image_y[ (its_output_roi[cols_end ] -1) * its_image_rows + its_output_roi[rows_start  ] - 1] ;
  const CoordT y_min = std::min(y_min_left, y_min_right);
  // maximum of values if image was off y-axis, value on y-axis otherwise
  const CoordT y_max_left = its_image_y[  (its_output_roi[cols_start] -1) * its_image_rows + its_output_roi[rows_end] -1] ;
  const CoordT y_max_right = its_image_y[ (its_output_roi[cols_end  ] -1) * its_image_rows + its_output_roi[rows_end  ] -1] ;
  CoordT y_max;
  if( ( x_min < 0.0 && x_max < 0.0 ) || ( x_min > 0.0 && x_max > 0.0 ) )
    y_max = std::max( y_max_left, y_max_right );
  else
    y_max = its_pivot_to_xdcr_dist + (its_image_rows-1)*its_sample_delta ;




  // apply default values if they have not yet been given
  if (its_transform_cols == 0 )
  {
    if ( its_transform_rows == 0 )
    {
      its_transform_rows = its_default_transform_rows;
    }
    // proper aspect ratio
    its_transform_cols = static_cast< unsigned int > ( its_transform_rows * ( (x_max - x_min)/(y_max - y_min) ) );
  }
  its_transform.resize( its_transform_rows*its_transform_cols, its_outside_bounds_value );



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

  CoordT x_vals[4], y_vals[4];
  ImageDataOutT data[4];

  std::size_t	lt_ind, // left-top index
		lb_ind,
		rb_ind,
		rt_ind;

  typename std::vector<CoordT >::const_iterator current_col, // column in the original image where the current_lt resides
				      current_row; // row in the original image where the current_lt resides


  CoordT theta = 0.0;
  CoordT r = 0.0;

  switch (its_interpolation_method)
  {
	case (NearestNeighborM):
	  its_interpolation = new NearestNeighbor<ImageDataOutT,CoordT>( x_vals, y_vals, data, x_pos, y_pos );
	  break;
	case (BilinearM):
	  its_interpolation = new Bilinear<ImageDataOutT, CoordT>( x_vals, y_vals, data, x_pos, y_pos );
	  break;
  }

  // step through the transformed image and find values column by column
  for( unsigned int i = 0; i < its_transform_cols; i++) // for every transformed image column
  {
    for( unsigned int j = 0; j < its_transform_rows; j++)
    {
      theta = std::atan(x_pos / y_pos); // because of the way the coordinate system is set up, it may be opposite from what we're used to
      r = std::sqrt( x_pos*x_pos + y_pos*y_pos );

      // make sure we are within bounds
      if( theta < *its_theta.begin() || theta > *(its_theta.end()-1) || r < *its_r.begin() || r > *(its_r.end()-1) )
      {
        y_pos = y_pos + delta_y;
	continue;
      }

      current_col = std::lower_bound( its_theta.begin(), its_theta.end() , theta);
      current_col--;
      current_row = std::lower_bound( its_r.begin()    , its_r.end()     , r    );
      current_row--;

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


      its_transform[ i*its_transform_rows + j ] = its_interpolation->interpolate();

       y_pos = y_pos + delta_y;
    }


    x_pos = x_pos + delta_x;
    y_pos = y_min;

  }


  delete its_interpolation;


}



template class VisualSonicsTransform<unsigned short, double, double>;
template class VisualSonicsTransform<unsigned short, double, float>;
template class VisualSonicsTransform<unsigned short, float, float>;

template class VisualSonicsTransform<short, double, double>;
template class VisualSonicsTransform<short, double, float>;
template class VisualSonicsTransform<short, float, float>;

template class VisualSonicsTransform<double, double, double>;
template class VisualSonicsTransform<double, double, float>;
template class VisualSonicsTransform<double, float, float>;

template class VisualSonicsTransform<bool, bool, float>;
template class VisualSonicsTransform<bool, bool, double>;
