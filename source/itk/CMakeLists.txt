include_directories( ${RDI_READER_SOURCE_DIR}/source/common )
include_directories( ${RDI_READER_SOURCE_DIR}/source/itk )

include( ${ITK_USE_FILE} )

add_library( ITKVisualSonicsImageIO 
  itkVisualSonicsImageIO.cxx
  itkVisualSonicsImageIOFactory.cxx
  )
target_link_libraries( ITKVisualSonicsImageIO
  rdiReader
  ITKCommon
  ITKIO
  )
install( TARGETS ITKVisualSonicsImageIO
  DESTINATION lib
  )
install( FILES
  itkVisualSonicsImageIO.h
  itkVisualSonicsImageIOFactory.h
  itkVisualSonicsSeriesReader.h
  itkVisualSonicsSeriesReader.txx
  DESTINATION include/rdiReader
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

