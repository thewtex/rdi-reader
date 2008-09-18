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
#include "vtkStreamingDemandDrivenPipeline.h"
#include "vtkStructuredGrid.h"
#include "vtkUnsignedCharArray.h"
#include "vtkUnsignedShortArray.h"


#include "common/ImageReader.h"
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
  this->Modified();
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



inline void vtkVisualSonicsReader::GetScalarRange( double range[2] )
{
  range[0] = static_cast< double > ( its_ir->get_rf_min() );
  range[1] = static_cast< double > ( its_ir->get_rf_max() );
}



int vtkVisualSonicsReader::ProcessRequest(vtkInformation* request,
                                      vtkInformationVector** inputVector,
                                      vtkInformationVector* outputVector)
{
  // specify which ports need to be updated
  if(request->Has(vtkDemandDrivenPipeline::REQUEST_DATA_NOT_GENERATED()))
  {
    return this->RequestDataNotGenerated( request, inputVector, outputVector);
  }

  return this->Superclass::ProcessRequest(request, inputVector, outputVector);
}



//! gives information on the output data set types for the filter class
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



int vtkVisualSonicsReader::RequestDataNotGenerated( vtkInformation* request,
    vtkInformationVector ** inputVector,
    vtkInformationVector *  outputVector)
{
  if( request->Has(vtkExecutive::FROM_OUTPUT_PORT() ) )
  {
    int port = request->Get( vtkExecutive::FROM_OUTPUT_PORT() );
    vtkInformation* outInfo;
    if( port < 3 )
    {
      // only generate the raw data
      for( int i=0; i<6; i++ )
      {
	if( i == port )
	  continue;
	outInfo = outputVector->GetInformationObject(i);
	outInfo->Set( vtkDemandDrivenPipeline::DATA_NOT_GENERATED(), 1);
      }
    }
    else
    {
      // generate the raw data and the scan converted data
      for( int i=0; i<6; i++ )
      {
	if( i == port || i == port - 3 )
	  continue;
	outInfo = outputVector->GetInformationObject(i);
	outInfo->Set( vtkDemandDrivenPipeline::DATA_NOT_GENERATED(), 1);
      }
    }
  }
  else
    return 0;

  return 1;
}



//! gives information on the data sets that the filter will produce for a particular instance
int vtkVisualSonicsReader::RequestInformation( vtkInformation* request,
    vtkInformationVector ** inputVector,
    vtkInformationVector *  outputVector)
{

  /*************** b mode image raw ***************/
  vtkInformation* outInfo = outputVector->GetInformationObject(0);

  int whole_extent[6] = { 0,
    static_cast< int >( its_ir->get_b_mode_image_cols() ) - 1,
    0,
    static_cast< int >( its_ir->get_b_mode_image_rows() ) - 1,
    0,
    0 };
  outInfo->Set(vtkStreamingDemandDrivenPipeline::WHOLE_EXTENT(),
      whole_extent, 6);

  /*************** saturation image raw ***************/
  outInfo = outputVector->GetInformationObject(1);

  whole_extent[1] = static_cast< int >( its_ir->get_saturation_image_cols() ) - 1;
  whole_extent[3] = static_cast< int >( its_ir->get_saturation_image_rows() ) - 1;
  outInfo->Set(vtkStreamingDemandDrivenPipeline::WHOLE_EXTENT(),
      whole_extent, 6);

  /*************** rf image raw ***************/
  outInfo = outputVector->GetInformationObject(2);

  whole_extent[1] = static_cast< int >( its_ir->get_rf_image_cols() ) - 1;
  whole_extent[3] = static_cast< int >( its_ir->get_rf_image_rows() ) - 1;
  whole_extent[5] = static_cast< int >( its_ir->get_rf_image_frames() ) - 1;
  outInfo->Set(vtkStreamingDemandDrivenPipeline::WHOLE_EXTENT(),
      whole_extent, 6);

  /*************** b mode image sc  ***************/
  outInfo = outputVector->GetInformationObject(3);

  // notice rows/ columns flipped from raw !
  whole_extent[1] = static_cast< int >( its_ir->get_b_mode_image_sc_rows() ) - 1;
  whole_extent[3] = static_cast< int >( its_ir->get_b_mode_image_sc_cols() ) - 1;
  outInfo->Set(vtkStreamingDemandDrivenPipeline::WHOLE_EXTENT(),
      whole_extent, 6);

  double origin[3] = { 0.0, 0.0, 0.0 };
  outInfo->Set(vtkDataObject::ORIGIN(), origin, 3 );

  double spacing[3] = { its_ir->get_b_mode_image_sc_delta_y(),
    its_ir->get_b_mode_image_sc_delta_x(),
    its_ir->get_rf_image_delta_z() };
  outInfo->Set(vtkDataObject::SPACING(), spacing, 3);

  /*************** saturation image sc ***************/
  outInfo = outputVector->GetInformationObject(4);

  whole_extent[1] = static_cast< int >( its_ir->get_saturation_image_sc_rows() ) - 1;
  whole_extent[3] = static_cast< int >( its_ir->get_saturation_image_sc_cols() ) - 1;
  outInfo->Set(vtkStreamingDemandDrivenPipeline::WHOLE_EXTENT(),
      whole_extent, 6);

  outInfo->Set(vtkDataObject::ORIGIN(), origin, 3 );

  spacing[0] = its_ir->get_saturation_image_sc_delta_y();
  spacing[1] = its_ir->get_saturation_image_sc_delta_x();
  spacing[2] = its_ir->get_rf_image_delta_z();
  outInfo->Set(vtkDataObject::SPACING(), spacing, 3);

  /*************** rf image sc ***************/
  outInfo = outputVector->GetInformationObject(5);

  whole_extent[1] = static_cast< int >( its_ir->get_rf_image_sc_rows() ) - 1;
  whole_extent[3] = static_cast< int >( its_ir->get_rf_image_sc_cols() ) - 1;
  whole_extent[5] = static_cast< int >( its_ir->get_rf_image_frames() ) - 1;
  outInfo->Set(vtkStreamingDemandDrivenPipeline::WHOLE_EXTENT(),
      whole_extent, 6);

  outInfo->Set(vtkDataObject::ORIGIN(), origin, 3 );

  spacing[0] = its_ir->get_rf_image_sc_delta_y();
  spacing[1] = its_ir->get_rf_image_sc_delta_x();
  spacing[2] = its_ir->get_rf_image_delta_z();
  outInfo->Set(vtkDataObject::SPACING(), spacing, 3);

  return 1;

}

int vtkVisualSonicsReader::RequestData(vtkInformation* request,
			     vtkInformationVector**,
			     vtkInformationVector* outputVector)
{

  // check to make sure its_ir has been prepared
  if(!this->its_ir)
  {
    vtkErrorMacro("A FilePrefix must be specified with SetFilePrefix( filename ).");
    return 0;
  }

  // do not generate the scan converted images if they are not requested
  bool do_scan_conv;
  int port;
  if( request->Has(vtkExecutive::FROM_OUTPUT_PORT() ) )
  {
    port = request->Get( vtkExecutive::FROM_OUTPUT_PORT() );
    if( port < 3 )
    {
      do_scan_conv = false;
      its_ir->set_do_scan_conv( false );
    }
    else
    {
      do_scan_conv = true;
      its_ir->set_do_scan_conv( true );
    }
  }
  else
    return 0;


  switch( port )
  {
    case 0:
      if(!this->ReadBMode(outputVector, do_scan_conv) )
        return 0;
      break;
    case 1:
      if(!this->ReadSaturation(outputVector, do_scan_conv) )
        return 0;
      break;
    case 2:
      if(!this->ReadRF(request, outputVector, do_scan_conv) )
        return 0;
      break;
    case 3:
      if(!this->ReadBMode(outputVector, do_scan_conv) )
        return 0;
      break;
    case 4:
      if(!this->ReadSaturation(outputVector, do_scan_conv) )
        return 0;
      break;
    case 5:
      if(!this->ReadRF(request, outputVector, do_scan_conv) )
        return 0;
      break;
    default:
      return 0;
  }


  return 1;
}




int vtkVisualSonicsReader::ReadBMode( vtkInformationVector* outputVector, const bool& do_scan_conv)
{

  vtkDebugMacro(<< "Executing read VisualSonics filter: b mode section" )

  // read in the image
  its_ir->read_b_mode_image();

  //---------- b_mode_image_raw -----------------
  vtkInformation* outInfo = outputVector->GetInformationObject(0);
  vtkStructuredGrid* vtk_b_mode_image_raw = vtkStructuredGrid::SafeDownCast( outInfo->Get(vtkDataObject::DATA_OBJECT()));
  if (!vtk_b_mode_image_raw)
    return 0;

  const unsigned int samples_per_line = its_rpd->its_rf_mode_rx_ad_gate_width;
  const unsigned int num_lines = its_rpd->its_rf_mode_tx_trig_tbl_trigs;

  vtk_b_mode_image_raw->SetDimensions( num_lines, samples_per_line, 1 );


  vtkSmartPointer<vtkPoints> b_mode_raw_points = vtkSmartPointer<vtkPoints>::New();
    b_mode_raw_points->SetNumberOfPoints( samples_per_line*num_lines*1 );

  vtkSmartPointer<vtkUnsignedShortArray> b_mode_raw_data = vtkSmartPointer<vtkUnsignedShortArray>::New();
    b_mode_raw_data->SetNumberOfComponents(1);
    b_mode_raw_data->SetNumberOfTuples( samples_per_line*num_lines*1 );

  vtkDebugMacro(<< "B Mode: "
      << " Samples per line: " << samples_per_line
      << ", Number of lines: " << num_lines );

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
  if( do_scan_conv )
  {
    const unsigned int rows = its_ir->get_b_mode_image_sc_rows();
    const unsigned int cols = its_ir->get_b_mode_image_sc_cols();

    vtkDebugMacro(<< "Saturation scan converted: "
        << " Rows: " << rows
        << ", Columns: " << cols );

    outInfo = outputVector->GetInformationObject(3);
    vtkImageData* vtk_b_mode_image_sc = vtkImageData::SafeDownCast( outInfo->Get(vtkDataObject::DATA_OBJECT()));
    if (!vtk_b_mode_image_sc)
      return 0;

    // Extent should be set before allocate scalars
    vtk_b_mode_image_sc->SetDimensions( rows, cols, 1 );
    vtk_b_mode_image_sc->SetScalarTypeToDouble();
    vtk_b_mode_image_sc->SetNumberOfScalarComponents(1);
    vtk_b_mode_image_sc->AllocateScalars();
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
  }


  return 1;

}




int vtkVisualSonicsReader::ReadSaturation( vtkInformationVector* outputVector, const bool& do_scan_conv)
{

  vtkDebugMacro(<< "Executing read VisualSonics filter: saturation section" )

  // read in the image
  its_ir->read_saturation_image();


  //---------- saturation image raw -----------------
  vtkInformation* outInfo = outputVector->GetInformationObject(1);
  vtkStructuredGrid* vtk_saturation_image_raw = vtkStructuredGrid::SafeDownCast( outInfo->Get(vtkDataObject::DATA_OBJECT()));
  if (!vtk_saturation_image_raw)
    return 0;

  const unsigned int samples_per_line = its_rpd->its_rf_mode_rx_ad_gate_width;
  const unsigned int num_lines = its_rpd->its_rf_mode_tx_trig_tbl_trigs;

  vtk_saturation_image_raw->SetDimensions( num_lines, samples_per_line, 1 );

  vtkSmartPointer<vtkPoints> saturation_raw_points = vtkSmartPointer<vtkPoints>::New();
    saturation_raw_points->SetNumberOfPoints( samples_per_line*num_lines*1 );

  vtkSmartPointer<vtkUnsignedShortArray> saturation_raw_data = vtkSmartPointer<vtkUnsignedShortArray>::New();
    saturation_raw_data->SetNumberOfComponents(1);
    saturation_raw_data->SetNumberOfTuples( samples_per_line*num_lines*1 );

  vtkDebugMacro(<< "Saturation: "
      << " Samples per line: " << samples_per_line
      << ", Number of lines: " << num_lines );

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
  if( do_scan_conv )
  {
    const unsigned int rows = its_ir->get_saturation_image_sc_rows();
    const unsigned int cols = its_ir->get_saturation_image_sc_cols();

    vtkDebugMacro(<< "Saturation scan converted: "
        << " Rows: " << rows
        << ", Columns: " << cols );

    outInfo = outputVector->GetInformationObject(4);
    vtkImageData* vtk_saturation_image_sc = vtkImageData::SafeDownCast( outInfo->Get(vtkDataObject::DATA_OBJECT()));
    if (!vtk_saturation_image_sc)
      return 0;

    // Extent should be set before allocate scalars
    vtk_saturation_image_sc->SetDimensions( rows, cols, 1 );
    vtk_saturation_image_sc->SetScalarTypeToUnsignedChar();
    vtk_saturation_image_sc->SetNumberOfScalarComponents(1);
    vtk_saturation_image_sc->AllocateScalars();
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
  }

  return 1;
}




int vtkVisualSonicsReader::ReadRF( vtkInformation* request,
    vtkInformationVector* outputVector,
    const bool& do_scan_conv)
{

  vtkDebugMacro(<< "Executing read VisualSonics filter: rf section" )

  vtkInformation* outInfo = outputVector->GetInformationObject(2);
  vtkStructuredGrid* vtk_rf_image_raw = vtkStructuredGrid::SafeDownCast( outInfo->Get(vtkDataObject::DATA_OBJECT()));
  if (!vtk_rf_image_raw)
    return 0;

  outInfo = outputVector->GetInformationObject(5);
  vtkImageData* vtk_rf_image_sc = vtkImageData::SafeDownCast( outInfo->Get(vtkDataObject::DATA_OBJECT()));
  if (!vtk_rf_image_sc)
    return 0;

  // tmp
  vtk_rf_image_raw->Print(cout);
  vtk_rf_image_sc->Print(cout);

  //---------- determine which frames need to be updated
  int* raw_update_extent = vtk_rf_image_raw->GetUpdateExtent();
  int* sc_update_extent = vtk_rf_image_sc->GetUpdateExtent();
  if( do_scan_conv )
  {
    raw_update_extent[4] = sc_update_extent[4];
    raw_update_extent[5] = sc_update_extent[5];
    vtk_rf_image_raw->SetUpdateExtent( raw_update_extent );
  }
  const unsigned int update_frames = raw_update_extent[5] - raw_update_extent[4] + 1;

  //---------- rf_image_raw ----------------- declarations
  const unsigned int samples_per_line = its_rpd->its_image_acquisition_size / sizeof( Int16 );
  const unsigned int num_lines = its_rpd->its_image_lines;

  vtkDebugMacro(<< "RF: "
      << " Samples per line: " << samples_per_line
      << ", Number of lines: " << num_lines );

  vtk_rf_image_raw->SetExtent( 0,
      num_lines,
      0,
      samples_per_line,
      raw_update_extent[4],
      raw_update_extent[5] );

  // set the cxx::ImageReader frames to process to the extent we are interested in
  std::vector< unsigned int > frames_vec( update_frames );
  for( int i = raw_update_extent[4], frame_counter = 0; i <= raw_update_extent[5]; i++ )
  {
    frames_vec[frame_counter] = static_cast< unsigned int >( i );
    frame_counter++;
  }
  its_ir->set_frames_to_read( frames_vec );


  vtkSmartPointer<vtkPoints> rf_raw_points = vtkSmartPointer<vtkPoints>::New();
    rf_raw_points->SetNumberOfPoints( samples_per_line*num_lines*update_frames );

  vtkSmartPointer<vtkShortArray> rf_raw_data  = vtkSmartPointer<vtkShortArray>::New();
    rf_raw_data->SetNumberOfComponents(1);
    rf_raw_data->SetNumberOfTuples( samples_per_line*num_lines*update_frames );


  std::vector<Int16>::const_iterator rf_image_it = its_ir->get_rf_image().begin();
  std::vector<double>::const_iterator rf_image_x = its_ir->get_rf_image_x().begin();
  std::vector<double>::const_iterator rf_image_y = its_ir->get_rf_image_y().begin();
  double rf_image_z_step = its_ir->get_rf_image_delta_z();


  //------------- rf image scan converted ----- declarations
  const unsigned int rows = its_ir->get_rf_image_sc_rows();
  const unsigned int cols = its_ir->get_rf_image_sc_cols();

    vtkDebugMacro(<< "RF scan converted: "
        << " Rows: " << rows
        << ", Columns: " << cols );

  if ( do_scan_conv )
  {
    // Extent should be set before allocate scalars
    vtk_rf_image_sc->SetExtent( 0, rows, 0, cols, raw_update_extent[4], raw_update_extent[5] );
    vtk_rf_image_sc->SetScalarTypeToDouble();
    vtk_rf_image_sc->SetNumberOfScalarComponents(1);
    vtk_rf_image_sc->AllocateScalars();
  }
  double* vtk_rf_image_sc_p = static_cast< double* >( vtk_rf_image_sc->GetScalarPointer() );
  std::vector<double>::const_iterator rf_image_sc_it = its_ir->get_rf_image_sc().begin();


  //---------- rf_image_raw and scan convert ---------------- reading
  unsigned int k = 0; // frame number
  unsigned int values_in_frame = num_lines * samples_per_line;
  while( its_ir->read_rf_image() )
  {
    // raw
    rf_image_it = its_ir->get_rf_image().begin();
    rf_image_x = its_ir->get_rf_image_x().begin();
    rf_image_y = its_ir->get_rf_image_y().begin();
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
    if( do_scan_conv )
    {
      rf_image_sc_it = its_ir->get_rf_image_sc().begin();
      for( unsigned int i=0; i<rows; i++)
      {
        for( unsigned int j=0; j<cols; j++)
        {
  	// scan convert
          *vtk_rf_image_sc_p = *rf_image_sc_it ;
          rf_image_sc_it++;
          vtk_rf_image_sc_p++;
        }
      }
    }

    k++; // next frame
  }

  vtk_rf_image_raw->SetPoints(rf_raw_points);
  vtk_rf_image_raw->GetPointData()->SetScalars( rf_raw_data );

  return 1;
}





void vtkVisualSonicsReader::PrintSelf(ostream& os, vtkIndent indent)
{
  os << indent << "ReadMethod:" << indent << this->GetReadMethod() << std::endl;
  os << indent << "SpecificAcquisition:" << indent << this->GetSpecificAcquisition() << std::endl;
  this->Superclass::PrintSelf(os, indent);
}


