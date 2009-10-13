include_directories(${RDI_READER_SOURCE_DIR}/source
  ${RDI_READER_SOURCE_DIR}/source/common)

set(RDI_READER_CLASS_SOURCES
  ../common/rdiReader.cpp
  ../common/formats/rdi.cxx
  ../common/ToXMLCh.cpp
  )


add_executable(rdi2xml
  rdi2xml.cpp
  ${RDI_READER_CLASS_SOURCES}
  )
target_link_libraries(rdi2xml
  ${XERCESC_LIBRARIES})
install(TARGETS rdi2xml
  RUNTIME
  DESTINATION bin
  )


if(BUILD_RDI2HTML)
  find_package(XalanC REQUIRED)

  add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/rdi2html.xsl.cstringliteral
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/file2cstringliteral.py rdi2html.xsl
    ${CMAKE_CURRENT_BINARY_DIR}/rdi2html.xsl.cstringliteral
    MAIN_DEPENDENCY rdi2html.xsl
    DEPENDS file2cstringliteral.py
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Generating rdi2html XSLT C/C++ string literal."
    )
  set_source_files_properties(rdi2html.cpp
    PROPERTIES OBJECT_DEPENDS
    ${CMAKE_CURRENT_BINARY_DIR}/rdi2html.xsl.cstringliteral
    )
  include_directories(${CMAKE_CURRENT_BINARY_DIR})
  add_executable(rdi2html
    rdi2html_main.cpp
    rdi2html.cpp
    ${RDI_READER_CLASS_SOURCES}
    )
  target_link_libraries(rdi2html
    ${XALANC_LIBRARIES})
  install(TARGETS rdi2html
    RUNTIME
    DESTINATION bin
    )
endif(BUILD_RDI2HTML)
