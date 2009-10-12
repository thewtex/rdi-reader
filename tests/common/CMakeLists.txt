include_directories(${read_visualsonics_digital_rf_SOURCE_DIR}/source/common)
set(rdiReader_test_sources rdiReader.cpp formats/rdi.cxx ToXMLCh.cpp)
foreach(f ${rdiReader_test_sources})
  set(rdiReader_test_sources_full_paths ${rdiReader_test_sources_full_paths}
    ${read_visualsonics_digital_rf_SOURCE_DIR}/source/common/${f})
endforeach(f)
configure_file(rdiReaderTest.cpp rdiReaderTestConfigured.cpp @ONLY)
set_source_files_properties(${read_visualsonics_digital_rf_BINARY_DIR}/tests/common/rdiReaderTestConfigured.cpp PROPERTIES GENERATED true)
add_executable(rdiReaderTest rdiReaderTestConfigured.cpp
  ${rdiReader_test_sources_full_paths})
target_link_libraries(rdiReaderTest ${XERCESC_LIBRARIES} ${GTEST_BOTH_LIBRARIES})
GTEST_ADD_TESTS(${CMAKE_CURRENT_BINARY_DIR}/rdiReaderTest " " rdiReaderTest.cpp)

