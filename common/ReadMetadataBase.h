/*! @file   ReadMetadataBase.h
 *  @brief  base class for holding and extracting VisualSonics Digital RF files and their metadata
 *
 *  @author Matt McCormick (thewtex) <matt@mmmccormick.com>
 *  @date   2008 February 25
 *
 */

#ifndef	READMETADATABASE_H
#define READMETADATABASE_H

#include <boost/filesystem/convenience.hpp>
namespace bf = boost::filesystem;


namespace visual_sonics
{
  class rdiParserData;
  namespace cxx { class ReadImage; };

  class ReadMetadataBase
  {
  public:
    friend class ReadImageBase;
    friend class cxx::ReadImage;

    ReadMetadataBase(const bf::path& in_file_path, 
	      const bf::path& in_file_name);

    virtual ~ReadMetadataBase();

    
  protected:
    //! directory path where the .rdi and .rdb files are located
    bf::path its_in_file_path;
    //! file name of the .rdi and .rdb files minus the .rdi and .rdb
    bf::path its_in_file_name;

    rdiParserData* its_rpd;

  private:
    //! parse the .rdi file metadata
    void parse_metadata();

  };
} // namespace visual_sonics


#endif // READMETADATABASE_H
