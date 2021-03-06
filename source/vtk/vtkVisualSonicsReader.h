/*! @file  vtkVisualSonicsReader.h
 *  @brief read the Visual Sonics Digital RF files into a vtkImageData
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

#include "common/ReadMethod.h"

#include "vtkSetGet.h"


using namespace visual_sonics;

namespace visual_sonics
{
  class rdiParserData;

  namespace cxx
  {
    template<class ImageDataOutT, class CoordT> class ImageReader;
  }
}

class vtkImageData;
class vtkUnsignedShortArray;

/*! @class vtkVisualSonicsReader
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
 *   1-2, 4-5 is a 2-D data set
 *   3, 6 are 3-D data set where the third dimension is the frame number
 */
class vtkVisualSonicsReader: public vtkMedicalImageReader2
{
public:

  static vtkVisualSonicsReader* New();

  vtkTypeMacro(vtkVisualSonicsReader, vtkMedicalImageReader2)


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

  virtual void SetReadMethod(visual_sonics::ReadMethod read_method);
  virtual visual_sonics::ReadMethod GetReadMethod();

  //! specific acquisition if read_method = specific_acquisition
  virtual void SetSpecificAcquisition(unsigned int specific_acquisition);
  unsigned int GetSpecificAcquisition();

  //! puts the rf raw image min,max in the input array, must have read the image to be useful
  virtual void GetScalarRange( double range[2] );


  //! add implementation for vtkDemandDrivenPipeline::REQUEST_DATA_NOT_GENERATED()
  virtual int ProcessRequest( vtkInformation*,
      vtkInformationVector**,
      vtkInformationVector*   );


  void PrintSelf(ostream&, vtkIndent);


protected:

  //! define what the outputs are
  virtual int FillOutputPortInformation( int, vtkInformation* );

  //! specifies information about the output, dimensions, etc
  virtual int RequestInformation(vtkInformation*,
      vtkInformationVector **,
      vtkInformationVector *);

  //! specifies which output ports will be updated given the request on a port
  //! if the request is on ports 0-2, we only generate those raw grid images
  //! if the request is on ports 3-5, we additionally generate the scan converted images
  virtual int RequestDataNotGenerated( vtkInformation*,
      vtkInformationVector**,
      vtkInformationVector* );

  //! does the actual data crunching at a pipeline request
  virtual int RequestData(vtkInformation*,
		      vtkInformationVector**,
		      vtkInformationVector* outputVector);

  //! break this->RequestData into 3 logical sections: ReadBMode, ReadSaturation, ReadRF
  int ReadBMode(vtkInformationVector* outputVector, const bool& do_scan_conv);
  int ReadSaturation(vtkInformationVector* outputVector, const bool& do_scan_conv);
  int ReadRF(vtkInformation* request,
      vtkInformationVector* outputVector,
      const bool& do_scan_conv);

  //! does the heavy lifting for reading in the data
  cxx::ImageReader<double,double>* its_ir;

  const static visual_sonics::ReadMethod its_default_read_method  = file_average;
  const static unsigned int its_default_specific_acquisition =  0;

  vtkVisualSonicsReader();
  virtual ~vtkVisualSonicsReader();


private:


  //! I have no need for these at this point --write them as needed
  vtkVisualSonicsReader( const vtkVisualSonicsReader& );
  vtkVisualSonicsReader&  operator=( const vtkVisualSonicsReader& );
};

#endif // VTK_READIMAGE_H inclusion guard

