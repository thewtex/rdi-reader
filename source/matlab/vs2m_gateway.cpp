/*!  @file  vs2m_gateway.cpp
     @brief Matlab mex file gateway for converting VisualSonics Vevo 770 Digital RF .rdb and .rdi to Matlab .mat

     @author Matt McCormick <matt@mmmccormick.com>
     @date   14 June 2007
  */

#ifdef MATLAB_MEX_FILE

#include <cstring>
#include <string>
#include <vector>

#include <mex.h>
#include <mat.h>

#include "rdiParserData.h"

#include "vs2m_gateway_helper_functions.h"
#include "vs2m_gateway_Input.h"
#include "vs2m_gateway_Output.h"
#include "vs2m_gateway_OutputFile.h"

/*!
  @remarks arguments
  @b in_file_path a Matlab string, prhs[0]
  @b in_file_name a Matlab string, prhs[1]
  @b out_file_path a Matlab string, prhs[2]
  @b out_file_name a Matlab string, prhs[3]
  @b frames the Matlab string ':' and a double vectorof the indices prhs[4]
  @b s_1_type either '()' or '{}' prhs[5]
  @b s_2_subs either '' or 'rf_data' or 'b_mode' or 'saturation' prhs[6]
  returns
  @b metadata info structure
  */
#pragma GCC visibility push(default)
void mexFunction( int nlhs, mxArray *plhs[],
                  int nrhs, const mxArray *prhs[] )
{
  using namespace vs2m_gateway;

  check_inputs(nlhs, nrhs, prhs);

  // parse and import inputs
  Input input(prhs);
  rdiParserData rpd = input.get_rpd();

  // generate outputs
  Output output(plhs, rpd);

  // save data to .mat files if necessary
  /* Allocate enough memory to hold the converted string. */
  int buflen = mxGetM(prhs[5]) * mxGetN(prhs[5]) + 1;
  char s_1_type[buflen];
  /* Copy the string data from string_array_ptr and place it into buf. */
  if (mxGetString(prhs[5], s_1_type, buflen) != 0)
    mexErrMsgTxt("Could not convert string data.");
  if (!std::strcmp(s_1_type,"()"))
    OutputFile output_file(prhs, plhs, rpd);
  else if (!std::strcmp(s_1_type,"{}"))
    ;
  else
    mexErrMsgTxt("Error: wrong subsref type.");

} // end mexFunction
#pragma GCC visibility pop

#endif // MATLAB_MEX_FILE
