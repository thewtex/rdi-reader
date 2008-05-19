set(spirit_parse_source_dir ${read_visualsonics_digital_rf_SOURCE_DIR}/source/common/spirit_parse)


include_directories( ${read_visualsonics_digital_rf_SOURCE_DIR}/source
  ${spirit_parse_source_dir}
  ${read_visualsonics_digital_rf_SOURCE_DIR}/source/common
  )


set(base_test_srcs  ${spirit_parse_source_dir}/rdiParser.cpp
                                    ${spirit_parse_source_dir}/BoostSpiritInputFile.cpp
                                    ${read_visualsonics_digital_rf_SOURCE_DIR}/source/common/interpolation/NearestNeighbor.cpp
                                    ${read_visualsonics_digital_rf_SOURCE_DIR}/source/cxx/ImageReader.cpp
                                    ${read_visualsonics_digital_rf_SOURCE_DIR}/source/common/ImageReaderBase.cpp
                                    ${read_visualsonics_digital_rf_SOURCE_DIR}/source/common/MetadataReaderBase.cpp
                                    ${read_visualsonics_digital_rf_SOURCE_DIR}/source/common/VisualSonicsTransform.cpp
				    )



set( cxx_test_srcs ${base_test_srcs}
  ${read_visualsonics_digital_rf_SOURCE_DIR}/tests/cxx_test.cpp )

#add_executable( cxx_test ${cxx_test_srcs} )
#target_link_libraries( convert_visualsonics_test parse_vs_rdi )



FIND_PACKAGE(VTK REQUIRED)
IF(NOT VTK_USE_RENDERING)
  MESSAGE(FATAL_ERROR "${PROJECT_NAME} requires VTK_USE_RENDERING.")
ENDIF(NOT VTK_USE_RENDERING)
INCLUDE(${VTK_USE_FILE})



set( vtk_test_srcs ${base_test_srcs}
  ${read_visualsonics_digital_rf_SOURCE_DIR}/source/vtk/vtkVisualSonicsReader.cpp
  ${read_visualsonics_digital_rf_SOURCE_DIR}/source/vtk/ImageViewer.cpp
  ${read_visualsonics_digital_rf_SOURCE_DIR}/tests/vtk_test.cpp
  )

add_executable( vtk_test ${vtk_test_srcs} )
target_link_libraries( vtk_test vtkRendering boost_filesystem)
