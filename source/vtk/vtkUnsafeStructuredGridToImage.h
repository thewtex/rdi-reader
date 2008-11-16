/*! @file  vtkUnsafeStructuredGridToImage.h
 *  @brief Passes data in a StructuredGrid to an ImageData without interpolation.  Preserves topology but not geometry.
 *
 *  @author Matt McCormick (thewtex) <matt@mmmccormick.com>
 *  @date   2008 November 15
 *
 */

#ifndef VTKUNSAFESTRUCTUREDGRIDTOIMAGE_H
#define VTKUNSAFESTRUCTUREDGRIDTOIMAGE_H

#include "vtkImageAlgorithm.h"
#include "vtkSetGet.h"


/*! @class vtkUnsafeStructuredGridToImage
 *
 * Takes in a StructuredGrid and outputs an ImageData.
 * No interpolation is applied.
 * Preserves topology but not geometry.
 *
 */
class vtkUnsafeStructuredGridToImage : public vtkImageAlgorithm
{
public:

  static vtkUnsafeStructuredGridToImage* New();

  vtkTypeMacro(vtkUnsafeStructuredGridToImage, vtkImageAlgorithm)

protected:

  //! define what the inputs are
  virtual int FillInputPortInformation( int, vtkInformation* );

  //! define what the outputs are
  virtual int FillOutputPortInformation( int, vtkInformation* );

  //! specifies information about the output, dimensions, etc
  virtual int RequestInformation(vtkInformation*,
      vtkInformationVector **,
      vtkInformationVector *);

  //! does the actual data crunching at a pipeline request
  virtual int RequestData(vtkInformation*,
		      vtkInformationVector**,
		      vtkInformationVector* outputVector);

  vtkUnsafeStructuredGridToImage(){}
  virtual ~vtkUnsafeStructuredGridToImage(){}

private:

  vtkUnsafeStructuredGridToImage( const vtkUnsafeStructuredGridToImage& );
  vtkUnsafeStructuredGridToImage& operator=( const vtkUnsafeStructuredGridToImage& );

};

#endif // VTKUNSAFESTRUCTUREDGRIDTOIMAGE_H inclusion guard

