/*!  @file  vs2m_gateway_helper_functions.cpp
     @brief implementation of vs2m_gateway_helper_functions.h

     @author Matt McCormick <matt@mmmccormick.com>
     @date   Wed Jul 25 2007

  */

#include "vs2m_gateway_helper_functions.h"


bf::path vs2m_gateway::matlab_string_2_bf_path(const mxArray * string_array_ptr)
{
  /* Allocate enough memory to hold the converted string. */
  int buflen = mxGetM(string_array_ptr) * mxGetN(string_array_ptr) + 1;
  char buf[buflen];

  /* Copy the string data from string_array_ptr and place it into buf. */
  if (mxGetString(string_array_ptr, buf, buflen) != 0)
    mexErrMsgTxt("Could not convert string data.");

  bf::path bf_path(buf);

  return bf_path;
}



void vs2m_gateway::check_inputs(const int & nlhs, const int & nrhs, const mxArray * prhs[])
{
  /* Check for proper number of arguments. */
  if(nrhs!=7) {
    mexErrMsgTxt("convert_visualsonics_2_mat: Wrong number of input arguments.");
  } else if(nlhs>1) {
    mexErrMsgTxt("convert_visualsonics_2_mat: Only one output argument allowed");
  }


  // check for the correct input types
  if(    !mxIsChar(prhs[0])
      || !mxIsChar(prhs[1])
      || !mxIsChar(prhs[2])
      || !mxIsChar(prhs[3])
      || !( mxIsChar(prhs[4]) || mxIsDouble(prhs[4]) )
      || !mxIsChar(prhs[5])
      || !mxIsChar(prhs[6])
      )
  {
    mexErrMsgTxt("convert_visualsonics_2_mat: wrong input types, must be: (string, string, string, string, string or double, string, string)");
  }
}


