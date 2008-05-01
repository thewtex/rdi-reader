/*!  @file  vs2m_gateway_helper_functions.h
     @brief

     @author Matt McCormick <matt@mmmccormick.com>
     @date   Wed Jul 25 2007

   */

#ifndef _vs2m_gateway_helper_functions_h
#define _vs2m_gateway_helper_functions_h

#include <fstream>
#include <vector>

#include <boost/filesystem/convenience.hpp>

namespace bf = boost::filesystem;

#include "mex.h"

#include "rdiParserData.h"

namespace vs2m_gateway
{

  /*!
    @param string_array_ptr
    */
  bf::path matlab_string_2_bf_path(const mxArray * string_array_ptr);

  void check_inputs(const int & nlhs, const int & nrhs, const mxArray * prhs[]);

} // end vs2m_gateway namespace

#endif //_vs2m_gateway_helper_functions_h
