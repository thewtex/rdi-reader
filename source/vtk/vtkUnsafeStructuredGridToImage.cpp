#include "vtkUnsafeStructuredGridToImage.h"

#include "vtkImageData.h"
#include "vtkInformation.h"
#include "vtkPointData.h"
#include "vtkSetGet.h"
#include "vtkStructuredGrid.h"

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



int vtkUnsafeStructuredGridToImage::RequestData(
  vtkInformation* request,
  vtkInformationVector** inputVector,
  vtkInformationVector*  outputVector)
{
  vtkStructuredGrid* input = vtkStructuredGrid::SafeDownCast( inputVector[0]->Get( vtkDataObject::DATA_OBJECT() ));
  vtkImageData* output = vtkImageData::SafeDownCast( outputVector->Get( vtkDataObject::DATA_OBJECT()  ));
  if(!input || !output)
    return 0;

  output->Print();
  output->DebugOn();

  vtkDebugMacro(<< "Doing unsafe (though intentional ;-) ) conversion from StructuredGrid to ImageData")


  //int* update_extent = input->
  //output->SetScalarType( input->GetScalarType() );
  //output->SetNumberOfScalarComponents( input->GetNumberOfScalarComponents() );
  //output->AllocateScalars();
  vtkPointData* input_point_data = input->GetPointData();
  vtkPointData* output_point_data = output->GetPointData();
  output_point_data->PassData( input_point_data );

  output->Print();



  return 1;
}
