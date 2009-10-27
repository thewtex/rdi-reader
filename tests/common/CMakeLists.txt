include_directories( ${RDI_READER_SOURCE_DIR}/source/common )

configure_file(rdiReaderTest.cpp
  rdiReaderTestConfigured.cpp @ONLY)
set_source_files_properties(${RDI_READER_BINARY_DIR}/tests/common/rdiReaderTestConfigured.cpp
  PROPERTIES GENERATED true)
add_executable(rdiReaderTest
  rdiReaderTestConfigured.cpp
  )
target_link_libraries(rdiReaderTest
  rdiReader
  ${XERCESC_LIBRARIES}
  ${GTEST_BOTH_LIBRARIES}
  )
GTEST_ADD_TESTS(${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/rdiReaderTest " " rdiReaderTest.cpp)

