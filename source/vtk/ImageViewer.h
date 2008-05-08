/*! @file  ImageViewer.h
 *  @brief render a vtk::ImageReader on the screen for visualization
 *
 *  @author Matt McCormick (thewtex) <matt@mmmccormick.com>
 *  @date   2008 March 17
 *
 */

#ifndef VIEWIMAGE_H
#define VIEWIMAGE_H

#include "vtk/ImageReader.h"

class vtkImageViewer;
class vtkInteractorStyleImage;
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

    protected:
      ImageReader* its_image_reader;

      vtkImageViewer* its_viewer;
      vtkInteractorStyleImage* its_interactor_style;
      vtkRenderWindowInteractor* its_iren;

    };
  }
}

#endif // VIEWIMAGE_H inclusion guard

