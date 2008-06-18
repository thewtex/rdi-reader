/*! @file  ImageViewer.h
 *  @brief render a vtkVisualSonicsReader on the screen for visualization
 *
 *  @author Matt McCormick (thewtex) <matt@mmmccormick.com>
 *  @date   2008 March 17
 *
 */

#ifndef VIEWIMAGE_H
#define VIEWIMAGE_H

#include "vtk/vtkVisualSonicsReader.h"

class vtkRenderWindow;
class vtkInteractorStyleImage;
class vtkInteractorStyleTrackballCamera;
class vtkRenderWindowInteractor;

namespace visual_sonics
{
  namespace vtk
  {

    class ImageViewer
    {
    public:
      //ImageViewer( const bf::path& in_file_path, const bf::path& in_file_name, std::vector<unsigned int>&  frames_to_read);
      //ImageViewer(const bf::path& in_file_path, const bf::path& in_file_name, std::vector<unsigned int>&  frames_to_read, ReadMethod read_method, unsigned int specific_acquisition = 0);

      ImageViewer( const bf::path& in_file_path,  ReadMethod read_method, unsigned int specific_acquisition = 0 );
      ImageViewer( const bf::path& in_file_path );

      virtual ~ImageViewer();


      void view_b_mode();
      void view_saturation();
      void view_rf();


    protected:
      vtkVisualSonicsReader* its_image_reader;

      vtkRenderWindow* its_ren_win;
      vtkInteractorStyleImage* its_interactor_style_image;
      vtkInteractorStyleTrackballCamera* its_interactor_style_trackball;
      vtkRenderWindowInteractor* its_iren;

      //! background for all render windows
      const static double its_background_color_red = 0.0;
      const static double its_background_color_green = 0.32;
      const static double its_background_color_blue = 0.0;

      //! default width for an image viewport
      const static int its_default_width = 512;


    };
  }
}

#endif // VIEWIMAGE_H inclusion guard

