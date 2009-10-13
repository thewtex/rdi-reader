find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

set(RDI_TESTING_FILEPATH
  ${read_visualsonics_digital_rf_SOURCE_DIR}/data_for_tests/machine/visualsonics/vevo770/rmv710b/pat143_seg2.rdi
  )

if(ANALYZE_RDI_FORMAT)
  add_test(
    format-evaluator
    ${read_visualsonics_digital_rf_SOURCE_DIR}/tests/python_unittests.py
    FormatEvaluatorSuite
    )
endif(ANALYZE_RDI_FORMAT)

set(spirit_parse_source_dir ${read_visualsonics_digital_rf_SOURCE_DIR}/source/common/spirit_parse)



set(base_test_srcs  ${spirit_parse_source_dir}/rdiParser.cpp
                                    ${spirit_parse_source_dir}/BoostSpiritInputFile.cpp
                                    ${read_visualsonics_digital_rf_SOURCE_DIR}/source/common/interpolation/Interpolation.cpp
                                    ${read_visualsonics_digital_rf_SOURCE_DIR}/source/common/interpolation/NearestNeighbor.cpp
                                    ${read_visualsonics_digital_rf_SOURCE_DIR}/source/common/interpolation/Bilinear.cpp
                                    ${read_visualsonics_digital_rf_SOURCE_DIR}/source/common/ImageReader.cpp
                                    ${read_visualsonics_digital_rf_SOURCE_DIR}/source/common/ImageReaderBase.cpp
                                    ${read_visualsonics_digital_rf_SOURCE_DIR}/source/common/MetadataReaderBase.cpp
                                    ${read_visualsonics_digital_rf_SOURCE_DIR}/source/common/VisualSonicsTransform.cpp
				    )



set( cxx_test_srcs ${base_test_srcs}
  ${read_visualsonics_digital_rf_SOURCE_DIR}/tests/cxx_test.cpp )

#add_executable( cxx_test ${cxx_test_srcs} )
#target_link_libraries( convert_visualsonics_test parse_vs_rdi )



if(USE_VTK)
  find_package(VTK REQUIRED)
  if(NOT VTK_USE_RENDERING)
    message(FATAL_ERROR "${PROJECT_NAME} requires VTK_USE_RENDERING.")
  endif(NOT VTK_USE_RENDERING)

include_directories( ${read_visualsonics_digital_rf_SOURCE_DIR}/source
  ${VTK_INCLUDE_DIRS}
  )
link_directories( ${VTK_LIBRARY_DIRS} )

  set( vtk_test_srcs ${base_test_srcs}
    ${read_visualsonics_digital_rf_SOURCE_DIR}/source/vtk/vtkVisualSonicsReader.cpp
    ${read_visualsonics_digital_rf_SOURCE_DIR}/source/vtk/ImageViewer.cpp
    ${read_visualsonics_digital_rf_SOURCE_DIR}/tests/vtk_test.cpp
    )

  add_executable( vtk_test ${vtk_test_srcs} )
  target_link_libraries( vtk_test vtkRendering vtkVolumeRendering boost_filesystem)
endif(USE_VTK)

add_subdirectory(common)
add_subdirectory(xml)
