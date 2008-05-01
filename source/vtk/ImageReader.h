/*! @file  ImageReader.h
 *  @brief read the visual sonics file into a vtk ImageData, and save or view
 *
 *  @author Matt McCormick (thewtex) <matt@mmmccormick.com>
 *  @date   2008 March 17
 *
 */

#ifndef VTK_READIMAGE_H
#define VTK_READIMAGE_H

#include "cxx/ImageReader.h"
#include "vtkMedicalImageReader2.h"

#include "vtkSetGet.h"


class vtkImageData;
class vtkUnsignedShortArray;

namespace visual_sonics
{

  namespace vtk
  {
    class ImageReader: public cxx::ImageReader, public vtkMedicalImageReader2
    {
    public:

      static ImageReader* New();

	  typedef visual_sonics::vtk::ImageReader visual_sonicsvtkImageReader;
      vtkTypeRevisionMacro(visual_sonicsvtkImageReader, vtkMedicalImageReader2)

      ImageReader( const bf::path& in_file_path, const bf::path& in_file_name, std::vector<unsigned int>&  frames_to_read);
      ImageReader(const bf::path& in_file_path, const bf::path& in_file_name, std::vector<unsigned int>&  frames_to_read, ReadMethod read_method, unsigned int specific_acquisition = 0);

      ImageReader( const bf::path& in_file_path, const bf::path& in_file_name, ReadMethod read_method, unsigned int specific_acquisition = 0 );
      ImageReader( const bf::path& in_file_path, const bf::path& in_file_name);

      virtual ~ImageReader();


      virtual void read_b_mode_image();
      virtual void read_saturation_image();
      virtual bool read_rf_data_image();

    protected:
      vtkUnsignedShortArray* its_vtk_b_mode_array;
      vtkImageData* its_vtk_b_mode_image;

    private:

      //! I have no need for these at this point --write them as needed
      ImageReader( const ImageReader& );
      ImageReader&  operator=( const ImageReader& );
  };

  }
}

#endif // VTK_READIMAGE_H inclusion guard

