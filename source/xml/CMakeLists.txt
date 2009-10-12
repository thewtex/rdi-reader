include_directories( ${read_visualsonics_digital_rf_SOURCE_DIR}/source
  #${spirit_parse_source_dir}
  ${read_visualsonics_digital_rf_SOURCE_DIR}/source/common
  )

add_executable(rdi2xml
  rdi2xml.cpp
  ../common/rdiReader.cpp
  ../common/formats/rdi.cxx
  ../common/ToXMLCh.cpp
  )
target_link_libraries(rdi2xml ${XERCESC_LIBRARIES})

if(BUILD_RDI2HTML)
  find_package(XalanC REQUIRED)

  add_executable(rdi2html
    rdi2html_main.cpp
    rdi2html.cpp
    )

  target_link_libraries(rdi2html
    ${XALANC_LIBRARIES})

endif(BUILD_RDI2HTML)
