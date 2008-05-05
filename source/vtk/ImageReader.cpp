#include "vtk/ImageReader.h"


#include <boost/filesystem/convenience.hpp>
namespace bf = boost::filesystem;


#include "vtkImageData.h"
#include "vtkUnsignedShortArray.h"
#include "vtkObjectFactory.h"
#include "vtkPointData.h"
#include "vtkSetGet.h"
#include "vtkSmartPointer.h"


#include "cxx/ImageReader.h"
#include "rdiParserData.h"
#include "MetadataReaderBase.h"


using namespace visual_sonics::vtk;

vtkStandardNewMacro(ImageReader);

ImageReader::ImageReader()
{
  this->SetNumberOfInputPorts(0);
  this->SetNumberOfOutputPorts(6);

  this->SetDataByteOrderToLittleEndian();


}



ImageReader::~ImageReader()
{
  delete its_ir;
}



/*!
 * set the filename prefix
 * can be full or relative path
 */
ImageReader::SetFilePrefix( const char* fileprefix)
{
  bf::path full_prefix_path = bs::system_complete( bs::path( fileprefix, bs::native) );

  if ( this->its_ir == 0 )
  {
    this->its_ir = new cxx::ImageReader( full_prefix_path.branch_path(), full_prefix_path.leaf() );
  }
  else
  {
    this->its_ir->its_metadata_reader->set_in_file_name( full_prefix_path.leaf() );
    this->its_ir->its_metadata_reader->set_in_file_path( full_prefix_path.branch_path() );
  }

  this->MedicalImageReader2->SetFilePrefix( fileprefix );

}


int ImageReader::FillOutputPortInformation( int port, vtkInformation* info)
{
  if (port < 3)
  {
    info->Set(vtkDataObject::DATA_TYPE_NAME(), "vtkStructuredGrid" );
    return 1;
  }
  else if (port < 6)
  {
    info->Set(vtkDataObject::DATA_TYPE_NAME(), "vtkImageData" );
    return 1;
  }
  else
    return 0;
}


int ImageReader::RequestData(vtkInformation*,
			     vtkInformationVector**,
			     vtkInformationVector* outputVector)
{
  if(!this->its_ir)
  {
    vtkErrorMacro("A FilePrefix must be specified with SetFilePrefix( filename ).");
    return 0;
  }

  this->cxx::ImageReader::read_b_mode_image();

  const unsigned int samples_per_line = this->its_metadata_reader->its_rpd->its_rf_mode_rx_ad_gate_width;
  const unsigned int num_lines = this->its_metadata_reader->its_rpd->its_rf_mode_tx_trig_tbl_trigs;

  // temporarily called sc until I really scan convert them
  // create scout b mode scan converted
  vtkSmartPointer<vtkImageData> vtk_b_mode_image_sc = vtkSmartPointer<vtkImageData>::New()
  vtk_b_mode_image_sc->SetDimensions( samples_per_line, num_lines, 1 );
  vtk_b_mode_image_sc->SetScalarTypeToUnsignedShort();
  vtk_b_mode_image_sc->SetNumberOfScalarComponents(1);
  vtk_b_mode_image_sc->AllocateScalars();
  vtk_b_mode_image_sc->SetSpacing( 1.0, 1.0, 1.0 );
  vtk_b_mode_image_sc->SetOrigin( 0.0, 0.0, 0.0 );
  // fill in scout b mode scan converted values
  unsigned short* vtk_b_mode_image_sc_p = static_cast< unsigned short* > vtk_b_mode_image_sc->GetScalarPointer();
  for( int i=0; i<num_lines; i++)
  {
    for( int j=0; j<samples_per_line; j++)
    {
      *vtk_b_mode_image_sc_p = its_ir.its_b_mode_image[ i*samples_per_line + j ] ;
      vtk_b_mode_image_sc_p++;
    }
  }





  return 1;
}






void ImageReader::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
}

