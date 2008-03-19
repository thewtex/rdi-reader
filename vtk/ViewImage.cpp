#include "ViewImage.h"


#include "vtkImageData.h"
#include "vtkImageViewer.h"
#include "vtkInteractorStyleImage.h"
#include "vtkRenderWindowInteractor.h"

#include "vtkImageActor.h"
#include "vtkImageShiftScale.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"

#include "vtk/ReadImage.h"


using namespace visual_sonics::vtk;


ViewImage::ViewImage( const bf::path& in_file_path, const bf::path& in_file_name, std::vector<unsigned int>&  frames_to_read):
  ReadImage( in_file_path, in_file_name, frames_to_read )
{
  its_viewer = vtkImageViewer::New();
  its_viewer->SetColorWindow( 4096.0 );
  its_viewer->SetColorLevel( 2047.5 );
  its_interactor_style = vtkInteractorStyleImage::New();
  its_iren = vtkRenderWindowInteractor::New();
}



ViewImage::ViewImage(const bf::path& in_file_path, const bf::path& in_file_name, std::vector<unsigned int>&  frames_to_read, ReadMethod read_method, unsigned int specific_acquisition ):
  ReadImage( in_file_path, in_file_name, frames_to_read, read_method, specific_acquisition )
{
  its_viewer = vtkImageViewer::New();
  its_viewer->SetColorWindow( 4096.0 );
  its_viewer->SetColorLevel( 2047.5 );
  its_interactor_style = vtkInteractorStyleImage::New();
  its_iren = vtkRenderWindowInteractor::New();
}



ViewImage::ViewImage( const bf::path& in_file_path, const bf::path& in_file_name, ReadMethod read_method, unsigned int specific_acquisition  ):
  ReadImage( in_file_path, in_file_name, read_method, specific_acquisition )
{
  its_viewer = vtkImageViewer::New();
  its_viewer->SetColorWindow( 4096.0 );
  its_viewer->SetColorLevel( 2047.5 );
  its_interactor_style = vtkInteractorStyleImage::New();
  its_iren = vtkRenderWindowInteractor::New();
}



ViewImage::ViewImage( const bf::path& in_file_path, const bf::path& in_file_name):
  ReadImage( in_file_path, in_file_name )
{
  its_viewer = vtkImageViewer::New();
  its_viewer->SetColorWindow( 4096.0 );
  its_viewer->SetColorLevel( 2047.5 );
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

 this-> ReadImage::read_b_mode_image();

 //its_viewer->SetInput( its_vtk_b_mode_image );
 //its_viewer->Render();
 int* dim = its_vtk_b_mode_image->GetDimensions() ;
 its_viewer->SetSize(dim[0], dim[1]);

 //its_viewer->SetupInteractor(its_iren);
 vtkImageActor* ia = vtkImageActor::New();
 vtkImageShiftScale* iss = vtkImageShiftScale::New();
 iss->SetShift( its_b_mode_min*-1 );
 iss->SetScale( 255.0 / static_cast<double>(its_b_mode_max - its_b_mode_min) );
 iss->ClampOverflowOn();
 iss->SetInput( its_vtk_b_mode_image );
 iss->SetOutputScalarTypeToUnsignedChar();

 vtkRenderer* ren = vtkRenderer::New();
 ia->SetInput( iss->GetOutput() );
 ren->AddViewProp( ia );

 vtkRenderWindow* renwin = vtkRenderWindow::New();
 renwin->AddRenderer( ren );
 renwin->SetSize( dim[0], dim[1] );

 its_iren->SetRenderWindow( renwin );
 its_iren->Initialize();
 its_iren->Start();


}
