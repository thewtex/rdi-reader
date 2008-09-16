#include "ImageViewer.h"


#include <boost/filesystem/convenience.hpp>
namespace bf = boost::filesystem;


#include "vtkCamera.h"
#include "vtkDataSetMapper.h"
#include "vtkLookupTable.h"
#include "vtkImageData.h"
#include "vtkInteractorStyleImage.h"
#include "vtkInteractorStyleTrackballCamera.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkSmartPointer.h"
#include "vtkStructuredGrid.h"


#include "vtk/vtkVisualSonicsReader.h"
#include "common/ReadMethod.h"


using namespace visual_sonics::vtk;


//ImageViewer::ImageViewer( const bf::path& in_file_path, const bf::path& in_file_name, std::vector<unsigned int>&  frames_to_read)
//{
  //this->its_image_reader = vtkVisualSonicsReader::New();
  //this->its_image_reader->SetFilePrefix( (in_file_path / in_file_name).native_file_string() );
  //this->its_interactor_style = vtkInteractorStyleTrackballCamera::New();
  //this->its_iren = vtkRenderWindowInteractor::New();
//}



//ImageViewer::ImageViewer(const bf::path& in_file_path, const bf::path& in_file_name, std::vector<unsigned int>&  frames_to_read, ReadMethod read_method, unsigned int specific_acquisition )
//{
  //this->its_image_reader = vtkVisualSonicsReader::New();
  //this->its_image_reader->SetFilePrefix( (in_file_path / in_file_name).native_file_string() );
  //this->its_image_reader->SetReadMethod( read_method );
  //this->its_image_reader->SetSpecificAcquisition( specific_acquisition );
  //this->its_interactor_style = vtkInteractorStyleTrackballCamera::New();
  //this->its_iren = vtkRenderWindowInteractor::New();
//}



ImageViewer::ImageViewer( const bf::path& in_file_path, ReadMethod read_method, unsigned int specific_acquisition  )
{
  its_image_reader = vtkVisualSonicsReader::New();
    its_image_reader->SetFilePrefix( in_file_path.native_file_string().c_str() );
    its_image_reader->SetReadMethod( read_method );
    its_image_reader->SetSpecificAcquisition( specific_acquisition );
  its_ren_win = vtkRenderWindow::New();
  its_interactor_style_image = vtkInteractorStyleImage::New();
  its_interactor_style_trackball = vtkInteractorStyleTrackballCamera::New();
  its_iren = vtkRenderWindowInteractor::New();
  its_iren->SetRenderWindow( its_ren_win );
}



ImageViewer::ImageViewer( const bf::path& in_file_path )
{
  its_image_reader = vtkVisualSonicsReader::New();
    its_image_reader->SetFilePrefix( in_file_path.native_file_string().c_str() );
  its_ren_win = vtkRenderWindow::New();
  its_interactor_style_image = vtkInteractorStyleImage::New();
  its_interactor_style_trackball = vtkInteractorStyleTrackballCamera::New();
  its_iren = vtkRenderWindowInteractor::New();
  its_iren->SetRenderWindow( its_ren_win );
}



ImageViewer::~ImageViewer()
{
  its_image_reader->Delete();
  its_ren_win->Delete();
  its_interactor_style_image->Delete();
  its_interactor_style_trackball->Delete();
  its_iren->Delete();
}



void ImageViewer::view_b_mode()
{
 // mapper ( has internal GeometryFilter so output is PolyData )
 vtkSmartPointer<vtkDataSetMapper> dsm = vtkSmartPointer<vtkDataSetMapper>::New();
 dsm->SetColorModeToMapScalars();
 dsm->SetScalarModeToUsePointData();
 dsm->SetInputConnection( its_image_reader->GetOutputPort(0) );
 dsm->Update();

 // get the output
 vtkStructuredGrid* vtk_b_mode_sg = vtkStructuredGrid::SafeDownCast( its_image_reader->GetOutputDataObject(0) );
 double* b_mode_range = vtk_b_mode_sg->GetScalarRange();

 // Lookup Table
 vtkSmartPointer<vtkLookupTable> lut = vtkSmartPointer<vtkLookupTable>::New();
 lut->SetNumberOfColors(65536);
 lut->SetHueRange(0.0, 1.0);
 lut->SetSaturationRange( 0.0, 0.0);
 lut->SetValueRange( 0.0, 1.0 );

 dsm->SetLookupTable(lut);
 dsm->SetScalarRange( b_mode_range[0], b_mode_range[1] );

 // actor
 vtkSmartPointer<vtkActor> pda = vtkSmartPointer<vtkActor>::New();
 pda->SetMapper( dsm );

 // renderer
 vtkSmartPointer<vtkRenderer> ren = vtkSmartPointer<vtkRenderer>::New();
 ren->SetBackground( its_background_color_red, its_background_color_green, its_background_color_blue   );
 ren->AddViewProp( pda );
 its_ren_win->AddRenderer( ren );

 // adjust camera location ( otherwise includes y=0 be included by default )
 double* bounds = vtk_b_mode_sg->GetBounds();
 double* first = vtk_b_mode_sg->GetPoints()->GetPoint(0);
 double center_y = (bounds[2] + first[1])/2.0;
 double camdist = ((first[1] - bounds[2]) / 0.57735)*1.2 ; // 0.57735 = tan(30 deg) = default ViewAngle

 vtkCamera* cam = ren->GetActiveCamera();
 cam->SetFocalPoint( 0.0, center_y, 0.0 );
 cam->SetPosition( 0.0, center_y, camdist );
 cam->SetClippingRange( camdist - 1.0,  camdist + 1.0 );
 cam->ComputeViewPlaneNormal();

 its_ren_win->SetSize( its_default_width, int( (first[1] - bounds[2])/(first[0]*-2.0) * its_default_width ) );

 // interactor
 its_iren->SetInteractorStyle( its_interactor_style_image );
 its_iren->Initialize();
 its_iren->Start();


}



void ImageViewer::view_saturation()
{

 // mapper ( has internal GeometryFilter so output is PolyData )
 vtkSmartPointer<vtkDataSetMapper> dsm = vtkSmartPointer<vtkDataSetMapper>::New();
 dsm->SetColorModeToMapScalars();
 dsm->SetScalarModeToUsePointData();
 dsm->SetInputConnection( its_image_reader->GetOutputPort(1) );
 dsm->Update();

 // get the output
 vtkStructuredGrid* vtk_saturation_sg = vtkStructuredGrid::SafeDownCast( its_image_reader->GetOutputDataObject(1) );
 
 // Lookup Table
 vtkSmartPointer<vtkLookupTable> lut = vtkSmartPointer<vtkLookupTable>::New();
 lut->SetNumberOfColors(2);
 lut->SetTableValue( 0, 0.0, 0.0, 0.0 );
 // we use the same colors the VisualSonics peeps use in their software
 lut->SetTableValue( 1, 0.0, 1.0, 1.0 );

 dsm->SetLookupTable(lut);
 dsm->SetScalarRange( 0.0, 1.0 );

 // actor
 vtkSmartPointer<vtkActor> pda = vtkSmartPointer<vtkActor>::New();
 pda->SetMapper( dsm );

 // renderer
 vtkSmartPointer<vtkRenderer> ren = vtkSmartPointer<vtkRenderer>::New();
 ren->SetBackground( its_background_color_red, its_background_color_green, its_background_color_blue   );
 ren->AddViewProp( pda );
 its_ren_win->AddRenderer( ren );

 // adjust camera location ( otherwise includes y=0 be included by default )
 double* bounds = vtk_saturation_sg->GetBounds();
 double* first = vtk_saturation_sg->GetPoints()->GetPoint(0);
 double center_y = (bounds[2] + first[1])/2.0;
 double camdist = ((first[1] - bounds[2]) / 0.57735)*1.2 ; // 0.57735 = tan(30 deg) = default ViewAngle

 vtkCamera* cam = ren->GetActiveCamera();
 cam->SetFocalPoint( 0.0, center_y, 0.0 );
 cam->SetPosition( 0.0, center_y, camdist );
 cam->SetClippingRange( camdist - 1.0,  camdist + 1.0 );
 cam->ComputeViewPlaneNormal();

 its_ren_win->SetSize( its_default_width, int( (first[1] - bounds[2])/(first[0]*-2.0) * its_default_width ) );

 // interactor
 its_iren->SetInteractorStyle( its_interactor_style_image );
 its_iren->Initialize();
 its_iren->Start();

}




void ImageViewer::view_rf()
{
 // mapper ( has internal GeometryFilter so output is PolyData )
 vtkSmartPointer<vtkDataSetMapper> dsm = vtkSmartPointer<vtkDataSetMapper>::New();
   dsm->SetColorModeToMapScalars();
   dsm->SetScalarModeToUsePointData();
   dsm->SetInputConnection( its_image_reader->GetOutputPort(2) );
   dsm->Update();

 // get the output
 vtkStructuredGrid* vtk_rf_sg = vtkStructuredGrid::SafeDownCast( its_image_reader->GetOutputDataObject(2) );

 // Lookup Table
 vtkSmartPointer<vtkLookupTable> lut = vtkSmartPointer<vtkLookupTable>::New();
   lut->SetNumberOfColors(65536);
   lut->SetHueRange(0.0, 1.0);
   lut->SetSaturationRange( 0.0, 0.0);
   lut->SetValueRange( 0.0, 1.0 );

 dsm->SetLookupTable(lut);
 double rf_range[2];
 its_image_reader->GetScalarRange( rf_range );
 //dsm->SetScalarRange( rf_range[0], rf_range[1] );
 dsm->SetScalarRange( 0.0, rf_range[1] / 6.0 );

 // actor
 vtkSmartPointer<vtkActor> pda = vtkSmartPointer<vtkActor>::New();
 pda->SetMapper( dsm );

 // renderer
 vtkSmartPointer<vtkRenderer> ren = vtkSmartPointer<vtkRenderer>::New();
 ren->SetBackground( its_background_color_red, its_background_color_green, its_background_color_blue   );
 ren->AddViewProp( pda );
 its_ren_win->AddRenderer( ren );

 // adjust camera location ( otherwise includes y=0 be included by default )
 double* bounds = vtk_rf_sg->GetBounds();
 double* first = vtk_rf_sg->GetPoints()->GetPoint(0);
 double center_y = (bounds[2] + first[1])/2.0;
 double camdist = ((first[1] - bounds[2]) / 0.57735)*1.2 ; // 0.57735 = tan(30 deg) = default ViewAngle

 vtk_rf_sg->Print(cout);
 cout << "range[0]: " << rf_range[0] << " range[1]: " << rf_range[1] << std::endl;
 cout << "range_ir[0]: " << rf_range[0] << " range[1]: " << rf_range[1] << std::endl;

 vtkCamera* cam = ren->GetActiveCamera();
 cam->SetFocalPoint( 0.0, center_y, 0.0 );
 cam->SetPosition( 0.0, center_y, camdist );
 cam->SetClippingRange( camdist - 1.0,  camdist + 1.0 );
 cam->ComputeViewPlaneNormal();

 its_ren_win->SetSize( its_default_width, int( (first[1] - bounds[2])/(first[0]*-2.0) * its_default_width ) );

 // interactor
 its_iren->SetInteractorStyle( its_interactor_style_trackball );
 its_iren->Initialize();
 its_iren->Start();

}

