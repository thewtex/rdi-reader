# rdi2xml tests
add_test(
  rdi2xml

  ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/rdi2xml
  ${RDI_TESTING_FILEPATH}
  rdi2xml_test_output.xml
  )

add_test(
  rdi2xml_invalid_file

  ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/rdi2xml
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

    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/rdi2html
    ${RDI_TESTING_FILEPATH}
    rdi2html_test_output.html
    )

  add_test(
    rdi2html_invalid_file

    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/rdi2html
    filepath_that_does_not_exist.rdi
    )
  set_tests_properties(
    rdi2html_invalid_file
    PROPERTIES WILL_FAIL TRUE
    )

  add_test(
    rdi2html_xalan_comparison

    ${RDI_READER_SOURCE_DIR}/tests/xml/xalan_comparison
    ${RDI_TESTING_FILEPATH}
    ${RDI_READER_SOURCE_DIR}/source/xml/rdi2html.xsl
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/rdi2xml
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/rdi2html
    )
endif()
