/*!  @file  vs2m_gateway_Output.cpp
     @brief implementation of vs2m_gateway_Output.h

     @author Matt McCormick <matt@mmmccormick.com>
     @date   Wed Jul 25 2007

  */

#include "vs2m_gateway_Output.h"

vs2m_gateway::Output::Output(mxArray * plhs[], const rdiParserData & rpd):
  its_plhs(plhs),
  its_rpd(rpd)
{

  create_base_struct();

  create_image_info_struct();

  create_image_parameters_struct();

}




void vs2m_gateway::Output::create_base_struct()
{
  const char * sections[] = {"image_info",
                           "image_data",
                           "image_parameters"};       /* pointers to field names */
  its_plhs[0] = mxCreateStructMatrix(1, 1, (sizeof(sections)/sizeof(*sections)), sections);

}



void vs2m_gateway::Output::create_image_info_struct()
{
  const char * image_info_fnames[] = {"Study_Name",
                                      "Image_Label",
                                      "Image_Frames",
                                      "Image_Lines",
                                      "Image_Acquisition_Per_Line",
                                      "Image_Acquisition_Size",
                                      "Acquisition_Date",
                                      "Acquisition_Time",
                                      "Acquisition_Operator"
                                      };
  mxArray * image_info_struct = mxCreateStructMatrix(1, 1, (sizeof(image_info_fnames)/sizeof(*image_info_fnames)), image_info_fnames);
  mxSetFieldByNumber(its_plhs[0], 0, 0, image_info_struct);

  int field_count = 0;
  mxArray * study_name = mxCreateString(its_rpd.its_study_name.c_str());
  mxSetFieldByNumber(image_info_struct, 0, field_count++, study_name);
  mxArray * image_label = mxCreateString(its_rpd.its_image_label.c_str());
  mxSetFieldByNumber(image_info_struct, 0, field_count++, image_label);
  mxArray * image_frames = mxCreateDoubleScalar( static_cast<double>( its_rpd.its_image_frames ) );
  mxSetFieldByNumber(image_info_struct, 0, field_count++, image_frames);
  mxArray * image_lines = mxCreateDoubleScalar( static_cast<double>( its_rpd.its_image_lines) );
  mxSetFieldByNumber(image_info_struct, 0, field_count++, image_lines);
  mxArray * image_acquisition_per_line = mxCreateDoubleScalar( static_cast<double>( its_rpd.its_image_acquisition_per_line) );
  mxSetFieldByNumber(image_info_struct, 0, field_count++, image_acquisition_per_line);
  mxArray * image_acquisition_size = mxCreateDoubleScalar( static_cast<double>( its_rpd.its_image_acquisition_size) );
  mxSetFieldByNumber(image_info_struct, 0, field_count++, image_acquisition_size);
  mxArray * acquisition_date = mxCreateString(its_rpd.its_acquisition_date.c_str());
  mxSetFieldByNumber(image_info_struct, 0, field_count++, acquisition_date);
  mxArray * acquisition_time = mxCreateString(its_rpd.its_acquisition_time.c_str());
  mxSetFieldByNumber(image_info_struct, 0, field_count++, acquisition_time);
  mxArray * acquisition_operator = mxCreateString(its_rpd.its_acquisition_operator.c_str());
  mxSetFieldByNumber(image_info_struct, 0, field_count++, acquisition_operator);

}


void vs2m_gateway::Output::create_image_parameters_struct()
{
  //! @note Matlab doesn't like '-' or '/' in its structure names
  const char * image_data_fnames[] = {"RF_Mode__ActiveProbe__Focal_Length___mm",
                                      "RF_Mode__RX__RF_Gain___dB",
                                      "RF_Mode__RX__V_Digi_Depth_Imaging___mm",
                                      "RF_Mode__ActiveProbe__F_Number",
                                      "RF_Mode__ActiveProbe__Pivot_Encoder_Dist___mm",
				      "RF_Mode__RfModeSoft__V_Lines_Pos___mm",
                                      "RF_Mode__BModeSoft__Refresh_Rate___frames_per_sec",
                                      "RF_Mode__RX__AD_Gate_Width___samples_per_line",
				      "RF_Mode__RX__Frequency___MHz",
				      "RF_Mode__TX__Frequency___MHz",
                                      "RF_Mode__ActiveProbe__Pivot_Transducer_Face_Dist___mm",
                                      "RF_Mode__TX__Trig_Tbl_Trigs___lines",
                                      "RF_Mode__Scan__Scan_Width___mm",
                                      "RF_Mode__RX__V_Delay_Length___mm",
                                      "B_Mode__RX__AD_Gate_Width___samples_per_line",
                                      "B_Mode__TX__Trig_Tbl_Trigs___lines",
                                      };

  mxArray * image_data_struct = mxCreateStructMatrix(1, 1, (sizeof(image_data_fnames)/sizeof(*image_data_fnames)), image_data_fnames);
  mxSetFieldByNumber(its_plhs[0], 0, 2, image_data_struct);

  int field_count = 0;
  mxArray * rf_mode_activeprobe_focal_length = mxCreateDoubleScalar( static_cast<double>( its_rpd.its_rf_mode_activeprobe_focal_length ));
  mxSetFieldByNumber( image_data_struct, 0, field_count++, rf_mode_activeprobe_focal_length);
  mxArray * rf_mode_rx_rf_gain = mxCreateDoubleScalar( static_cast<double>( its_rpd.its_rf_mode_rx_rf_gain));
  mxSetFieldByNumber( image_data_struct, 0, field_count++, rf_mode_rx_rf_gain);
  mxArray * rf_mode_rx_v_digi_depth_imaging = mxCreateDoubleScalar( static_cast<double>( its_rpd.its_rf_mode_rx_v_digi_depth_imaging));
  mxSetFieldByNumber( image_data_struct, 0, field_count++, rf_mode_rx_v_digi_depth_imaging);
  mxArray * rf_mode_activeprobe_f_number = mxCreateDoubleScalar( static_cast<double>( its_rpd.its_rf_mode_activeprobe_f_number));
  mxSetFieldByNumber( image_data_struct, 0, field_count++, rf_mode_activeprobe_f_number);
  mxArray * rf_mode_activeprobe_pivot_encoder_dist = mxCreateDoubleScalar( static_cast<double>( its_rpd.its_rf_mode_activeprobe_pivot_encoder_dist));
  mxSetFieldByNumber( image_data_struct, 0, field_count++, rf_mode_activeprobe_pivot_encoder_dist);
  mxArray * rf_mode_rfmodesoft_v_lines_pos = mxCreateDoubleMatrix( its_rpd.its_rf_mode_rfmodesoft_v_lines_pos_vec.size(), 1, mxREAL);
  double* angle_p = mxGetPr( rf_mode_rfmodesoft_v_lines_pos );
  for ( unsigned int i = 0; i < its_rpd.its_rf_mode_rfmodesoft_v_lines_pos_vec.size(); i++)
  {
    *(angle_p + i) = its_rpd.its_rf_mode_rfmodesoft_v_lines_pos_vec[i];
  }
  mxSetFieldByNumber( image_data_struct, 0, field_count++, rf_mode_rfmodesoft_v_lines_pos );
  mxArray * rf_mode_bmodesoft_refresh_rate= mxCreateDoubleScalar( static_cast<double>( its_rpd.its_rf_mode_bmodesoft_refresh_rate));
  mxSetFieldByNumber( image_data_struct, 0, field_count++, rf_mode_bmodesoft_refresh_rate);
  mxArray * rf_mode_rx_ad_gate_width = mxCreateDoubleScalar( static_cast<double>( its_rpd.its_rf_mode_rx_ad_gate_width ));
  mxSetFieldByNumber( image_data_struct, 0, field_count++, rf_mode_rx_ad_gate_width);
  mxArray * rf_mode_rx_frequency = mxCreateDoubleScalar( static_cast<double>( its_rpd.its_rf_mode_rx_frequency ));
  mxSetFieldByNumber( image_data_struct, 0, field_count++, rf_mode_rx_frequency );
  mxArray * rf_mode_tx_frequency = mxCreateDoubleScalar( static_cast<double>( its_rpd. its_rf_mode_tx_frequency ));
  mxSetFieldByNumber( image_data_struct, 0, field_count++, rf_mode_tx_frequency );
  mxArray * rf_mode_activeprobe_pivot_transducer_face_dist = mxCreateDoubleScalar( static_cast<double>( its_rpd.its_rf_mode_activeprobe_pivot_transducer_face_dist ));
  mxSetFieldByNumber( image_data_struct, 0, field_count++, rf_mode_activeprobe_pivot_transducer_face_dist);
  mxArray * rf_mode_tx_trig_tbl_trigs = mxCreateDoubleScalar( static_cast<double>( its_rpd.its_rf_mode_tx_trig_tbl_trigs ));
  mxSetFieldByNumber( image_data_struct, 0, field_count++, rf_mode_tx_trig_tbl_trigs);
  mxArray * rf_mode_scan_scan_width = mxCreateDoubleScalar( static_cast<double>( its_rpd.its_rf_mode_scan_scan_width));
  mxSetFieldByNumber( image_data_struct, 0, field_count++, rf_mode_scan_scan_width);
  mxArray * rf_mode_rx_v_delay_length = mxCreateDoubleScalar( static_cast<double>( its_rpd.its_rf_mode_rx_v_delay_length));
  mxSetFieldByNumber( image_data_struct, 0, field_count++, rf_mode_rx_v_delay_length);
  mxArray * b_mode_rx_ad_gate_width = mxCreateDoubleScalar( static_cast<double>( its_rpd.its_b_mode_rx_ad_gate_width ));
  mxSetFieldByNumber( image_data_struct, 0, field_count++, b_mode_rx_ad_gate_width);
  mxArray * b_mode_tx_trig_tbl_trigs = mxCreateDoubleScalar( static_cast<double>( its_rpd.its_b_mode_tx_trig_tbl_trigs ));
  mxSetFieldByNumber( image_data_struct, 0, field_count++, b_mode_tx_trig_tbl_trigs);

}
