/*!  @file  rdiParser.cpp
     @brief implementation of rdiParser.h
     
     @author  Matt McCormick <matt@mmmccormick.com>
     @date   Mon Jun 18 2007
     
  */
  
#include "rdiParser.h"

#include <sstream>

#include "boost/spirit/core.hpp"
#include "boost/spirit/actor/assign_actor.hpp"
#include "boost/spirit/utility/loops.hpp"

#ifdef MATLAB_MEX_FILE
  #include "mex.h"
#else
  #include <iostream>
#endif

rdiParser::rdiParser(std::string filename) :
  its_input(filename + ".rdi")
{
}



rdiParser::~rdiParser()
{
}



rdiParserData rdiParser::parse()
{
    //! Define types of SPIRIT constructs
  namespace bs = boost::spirit;
  
  typedef const char * iterator_t;
/* //to find the skip parser type
     #include <typeinfo>
     std::ostringstream err_msg (std::ostringstream::out);
     err_msg << "the typeid of bs::ch_p('\"') is: " << typeid(bs::ch_p('"')).name() << '\n';
     mexErrMsgTxt( err_msg.str().c_str() );*/
  typedef bs::chlit<char> skip_t;
  typedef bs::skip_parser_iteration_policy<skip_t> iter_policy_t;
  typedef bs::scanner_policies<iter_policy_t> scanner_policies_t;
  typedef bs::scanner<iterator_t, scanner_policies_t> scanner_t;
  /* all this is necessary because of 
     http://www.boost.org/libs/spirit/doc/faq.html
     http://www.boost.org/libs/spirit/doc/scanner.html
     http://www.boost.org/libs/spirit/doc/rule.html
     */
  typedef bs::rule<scanner_t> rule_t;
  
  rule_t yada_line_p = *(bs::print_p | bs::blank_p| bs::ch_p(static_cast<char>(0xb5)) )>> bs::eol_p;  // 0x  b5 aka 181 aka mu aka micro
 
  
  rule_t image_info_p =  bs::str_p("=== IMAGE INFO ===") >> bs::eol_p >>
                   bs::str_p("Study Name") >> bs::ch_p(',') >> *(bs::anychar_p[ bs::push_back_a(its_rpd.its_study_name) ] ^ bs::eol_p) >> bs::eol_p >> 
                   bs::str_p("Image Label") >> bs::ch_p(',') >> *(bs::anychar_p[ bs::push_back_a(its_rpd.its_image_label) ] ^ bs::eol_p) >> bs::eol_p >>
                   bs::str_p("Image Frames") >> bs::ch_p(',') >> bs::uint_p[ bs::assign_a(its_rpd.its_image_frames) ] >> bs::eol_p >>
                   bs::str_p("Image Lines") >> bs::ch_p(',') >> bs::uint_p[ bs::assign_a(its_rpd.its_image_lines) ] >>  bs::eol_p >>
                   bs::str_p("Image Acquisition Per Line") >> bs::ch_p(',') >> bs::uint_p[ bs::assign_a(its_rpd.its_image_acquisition_per_line) ] >> bs::eol_p >>
                   bs::str_p("Image Acquisition Size") >> bs::ch_p(',') >> bs::uint_p[ bs::assign_a(its_rpd.its_image_acquisition_size) ] >> bs::ch_p(',') >> bs::str_p("bytes") >> bs::eol_p >>
                   bs::str_p("Animal ID") >> bs::ch_p(',') >> *(bs::anychar_p ^ bs::eol_p) >> bs::eol_p >>
                   bs::str_p("Acquisition Mode") >> bs::ch_p(',') >> *(bs::anychar_p ^ bs::eol_p) >> bs::eol_p >>
                   bs::str_p("Acquisition Date") >> bs::ch_p(',') >> *(bs::anychar_p[ bs::push_back_a(its_rpd.its_acquisition_date) ] ^ bs::eol_p) >> bs::eol_p >>
                   bs::str_p("Acquisition Time") >> bs::ch_p(',') >> *(bs::anychar_p[ bs::push_back_a(its_rpd.its_acquisition_time) ] ^ bs::eol_p) >> bs::eol_p >>  
                   bs::str_p("Acquisition Operator") >> bs::ch_p(',') >> *(bs::anychar_p[ bs::push_back_a(its_rpd.its_acquisition_operator) ] ^ bs::eol_p) >> bs::eol_p 
                   ;
                   
  rule_t image_data_p =  bs::str_p("=== IMAGE DATA ===") >> bs::eol_p >>
                   * (yada_line_p ^ ( bs::str_p("=== IMAGE PARAMETERS ===") >> bs::eol_p ) ) // XOR, Match a or b, but not both
                   ;
                   
  rule_t image_parameters_p = bs::str_p("=== IMAGE PARAMETERS ===") >> bs::eol_p >>
                  // bs::repeat_p(3)[ yada_line_p ] >>
		  //
		  *( yada_line_p ^ bs::str_p("RF-Mode/ActiveProbe/Focal-Length") ) >> 
		    bs::str_p("RF-Mode/ActiveProbe/Focal-Length") >> bs::ch_p(',') >>
                    bs::real_p[ bs::assign_a( its_rpd.its_rf_mode_activeprobe_focal_length) ] >>  bs::ch_p(',') >> 
		    bs::str_p("mm") >> bs::eol_p >> 

		  *( yada_line_p ^ bs::str_p("RF-Mode/RX/RF-Gain") ) >>
                    bs::str_p("RF-Mode/RX/RF-Gain") >> bs::ch_p(',') >>
		    bs::real_p[ bs::assign_a( its_rpd.its_rf_mode_rx_rf_gain) ] >>  bs::ch_p(',') >> 
		    bs::str_p("dB") >> bs::eol_p >>

		  *( yada_line_p ^ bs::str_p("RF-Mode/RX/V-Digi-Depth-Imaging") ) >>
		    bs::str_p("RF-Mode/RX/V-Digi-Depth-Imaging") >> bs::ch_p(',') >>
		    bs::real_p[ bs::assign_a( its_rpd.its_rf_mode_rx_v_digi_depth_imaging) ] >>  bs::ch_p(',') >> 
		    bs::str_p("mm") >> bs::eol_p >> 

		  *( yada_line_p ^ bs::str_p("RF-Mode/ActiveProbe/F-Number") ) >>
		    bs::str_p("RF-Mode/ActiveProbe/F-Number") >> bs::ch_p(',') >> 
		    bs::real_p[ bs::assign_a( its_rpd.its_rf_mode_activeprobe_f_number) ] >>  
		    bs::eol_p >>

		  *( yada_line_p ^ bs::str_p("RF-Mode/ActiveProbe/Pivot-Encoder-Dist") ) >>
		    bs::str_p("RF-Mode/ActiveProbe/Pivot-Encoder-Dist") >> bs::ch_p(',') >> 
		    bs::real_p[ bs::assign_a( its_rpd.its_rf_mode_activeprobe_pivot_encoder_dist) ] >> bs::ch_p(',') >> 
		    bs::str_p("mm") >> bs::eol_p >>
                  
		    *( yada_line_p ^ bs::str_p("RF-Mode/RfModeSoft/V-Lines-Pos") ) >>
		    bs::str_p("RF-Mode/RfModeSoft/V-Lines-Pos") >> bs::ch_p(',') >> 
		    *( bs::digit_p[ bs::push_back_a( its_rpd.its_rf_mode_rfmodesoft_v_lines_pos_str) ] 
			| bs::ch_p(',')[ bs::push_back_a( its_rpd.its_rf_mode_rfmodesoft_v_lines_pos_str) ]
			| bs::ch_p('-')[ bs::push_back_a( its_rpd.its_rf_mode_rfmodesoft_v_lines_pos_str) ]
			| bs::ch_p('.')[ bs::push_back_a( its_rpd.its_rf_mode_rfmodesoft_v_lines_pos_str) ] 
			)
		    >> bs::eol_p >>

		  *( yada_line_p ^ bs::str_p("RF-Mode/BModeSoft/Refresh-Rate") ) >>
		    bs::str_p("RF-Mode/BModeSoft/Refresh-Rate") >> bs::ch_p(',') >> 
		    bs::real_p[ bs::assign_a( its_rpd.its_rf_mode_bmodesoft_refresh_rate) ] >> bs::ch_p(',') >> 
		    bs::str_p("frames/sec") >> bs::eol_p >>  
                  
		  *( yada_line_p ^ bs::str_p("RF-Mode/RX/AD-Gate-Width") ) >> 
		    bs::str_p("RF-Mode/RX/AD-Gate-Width") >> bs::ch_p(',') >> 
		    bs::uint_p[ bs::assign_a( its_rpd.its_rf_mode_rx_ad_gate_width ) ] >> bs::ch_p(',') >> 
		    bs::str_p("samples") >> bs::eol_p >> 

		    *( yada_line_p ^ bs::str_p("RF-Mode/RX/Frequency" ) ) >> 
		    bs::str_p("RF-Mode/RX/Frequency" ) >> bs::ch_p(',') >> 
		    bs::real_p[ bs::assign_a( its_rpd.its_rf_mode_rx_frequency ) ] >> bs::ch_p(',') >> 
		    bs::str_p("MHz") >> bs::eol_p >> 

		    *( yada_line_p ^ bs::str_p("RF-Mode/TX/Frequency" ) ) >> 
		    bs::str_p("RF-Mode/TX/Frequency" ) >> bs::ch_p(',') >> 
		    bs::real_p[ bs::assign_a( its_rpd.its_rf_mode_tx_frequency ) ] >> bs::ch_p(',') >> 
		    bs::str_p("MHz") >> bs::eol_p >> 

                  *( yada_line_p ^ bs::str_p("RF-Mode/ActiveProbe/Pivot-Transducer-Face-Dist") ) >> 
		    bs::str_p("RF-Mode/ActiveProbe/Pivot-Transducer-Face-Dist") >> bs::ch_p(',') >> 
		    bs::real_p[ bs::assign_a( its_rpd.its_rf_mode_activeprobe_pivot_transducer_face_dist ) ] >> bs::ch_p(',') >> 
		    bs::str_p("mm") >> bs::eol_p >>

                  *( yada_line_p ^ bs::str_p("RF-Mode/TX/Trig-Tbl-Trigs") ) >> 
		    bs::str_p("RF-Mode/TX/Trig-Tbl-Trigs") >> bs::ch_p(',') >> 
		    bs::uint_p[ bs::assign_a( its_rpd.its_rf_mode_tx_trig_tbl_trigs ) ] >> bs::ch_p(',') >> 
		    bs::str_p("trigs") >> bs::eol_p >>

                  *( yada_line_p ^ bs::str_p("RF-Mode/Scan/Scan-Width") ) >> 
		    bs::str_p("RF-Mode/Scan/Scan-Width") >> bs::ch_p(',') >> 
		    bs::real_p[ bs::assign_a( its_rpd.its_rf_mode_scan_scan_width) ] >> bs::ch_p(',') >> 
		    bs::str_p("mm") >> bs::eol_p >>

                  *( yada_line_p ^ bs::str_p("RF-Mode/RX/V-Delay-Length") ) >> 
		    bs::str_p("RF-Mode/RX/V-Delay-Length") >> bs::ch_p(',') >> 
		    bs::real_p[ bs::assign_a( its_rpd.its_rf_mode_rx_v_delay_length) ] >> bs::ch_p(',') >> 
		    bs::str_p("mm") >> bs::eol_p >> 

                  *( yada_line_p ^ bs::str_p("B-Mode/RX/AD-Gate-Width") ) >>
		    bs::str_p("B-Mode/RX/AD-Gate-Width") >> bs::ch_p(',') >> 
		    bs::uint_p[ bs::assign_a( its_rpd.its_b_mode_rx_ad_gate_width ) ] >> bs::ch_p(',') >> 
		    bs::str_p("samples") >> bs::eol_p >> 

                  *( yada_line_p ^ bs::str_p("B-Mode/TX/Trig-Tbl-Trigs") ) >>
		    bs::str_p("B-Mode/TX/Trig-Tbl-Trigs") >> bs::ch_p(',') >> 
		    bs::uint_p[ bs::assign_a( its_rpd.its_b_mode_tx_trig_tbl_trigs ) ] >> bs::ch_p(',') >> 
		    bs::str_p("trigs") >> bs::eol_p

                  ;
                  
  rule_t rdi_file_p = image_info_p >>
                 image_data_p >>
                 image_parameters_p >>
		 *yada_line_p
                 ;
                 
  iterator_t first( its_input.get_buffer() );
  iterator_t last = first + its_input.get_size() ;
/*      an alternate to the line below would be:
  iter_policy_t iter_policy(bs::ch_p('"'));
  scanner_policies_t policies(iter_policy);
  scanner_t scan( first, last, policies );
  rdi_file_p.parse(scan);
  */
  bs::parse_info<iterator_t> info = bs::parse< iterator_t, rule_t, skip_t> ( first, last, rdi_file_p, bs::ch_p('"') );
  if (!info.full)
  {
    std::ostringstream err_msg (std::ostringstream::out);
    err_msg << "convert_visualsonics_2_mat: Not the entire input file " << its_input.get_filename() << 
    "\n got parsed or the input parser could not recognize the input string\n" <<
    "Only " << info.length << " characters were consumed by the parser.\n";
#ifdef MATLAB_MEX_FILE
    mexErrMsgTxt( err_msg.str().c_str() );
#else
    std::cerr << err_msg.str() << std::endl;
#endif
  }
  

  parse_angle(); //parse the angles string


  return its_rpd;
}



void rdiParser::parse_angle()
{
    //! Define types of SPIRIT constructs
  namespace bs = boost::spirit;
  
  typedef const char * iterator_t;
/* //to find the skip parser type
     #include <typeinfo>
     std::ostringstream err_msg (std::ostringstream::out);
     err_msg << "the typeid of bs::ch_p('\"') is: " << typeid(bs::ch_p('"')).name() << '\n';
     mexErrMsgTxt( err_msg.str().c_str() );*/
  typedef bs::chlit<char> skip_t;
  typedef bs::skip_parser_iteration_policy<skip_t> iter_policy_t;
  typedef bs::scanner_policies<iter_policy_t> scanner_policies_t;
  typedef bs::scanner<iterator_t, scanner_policies_t> scanner_t;
  /* all this is necessary because of 
     http://www.boost.org/libs/spirit/doc/faq.html
     http://www.boost.org/libs/spirit/doc/scanner.html
     http://www.boost.org/libs/spirit/doc/rule.html
     */
  typedef bs::rule<scanner_t> rule_t;
  
  rule_t angle_p = *bs::real_p[ bs::push_back_a( its_rpd.its_rf_mode_rfmodesoft_v_lines_pos_vec ) ] ;
  iterator_t first( its_rpd.its_rf_mode_rfmodesoft_v_lines_pos_str.data() );
  iterator_t last = first +  its_rpd.its_rf_mode_rfmodesoft_v_lines_pos_str.length();
/*      an alternate to the line below would be:
  iter_policy_t iter_policy(bs::ch_p('"'));
  scanner_policies_t policies(iter_policy);
  scanner_t scan( first, last, policies );
  rdi_file_p.parse(scan);
  */
  bs::parse_info<iterator_t> info = bs::parse< iterator_t, rule_t, skip_t> ( first, last, angle_p, bs::ch_p(',') );
  if (!info.full)
  {
    std::ostringstream err_msg (std::ostringstream::out);
    err_msg << "convert_visualsonics_2_mat: Not the entire angle string " << its_input.get_filename() << 
    "\n got parsed or the input parser could not recognize the input string\n" <<
    "Only " << info.length << " characters were consumed by the parser.\n";
    mexErrMsgTxt( err_msg.str().c_str() );
  }
  
  assert( its_rpd.its_rf_mode_rfmodesoft_v_lines_pos_vec.size() == its_rpd.its_image_lines );

  its_rpd.its_rf_mode_rfmodesoft_v_lines_pos_str.clear(); // clean up
}

