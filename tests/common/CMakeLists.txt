add_test(rdi2xml ${read_visualsonics_digital_rf_BINARY_DIR}/bin/rdi2xml
  ${read_visualsonics_digital_rf_SOURCE_DIR}/test_data/machine/visualsonics/vevo770/rmv710b/pat143_seg2.rdi
  test.xml)

add_test(rdi2xml_invalid_file ${read_visualsonics_digital_rf_BINARY_DIR}/bin/rdi2xml
  ${read_visualsonics_digital_rf_SOURCE_DIR}/test_data/machine/visualsonics/vevo770/rmv710b/pat143_seg22.rdi)
set_tests_properties(rdi2xml_invalid_file PROPERTIES WILL_FAIL TRUE)
