set(RDI_READER_CLASS_SOURCES
  ${RDI_READER_SOURCE_DIR}/source/common/rdiReader.cpp
  ${RDI_READER_SOURCE_DIR}/source/common/formats/rdi.cxx
  ${RDI_READER_SOURCE_DIR}/source/common/ToXMLCh.cpp
  )
add_library( rdiReader SHARED ${RDI_READER_CLASS_SOURCES} )
target_link_libraries( rdiReader
  ${XERCESC_LIBRARIES}
  )
install( TARGETS rdiReader
  LIBRARY DESTINATION lib
  )

if(ANALYZE_RDI_FORMAT)
  add_subdirectory(formats)
endif(ANALYZE_RDI_FORMAT)
