# rdi2xml tests
add_test(
  rdi2xml

  ${read_visualsonics_digital_rf_BINARY_DIR}/bin/rdi2xml
  ${RDI_TESTING_FILEPATH}
  rdi2xml_test_output.xml
  )

add_test(
  rdi2xml_invalid_file

  ${read_visualsonics_digital_rf_BINARY_DIR}/bin/rdi2xml
  filepath_that_does_not_exist.rdi
  )
set_tests_properties(
  rdi2xml_invalid_file
  PROPERTIES WILL_FAIL TRUE
  )


if(BUILD_RDI2HTML)
# rdi2html tests
  add_test(
    rdi2html

    ${read_visualsonics_digital_rf_BINARY_DIR}/bin/rdi2html
    ${RDI_TESTING_FILEPATH}
    rdi2html_test_output.html
    )

  add_test(
    rdi2html_invalid_file

    ${read_visualsonics_digital_rf_BINARY_DIR}/bin/rdi2html
    filepath_that_does_not_exist.rdi
    )
  set_tests_properties(
    rdi2html_invalid_file
    PROPERTIES WILL_FAIL TRUE
    )

  add_test(
    rdi2html_xalan_comparison

    ${read_visualsonics_digital_rf_SOURCE_DIR}/tests/xml/xalan_comparison
    ${RDI_TESTING_FILEPATH}
    ${read_visualsonics_digital_rf_SOURCE_DIR}/source/xml/rdi2html.xsl
    ${read_visualsonics_digital_rf_BINARY_DIR}/bin/rdi2xml
    ${read_visualsonics_digital_rf_BINARY_DIR}/bin/rdi2html
    )
endif()
