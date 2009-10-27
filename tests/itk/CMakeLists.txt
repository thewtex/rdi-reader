find_path( BMODE_INCLUDE
  NAMES itkBModeImageFilter.h
  )
if( BMODE_INCLUDE-NOTFOUND )
  message( SEND_ERROR "The itkBModeImageFilter.h header location must be
  specified." )
endif()
include_directories( ${BMODE_INCLUDE} )

include( ${ITK_USE_FILE} )

include_directories( ${RDI_READER_SOURCE_DIR}/source/itk )
include_directories( ${RDI_READER_SOURCE_DIR}/source/common )

set_source_files_properties(
  vs-convert-b-mode.cxx
  PROPERTIES COMPILE_FLAGS -DITK_MANUAL_INSTANTIATION
  )
add_executable( vs-convert-b-mode
  vs-convert-b-mode.cxx
  b-mode-instan.cxx
  reader-instan.cxx
  writer-instan.cxx
  )
target_link_libraries( vs-convert-b-mode
  ITKVisualSonicsImageIO
  )


add_test( vs-convert-b-mode
  ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/vs-convert-b-mode
  ${RDI_TESTING_FILEPATH}
  vs-convert-b-mode_output.mhd
  )

