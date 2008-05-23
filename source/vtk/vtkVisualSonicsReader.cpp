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
#include "vtkSmartPointer.h"
#include "vtkStructuredGrid.h"
#include "vtkUnsignedShortArray.h"


#include "cxx/ImageReader.h"
#include "rdiParserData.h"
#include "MetadataReaderBase.h"
#include "sized_ints.h"


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


  this->Modified();

  return 1;
}




int vtkVisualSonicsReader::ReadBMode( vtkInformationVector* outputVector)
{
  // read in the image
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
  const UInt16* cxx_b_mode_image_sc_p = its_ir->get_b_mode_image_p();

  for(unsigned int i=0; i<num_lines; i++)
  {
    for(unsigned int j=0; j<samples_per_line; j++)
    {
      *vtk_b_mode_image_sc_p = *cxx_b_mode_image_sc_p ;
      cxx_b_mode_image_sc_p++;
      vtk_b_mode_image_sc_p++;
    }
  }


  //---------- b_mode_image_raw -----------------
  outInfo = outputVector->GetInformationObject(0);
  vtkStructuredGrid* vtk_b_mode_image_raw = vtkStructuredGrid::SafeDownCast( outInfo->Get(vtkDataObject::DATA_OBJECT()));
  if (!vtk_b_mode_image_raw)
    return 0;

  vtk_b_mode_image_raw->SetWholeExtent( 0, num_lines - 1, 0, samples_per_line - 1, 0, 0 );
  //vtk_b_mode_image_raw->SetWholeExtent( 98, 100 - 1, 148, 150 - 1, 0, 0 );
  vtk_b_mode_image_raw->SetDimensions( num_lines, samples_per_line, 1 );


  vtkSmartPointer<vtkPoints> b_mode_raw_points = vtkSmartPointer<vtkPoints>::New();
  b_mode_raw_points->Allocate( samples_per_line*num_lines );
  b_mode_raw_points->SetNumberOfPoints( samples_per_line*num_lines*1 );

  vtkSmartPointer<vtkUnsignedShortArray> b_mode_raw_data = vtkSmartPointer<vtkUnsignedShortArray>::New();
  b_mode_raw_data->SetNumberOfComponents(1);
  b_mode_raw_data->SetNumberOfTuples( samples_per_line*num_lines*1 );

  
  const UInt16* cxx_b_mode_image_p = its_ir->get_b_mode_image_p();
  const double* cxx_b_mode_image_x_p = its_ir->get_b_mode_image_x_p();
  const double* cxx_b_mode_image_y_p = its_ir->get_b_mode_image_y_p();

  UInt16* vtk_b_mode_raw_data_p = static_cast< UInt16* >( b_mode_raw_data->GetPointer(0) );
 double second[3]; 
  for( unsigned int i=0; i<num_lines; i++)
  {
    for(unsigned int j=0; j<samples_per_line; j++)
    {
      vtk_b_mode_raw_data_p[ i + num_lines*j ] = cxx_b_mode_image_p[ j + samples_per_line*i ];
      //*vtk_b_mode_raw_data_p = *cxx_b_mode_image_p;
      //vtk_b_mode_raw_data_p++;
      //cxx_b_mode_image_p++;

      // multiply by negative unity so +y coorresponds to VTK coord sys
      //b_mode_raw_points->SetPoint( i*samples_per_line + j, *cxx_b_mode_image_x_p, *cxx_b_mode_image_y_p * -1, 0.0);
      //if( i < 3 )
	//cout << "x: " << *cxx_b_mode_image_x_p << " y: " << *cxx_b_mode_image_y_p << " image: " << cxx_b_mode_image_p[ j + samples_per_line*i ] << std::endl;

      b_mode_raw_points->SetPoint( i + num_lines*j, *cxx_b_mode_image_x_p, *cxx_b_mode_image_y_p * -1, 0.0);
      cxx_b_mode_image_x_p++;
      cxx_b_mode_image_y_p++;
      b_mode_raw_points->GetPoint(i + num_lines*j, second );
      //cout << "line: " << i << " samp: " << j << " x: " << second[0] << " y: " << second[1] << std::endl;

      //b_mode_raw_points->SetPoint( i + num_lines*j, cxx_b_mode_image_x_p[ j + samples_per_line*i ], cxx_b_mode_image_y_p[ j + samples_per_line*i ] * -1, 0.0);
    }
  }
    //for(unsigned int j=0; j<samples_per_line; j++)
  //{
  //for( unsigned int i=0; i<num_lines; i++)
    //{
      ////vtk_b_mode_raw_data_p[ i + num_lines*j ] = cxx_b_mode_image_p[ j + samples_per_line*i ];
      ///[>vtk_b_mode_raw_data_p = *cxx_b_mode_image_p;
      ////vtk_b_mode_raw_data_p++;
      ////cxx_b_mode_image_p++;

      //// multiply by negative unity so +y coorresponds to VTK coord sys
      ////b_mode_raw_points->SetPoint( i*samples_per_line + j, *cxx_b_mode_image_x_p, *cxx_b_mode_image_y_p * -1, 0.0);
      ////if( i < 3 )
	////cout << "x: " << *cxx_b_mode_image_x_p << " y: " << *cxx_b_mode_image_y_p << " image: " << cxx_b_mode_image_p[ j + samples_per_line*i ] << std::endl;

      ////b_mode_raw_points->SetPoint( i + num_lines*j, *cxx_b_mode_image_x_p, *cxx_b_mode_image_y_p * -1, 0.0);
      ////cxx_b_mode_image_x_p++;
      ////cxx_b_mode_image_y_p++;
      //b_mode_raw_points->SetPoint(static_cast< vtkIdType > ( i + num_lines*j ), cxx_b_mode_image_x_p[ j + samples_per_line*i ], cxx_b_mode_image_y_p[ j + samples_per_line*i ]*-1 , 0.0);
      //b_mode_raw_points->GetPoint(i + num_lines*j, second );

    //}
  //}

 b_mode_raw_points->GetPoint(2000, second );
 cout << "2000 read sgrid point: x: " << second[0] << " y: " << second[1] << " z: " << second[2] << std::endl;
 cout << "real x: " << cxx_b_mode_image_x_p[2000] << "   real y: " << cxx_b_mode_image_y_p[2000] << std::endl;
 b_mode_raw_points->GetPoint(0, second );
 cout << "0 read sgrid point: x: " << second[0] << " y: " << second[1] << " z: " << second[2] << std::endl;

  vtk_b_mode_image_raw->SetPoints(b_mode_raw_points);
  vtk_b_mode_image_raw->GetPointData()->SetScalars( b_mode_raw_data );

  return 1;

}




int vtkVisualSonicsReader::ReadSaturation( vtkInformationVector* outputVector)
{


  return 1;
}




int vtkVisualSonicsReader::ReadRF( vtkInformationVector* outputVector)
{

  return 1;
}





void vtkVisualSonicsReader::PrintSelf(ostream& os, vtkIndent indent)
{
  os << indent << "ReadMethod:" << indent << this->GetReadMethod() << std::endl;
  os << indent << "SpecificAcquisition:" << indent << this->GetSpecificAcquisition() << std::endl;
  this->Superclass::PrintSelf(os, indent);
}

