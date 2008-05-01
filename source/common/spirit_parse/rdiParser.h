/*!  @file  rdiParser.h
     @brief object for parsing a VisualSonics .rdi file with Boost Spirit

     @author Matt McCormick <matt@mmmccormick.com>
     @date   Mon Jun 18 2007

   */

#ifndef _rdiParser_h
#define _rdiParser_h

#include <string>

#include "BoostSpiritInputFile.h"

namespace visual_sonics
{
  class rdiParserData;

  /*! @class rdiParser object for parsing a VisualSonics .rdi file with Boost Spirit */
  class rdiParser
  {
  public:

    /*! constructor
       @param file_name filename (and path if needed) of the file to be parsed
    */
    rdiParser(std::string filename);
    //! destructor
    ~rdiParser();
    rdiParserData parse();

  private:

    BoostSpiritInputFile its_input;
    rdiParserData* its_rpd;
    //! to parse the angular position
    void parse_angle();

  };
} //end namespace visual_sonics

#endif //_rdiParser_h
