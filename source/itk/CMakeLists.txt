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


find_path( TCLAP_INCLUDE_DIR tclap/CmdLine.h )
if( TCLAP_INCLUDE_DIR-NOTFOUND )
  message( SEND_ERROR "Tclap was not found.  Either install it or download it
  and specify TCLAP_INCLUDE_DIR." )
endif()
include_directories( ${TCLAP_INCLUDE_DIR} )
#add_executable( ibsc
  #ibsc.cxx
  #)
#target_link_libraries( ibsc
  #ITKVisualSonicsImageIO
  #IOCommon
  #ITKIO
  #)
#GDB_PRETTY_ITK( ibsc )

add_executable( phantom-spectrum
  phantomSpectrum.cxx
  )
target_link_libraries( phantom-spectrum
  ITKVisualSonicsImageIO
  ITKCommon
  ITKIO
  ITKAlgorithms
  )
GDB_PRETTY_ITK( phantom-spectrum )
