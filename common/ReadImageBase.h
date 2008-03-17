/*! @file   ReadImageBase.h
 *  @brief  base class for reading VisualSonics Digital RF file's image data
 *
 *  @author Matt McCormick (thewtex) <matt@mmmccormick.com>
 *  @date   2008 February 25
 *
 */

#ifndef	READIMAGEBASE_H
#define READIMAGEBASE_H

#include <vector>


#include <boost/filesystem/convenience.hpp>
namespace bf = boost::filesystem;



namespace visual_sonics
{
  /**
   * @enum ReadMethod Method for reading the data from the file.
   *
   * Since there can be multiple acquisitions per frame, there
   * are many ways of picking or averaging the acquisitions
   */
  enum ReadMethod {
    file_average,  //!< take the average of all acquisitions that exists in the file
    specific_acquisition //!< get a specific acquisition only (must be specified)
    };

  // forward declaration
  class ReadMetadataBase;

  class ReadImageBase
  {
  public:
    ReadImageBase( const bf::path& in_file_path, const bf::path& in_file_name, std::vector<unsigned int>& frames_to_read );
    ReadImageBase( const bf::path& in_file_path, //!< path to the .rdi/.rdb files
	const bf::path& in_file_name, //!< filename of the .rdi/.rdb files less the extension 
	std::vector<unsigned int>& frames_to_read, //!< which frames to read from the file
	ReadMethod read_method , //!< the method for reading data from the file since there can be many acquisitions per line
	unsigned int specific_acquisition = 0 //!< which acquisition to use with using the specific_acquisition ReadMethod
	);

    //! read all frames available in the file
    ReadImageBase( const bf::path& in_file_path, const bf::path& in_file_name, ReadMethod read_method, unsigned int specific_acquisition = 0 );
    ReadImageBase( const bf::path& in_file_path, const bf::path& in_file_name );

    virtual ~ReadImageBase();

  protected:
    ReadMetadataBase* its_read_metadata;
    //! for use in constructors
    virtual void create_its_read_metadata( const bf::path& in_file_path, const bf::path& in_file_name);

    bf::path its_rdb_file_path;


    //! read the b-mode data from the file and store it in the member variable
    virtual void read_b_mode_image() = 0;
    //! read the saturation image from the file and store it in the member variable
    virtual void read_saturation_image() = 0;
      /** read the next its_frame_to_read rf data image frame from the file and store it in the 
       * member variable
       * @return frame_status if there is another frame to read
       * use it like an iterator
       */
    virtual bool read_rf_data_image() = 0;

    //! which frames to read from the file
    std::vector<unsigned int>	its_frames_to_read;
    //! current index of its_frames_to_read
    unsigned int its_frames_to_read_ind;
    //! if there is another frame to read
    bool its_frame_status;

    ReadMethod its_read_method;

    //! used if its_read_method is specific_acquisition
    unsigned int its_specific_acquisition;

  private:

    //! for use in the constructor, checking specified frames
    void check_if_frames_valid();
    //! for use in constructor, set frames to all valid frames available
    void read_all_frames();


    //! I have no need for these at this point --write them as needed, carefully
    ReadImageBase( const ReadImageBase& );
    ReadImageBase&  operator=( const ReadImageBase& );
  };
}

#endif // READIMAGEBASE_H
