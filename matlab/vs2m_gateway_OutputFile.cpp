/*!  @file  vs2m_gateway_OutputFile.cpp
     @brief implementation of vs2m_gateway_OutputFile.h
     
     @author Matt McCormick <matt@mmmccormick.com>
     @date   Wed Jul 25 2007
     
  */
  
#include "vs2m_gateway_OutputFile.h"


vs2m_gateway::OutputFile::OutputFile( const mxArray * prhs[], mxArray * plhs[], const rdiParserData & rpd):
  its_prhs(prhs),
  its_plhs(plhs),
  its_rpd(rpd)
{
  // input rdb file information
  bf::path in_file_path = vs2m_gateway::matlab_string_2_bf_path(its_prhs[0]);
  bf::path in_file_name = vs2m_gateway::matlab_string_2_bf_path(its_prhs[1]);
  its_rdb_file_path = in_file_path / (in_file_name.leaf() + ".rdb");
  
  
  
  its_out_file_path  = matlab_string_2_bf_path(its_prhs[2]);
  its_out_file_name  = check_for_frame_in_name();  
  // make sure path for output file exists
  if( ! bf::exists( its_out_file_path ) )
    bf::create_directory( its_out_file_path );
  if( ! bf::exists( its_out_file_path / its_out_file_name ) )
    bf::create_directory( its_out_file_path / its_out_file_name );
    
    
  // get the frames to be processed
  get_frames();
  
  
  // output file for the roi b mode and saturation image
  std::string output_file_str = (its_out_file_path / its_out_file_name / its_out_file_name).native_file_string() + "_roi_b_mode_and_saturation.mat" ;
  
  /* Allocate enough memory to hold the converted string. */ 
  int buflen = mxGetM(prhs[6]) * mxGetN(prhs[6]) + 1;
  char s_2_subs[buflen]; 
  /* Copy the string data from string_array_ptr and place it into buf. */ 
  if (mxGetString(prhs[6], s_2_subs, buflen) != 0)
    mexErrMsgTxt("Could not convert string data.");
  if ( !std::strcmp(s_2_subs,"rf_data") )
  {
    for(unsigned int i = 0; i < its_frames.size(); i++)
    { 
      if (its_frames[i] < 1 || its_frames[i] > its_rpd.its_image_frames)
      {
        std::ostringstream err_msg;
        err_msg << "Frame " << its_frames[i] << " is invalid for file " << its_rdb_file_path.native_file_string() << '\n';
        mexErrMsgTxt(err_msg.str().c_str());
      }
      open_output_file( its_frames[i] );
      get_rf_data_image();
      matPutVariable( its_output_file, "metadata", its_plhs[0]);
      matClose( its_output_file );    
    } 
  }
  else if ( !std::strcmp(s_2_subs, "b_mode") )
  {
    std::ostringstream inform_msg;
    inform_msg << "Converting input .rdb file binary B Mode reference image to " << output_file_str << " MAT file .....\n";
    mexPrintf( inform_msg.str().c_str() );
    
    its_output_file = matOpen( output_file_str.c_str() , "w");
    get_b_mode_image();
    matPutVariable( its_output_file, "metadata", its_plhs[0]);
    matClose( its_output_file );
  }
  else if ( !std::strcmp(s_2_subs, "saturation") )
  {
    std::ostringstream inform_msg;
    inform_msg << "Converting input .rdb file binary Saturation reference image to " << output_file_str << " MAT file .....\n";
    mexPrintf( inform_msg.str().c_str() );
    
    its_output_file = matOpen( output_file_str.c_str() , "w");
    get_saturation_image();
    matPutVariable( its_output_file, "metadata", its_plhs[0]);
    matClose( its_output_file );
  }
  else
  {
    for(unsigned int i = 0; i < its_frames.size(); i++)
    { 
      if (its_frames[i] < 1 || its_frames[i] > its_rpd.its_image_frames)
      {
        std::ostringstream err_msg;
        err_msg << "Frame " << its_frames[i] << " is invalid for file " << its_rdb_file_path.native_file_string() << '\n';
        mexErrMsgTxt(err_msg.str().c_str());
      }
      open_output_file( its_frames[i] );
      get_rf_data_image();
      matPutVariable( its_output_file, "metadata", its_plhs[0]);
      matClose( its_output_file );    
    } 
    its_output_file = matOpen( output_file_str.c_str() , "w");
      
    std::ostringstream inform_msg;
    inform_msg << "Converting input .rdb file binary B Mode reference image to " << output_file_str << " MAT file .....\n";
    mexPrintf( inform_msg.str().c_str() );
    inform_msg.str("");
    inform_msg << "Converting input .rdb file binary Saturation reference image to " << output_file_str << " MAT file .....\n";
    mexPrintf( inform_msg.str().c_str() );
    
    get_b_mode_image();
    get_saturation_image();
    matClose( its_output_file );
  }
   
}




vs2m_gateway::OutputFile::~OutputFile()
{

}




void vs2m_gateway::OutputFile::open_output_file(const unsigned int frame_num)
{
  // open the output file
  std::string output_file_str;
  bf::path original_name = matlab_string_2_bf_path(its_prhs[3]);
  if( its_out_file_name == original_name)
  {
    std::ostringstream frame_num_oss;
    frame_num_oss << std::setw(4) << std::setfill('0') << frame_num;
    output_file_str = (its_out_file_path / its_out_file_name / its_out_file_name).native_file_string() + "_frame_" + frame_num_oss.str() + ".mat" ;
  }
  else // manual 3D hack
    output_file_str = (its_out_file_path / its_out_file_name / original_name).string() + ".mat";
    
  std::ostringstream inform_msg;
  inform_msg << "Converting input .rdb file binary data to " << output_file_str << " MAT file .....\n";
  mexPrintf( inform_msg.str().c_str() );
  
  its_output_file = matOpen( output_file_str.c_str() , "w");
}




bf::path vs2m_gateway::OutputFile::check_for_frame_in_name()
{
  bf::path out_file_prefix = matlab_string_2_bf_path(its_prhs[3]);
  try
  {
    if ( !out_file_prefix.native_file_string().compare( out_file_prefix.string().size() - 11, 7, "_frame_") ) // returns 0 if compare is true
    {
      bf::path temp_path( out_file_prefix.string().substr( 0, out_file_prefix.native_file_string().size() - 11));
      out_file_prefix = temp_path;
    }
  }
  catch(std::out_of_range & e)
  {
  }
  return out_file_prefix;
}




void vs2m_gateway::OutputFile::get_b_mode_image()
{
  std::ifstream rdb_file( its_rdb_file_path.native_file_string().c_str(), std::ios::in | std::ios::binary);

  if (!rdb_file.is_open())
  {
    std::ostringstream err_msg (std::ostringstream::out);
    err_msg << "\nFile: " << its_rdb_file_path.native_file_string() << " couldn't be opened :(\n";
    mexErrMsgTxt( err_msg.str().c_str() );
  }
  
  const unsigned int samples_per_line = its_rpd.its_rf_mode_rx_ad_gate_width;
  const unsigned int num_lines = its_rpd.its_rf_mode_tx_trig_tbl_trigs;
  
  mxArray * b_mode = mxCreateDoubleMatrix(samples_per_line, num_lines, mxREAL);
  if(!b_mode) {mexErrMsgTxt("Couldn't make b_mode.");};
  double * b_mode_p = mxGetPr(b_mode);
  
  char * u_short_data = static_cast< char * > (mxCalloc(1, sizeof(unsigned short) ) );
  unsigned short * u_short_data_p = reinterpret_cast< unsigned short *> (u_short_data);

  for( unsigned int i = 0; i < num_lines; i++)
  {
    for( unsigned int j = 0; j < samples_per_line; j++)
    {
      rdb_file.read(u_short_data, sizeof(unsigned short));
      *(b_mode_p + i*samples_per_line + j ) = static_cast<double> ( *u_short_data_p);
    }
  }
  
  u_short_data_p = 0;
  mxFree(u_short_data);
   
  matPutVariable( its_output_file, "b_mode", b_mode);
  
  mxDestroyArray(b_mode);
  rdb_file.close();
}




void vs2m_gateway::OutputFile::get_saturation_image()
{
  std::ifstream rdb_file( its_rdb_file_path.native_file_string().c_str(), std::ios::in | std::ios::binary);

  if (!rdb_file.is_open())
  {
    std::ostringstream err_msg (std::ostringstream::out);
    err_msg << "\nFile: " << its_rdb_file_path.native_file_string() << " couldn't be opened :(\n";
    mexErrMsgTxt( err_msg.str().c_str() );
  }
  
  const unsigned int samples_per_line = its_rpd.its_rf_mode_rx_ad_gate_width;
  const unsigned int num_lines = its_rpd.its_rf_mode_tx_trig_tbl_trigs;
  
  mxArray * saturation = mxCreateLogicalMatrix(samples_per_line, num_lines);
  if(!saturation) {mexErrMsgTxt("Couldn't make saturation.");};
  mxLogical * saturation_p = mxGetLogicals(saturation);
  
  char * u_short_data = static_cast< char * > (mxCalloc(1, sizeof(unsigned short) ) );
  unsigned short * u_short_data_p = reinterpret_cast< unsigned short *> (u_short_data);
  bool bool_data = false;
  
  rdb_file.seekg( num_lines * samples_per_line * sizeof(unsigned short), std::ios::beg);
  
  for( unsigned int i = 0; i < num_lines; i++)
  {
    for( unsigned int j = 0; j < samples_per_line; j++)
    {
      rdb_file.read(u_short_data, sizeof(unsigned short));
      bool_data = (4095 == *u_short_data_p);
      *(saturation_p + i*samples_per_line + j ) = bool_data;
    }
  }
  
  u_short_data_p = 0;
  mxFree(u_short_data);
   
  matPutVariable( its_output_file, "saturation", saturation);
  
  mxDestroyArray(saturation);
  rdb_file.close();
}



void vs2m_gateway::OutputFile::get_rf_data_image(const unsigned int frame_num)
{
  std::ifstream rdb_file( its_rdb_file_path.native_file_string().c_str(), std::ios::in | std::ios::binary);

  if (!rdb_file.is_open())
  {
    std::ostringstream err_msg (std::ostringstream::out);
    err_msg << "\nFile: " << its_rdb_file_path.native_file_string() << " couldn't be opened :(\n";
    mexErrMsgTxt( err_msg.str().c_str() );
  }
  
  const unsigned int samples_per_line = its_rpd.its_image_acquisition_size / sizeof(unsigned short);
  const unsigned int num_lines = its_rpd.its_image_lines;
  
  mxArray * rf_data = mxCreateDoubleMatrix(samples_per_line, num_lines, mxREAL);
  if(!rf_data) {mexErrMsgTxt("Couldn't make rf_data.");};
  double * rf_data_p = mxGetPr(rf_data);
  
  char * short_data = static_cast< char * > (mxCalloc(1, sizeof(short) ) );
  short * short_data_p = reinterpret_cast< short *> (short_data);
  
  rdb_file.seekg( its_rpd.its_rf_mode_tx_trig_tbl_trigs * its_rpd.its_rf_mode_rx_ad_gate_width * sizeof(unsigned short) * 2, std::ios::beg);
  unsigned int skip_amount = samples_per_line * sizeof(short) * its_rpd.its_image_acquisition_per_line;
  
  for( unsigned int i = 0; i < num_lines; i++)
  {
    rdb_file.seekg( skip_amount, std::ios::cur );
    for( unsigned int j = 0; j < samples_per_line; j++)
    {
      rdb_file.read( short_data, sizeof( short));
      *(rf_data_p + i*samples_per_line + j ) = static_cast<double> ( *short_data_p);
    }
  }
  
  short_data_p = 0;
  mxFree(short_data);
   
  matPutVariable( its_output_file, "rf_data", rf_data);
  
  mxDestroyArray(rf_data);
  rdb_file.close();
}



void vs2m_gateway::OutputFile::get_frames()
{
  if ( mxIsChar(its_prhs[4]) )
  {
    its_frames.resize( its_rpd.its_image_frames, 0);
    for( unsigned int i = 1; i<= its_rpd.its_image_frames; i++ )
    {
      its_frames[i-1] = i;
    }
  }
  else
  {
    unsigned int m = mxGetM(its_prhs[4]),
           n = mxGetN(its_prhs[4]);
    if ( !( m==1 || n==1 ) )
    {
      mexErrMsgTxt("Error in the size of the frames property.");
    }
    unsigned int size;
    if (m>=n)
      size = m;
    else
      size = n;
      
    its_frames.resize( size, 0 );
    double * frame_double = mxGetPr( its_prhs[4] );
    for( unsigned int i = 0; i < size; i++ )
    {
      its_frames[i] = static_cast< unsigned int > ( *(frame_double + i ) );
    }
  }
    
}
