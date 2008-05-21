#include "ImageViewer.h"


#include <boost/filesystem/convenience.hpp>
namespace bf = boost::filesystem;


#include "vtkDataSetMapper.h"
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


#include "vtkStructuredGrid.h"
#include "vtkLookupTable.h"
#include "vtkInteractorStyleTrackballCamera.h"
#include "vtkCamera.h"
void ImageViewer::view_b_mode()
{
 its_image_reader->Update();
 //its_image_reader->DebugOn();
 //its_image_reader->Print(cerr);
 
 vtkImageData* vtk_b_mode_image_sc = vtkImageData::SafeDownCast(its_image_reader->GetOutputDataObject(3));
 int* dim = vtk_b_mode_image_sc->GetDimensions() ;
 double* b_mode_range = vtk_b_mode_image_sc->GetScalarRange();
 int xmina, xmaxa, ymina, ymaxa, zmina, zmaxa;
 vtk_b_mode_image_sc->GetExtent( xmina, xmaxa, ymina, ymaxa, zmina, zmaxa );
 //vtk_b_mode_image_sc->DebugOn();
 //vtk_b_mode_image_sc->Print(cerr);

 //its_viewer->SetupInteractor(its_iren);
 vtkImageShiftScale* iss = vtkImageShiftScale::New();
 iss->SetShift( b_mode_range[0] * -1 );
 iss->SetScale( 255.0 / static_cast<double>(b_mode_range[1] - b_mode_range[0] ) );
 iss->ClampOverflowOn();
 iss->SetInputConnection( its_image_reader->GetOutputPort(3) );
 iss->SetOutputScalarTypeToUnsignedChar();


 //vtkImageViewer* viewer = vtkImageViewer::New();
 //viewer->SetInputConnection( iss->GetOutputPort() );
 ////viewer->SetInput( vtk_b_mode_image_sc );
 ////viewer->SetInput( pngo );
 //viewer->SetupInteractor( its_iren );

 //viewer->SetZSlice(0);
 //viewer->SetColorWindow( 256.0 );
 ////viewer->SetColorWindow( b_mode_range[1] - b_mode_range[0] );
 //viewer->SetColorLevel( 127.0 );
 ////viewer->SetColorLevel(b_mode_range[0] +  (b_mode_range[1] - b_mode_range[0])/2.0 );
 //viewer->Render();


 vtkStructuredGrid* sg = vtkStructuredGrid::SafeDownCast( its_image_reader->GetOutputDataObject(0) );
 double* sgrange = sg->GetScalarRange();
 cout << "Image range: " << b_mode_range[0] << "  " << b_mode_range[1] << '\n';
 cout << "S Grid range:" << sgrange[0] << "  " << sgrange[1] << '\n';
 vtkDataSetMapper* dsm = vtkDataSetMapper::New();
 dsm->SetColorModeToMapScalars();
 dsm->SetScalarModeToUsePointData();
 dsm->SetInputConnection( its_image_reader->GetOutputPort(0) );
 //dsm->SetInputConnection( its_image_reader->GetOutputPort(3) );
 vtkLookupTable* lut = vtkLookupTable::New();
 lut->SetNumberOfColors(128);
 lut->SetHueRange(0.0, 1.0);
 lut->SetSaturationRange( 0.0, 0.0);
 lut->SetValueRange( 0.0, 1.0 );
 dsm->SetLookupTable(lut);
 dsm->SetScalarRange( b_mode_range[0], b_mode_range[1] );
 vtkActor* pda = vtkActor::New();
 pda->SetMapper( dsm );

 vtkImageActor* ia = vtkImageActor::New();
 ia->SetInput( iss->GetOutput() );

 vtkRenderer* ren = vtkRenderer::New();
 ren->SetBackground(0.0,0.8,0.0);
 //ren->AddViewProp( ia );
 ren->AddViewProp( pda );

 ia->RotateZ(-90.0);
 ia->SetScale( 3.0, 3.0, 1.0 );

 vtkRenderWindow* renwin = vtkRenderWindow::New();
 renwin->AddRenderer( ren );

 ren->Render();
 cout << "Focal Point: " << ren->GetActiveCamera()->GetFocalPoint()[0] << " " << ren->GetActiveCamera()->GetFocalPoint()[1] << " " << ren->GetActiveCamera()->GetFocalPoint()[2] << std::endl;
 cout << "Position:    " << ren->GetActiveCamera()->GetPosition()[0] << " " << ren->GetActiveCamera()->GetPosition()[1] << " " << ren->GetActiveCamera()->GetPosition()[2] << std::endl;

 double* bounds = sg->GetBounds();
 cout << "Bounds: xmin: " << bounds[0] << " xmax: " << bounds[1] << " ymin: " << bounds[2] << " ymax: " << bounds[3] << " zmin: " << bounds[4] << " zmax: " << bounds[5] << std::endl;

 double* first = sg->GetPoints()->GetPoint(0);
 cout << "First point: x: " << first[0] << " y: " << first[1] << " z: " << first[2] << std::endl;

 cout << "Clipping Range: Front: " << ren->GetActiveCamera()->GetClippingRange()[0] << " Back: " << ren->GetActiveCamera()->GetClippingRange()[1] << std::endl;

 vtkCamera* cam = ren->GetActiveCamera();

 double center_y = (bounds[2] + first[1])/2.0;
 double camdist = ((first[1] - bounds[2]) / 0.57735)*1.2 ; // 0.57735 = tan(30 deg) = default ViewAngle
 cam->SetFocalPoint( 0.0, center_y, 0.0 );
 cam->SetPosition( 0.0, center_y, camdist ); 
 cam->SetClippingRange( camdist - 1.0,  camdist + 1.0 );
 cam->ComputeViewPlaneNormal();
 cout << "Clipping Range: Front: " << ren->GetActiveCamera()->GetClippingRange()[0] << " Back: " << ren->GetActiveCamera()->GetClippingRange()[1] << std::endl;
 ren->Render();
 cout << "Position:    " << ren->GetActiveCamera()->GetPosition()[0] << " " << ren->GetActiveCamera()->GetPosition()[1] << " " << ren->GetActiveCamera()->GetPosition()[2] << std::endl;

 renwin->SetSize( 512, int( (first[1] - bounds[2])/(first[0]*-2.0) * 512 ) );


 its_iren->SetRenderWindow( renwin );
 vtkInteractorStyleTrackballCamera* ista= vtkInteractorStyleTrackballCamera::New();
 //its_iren->SetInteractorStyle( its_interactor_style );
 its_iren->SetInteractorStyle( ista );
 its_iren->Initialize();
 its_iren->Start();

 //ren->Delete();
 //renwin->Delete();


}

