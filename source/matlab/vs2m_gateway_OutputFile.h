/*!  @file  vs2m_gateway_OutputFile.h
     @brief

     @author Matt McCormick <matt@mmmccormick.com>
     @date   Wed Jul 25 2007

   */

#ifndef _vs2m_gateway_OutputFile_h
#define _vs2m_gateway_OutputFile_h

#include <fstream>
#include <sstream>
#include <cstring>
#include <vector>
#include <iomanip>
#include <stdexcept> //for exception, out_of_range

#include <boost/filesystem/convenience.hpp>

namespace bf = boost::filesystem;

#include "mex.h"
#include "mat.h"

#include "vs2m_gateway_helper_functions.h"

namespace vs2m_gateway
{
  class OutputFile
  {
  public:
    OutputFile( const mxArray * prhs[], mxArray * plhs[], const rdiParserData & rpd);
    ~OutputFile();

  private:
    const mxArray ** its_prhs;
    mxArray ** its_plhs;
    std::vector< unsigned int> its_frames;

    const rdiParserData & its_rpd;

    MATFile * its_output_file;

    bf::path its_out_file_path;
    bf::path its_out_file_name;
    bf::path its_rdb_file_path;

    /*!
      @param frame_num = 1, frame number for putting in the file name
      */
    void open_output_file(const unsigned int frame_num = 1);
    //! check to see if the text "_frame_" is in end of the output file name
    //! for the manual 3D hack
    bf::path check_for_frame_in_name();

    //! get the b_mode image from the rdb file
    //! @param frame_num = 1, frame number for putting in the file name
    void get_b_mode_image();
    //! get the saturation image from the rdb file
    //! @param frame_num = 1, frame number for putting in the file name
    void get_saturation_image();
    //! get the rf data image from the rdb file
    //! @param frame_num = 1, frame number for putting in the file name
    void get_rf_data_image(const unsigned int frame_num = 1);
    //! get the frames to be processed
    void get_frames();

  };
}

#endif //_vs2m_gateway_OutputFile_h
