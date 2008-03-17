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
    ReadImageBase( std::vector<unsigned int>& frames_to_read ): 
      its_frames_to_read( frames_to_read ),
      its_frames_to_read_ind( 0 ),
      its_frame_status( true ),
      its_read_method ( file_average )
	{};
    ReadImageBase( std::vector<unsigned int>& frames_to_read, ReadMethod read_method , unsigned int specific_acquisition = 0): 
      its_frames_to_read( frames_to_read ),
      its_frames_to_read_ind( 0 ),
      its_frame_status( true ),
      its_read_method(read_method), 
      its_specific_acquisition( specific_acquisition )
	{};
    ReadImageBase( ReadMethod read_method, unsigned int specific_acquisition = 0 ):
      its_frames_to_read_ind( 0 ),
      its_frame_status( true ),
      its_read_method( read_method ), 
      its_specific_acquisition( specific_acquisition )
	{};
    ReadImageBase():
      its_frames_to_read_ind( 0 ),
      its_frame_status( true ),
      its_read_method ( file_average )
	{};

    virtual ~ReadImageBase(){};

  protected:
    //! this MUST be assigned in child classes
    ReadMetadataBase* its_read_metadata;

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
    //! I have no need for these at this point --write them as needed, carefully
    ReadImageBase( const ReadImageBase& );
    ReadImageBase&  operator=( const ReadImageBase& );
  };
}

#endif // READIMAGEBASE_H