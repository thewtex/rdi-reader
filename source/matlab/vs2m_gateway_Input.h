/*!  @file  vs2m_gateway_Input.h
     @brief parse and import inputs

     @author Matt McCormick <matt@mmmccormick.com>
     @date   Wed Jul 25 2007

   */

#ifndef _vs2m_gateway_Input_h
#define _vs2m_gateway_Input_h

#include <boost/filesystem/convenience.hpp>

namespace bf = boost::filesystem;

#include "mex.h"

#include "vs2m_gateway_helper_functions.h"

#include "rdiParserData.h"

namespace vs2m_gateway
{

  class Input
  {
  public:
    Input(const mxArray * prhs[]);

    rdiParserData get_rpd() { return its_rpd; }

  private:
    const mxArray ** its_prhs;

    bf::path its_in_file_path;
    bf::path its_in_file_name;

    rdiParserData its_rpd;

    //! check proper number of arguments and for proper types
    void check_inputs();

    //! parse the .rdi file meta data
    rdiParserData parse_meta_data();

  };

}



#endif //_vs2m_gateway_Input_h
