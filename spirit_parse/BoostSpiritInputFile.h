/*!  @file  BoostSpiritInputFile.h
     @brief class to read an input file and put it into a char* for parsing with Boost Spirit
     
     @author Matt McCormick <matt@mmmccormick.com>
     @date   Mon Jun 18 2007 
      
   */

#ifndef _BoostSpiritInputFile_h
#define _BoostSpiritInputFile_h

#include <string>
#include <sstream>
#include <fstream>

#include "mex.h"

/*! 
  @class BoostSpiritInputFile to read an input file and put it into a char*
  for parsing with Boost Spirit
*/
class BoostSpiritInputFile
{
public:
  /*! constructor
     @param file_name filename (and path if needed) of the file to be parsed
  */
  BoostSpiritInputFile(std::string filename);
  //! destructor
  ~BoostSpiritInputFile();
  
  /*! the ascii file contents in a char* buffer for parsing by Boost Spirit
    @return buffer of file contents
  */
  char* get_buffer() const { return its_buffer; }
  
  unsigned long get_size() const { return its_size; }
  
  std::string get_filename() const { return its_filename;}
  
private:  
  //! buffer that holds the ascii file contents
  char* its_buffer;
  
  std::string its_filename;
  unsigned long its_size;
  
};



#endif //_BoostSpiritInputFile_h
