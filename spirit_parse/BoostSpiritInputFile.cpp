/*!  @file  BoostSpiritInputFile.cpp
     @brief implementation of BoostSpiritInputFile.h
     
     @author Matt McCormick <matt@mmmccormick.com>
     @date   Mon Jun 18 2007
     
  */
  
#include "BoostSpiritInputFile.h"

BoostSpiritInputFile::BoostSpiritInputFile(std::string filename) :
its_filename(filename)
{
  its_size = 0;
  
  std::ifstream infile(filename.c_str());

  if (!infile)
  {
    std::ostringstream err_msg (std::ostringstream::out);
    err_msg << "\nFile: " << filename << " couldn't be opened :(\n\nPress enter gently to continue...";
    mexErrMsgTxt( err_msg.str().c_str() );
  }
    
  // get size of file
  infile.seekg(0,std::ifstream::end);
  its_size=infile.tellg();
  infile.seekg(0);

  // allocate memory for file content
//   its_buffer = new char[its_size];
  its_buffer = static_cast<char *>(mxCalloc(its_size, sizeof(char)));
  
  // read content of infile
  infile.read(its_buffer,its_size);

  infile.close();

}

BoostSpiritInputFile::~BoostSpiritInputFile()
{
//   delete[] its_buffer;
  mxFree(its_buffer);
}
