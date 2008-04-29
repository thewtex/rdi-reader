set(spirit_parse_source_dir ${read_visualsonics_digital_rf_SOURCE_DIR}/source/common/spirit_parse)


include_directories( ${read_visualsonics_digital_rf_SOURCE_DIR}/source 
  ${spirit_parse_source_dir} 
  ${read_visualsonics_digital_rf_SOURCE_DIR}/source/common
  )


set(base_test_srcs  ${spirit_parse_source_dir}/rdiParser.cpp
                                    ${spirit_parse_source_dir}/BoostSpiritInputFile.cpp
                                    ${read_visualsonics_digital_rf_SOURCE_DIR}/source/cxx/ReadImage.cpp 
                                    ${read_visualsonics_digital_rf_SOURCE_DIR}/source/common/ReadImageBase.cpp 
                                    ${read_visualsonics_digital_rf_SOURCE_DIR}/source/common/ReadMetadataBase.cpp 
				    )



set( cxx_test_srcs ${base_test_srcs} 
  ${read_visualsonics_digital_rf_SOURCE_DIR}/tests/cxx_test.cpp )

#add_executable( cxx_test ${cxx_test_srcs} )
#target_link_libraries( convert_visualsonics_test parse_vs_rdi )



find_package(VTK REQUIRED)
if(NOT VTK_USE_RENDERING)
  message(FATAL_ERROR "${PROJECT_NAME} requires VTK_USE_RENDERING.")
endif(NOT VTK_USE_RENDERING)
include(${VTK_USE_FILE})
include_directories(${VTK_INCLUDE_DIRS})


set( vtk_test_srcs ${base_test_srcs}
  ${read_visualsonics_digital_rf_SOURCE_DIR}/source/vtk/ReadImage.cpp 
  ${read_visualsonics_digital_rf_SOURCE_DIR}/source/vtk/ViewImage.cpp 
  ${read_visualsonics_digital_rf_SOURCE_DIR}/tests/vtk_test.cpp 
  )

add_executable( vtk_test ${vtk_test_srcs} )
target_link_libraries( vtk_test vtkRendering )
add_test( vtk_tester vtk_test )
