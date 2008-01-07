/*!  @file  vs2m_gateway_Output.h
     @brief 
     
     @author Matt McCormick <matt@mmmccormick.com>
     @date   Wed Jul 25 2007 
      
   */

#ifndef _vs2m_gateway_Output_h
#define _vs2m_gateway_Output_h

#include <boost/filesystem/convenience.hpp>

namespace bf = boost::filesystem;

#include "mex.h"

#include "rdiParserData.h"

namespace vs2m_gateway
{
  class Output
  {
  public:
    Output(mxArray * plhs[], const rdiParserData & rpd);
    
  private:
    mxArray ** its_plhs;
        
    const rdiParserData & its_rpd;
    
    // export meta data to the output structure.
    //! sections defined by the .rdi file
    void create_base_struct();
    //! === IMAGE INFO ===
    void create_image_info_struct();
    //! === IMAGE PARAMETERS ===
    void create_image_parameters_struct();
    
  };
  
} //end vs2m_gateway namespace

#endif //_vs2m_gateway_Output_h
