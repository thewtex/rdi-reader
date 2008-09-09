#include "vtk/vtkVisualSonicsReader.h"


#include <boost/filesystem/convenience.hpp>
namespace bf = boost::filesystem;


#include "vtkImageData.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkObjectFactory.h"
#include "vtkPointData.h"
#include "vtkPoints.h"
#include "vtkSetGet.h"
#include "vtkShortArray.h"
#include "vtkSmartPointer.h"
#include "vtkStructuredGrid.h"
#include "vtkUnsignedCharArray.h"
#include "vtkUnsignedShortArray.h"


#include "cxx/ImageReader.h"
#include "common/spirit_parse/rdiParserData.h"
#include "common/MetadataReaderBase.h"
#include "common/sized_ints.h"


using namespace visual_sonics::vtk;

vtkStandardNewMacro(vtkVisualSonicsReader);

vtkVisualSonicsReader::vtkVisualSonicsReader():
  its_ir( 0 )
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
    this->its_ir = new cxx::ImageReader<double,double>( full_prefix_path );
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



int vtkVisualSonicsReader::RequestInformation( vtkInformation* request,
    vtkInformationVector ** inputVector, 
    vtkInformationVector *  outputVector)
{

}

int vtkVisualSonicsReader::RequestData(vtkInformation*,
			     vtkInformationVector**,
			     vtkInformationVector* outputVector)
{

  // check to make sure its_ir has been prepared
  if(!this->its_ir)
  {
    vtkErrorMacro("A FilePrefix must be specified with SetFilePrefix( filename ).");
    return 0;
  }


  if(!this->ReadBMode(outputVector) )
    return 0;

  if(!this->ReadSaturation(outputVector) )
    return 0;

  if(!this->ReadRF(outputVector) )
    return 0;

  this->Modified();

  return 1;
}




int vtkVisualSonicsReader::ReadBMode( vtkInformationVector* outputVector)
{
  // read in the image
  its_ir->read_b_mode_image();

  //---------- b_mode_image_raw -----------------
  vtkInformation* outInfo = outputVector->GetInformationObject(0);
  vtkStructuredGrid* vtk_b_mode_image_raw = vtkStructuredGrid::SafeDownCast( outInfo->Get(vtkDataObject::DATA_OBJECT()));
  if (!vtk_b_mode_image_raw)
    return 0;

  const unsigned int samples_per_line = its_rpd->its_rf_mode_rx_ad_gate_width;
  const unsigned int num_lines = its_rpd->its_rf_mode_tx_trig_tbl_trigs;

  vtk_b_mode_image_raw->SetWholeExtent( 0, num_lines - 1, 0, samples_per_line - 1, 0, 0 );
  vtk_b_mode_image_raw->SetDimensions( num_lines, samples_per_line, 1 );


  vtkSmartPointer<vtkPoints> b_mode_raw_points = vtkSmartPointer<vtkPoints>::New();
  b_mode_raw_points->SetNumberOfPoints( samples_per_line*num_lines*1 );

  vtkSmartPointer<vtkUnsignedShortArray> b_mode_raw_data = vtkSmartPointer<vtkUnsignedShortArray>::New();
  b_mode_raw_data->SetNumberOfComponents(1);
  b_mode_raw_data->SetNumberOfTuples( samples_per_line*num_lines*1 );


  std::vector<UInt16>::const_iterator b_mode_image_it = its_ir->get_b_mode_image().begin();
  std::vector<double>::const_iterator b_mode_image_x = its_ir->get_b_mode_image_x().begin();
  std::vector<double>::const_iterator b_mode_image_y = its_ir->get_b_mode_image_y().begin();

  UInt16* vtk_b_mode_raw_data_p = static_cast< UInt16* >( b_mode_raw_data->GetPointer(0) );
  for( unsigned int i=0; i<num_lines; i++)
  {
    for(unsigned int j=0; j<samples_per_line; j++)
    {
      vtk_b_mode_raw_data_p[ i + num_lines*j ] = *b_mode_image_it;
      b_mode_image_it++;


      b_mode_raw_points->SetPoint( i + num_lines*j, *b_mode_image_x, *b_mode_image_y * -1, 0.0);
      b_mode_image_x++;
      b_mode_image_y++;
    }
  }

  vtk_b_mode_image_raw->SetPoints(b_mode_raw_points);
  vtk_b_mode_image_raw->GetPointData()->SetScalars( b_mode_raw_data );


  //------------- b_mode image scan converted -----
  const unsigned int rows = its_ir->get_b_mode_image_sc_rows();
  const unsigned int cols = its_ir->get_b_mode_image_sc_cols();

  outInfo = outputVector->GetInformationObject(3);
  vtkImageData* vtk_b_mode_image_sc = vtkImageData::SafeDownCast( outInfo->Get(vtkDataObject::DATA_OBJECT()));
  if (!vtk_b_mode_image_sc)
    return 0;

  // Extent should be set before allocate scalars
  vtk_b_mode_image_sc->SetWholeExtent( 0 , rows - 1, 0, cols - 1, 0, 0 );
  vtk_b_mode_image_sc->SetDimensions( rows, cols, 1 );
  vtk_b_mode_image_sc->SetScalarTypeToDouble();
  vtk_b_mode_image_sc->SetNumberOfScalarComponents(1);
  vtk_b_mode_image_sc->AllocateScalars();
  vtk_b_mode_image_sc->SetSpacing( its_ir->get_b_mode_image_sc_delta_x(), its_ir->get_b_mode_image_sc_delta_y(), 1.0 );
  vtk_b_mode_image_sc->SetOrigin( 0.0, 0.0, 0.0 );
  // fill in scout b mode scan converted values
  double* vtk_b_mode_image_sc_p = static_cast< double* >( vtk_b_mode_image_sc->GetScalarPointer() );
  std::vector<double>::const_iterator b_mode_image_sc_it = its_ir->get_b_mode_image_sc().begin();

  for(unsigned int i=0; i<cols; i++)
  {
    for(unsigned int j=0; j<rows; j++)
    {
      *vtk_b_mode_image_sc_p = *b_mode_image_sc_it ;
      b_mode_image_sc_it++;
      vtk_b_mode_image_sc_p++;
    }
  }


  return 1;

}




int vtkVisualSonicsReader::ReadSaturation( vtkInformationVector* outputVector)
{
  // read in the image
  its_ir->read_saturation_image();


  //---------- saturation image raw -----------------
  vtkInformation* outInfo = outputVector->GetInformationObject(1);
  vtkStructuredGrid* vtk_saturation_image_raw = vtkStructuredGrid::SafeDownCast( outInfo->Get(vtkDataObject::DATA_OBJECT()));
  if (!vtk_saturation_image_raw)
    return 0;

  const unsigned int samples_per_line = its_rpd->its_rf_mode_rx_ad_gate_width;
  const unsigned int num_lines = its_rpd->its_rf_mode_tx_trig_tbl_trigs;

  vtk_saturation_image_raw->SetWholeExtent( 0, num_lines - 1, 0, samples_per_line - 1, 0, 0 );
  vtk_saturation_image_raw->SetDimensions( num_lines, samples_per_line, 1 );


  vtkSmartPointer<vtkPoints> saturation_raw_points = vtkSmartPointer<vtkPoints>::New();
  saturation_raw_points->SetNumberOfPoints( samples_per_line*num_lines*1 );

  vtkSmartPointer<vtkUnsignedCharArray> saturation_raw_data = vtkSmartPointer<vtkUnsignedCharArray>::New();
  saturation_raw_data->SetNumberOfComponents(1);
  saturation_raw_data->SetNumberOfTuples( samples_per_line*num_lines*1 );


  std::vector<bool>::const_iterator saturation_image_it = its_ir->get_saturation_image().begin();
  std::vector<double>::const_iterator saturation_image_x = its_ir->get_saturation_image_x().begin();
  std::vector<double>::const_iterator saturation_image_y = its_ir->get_saturation_image_y().begin();

  for( unsigned int i=0; i<num_lines; i++)
  {
    for(unsigned int j=0; j<samples_per_line; j++)
    {
      saturation_raw_data->SetValue( i + num_lines*j, static_cast< unsigned char > ( *saturation_image_it ) );
      saturation_image_it++;


      saturation_raw_points->SetPoint( i + num_lines*j, *saturation_image_x, *saturation_image_y * -1, 0.0);
      saturation_image_x++;
      saturation_image_y++;
    }
  }

  vtk_saturation_image_raw->SetPoints(saturation_raw_points);
  vtk_saturation_image_raw->GetPointData()->SetScalars( saturation_raw_data );



  //------------- saturation image scan converted -----
  const unsigned int rows = its_ir->get_saturation_image_sc_rows();
  const unsigned int cols = its_ir->get_saturation_image_sc_cols();

  outInfo = outputVector->GetInformationObject(4);
  vtkImageData* vtk_saturation_image_sc = vtkImageData::SafeDownCast( outInfo->Get(vtkDataObject::DATA_OBJECT()));
  if (!vtk_saturation_image_sc)
    return 0;

  // Extent should be set before allocate scalars
  vtk_saturation_image_sc->SetWholeExtent( 0 , rows - 1, 0, cols - 1, 0, 0 );
  vtk_saturation_image_sc->SetDimensions( rows, cols, 1 );
  vtk_saturation_image_sc->SetScalarTypeToUnsignedChar();
  vtk_saturation_image_sc->SetNumberOfScalarComponents(1);
  vtk_saturation_image_sc->AllocateScalars();
  vtk_saturation_image_sc->SetSpacing( its_ir->get_saturation_image_sc_delta_x(), its_ir->get_saturation_image_sc_delta_y(), 1.0 );
  vtk_saturation_image_sc->SetOrigin( 0.0, 0.0, 0.0 );
  // fill in scout b mode scan converted values
  unsigned char* vtk_saturation_image_sc_p = static_cast< unsigned char* >( vtk_saturation_image_sc->GetScalarPointer() );
  std::vector<bool>::const_iterator saturation_image_sc_it = its_ir->get_saturation_image_sc().begin();

  for(unsigned int i=0; i<cols; i++)
  {
    for(unsigned int j=0; j<rows; j++)
    {
      *vtk_saturation_image_sc_p = static_cast< unsigned char> (*saturation_image_sc_it) ;
      saturation_image_sc_it++;
      vtk_saturation_image_sc_p++;
    }
  }

  return 1;
}




int vtkVisualSonicsReader::ReadRF( vtkInformationVector* outputVector)
{

  // read in the image
  its_ir->read_rf_image();

  //---------- rf_image_raw ----------------- declarations
  vtkInformation* outInfo = outputVector->GetInformationObject(2);
  vtkStructuredGrid* vtk_rf_image_raw = vtkStructuredGrid::SafeDownCast( outInfo->Get(vtkDataObject::DATA_OBJECT()));
  if (!vtk_rf_image_raw)
    return 0;

  const unsigned int samples_per_line = its_rpd->its_image_acquisition_size / sizeof( Int16 );
  const unsigned int num_lines = its_rpd->its_image_lines;
  const unsigned int frames = its_rpd->its_image_frames;

  cout << "spl: " << samples_per_line << " nl: " << num_lines << " frames: " << frames << endl;

  vtk_rf_image_raw->SetWholeExtent( 0, num_lines - 1, 0, samples_per_line - 1, 0, frames - 1 );
  vtk_rf_image_raw->SetDimensions( num_lines, samples_per_line, frames );


  vtkPoints* rf_raw_points = vtkPoints::New();
  rf_raw_points->SetNumberOfPoints( samples_per_line*num_lines*frames );

  vtkShortArray* rf_raw_data  = vtkShortArray::New();
  rf_raw_data->SetNumberOfComponents(1);
  rf_raw_data->SetNumberOfTuples( samples_per_line*num_lines*frames );


  std::vector<Int16>::const_iterator rf_image_it = its_ir->get_rf_image().begin();
  std::vector<double>::const_iterator rf_image_x = its_ir->get_rf_image_x().begin();
  std::vector<double>::const_iterator rf_image_y = its_ir->get_rf_image_y().begin();
  const double rf_image_z_step = (its_rpd->its_rf_mode_3d_scan_distance)/(frames - 1);


  //------------- rf image scan converted ----- declarations
  const unsigned int rows = its_ir->get_rf_image_sc_rows();
  const unsigned int cols = its_ir->get_rf_image_sc_cols();

  outInfo = outputVector->GetInformationObject(5);
  vtkImageData* vtk_rf_image_sc = vtkImageData::SafeDownCast( outInfo->Get(vtkDataObject::DATA_OBJECT()));
  if (!vtk_rf_image_sc)
    return 0;

  // Extent should be set before allocate scalars
  vtk_rf_image_sc->SetWholeExtent( 0 , rows - 1, 0, cols - 1, 0, frames - 1 );
  vtk_rf_image_sc->SetDimensions( rows, cols, 1 );
  vtk_rf_image_sc->SetScalarTypeToDouble();
  vtk_rf_image_sc->SetNumberOfScalarComponents(1);
  vtk_rf_image_sc->AllocateScalars();
  vtk_rf_image_sc->SetSpacing( its_ir->get_rf_image_sc_delta_x(), its_ir->get_rf_image_sc_delta_y(), rf_image_z_step );
  vtk_rf_image_sc->SetOrigin( 0.0, 0.0, 0.0 );
  // fill in scout b mode scan converted values
  double* vtk_rf_image_sc_p = static_cast< double* >( vtk_rf_image_sc->GetScalarPointer() );
  std::vector<double>::const_iterator rf_image_sc_it = its_ir->get_rf_image_sc().begin();


  //---------- rf_image_raw and scan convert ---------------- reading
  unsigned int k = 0; // frame number
  unsigned int values_in_frame = num_lines * samples_per_line;
  while( its_ir->read_rf_image() )
  {
    // raw
    for( unsigned int i=0; i<num_lines; i++)
    {
      for(unsigned int j=0; j<samples_per_line; j++)
      {
        rf_raw_data->SetValue( i + num_lines*j + values_in_frame*k, static_cast< Int16 > ( *rf_image_it ) );
        rf_image_it++;
  
        rf_raw_points->SetPoint( i + num_lines*j + values_in_frame*k, *rf_image_x, *rf_image_y * -1, rf_image_z_step*k );
        rf_image_x++;
        rf_image_y++;
      }
    }
    
    // scan converted
    for( unsigned int i=0; i<cols; i++)
    {
      for( unsigned int j=0; j<rows; j++)
      {
	// scan convert
        *vtk_rf_image_sc_p = *rf_image_sc_it ;
        rf_image_sc_it++;
        vtk_rf_image_sc_p++;
      }
    }
    rf_image_it = its_ir->get_rf_image().begin();
    rf_image_x = its_ir->get_rf_image_x().begin();
    rf_image_y = its_ir->get_rf_image_y().begin();

    rf_image_sc_it = its_ir->get_rf_image_sc().begin();

    k++;
  }

  vtk_rf_image_raw->SetPoints(rf_raw_points);
  vtk_rf_image_raw->GetPointData()->SetScalars( rf_raw_data );

  rf_raw_points->Delete();
  rf_raw_data->Delete();

  return 1;
}





void vtkVisualSonicsReader::PrintSelf(ostream& os, vtkIndent indent)
{
  os << indent << "ReadMethod:" << indent << this->GetReadMethod() << std::endl;
  os << indent << "SpecificAcquisition:" << indent << this->GetSpecificAcquisition() << std::endl;
  this->Superclass::PrintSelf(os, indent);
}

