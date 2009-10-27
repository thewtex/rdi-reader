include_directories( ${RDI_READER_SOURCE_DIR}/source/common )
include_directories( ${RDI_READER_SOURCE_DIR}/source/itk )

include( ${ITK_USE_FILE} )

add_library( ITKVisualSonicsImageIO SHARED
  itkVisualSonicsImageIO.cxx
  itkVisualSonicsImageIOFactory.cxx
  )
target_link_libraries( ITKVisualSonicsImageIO
  rdiReader
  ITKCommon
  ITKIO
  ITKStatistics
  )

add_executable( vs-convert
  vs-convert.cxx
  )
target_link_libraries( vs-convert
  ITKVisualSonicsImageIO
  )
install( TARGETS vs-convert
  RUNTIME DESTINATION bin
  )

