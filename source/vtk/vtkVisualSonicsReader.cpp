#include "vtk/vtkVisualSonicsReader.h"


#include <boost/filesystem/convenience.hpp>
namespace bf = boost::filesystem;


#include "vtkImageData.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkUnsignedShortArray.h"
#include "vtkObjectFactory.h"
#include "vtkPointData.h"
#include "vtkSetGet.h"
#include "vtkSmartPointer.h"


#include "cxx/ImageReader.h"
#include "rdiParserData.h"
#include "MetadataReaderBase.h"
#include "sized_ints.h"


using namespace visual_sonics::vtk;

vtkStandardNewMacro(vtkVisualSonicsReader);

vtkVisualSonicsReader::vtkVisualSonicsReader():
  its_image_data_out_type( ImageDouble ),
  its_coord_data_type( CoordDouble )
{
  this->SetNumberOfInputPorts(0);
  this->SetNumberOfOutputPorts(6);

  this->SetDataByteOrderToLittleEndian();
}



vtkVisualSonicsReader::~vtkVisualSonicsReader()
{
  if( its_ir)
  {
    delete its_ir;
    its_ir = 0;
  }
}



/*!
 * set the filename prefix
 * can be full or relative path
 */
void vtkVisualSonicsReader::SetFilePrefix( const char* fileprefix)
{
  bf::path full_prefix_path = bf::system_complete( bf::path( fileprefix ) );

  if ( this->its_ir == 0 )
  {
    // vtk doesn't like to use templates :P
    switch (its_image_data_out_type)
    {
      case ImageFloat:
        switch (its_coord_data_type)
	{
	  case CoordFloat:
	    this->its_ir = new cxx::ImageReader<float,float>( full_prefix_path );
	    break;
	  case CoordDouble:
	    this->its_ir = new cxx::ImageReader<float,double>( full_prefix_path );
	    break;
	}
	break;
      case ImageDouble:
        switch (its_coord_data_type)
	{
	  case CoordFloat:
	    this->its_ir = new cxx::ImageReader<double,float>( full_prefix_path );
	    break;
	  case CoordDouble:
	    this->its_ir = new cxx::ImageReader<double,double>( full_prefix_path );
	    break;
	}
	break;
    }


  }
  else
  {
    this->its_ir->set_in_file_path( full_prefix_path );
  }

  its_rpd = its_ir->get_rpd();
  this->vtkMedicalImageReader2::SetFilePrefix( fileprefix );

}



inline void vtkVisualSonicsReader::SetReadMethod(visual_sonics::ReadMethod read_method)
{
  this->Modified();
  its_ir->set_read_method( read_method);
}



inline visual_sonics::ReadMethod vtkVisualSonicsReader::GetReadMethod()
{
  return	its_ir->get_read_method();
}

inline void vtkVisualSonicsReader::SetSpecificAcquisition(unsigned int specific_acquisition)
{
  this->Modified();
  its_ir->set_specific_acquisition( specific_acquisition );
}



inline unsigned int vtkVisualSonicsReader::GetSpecificAcquisition()
{
  return its_ir->get_specific_acquisition();
}



int vtkVisualSonicsReader::FillOutputPortInformation( int port, vtkInformation* info)
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


int vtkVisualSonicsReader::RequestData(vtkInformation*,
			     vtkInformationVector**,
			     vtkInformationVector* outputVector)
{
  if(!this->its_ir)
  {
    vtkErrorMacro("A FilePrefix must be specified with SetFilePrefix( filename ).");
    return 0;
  }

  its_ir->read_b_mode_image();

  const unsigned int samples_per_line = its_rpd->its_rf_mode_rx_ad_gate_width;
  const unsigned int num_lines = its_rpd->its_rf_mode_tx_trig_tbl_trigs;

  vtkInformation* outInfo = outputVector->GetInformationObject(3);
  vtkImageData* vtk_b_mode_image_sc = vtkImageData::SafeDownCast( outInfo->Get(vtkDataObject::DATA_OBJECT()));
  if (!vtk_b_mode_image_sc)
    return 0;

  // temporarily called sc until I really scan convert them
  // create scout b mode scan converted
  //
  // Extent should be set befor allocate scalars
  vtk_b_mode_image_sc->SetWholeExtent( 0 , samples_per_line - 1, 0, num_lines - 1, 0, 0 );
  //vtk_b_mode_image_sc->SetUpdateExtent( vtk_b_mode_image_sc->GetWholeExtent() );
  vtk_b_mode_image_sc->SetDimensions( samples_per_line, num_lines, 1 );
  vtk_b_mode_image_sc->SetScalarTypeToUnsignedShort();
  vtk_b_mode_image_sc->SetNumberOfScalarComponents(1);
  vtk_b_mode_image_sc->AllocateScalars();
  vtk_b_mode_image_sc->SetSpacing( 1.0, 1.0, 1.0 );
  vtk_b_mode_image_sc->SetOrigin( 0.0, 0.0, 0.0 );
  // fill in scout b mode scan converted values
  UInt16* vtk_b_mode_image_sc_p = static_cast< UInt16* >( vtk_b_mode_image_sc->GetScalarPointer() );
  UInt16* cxx_b_mode_image_sc_p = its_ir->get_b_mode_image_p();

  for(unsigned int i=0; i<num_lines; i++)
  {
    for(unsigned int j=0; j<samples_per_line; j++)
    {
      *vtk_b_mode_image_sc_p = *cxx_b_mode_image_sc_p ;
      cxx_b_mode_image_sc_p++;
      vtk_b_mode_image_sc_p++;
    }
  }





  this->Modified();

  return 1;
}






void vtkVisualSonicsReader::PrintSelf(ostream& os, vtkIndent indent)
{
  os << indent << "ReadMethod:" << indent << this->GetReadMethod() << std::endl;
  os << indent << "SpecificAcquisition:" << indent << this->GetSpecificAcquisition() << std::endl;
  this->Superclass::PrintSelf(os, indent);
}

