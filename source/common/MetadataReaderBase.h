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
  namespace cxx { template<class ImageDataOutT, class CoordT> class ImageReader; };
  namespace vtk { class ImageReader; };

  class MetadataReaderBase
  {
  public:
    friend class ImageReaderBase;
    template< class ImageDataOutT, class CoordT > friend class cxx::ImageReader ;
    friend class vtk::ImageReader;

    MetadataReaderBase(const bf::path& in_file_path);

    void set_in_file_path( const bf::path& ifp );

    virtual ~MetadataReaderBase();


  protected:
    //! directory path where the .rdi and .rdb files are located,  the file name has the .rdi and .rdb removed
    bf::path its_in_file_path;

    rdiParserData* its_rpd;

  private:
    //! parse the .rdi file metadata
    void parse_metadata();

  };
} // namespace visual_sonics


#endif // METADATAREADERBASE_H
