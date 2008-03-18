#include "vtk/ReadImage.h"


#include <boost/filesystem/convenience.hpp>
namespace bf = boost::filesystem;


#include "vtkImageData.h"
#include "vtkUnsignedShortArray.h"
#include "vtkPointData.h"


#include "cxx/ReadImage.h"
#include "rdiParserData.h"
#include "ReadMetadataBase.h"


using namespace visual_sonics::vtk;


ReadImage::ReadImage( const bf::path& in_file_path, const bf::path& in_file_name, std::vector<unsigned int>&  frames_to_read):
  visual_sonics::cxx::ReadImage( in_file_path, in_file_name, frames_to_read)
{
  its_vtk_b_mode_array = vtkUnsignedShortArray::New();
  its_vtk_b_mode_image = vtkImageData::New();
  its_vtk_b_mode_image->SetScalarType(VTK_UNSIGNED_SHORT);
  its_vtk_b_mode_image->SetSpacing( 1.0, 1.0, 1.0 );
  its_vtk_b_mode_image->SetOrigin( 0.0, 0.0, 0.0 );
}



ReadImage::ReadImage(const bf::path& in_file_path, const bf::path& in_file_name, std::vector<unsigned int>&  frames_to_read, ReadMethod read_method, unsigned int specific_acquisition ):
  visual_sonics::cxx::ReadImage( in_file_path, in_file_name, frames_to_read, read_method, specific_acquisition )
{
  its_vtk_b_mode_array = vtkUnsignedShortArray::New();
  its_vtk_b_mode_image = vtkImageData::New();
  its_vtk_b_mode_image->SetScalarType(VTK_UNSIGNED_SHORT);
  its_vtk_b_mode_image->SetSpacing( 1.0, 1.0, 1.0 );
  its_vtk_b_mode_image->SetOrigin( 0.0, 0.0, 0.0 );
}



ReadImage::ReadImage( const bf::path& in_file_path, const bf::path& in_file_name, ReadMethod read_method, unsigned int specific_acquisition  ): 
  visual_sonics::cxx::ReadImage( in_file_path, in_file_name, read_method, specific_acquisition)
{
  its_vtk_b_mode_array = vtkUnsignedShortArray::New();
  its_vtk_b_mode_image = vtkImageData::New();
  its_vtk_b_mode_image->SetScalarType(VTK_UNSIGNED_SHORT);
  its_vtk_b_mode_image->SetSpacing( 1.0, 1.0, 1.0 );
  its_vtk_b_mode_image->SetOrigin( 0.0, 0.0, 0.0 );
}



ReadImage::ReadImage( const bf::path& in_file_path, const bf::path& in_file_name):
  visual_sonics::cxx::ReadImage( in_file_path, in_file_name )
{
  its_vtk_b_mode_array = vtkUnsignedShortArray::New();
  its_vtk_b_mode_image = vtkImageData::New();
  its_vtk_b_mode_image->SetScalarType(VTK_UNSIGNED_SHORT);
  its_vtk_b_mode_image->SetSpacing( 1.0, 1.0, 1.0 );
  its_vtk_b_mode_image->SetOrigin( 0.0, 0.0, 0.0 );
}



ReadImage::~ReadImage()
{
  its_vtk_b_mode_array->Delete();
  its_vtk_b_mode_image->Delete();
}


void ReadImage::read_b_mode_image()
{
  this->cxx::ReadImage::read_b_mode_image();


  const unsigned int samples_per_line = this->its_read_metadata->its_rpd->its_rf_mode_rx_ad_gate_width;
  const unsigned int num_lines = this->its_read_metadata->its_rpd->its_rf_mode_tx_trig_tbl_trigs;

  its_vtk_b_mode_array->SetArray( &cxx::ReadImage::its_b_mode_image[0], samples_per_line*num_lines, 1);
  its_vtk_b_mode_image->GetPointData()->SetScalars( its_vtk_b_mode_array );
  its_vtk_b_mode_image->SetDimensions( samples_per_line, num_lines, 1 );
  


  
}



void ReadImage::read_saturation_image()
{
}



bool ReadImage::read_rf_data_image()
{
  return true;
}

