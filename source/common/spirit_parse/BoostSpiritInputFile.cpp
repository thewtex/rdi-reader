/*!  @file  BoostSpiritInputFile.cpp
     @brief implementation of BoostSpiritInputFile.h

     @author Matt McCormick <matt@mmmccormick.com>
     @date   Mon Jun 18 2007

  */

#include "BoostSpiritInputFile.h"

#include <sstream>
#include <fstream>


#ifdef MATLAB_MEX_FILE
  #include "mex.h" // for errors, memory allocation
#else
  #include <iostream>
#endif

BoostSpiritInputFile::BoostSpiritInputFile(std::string filename) :
its_filename(filename)
{
  its_size = 0;

  std::ifstream infile(filename.c_str());

  if (!infile)
  {
    std::ostringstream err_msg (std::ostringstream::out);
    err_msg << "\nFile: " << filename << " couldn't be opened :(\n\nPress enter gently to continue...";
#ifdef MATLAB_MEX_FILE
    mexErrMsgTxt( err_msg.str().c_str() );
#else
    std::cerr << err_msg.str() << std::endl;
#endif
  }

  // get size of file
  infile.seekg(0,std::ifstream::end);
  its_size=infile.tellg();
  infile.seekg(0);

  // allocate memory for file content
#ifdef MATLAB_MEX_FILE
  its_buffer = static_cast<char *>(mxCalloc(its_size, sizeof(char)));
#else
  its_buffer = new char[its_size];
#endif

  // read content of infile
  infile.read(its_buffer,its_size);

  infile.close();

}

BoostSpiritInputFile::~BoostSpiritInputFile()
{
#ifdef MATLAB_MEX_FILE
  mxFree(its_buffer);
#else
  delete[] its_buffer;
#endif
}
