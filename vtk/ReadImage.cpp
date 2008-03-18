#include "vtk/ReadImage.h"


#include <boost/filesystem/convenience.hpp>
namespace bf = boost::filesystem;


#include "vtkUnsignedShortArray.h"


#include "cxx/ReadImage.h"


using namespace visual_sonics::vtk;


ReadImage::ReadImage( const bf::path& in_file_path, const bf::path& in_file_name, std::vector<unsigned int>&  frames_to_read):
  visual_sonics::cxx::ReadImage( in_file_path, in_file_name, frames_to_read)
{
  its_vtk_b_mode_image = vtkUnsignedShortArray::New();
}



ReadImage::ReadImage(const bf::path& in_file_path, const bf::path& in_file_name, std::vector<unsigned int>&  frames_to_read, ReadMethod read_method, unsigned int specific_acquisition ):
  visual_sonics::cxx::ReadImage( in_file_path, in_file_name, frames_to_read, read_method, specific_acquisition )
{
  its_vtk_b_mode_image = vtkUnsignedShortArray::New();
}



ReadImage::ReadImage( const bf::path& in_file_path, const bf::path& in_file_name, ReadMethod read_method, unsigned int specific_acquisition  ): 
  visual_sonics::cxx::ReadImage( in_file_path, in_file_name, read_method, specific_acquisition)
{
  its_vtk_b_mode_image = vtkUnsignedShortArray::New();
}



ReadImage::ReadImage( const bf::path& in_file_path, const bf::path& in_file_name):
  visual_sonics::cxx::ReadImage( in_file_path, in_file_name )
{
  its_vtk_b_mode_image = vtkUnsignedShortArray::New();
}



ReadImage::~ReadImage()
{
  its_vtk_b_mode_image->Delete();
}


void ReadImage::read_b_mode_image()
{
  this->cxx::ReadImage::read_b_mode_image();

  
}



void ReadImage::read_saturation_image()
{
}



bool ReadImage::read_rf_data_image()
{
  return true;
}

