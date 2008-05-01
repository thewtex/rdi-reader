/*! @file   MetadataReaderBase.h
 *  @brief  base class for holding and extracting VisualSonics Digital RF files and their metadata
 *
 *  @author Matt McCormick (thewtex) <matt@mmmccormick.com>
 *  @date   2008 February 25
 *
 */

#ifndef	METADATAREADERBASE_H
#define METADATAREADERBASE_H

#include <boost/filesystem/convenience.hpp>
namespace bf = boost::filesystem;


namespace visual_sonics
{
  class rdiParserData;
  namespace cxx { class ImageReader; };
  namespace vtk { class ImageReader; };

  class MetadataReaderBase
  {
  public:
    friend class ImageReaderBase;
    friend class cxx::ImageReader;
    friend class vtk::ImageReader;

    MetadataReaderBase(const bf::path& in_file_path,
	      const bf::path& in_file_name);

    virtual ~MetadataReaderBase();


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


#endif // METADATAREADERBASE_H
