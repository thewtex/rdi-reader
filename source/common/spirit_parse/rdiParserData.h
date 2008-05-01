/*!  @file  rdiParserData.h
     @brief object for holding the extracted .rdi file data

     @author Matt McCormick <matt@mmmccormick.com>
     @date   Mon Jun 18 2007

   */

#ifndef _rdiParserData_h
#define _rdiParserData_h

#include <string>
#include <vector>

namespace visual_sonics
{
#pragma GCC visibility push(default)
  /*! @class rdiParserData object for holding the extracted .rdi file data */
  struct rdiParserData
  {
  public:
  //   rdiParserData();
  //   ~rdiParserData();
  //   rdiParserData(const rdiParserData & rhs);
  //   rdiParserData & operator=(const rdiParserData & rhs);

  /*  void set_study_name( const std::string str );
    void set_study_name( const char * str);
    char * get_study_name() const { return its_study_name; }*/

      //! from "=== IMAGE INFO ==="
    //! the name of the study, as assigned by the system (as for a Quick Study) or as specified by the operator
    std::string its_study_name;
    //! the operator-assigned label ( may be blank)
    std::string its_image_label;
    //! the number of RF data frames acquired
    unsigned int its_image_frames;
    //! the number of RF data lines in each frame
    unsigned int its_image_lines;
    //! the number of RF data acquisitions for each line
    unsigned int its_image_acquisition_per_line;
    //! the number of RF data sample bytes per acquisition
    unsigned int its_image_acquisition_size;
    //! the calendar date, in GMT format, on which the image was acquired
    std::string its_acquisition_date;
    //! the time of the day, in GMT format, at which the image was acquired
    std::string its_acquisition_time;
    //! the name of the orerator who acquired the image
    std::string its_acquisition_operator;


    //! from "=== IMAGE PARAMETERS ==="
    //! RF-Mode/ActiveProbe/Focal-Length (mm)
    double its_rf_mode_activeprobe_focal_length;
    //! RF-Mode/RX/RF-Gain (dB)
    double its_rf_mode_rx_rf_gain;
    //! RF-Mode/RX/V-Digi-Depth-Imaging (mm)
    double its_rf_mode_rx_v_digi_depth_imaging;
    //! RF-Mode/ActiveProbe/F-Number
    double its_rf_mode_activeprobe_f_number;
    //! RF-Mode/ActiveProbe/Pivot-Encoder-Dist (mm) Pivot to encoder distance
    double its_rf_mode_activeprobe_pivot_encoder_dist;
    //! RF-Mode/RfModeSoft/V-Lines-Pos (mm) angle on the encoder (I'm guessing)
    std::string its_rf_mode_rfmodesoft_v_lines_pos_str; // only for further parsing
    std::vector<double> its_rf_mode_rfmodesoft_v_lines_pos_vec;
    //! RF-Mode/BModeSoft/Refresh-Rate (frames/sec)
    double its_rf_mode_bmodesoft_refresh_rate;
    //! RF-Mode/RX/AD-Gate-Width number of samples on each scan line (samples/line) in the RF-mode ROI
    unsigned int its_rf_mode_rx_ad_gate_width;
    //! RF-Mode/RX/Frequency (MHz)
    double its_rf_mode_rx_frequency;
    //! RF-Mode/TX/Frequency (MHz)
    double its_rf_mode_tx_frequency;
    //! RF-Mode/ActiveProbe/Pivot-Transducer-Face-Dist (mm) Shaft Length
    double its_rf_mode_activeprobe_pivot_transducer_face_dist;
    //! RF-Mode/TX/Trig-Tbl-Trigs number of triggered scan lines acquired (lines) in the  RF-mode ROI
    unsigned int its_rf_mode_tx_trig_tbl_trigs;
    //! RF-Mode/Scan/Scan-Width (mm) Scan width along encoder
    double its_rf_mode_scan_scan_width;
    //! RF-Mode/RX/V-Delay-Length (mm) Delay Length (scan head face to acquisition start)
    double its_rf_mode_rx_v_delay_length;
    //! B-Mode/RX/AD-Gate-Width number of samples on each scan line (samples/line) in the B-mode on screen viewfinder image
    unsigned int its_b_mode_rx_ad_gate_width;
    //! B-Mode/TX/Trig-Tbl-Trigs number of triggered scan lines acquired (lines) in the  B-mode on screen viewfinder image
    unsigned int its_b_mode_tx_trig_tbl_trigs;


  };
#pragma GCC visibility pop
} //end namespace

#endif //_rdiParserData_h
