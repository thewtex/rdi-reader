/*! @file  ImageReader.h
 *  @brief read the visual sonics file into a vtk ImageData, and save or view
 *
 *  @author Matt McCormick (thewtex) <matt@mmmccormick.com>
 *  @date   2008 March 17
 *
 */

#ifndef VTK_READIMAGE_H
#define VTK_READIMAGE_H

#include <boost/filesystem/convenience.hpp>
namespace bf = boost::filesystem;

#include "vtkMedicalImageReader2.h"

#include "vtkSetGet.h"

class visual_sonics::cxx::ImageReader;

class vtkImageData;
class vtkUnsignedShortArray;

namespace visual_sonics
{

  namespace vtk
  {
    /*! @class visual_sonics::vtk::ImageReader
     *
     * A vtk Reader (source algorithm)
     * has six output ports:
     *   1. raw scout window b-mode image
     *   2. raw scout window saturation image
     *   3. raw rfdata image 
     *   4. scan converted scout window b-mode image
     *   5. scan converted scout window saturation image
     *   6. scan converted rfdata image
     *
     *   1-3 output vtkStructuredGrid
     *   4-6 output vtkImageData
     *
     *   1-2, 4-5 is a 2-D image
     *   3, 6 are 3-D images where the third dimension is the frame number
     */
    class ImageReader: public vtkMedicalImageReader2
    {
    public:

      static ImageReader* New();

      typedef visual_sonics::vtk::ImageReader visual_sonicsvtkImageReader;
      vtkTypeRevisionMacro(visual_sonicsvtkImageReader, vtkMedicalImageReader2)


      virtual const char* GetFileExtensions()
      {
	return ".rdi .rdb";
      }

      virtual const char* GetDescriptiveName()
      {
	return "VisualSonicsDigitalRF";
      }

      //! set the input filename prefix
      //! this is the prefix less the '.rdi' and '.rdb' extension
      //! both files must be present in the same directory
      virtual void SetFilePrefix( const char *);

      void SetReadMethod(ReadMethod read_method)
      {
	this->its_read_method = read_method;
      }

      //! specific acquisition if its_read_method = specific_acquisition
      void SetSpecificAcquisition(unsigned int specific_acquisition)
      {
	this->its_specific_acquisition = specific_acquisition;
      }

      virtual void Delete();


    protected:
      vtkUnsignedShortArray* its_vtk_b_mode_array;
      vtkImageData* its_vtk_b_mode_image;

      virtual int FillOutputPortInformation( int, vtkInformation* );

      int RequestData(vtkInformation*,
		      vtkInformationVector**,
		      vtkInformationVector*);

      cxx::ImageReader* its_ir = 0;

      const static ReadMethod its_default_read_method( file_average );
      const static unsigned int its_default_specific_acquisition(0);

      ImageReader(){}
      ~ImageReader(){}


    private:


      //! I have no need for these at this point --write them as needed
      ImageReader( const ImageReader& );
      ImageReader&  operator=( const ImageReader& );
  };

  }
}

#endif // VTK_READIMAGE_H inclusion guard

