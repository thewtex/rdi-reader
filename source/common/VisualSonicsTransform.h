/*! @file   VisualSonicsTransform.h
 *  @brief  class for transforming (scan converting) a set of Vivo VisualSonics 
 *	    data to a 'true' cartesian space
 *  @author Matt McCormick <matt@mmmccormick.com>
 *  @date   2007 December 10 
 *
 */

#ifndef VISUALSONICSTRANSFORM_H
#define VISUALSONICSTRANSFORM_H


#include <vector>


#ifdef MATLAB_MEX_FILE

#include "mex.h"

#endif


#include "common/interpolation/Interpolation.h"
#include "common/interpolation/InterpolationMethod.h"


namespace visual_sonics
{
  class rdiParserData;

  //! ImageDataT is the type of the RF data we are working with, e.g. double, short 
  template <class ImageDataInT, class ImageDataOutT, class CoordT>
  class VisualSonicsTransform
  {
    public:
  
  #ifdef MATLAB_MEX_FILE
      /*! @param image 
       *	@param metadata Matlab structure with the machine collection parameters
       *	@param output_roi 
       *	@param output_size
       *	@param interpmethod interpolation method to use 
       *
       *	@warning if you are using this constructor, then the ImageDataT should be 'double' since Matlab uses double
       */
      VisualSonicsTransform( const mxArray* const image,
  			    const mxArray* const metadata,
  			    const mxArray* const output_roi,
  			    const mxArray* const output_size,
  			    const visual_sonics::InterpolationMethod& interpmethod
  	);
  #else
      /*! @param image 
       *	@param metadata rdiParserData structure with the machine collection parameters
       *	@param output_roi 
       *	@param output_size
       *	@param interpmethod interpolation method to use 
       *
       *	@warning if you are using this constructor, then the ImageDataT should be 'double' since Matlab uses double
       */
      //VisualSonicsTransform( const mxArray* const image,
  			    //const mxArray* const metadata,
  			    //const mxArray* const output_roi,
  			    //const mxArray* const output_size,
  			    //const int& interpmethod
  	//);
  #endif // MATLAB_MEX_FILE
  
        VisualSonicsTransform( const std::vector<ImageDataInT> & image, 
			     std::vector<ImageDataOutT> & transform,
			     std::vector<CoordT> & image_x,
			     std::vector<CoordT> & image_y,
			     const rdiParserData * const rpd,
			     const unsigned int * const output_roi = 0,
			     const unsigned int * const output_size = 0,
			     const visual_sonics::InterpolationMethod& interpmethod = BilinearM);
  
      ~VisualSonicsTransform();

      //! get the x and y position of the original image points
      void get_coords( const std::vector<CoordT>* x_coords,
	                               const std::vector<CoordT>* y_coords );

      //! perform the transformation/scan conversion and return the transformed image
      std::vector<ImageDataOutT> transform();
  
      //! returned the transformed image, performing the calculation only if neede 
      std::vector<ImageDataOutT> get_transformed_image();
  
  
    private:
  
  
      //! calculates its_col_cos, its_col_sin, its_theta, its_r, its_image_x, its_image_y
      void calc_coords();

  
      //! 
      //! the preconverted image data
      const unsigned int its_image_rows;
      const unsigned int its_image_cols;
      //! image data
      const std::vector<ImageDataInT>  its_image;
     /*! Coordinate system:
     * Post scan conversion space
     * ------> +x
     * |
     * |
     * |
     * V
     * +y
     * origin = pivot point (same)
     *
     */
     //! x positions for the points in the image
      std::vector<CoordT> its_image_x;
      //! y positions fro the points in the image
      std::vector<CoordT> its_image_y;
  
  
  
      // parameters for conversion
  
      //! mm
      CoordT its_pivot_to_encoder_dist;
      //! mm
      CoordT its_pivot_to_xdcr_dist;
      //! mm distance along the encoder (arch distance)
      std::vector<CoordT> its_encoder_positions;
      //! cosine(theta) for each column's shaft position
      std::vector<CoordT> its_col_cos;
      //! sine(theta) for each column's shaft position
      std::vector<CoordT> its_col_sin;
      //! angle (theta) for each column's shaft position
      std::vector<CoordT> its_theta;
      //! length (radius) to image points
      std::vector<CoordT> its_r;
      //! distance between points along an A-line
      CoordT its_sample_delta;
  
      //! interpolation method
      Interpolation<ImageDataOutT,CoordT>* its_interpolation;
  
      enum output_roi_specs { rows_start, rows_end, cols_start, cols_end };
      //! [rows_start rows_end cols_start cols_end] @warning counting starts from 1 
      unsigned int* its_output_roi;
  
      //! default number of rows in the transformed image
      //! the number of columns will be chosen for the proper aspect ratio
      //! non-static so you can choose different values in different constructors
      const unsigned int its_default_transform_rows;
      //! number of rows in the transformed image 
      unsigned int its_transform_rows;
      //! number of columns in the transformed image
      unsigned int its_transform_cols;
  
      //! the post-converted image data
      std::vector<ImageDataOutT> its_transform;
  
  
      //! has transform() been called yet?
      bool its_has_been_transformed;

  
      //! interpolation method 
      visual_sonics::InterpolationMethod its_interpolation_method;
  
      //! value when you are outside the bounds of the original image
      const static CoordT its_outside_bounds_value = 0.0;
  };
} // end namespace visual_sonics

// #include "VisualSonicsTransform.cpp"
#endif // inclusion guard

