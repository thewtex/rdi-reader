/*!  @file  vs2m_gateway_Input.cpp
     @brief implementation of vs2m_gateway_Input.h

     @author Matt McCormick <matt@mmmccormick.com>
     @date   Wed Jul 25 2007

  */

#include "vs2m_gateway_Input.h"

#include "rdiParser.h"

vs2m_gateway::Input::Input(const mxArray * prhs[]):
    its_prhs(prhs)
{
  using namespace vs2m_gateway;

  its_rpd = parse_meta_data();
}



rdiParserData vs2m_gateway::Input::parse_meta_data()
{
  its_in_file_path = vs2m_gateway::matlab_string_2_bf_path(its_prhs[0]);
  its_in_file_name = vs2m_gateway::matlab_string_2_bf_path(its_prhs[1]);
  rdiParser rdi_parser( (its_in_file_path / its_in_file_name).native_file_string() );
  return rdi_parser.parse();
}

