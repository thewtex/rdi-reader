#include "vtkUnsafeStructuredGridToImage.h"

#include "vtkImageData.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkObjectFactory.h"
#include "vtkPointData.h"
#include "vtkSetGet.h"
#include "vtkStreamingDemandDrivenPipeline.h"
#include "vtkStructuredGrid.h"

#include <cmath>


vtkStandardNewMacro(vtkUnsafeStructuredGridToImage);


int vtkUnsafeStructuredGridToImage::FillInputPortInformation(
  int port,
  vtkInformation* info)
{
  if( port == 0 )
    {
    info->Set(vtkAlgorithm::INPUT_REQUIRED_DATA_TYPE(), "vtkStructuredGrid" );
    return 1;
    }
  else
    return 0;
}



int vtkUnsafeStructuredGridToImage::FillOutputPortInformation(
  int port,
  vtkInformation* info)
{
  if( port == 0 )
    {
    info->Set(vtkDataObject::DATA_TYPE_NAME(), "vtkImageData" );
    return 1;
    }
  else
    return 0;
}



int vtkUnsafeStructuredGridToImage::RequestInformation(
  vtkInformation* request,
  vtkInformationVector** inputVector,
  vtkInformationVector*  outputVector)
{

  vtkInformation *info =
    this->GetInputArrayFieldInformation(0, inputVector);
  if (info)
    {
    int scalarType = info->Get( vtkDataObject::FIELD_ARRAY_TYPE());
    int numComp = info->Get( vtkDataObject::FIELD_NUMBER_OF_COMPONENTS());
    vtkInformation* outInfo = outputVector->GetInformationObject(0);
    // copy scalar type and scalar number of components
    vtkDataObject::SetPointDataActiveScalarInfo(outInfo,
                                                    scalarType, numComp);
    }
  else
    {
    vtkDebugMacro( << " fail in RequestInformation" )
    return 0;
    }

  return 1;

}



int vtkUnsafeStructuredGridToImage::RequestData(
  vtkInformation* request,
  vtkInformationVector** inputVector,
  vtkInformationVector*  outputVector)
{
  vtkInformation* input_info = inputVector[0]->GetInformationObject(0);
  vtkStructuredGrid* input = vtkStructuredGrid::SafeDownCast( input_info->Get( vtkDataObject::DATA_OBJECT() ));
  vtkInformation* output_info = outputVector->GetInformationObject(0);
  vtkImageData* output = vtkImageData::SafeDownCast( output_info->Get( vtkDataObject::DATA_OBJECT()  ));
  if(!input || !output)
    return 0;

  output->Print(cout);
  //output->DebugOn();

  vtkDebugMacro(<< "Doing unsafe (though intentional ;-) ) conversion from StructuredGrid to ImageData")

  //int* update_extent = input->GetUpdateExtent();

  //output->SetScalarType( input->GetScalarType() );
  //output->SetNumberOfScalarComponents( input->GetNumberOfScalarComponents() );
  //output->AllocateScalars();
  //vtkPointData* input_point_data = input->GetPointData();
  //vtkPointData* output_point_data = output->GetPointData();

  //vtkDataArray* input_data_array = input->GetPointData()->GetScalars();
  //vtkPointData* output_data_array = output->GetPointData()->GetScalars();

  //output_point_data->PassData( input_point_data );
  int* whole_ext = input_info->Get(vtkStreamingDemandDrivenPipeline::WHOLE_EXTENT());
  int* input_ext = input->GetExtent();
  output->SetExtent( input_ext );
  output->CopyAttributes( input );

  //vtkInformation* scalarInfo = vtkDataObject::GetActiveFieldInformation(input_info, vtkDataObject::FIELD_ASSOCIATION_POINTS, vtkDataSetAttributes::SCALARS);
  //if( scalarInfo )
    //output->SetScalarType( input->Get( vtkDataObject::FIELD_ARRAY_TYPE() ) );
  //else
    //return 0;

  vtkPoints* input_pts = input->GetPoints();
  vtkIdType num_pts = input_pts->GetNumberOfPoints();
  double pt0[3], pt1[3];
  if( num_pts > 1 )
  {
    input_pts->GetPoint( 0, pt0 );
    input_pts->GetPoint( 1, pt1 );
  }

  // we try to use the spacing of the first point
  double* spacing = output->GetSpacing();
  if( whole_ext[1] - whole_ext[0] > 0 &&
      input_ext[0] == 0 && 
      input_ext[1] > 0 )
  {
    spacing[0] = fabs( pt1[1] - pt0[0] );
  }
  if( whole_ext[3] - whole_ext[2] > 0 &&
      input_ext[2] == 0 && 
      input_ext[3] > 0 )
  {
    spacing[1] = fabs( pt1[1] - pt0[1] );
  }
  if( whole_ext[5] - whole_ext[4] > 0 &&
      input_ext[4] == 0 && 
      input_ext[5] > 0 )
  {
    spacing[2] = fabs( pt1[2] - pt0[2] );
  }
  //output->SetSpacing( spacing );

  output->SetOrigin( 0.0, 0.0, 0.0 );

  output->Print(cout);

  return 1;
}
