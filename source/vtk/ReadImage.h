/*! @file  ReadImage.h
 *  @brief read the visual sonics file into a vtk ImageData, and save or view
 *
 *  @author Matt McCormick (thewtex) <matt@mmmccormick.com>
 *  @date   2008 March 17
 *
 */

#ifndef VTK_READIMAGE_H
#define VTK_READIMAGE_H

#include "cxx/ReadImage.h"
#include "vtkMedicalImageReader.h"


class vtkImageData;
class vtkUnsignedShortArray;

namespace visual_sonics
{

  namespace vtk
  {
    class ReadImage: public cxx::ReadImage, public vtkMedicalImageReader2
    {
    public:
      ReadImage( const bf::path& in_file_path, const bf::path& in_file_name, std::vector<unsigned int>&  frames_to_read);
      ReadImage(const bf::path& in_file_path, const bf::path& in_file_name, std::vector<unsigned int>&  frames_to_read, ReadMethod read_method, unsigned int specific_acquisition = 0); 

      ReadImage( const bf::path& in_file_path, const bf::path& in_file_name, ReadMethod read_method, unsigned int specific_acquisition = 0 );
      ReadImage( const bf::path& in_file_path, const bf::path& in_file_name);

      virtual ~ReadImage();
  

      virtual void read_b_mode_image();
      virtual void read_saturation_image();
      virtual bool read_rf_data_image();

    protected:
      vtkUnsignedShortArray* its_vtk_b_mode_array;
      vtkImageData* its_vtk_b_mode_image;

    private:

      //! I have no need for these at this point --write them as needed
      ReadImage( const ReadImage& );
      ReadImage&  operator=( const ReadImage& );
  };

  }
}

#endif // VTK_READIMAGE_H inclusion guard

