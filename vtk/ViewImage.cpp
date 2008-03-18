#include "ViewImage.h"


#include "vtkImageData.h"
#include "vtkImageViewer.h"
#include "vtkInteractorStyleImage.h"
#include "vtkRenderWindowInteractor.h"


#include "vtk/ReadImage.h"


using namespace visual_sonics::vtk;


ViewImage::ViewImage( const bf::path& in_file_path, const bf::path& in_file_name, std::vector<unsigned int>&  frames_to_read):
  ReadImage( in_file_path, in_file_name, frames_to_read )
{
  its_viewer = vtkImageViewer::New();
  its_viewer->SetColorWindow( 65536.0 );
  its_viewer->SetColorLevel( 32767.5 );
  its_interactor_style = vtkInteractorStyleImage::New();
  its_iren = vtkRenderWindowInteractor::New();
}



ViewImage::ViewImage(const bf::path& in_file_path, const bf::path& in_file_name, std::vector<unsigned int>&  frames_to_read, ReadMethod read_method, unsigned int specific_acquisition ):
  ReadImage( in_file_path, in_file_name, frames_to_read, read_method, specific_acquisition )
{
  its_viewer = vtkImageViewer::New();
  its_viewer->SetColorWindow( 65536.0 );
  its_viewer->SetColorLevel( 32767.5 );
  its_interactor_style = vtkInteractorStyleImage::New();
  its_iren = vtkRenderWindowInteractor::New();
}



ViewImage::ViewImage( const bf::path& in_file_path, const bf::path& in_file_name, ReadMethod read_method, unsigned int specific_acquisition  ):
  ReadImage( in_file_path, in_file_name, read_method, specific_acquisition )
{
  its_viewer = vtkImageViewer::New();
  its_viewer->SetColorWindow( 65536.0 );
  its_viewer->SetColorLevel( 32767.5 );
  its_interactor_style = vtkInteractorStyleImage::New();
  its_iren = vtkRenderWindowInteractor::New();
}



ViewImage::ViewImage( const bf::path& in_file_path, const bf::path& in_file_name):
  ReadImage( in_file_path, in_file_name )
{
  its_viewer = vtkImageViewer::New();
  its_viewer->SetColorWindow( 65536.0 );
  its_viewer->SetColorLevel( 32767.5 );
  its_interactor_style = vtkInteractorStyleImage::New();
  its_iren = vtkRenderWindowInteractor::New();
}



ViewImage::~ViewImage()
{
  its_viewer->Delete();
  its_interactor_style->Delete();
  its_iren->Delete();
}



void ViewImage::view_b_mode()
{

  ReadImage::read_b_mode_image();
 its_viewer->SetInput( its_vtk_b_mode_image );
 its_viewer->Render();
 int* dim = its_vtk_b_mode_image->GetDimensions() ;
 its_viewer->SetSize(dim[0], dim[1]);

 its_viewer->SetupInteractor(its_iren);
 its_iren->Initialize();
 its_iren->Start();


}
