#include "ImageViewer.h"


#include <boost/filesystem/convenience.hpp>
namespace bf = boost::filesystem;


#include "vtkImageData.h"
#include "vtkImageViewer.h"
#include "vtkInteractorStyleImage.h"
#include "vtkRenderWindowInteractor.h"

#include "vtkImageActor.h"
#include "vtkImageShiftScale.h"
#include "vtkImageViewer.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"

#include "vtk/vtkVisualSonicsReader.h"
#include "common/ReadMethod.h"


using namespace visual_sonics::vtk;


//ImageViewer::ImageViewer( const bf::path& in_file_path, const bf::path& in_file_name, std::vector<unsigned int>&  frames_to_read)
//{
  //this->its_image_reader = vtkVisualSonicsReader::New();
  //this->its_image_reader->SetFilePrefix( (in_file_path / in_file_name).native_file_string() );
  //this->its_interactor_style = vtkInteractorStyleImage::New();
  //this->its_iren = vtkRenderWindowInteractor::New();
//}



//ImageViewer::ImageViewer(const bf::path& in_file_path, const bf::path& in_file_name, std::vector<unsigned int>&  frames_to_read, ReadMethod read_method, unsigned int specific_acquisition )
//{
  //this->its_image_reader = vtkVisualSonicsReader::New();
  //this->its_image_reader->SetFilePrefix( (in_file_path / in_file_name).native_file_string() );
  //this->its_image_reader->SetReadMethod( read_method );
  //this->its_image_reader->SetSpecificAcquisition( specific_acquisition );
  //this->its_interactor_style = vtkInteractorStyleImage::New();
  //this->its_iren = vtkRenderWindowInteractor::New();
//}



ImageViewer::ImageViewer( const bf::path& in_file_path, ReadMethod read_method, unsigned int specific_acquisition  )
{
  its_image_reader = vtkVisualSonicsReader::New();
  its_image_reader->SetFilePrefix( in_file_path.native_file_string().c_str() );
  its_image_reader->SetReadMethod( read_method );
  its_image_reader->SetSpecificAcquisition( specific_acquisition );
  its_interactor_style = vtkInteractorStyleImage::New();
  its_iren = vtkRenderWindowInteractor::New();
}



ImageViewer::ImageViewer( const bf::path& in_file_path )
{
  its_image_reader = vtkVisualSonicsReader::New();
  its_image_reader->SetFilePrefix( in_file_path.native_file_string().c_str() );
  its_interactor_style = vtkInteractorStyleImage::New();
  its_iren = vtkRenderWindowInteractor::New();
}



ImageViewer::~ImageViewer()
{
  its_image_reader->Delete();
  its_interactor_style->Delete();
  its_iren->Delete();
}


#include "vtkPNGReader.h"

void ImageViewer::view_b_mode()
{
 its_image_reader->Update();
 //its_image_reader->DebugOn();
 //its_image_reader->Print(cerr);

 vtkImageData* vtk_b_mode_image_sc = vtkImageData::SafeDownCast(its_image_reader->GetOutputDataObject(3));
 int* dim = vtk_b_mode_image_sc->GetDimensions() ;
 double* b_mode_range = vtk_b_mode_image_sc->GetScalarRange();
 int xmina, xmaxa, ymina, ymaxa, zmina, zmaxa;
 unsigned short* dp = static_cast< unsigned short* > (vtk_b_mode_image_sc->GetScalarPointer());
 for( int i = 0; i<10; i++)
 {
   std::cerr << *dp << std::endl;
   dp++;
 }
 vtk_b_mode_image_sc->GetExtent( xmina, xmaxa, ymina, ymaxa, zmina, zmaxa );
 //vtk_b_mode_image_sc->DebugOn();
 vtk_b_mode_image_sc->Print(cerr);

 //its_viewer->SetupInteractor(its_iren);
 vtkImageShiftScale* iss = vtkImageShiftScale::New();
 iss->SetShift( b_mode_range[0] *-1 );
 iss->SetScale( 255.0 / static_cast<double>(b_mode_range[1] - b_mode_range[0] ) );
 iss->ClampOverflowOn();
 iss->SetInputConnection( its_image_reader->GetOutputPort(3) );
 iss->SetOutputScalarTypeToUnsignedChar();

 vtkPNGReader* pngr = vtkPNGReader::New();
 pngr->SetFileName("chart.png");
 pngr->Update();
 vtkImageData* pngo = pngr->GetOutput();
 pngo->Print(cerr);





 vtkImageViewer* viewer = vtkImageViewer::New();
 //viewer->SetInputConnection( iss->GetOutputPort() );
 //vtkPNGReader* pngr = vtkPNGReader::New();
 //pngr->SetFileName( "a07118.png" );
 //viewer->SetInput( pngr->GetOutput() );
 viewer->SetInput( vtk_b_mode_image_sc );
 //viewer->SetInput( pngo );
 viewer->SetupInteractor( its_iren );

 viewer->SetZSlice(0);
 viewer->SetColorWindow( 256.0 );
 //viewer->SetColorWindow( b_mode_range[1] - b_mode_range[0] );
 viewer->SetColorLevel( 127.0 );
 //viewer->SetColorLevel( (b_mode_range[1] - b_mode_range[0])/2.0 );
 viewer->Render();

 //vtkImageActor* ia = vtkImageActor::New();
 //ia->SetInput( iss->GetOutput() );

 //vtkRenderer* ren = vtkRenderer::New();
 //ren->SetBackground(0.0,0.8,0.0);
 //ren->AddViewProp( ia );

 //ia->RotateZ(-90.0);
 //ia->SetScale( 3.0, 3.0, 1.0 );

 //vtkRenderWindow* renwin = vtkRenderWindow::New();
 //renwin->AddRenderer( ren );
 //renwin->SetSize( dim[1], dim[0] );

 //its_iren->SetRenderWindow( renwin );
 its_iren->Initialize();
 its_iren->Start();

 //ren->Delete();
 //renwin->Delete();

 vtkImageData* cd = iss->GetOutput();
 cd->Print(cerr);
 cerr << "output ports: " << iss->GetNumberOfOutputPorts() << '\n';
 unsigned char* dpc = static_cast<unsigned char*>( cd->GetScalarPointer( ) );
 for(int i= 0; i<10; i++)
 {
   cerr << *dpc << std::endl;
   dpc++;
  }



}
