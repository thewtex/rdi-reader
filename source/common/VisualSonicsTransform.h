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
  #endif // MATLAB_MEX_FILE

      /*!
       * Constructor for the class using std::vectors
       */
        VisualSonicsTransform( const std::vector<ImageDataInT> & image,  //!< raw data to be transformed/scan converted
			     std::vector<ImageDataOutT> & transform, //!< where the transformed image is put @warning the reference is modified
			     unsigned int & transform_rows, //!< where number of rows in the transformed image is place, will follow output_size if values are given, or will be dynamically determined otherwise, @warning the reference is modified
			     unsigned int & transform_cols, //!< where number of cols in the transformed image is place, will follow output_size if values are given, or will be dynamically determined otherwise, @warning the reference is modified
			     std::vector<CoordT> & image_x, //!< where the x-coordinates for the image points are put @warning the reference is modified by the class
			     std::vector<CoordT> & image_y, //!< where the y-coordinates for the image points are put @warning the reference is modified by the class
			     const rdiParserData * const rpd, //!< the metadata used to determine how scan conversion should take place, shaft distances and encoder angles are referenced
			     const bool& is_scout, //!< whether or not this is a scout (b_mode or saturation) image as opposed to an raw rf image.  has an effect on how man scan lines there are in the image
			     const visual_sonics::InterpolationMethod& interpmethod = BilinearM, //!< which interpolation method is used
			     const unsigned int * const output_roi = 0, //!< which part of the original image needs to be scan converted
			     const unsigned int * const output_size = 0 //!< the dimensions of the output image
			     );

      ~VisualSonicsTransform();


      //! set the value for the output that is outside the boundary of the original data
      void set_outside_bounds_value( const ImageDataOutT& obv ) { its_outside_bounds_value = obv; }
      //! set whether or not the coordintes need to be calculated 
      void set_do_calc_coords( const bool & do_calc){ its_do_calc_coords = do_calc; }

      //! perform the transformation/scan conversion and return the transformed image
      void transform();


    private:


      //! calculates its_col_cos, its_col_sin, its_theta, its_r, its_image_x, its_image_y
      void calc_coords();
      //! whether or not the coordinates need to be calculated
      bool its_do_calc_coords;


      //!whether or not this is a scout (b_mode or saturation) image as opposed to an raw rf image.  has an effect on how man scan lines there are in the image
      const bool & its_is_scout;
      //!
      //! the preconverted image data
      unsigned int its_image_rows;
      unsigned int its_image_cols;
      //! image data
      const std::vector<ImageDataInT>&  its_image;
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
      std::vector<CoordT>& its_image_x;
      //! y positions fro the points in the image
      std::vector<CoordT>& its_image_y;



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
      unsigned int& its_transform_rows;
      //! number of columns in the transformed image
      unsigned int& its_transform_cols;

      //! the post-converted image data
      std::vector<ImageDataOutT>& its_transform;



      //! interpolation method
      visual_sonics::InterpolationMethod its_interpolation_method;

      //! value when you are outside the bounds of the original image
      ImageDataOutT its_outside_bounds_value;

    private:
      //! if its_output_roi needs to have delete[] called on destruction
      bool its_delete_output_roi;
  };
} // end namespace visual_sonics

// #include "VisualSonicsTransform.cpp"
#endif // inclusion guard

