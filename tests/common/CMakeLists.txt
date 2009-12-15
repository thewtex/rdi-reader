include_directories( ${RDI_READER_SOURCE_DIR}/source/common )

add_executable(rdiReaderTest
  rdiReaderTest.cpp
  )
target_link_libraries(rdiReaderTest
  rdiReader
  ${XERCESC_LIBRARIES}
  ${GTEST_LIBRARIES}
  )
add_test(rdiReaderGTest 
  ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/rdiReaderTest 
  ${RDI_TESTING_FILEPATH}
  )

