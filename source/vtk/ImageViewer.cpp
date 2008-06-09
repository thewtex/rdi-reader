#include "ImageViewer.h"


#include <boost/filesystem/convenience.hpp>
namespace bf = boost::filesystem;


#include "vtkDataSetMapper.h"
#include "vtkLookupTable.h"
#include "vtkImageData.h"
#include "vtkImageViewer.h"
#include "vtkInteractorStyleTrackballCamera.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkSmartPointer.h"
#include "vtkStructuredGrid.h"

#include "vtkImageActor.h"
#include "vtkImageShiftScale.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"

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
  its_interactor_style = vtkInteractorStyleTrackballCamera::New();
  its_iren = vtkRenderWindowInteractor::New();
}



ImageViewer::ImageViewer( const bf::path& in_file_path )
{
  its_image_reader = vtkVisualSonicsReader::New();
  its_image_reader->SetFilePrefix( in_file_path.native_file_string().c_str() );
  its_interactor_style = vtkInteractorStyleTrackballCamera::New();
  its_iren = vtkRenderWindowInteractor::New();
}



ImageViewer::~ImageViewer()
{
  its_image_reader->Delete();
  its_interactor_style->Delete();
  its_iren->Delete();
}


#include "vtkCamera.h"
#include "vtkPointData.h"
#include "vtkDataArray.h"
#include "vtkUnsignedShortArray.h"
void ImageViewer::view_b_mode()
{
 its_image_reader->Update();

 vtkStructuredGrid* vtk_b_mode_sg = vtkStructuredGrid::SafeDownCast( its_image_reader->GetOutputDataObject(0) );
 double* b_mode_range = vtk_b_mode_sg->GetScalarRange();

 vtkSmartPointer<vtkDataSetMapper> dsm = vtkSmartPointer<vtkDataSetMapper>::New();
 dsm->SetColorModeToMapScalars();
 dsm->SetScalarModeToUsePointData();
 dsm->SetInputConnection( its_image_reader->GetOutputPort(0) );

 vtkSmartPointer<vtkLookupTable> lut = vtkSmartPointer<vtkLookupTable>::New();
 lut->SetNumberOfColors(65536);
 lut->SetHueRange(0.0, 1.0);
 lut->SetSaturationRange( 0.0, 0.0);
 lut->SetValueRange( 0.0, 1.0 );

 dsm->SetLookupTable(lut);
 dsm->SetScalarRange( b_mode_range[0], b_mode_range[1] );

 vtkSmartPointer<vtkActor> pda = vtkSmartPointer<vtkActor>::New();
 pda->SetMapper( dsm );

 vtkRenderer* ren = vtkRenderer::New();
 ren->SetBackground( 0.0, 0.367 , 0.0);
 ren->AddViewProp( pda );

 vtkSmartPointer<vtkRenderWindow> renwin = vtkSmartPointer<vtkRenderWindow>::New();
 renwin->AddRenderer( ren );

 ren->Render();



 double* bounds = vtk_b_mode_sg->GetBounds();
 double* first = vtk_b_mode_sg->GetPoints()->GetPoint(0);

 vtkCamera* cam = ren->GetActiveCamera();

 double center_y = (bounds[2] + first[1])/2.0;
 double camdist = ((first[1] - bounds[2]) / 0.57735)*1.2 ; // 0.57735 = tan(30 deg) = default ViewAngle
 cam->SetFocalPoint( 0.0, center_y, 0.0 );
 cam->SetPosition( 0.0, center_y, camdist );
 cam->SetClippingRange( camdist - 1.0,  camdist + 1.0 );
 cam->ComputeViewPlaneNormal();
 ren->Render();

 renwin->SetSize( 512, int( (first[1] - bounds[2])/(first[0]*-2.0) * 512 ) );


 its_iren->SetRenderWindow( renwin );
 its_iren->SetInteractorStyle( its_interactor_style );
 its_iren->Initialize();
 its_iren->Start();

 //ren->Delete();
 //renwin->Delete();


}

