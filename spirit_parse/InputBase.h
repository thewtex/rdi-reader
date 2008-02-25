/*! @file   InputBase.h
 *  @brief  base class for holding and extracting VisualSonics Digital RF files and their metadata
 *
 *  @author Matt McCormick (thewtex) <matt@mmmccormick.com>
 *  @date   2008 February 25
 *
 */

#ifndef	INPUTBASE_H
#define INPUTBASE_H

#include <boost/filesystem/convenience.h>

namespace bf = boost::filesystem;

#include "OutputBase.h"
#include "rdiParserData.h"

namespace visual_sonics
{
  class InputBase
  {
  public:
    friend class OutputBase;

    InputBase(const bf::path& in_file_path, 
	      const bf::path& in_file_name);

    virtual ~InputBase(){};

    
  protected:
    //! directory path where the .rdi and .rdb files are located
    bf::path its_in_file_path;
    //! file name of the .rdi and .rdb files minus the .rdi and .rdb
    bf::path its_in_file_name;

    rdiParserData its_rpd;

  private:
    //! parse the .di file metadata
    void parse_metadata();

  };
} // namespace visual_sonics


#endif // INPUTBASE_H
