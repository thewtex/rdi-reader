#include "ImageViewer.h"


#include <boost/filesystem/convenience.hpp>
namespace bf = boost::filesystem;


#include "vtkImageData.h"
#include "vtkImageViewer.h"
#include "vtkInteractorStyleImage.h"
#include "vtkRenderWindowInteractor.h"

#include "vtkImageActor.h"
#include "vtkImageShiftScale.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"

#include "vtk/ImageReader.h"
#include "common/ReadMethod.h"


using namespace visual_sonics::vtk;


//ImageViewer::ImageViewer( const bf::path& in_file_path, const bf::path& in_file_name, std::vector<unsigned int>&  frames_to_read)
//{
  //this->its_image_reader = ImageReader::New();
  //this->its_image_reader->SetFilePrefix( (in_file_path / in_file_name).native_file_string() );
  //this->its_interactor_style = vtkInteractorStyleImage::New();
  //this->its_iren = vtkRenderWindowInteractor::New();
//}



//ImageViewer::ImageViewer(const bf::path& in_file_path, const bf::path& in_file_name, std::vector<unsigned int>&  frames_to_read, ReadMethod read_method, unsigned int specific_acquisition )
//{
  //this->its_image_reader = ImageReader::New();
  //this->its_image_reader->SetFilePrefix( (in_file_path / in_file_name).native_file_string() );
  //this->its_image_reader->SetReadMethod( read_method );
  //this->its_image_reader->SetSpecificAcquisition( specific_acquisition );
  //this->its_interactor_style = vtkInteractorStyleImage::New();
  //this->its_iren = vtkRenderWindowInteractor::New();
//}



ImageViewer::ImageViewer( const bf::path& in_file_path, const bf::path& in_file_name, ReadMethod read_method, unsigned int specific_acquisition  )
{
  this->its_image_reader = ImageReader::New();
  this->its_image_reader->SetFilePrefix( (in_file_path / in_file_name).native_file_string() );
  this->its_image_reader->SetReadMethod( read_method );
  this->its_image_reader->SetSpecificAcquisition( specific_acquisition );
  this->its_interactor_style = vtkInteractorStyleImage::New();
  this->its_iren = vtkRenderWindowInteractor::New();
}



ImageViewer::ImageViewer( const bf::path& in_file_path, const bf::path& in_file_name)
{
  this->its_image_reader = ImageReader::New();
  this->its_image_reader->SetFilePrefix( (in_file_path / in_file_name).native_file_string() );
  this->its_interactor_style = vtkInteractorStyleImage::New();
  this->its_iren = vtkRenderWindowInteractor::New();
}



ImageViewer::~ImageViewer()
{
  its_image_reader->Delete();
  its_interactor_style->Delete();
  its_iren->Delete();
}



void ImageViewer::view_b_mode()
{

 its_image_reader

 this->ImageReader::read_b_mode_image();

 int* dim = its_vtk_b_mode_image->GetDimensions() ;

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

 ia->RotateZ(-90.0);
 ia->SetScale( 3.0, 3.0, 1.0 );

 vtkRenderWindow* renwin = vtkRenderWindow::New();
 renwin->AddRenderer( ren );
 renwin->SetSize( dim[1], dim[0] );

 its_iren->SetRenderWindow( renwin );
 its_iren->Initialize();
 its_iren->Start();


}
