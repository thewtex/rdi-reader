find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

set(RDI_TESTING_FILEPATH
  ${RDI_READER_SOURCE_DIR}/tests/data/machine/visualsonics/vevo770/rmv710b/uniform_phantom_0dB_2.rdi
  )

if(ANALYZE_RDI_FORMAT)
  add_test(
    format-evaluator
    ${RDI_READER_SOURCE_DIR}/tests/python_unittests.py
    FormatEvaluatorSuite
    )
endif(ANALYZE_RDI_FORMAT)


set(spirit_parse_source_dir ${RDI_READER_SOURCE_DIR}/source/common/spirit_parse)

set(base_test_srcs  ${spirit_parse_source_dir}/rdiParser.cpp
                                    ${spirit_parse_source_dir}/BoostSpiritInputFile.cpp
                                    ${RDI_READER_SOURCE_DIR}/source/common/interpolation/Interpolation.cpp
                                    ${RDI_READER_SOURCE_DIR}/source/common/interpolation/NearestNeighbor.cpp
                                    ${RDI_READER_SOURCE_DIR}/source/common/interpolation/Bilinear.cpp
                                    ${RDI_READER_SOURCE_DIR}/source/common/ImageReader.cpp
                                    ${RDI_READER_SOURCE_DIR}/source/common/ImageReaderBase.cpp
                                    ${RDI_READER_SOURCE_DIR}/source/common/MetadataReaderBase.cpp
                                    ${RDI_READER_SOURCE_DIR}/source/common/VisualSonicsTransform.cpp
				    )

if(USE_VTK)
  find_package(VTK REQUIRED)
  if(NOT VTK_USE_RENDERING)
    message(FATAL_ERROR "${PROJECT_NAME} requires VTK_USE_RENDERING.")
  endif(NOT VTK_USE_RENDERING)

include_directories( ${RDI_READER_SOURCE_DIR}/source
  ${VTK_INCLUDE_DIRS}
  )
link_directories( ${VTK_LIBRARY_DIRS} )

  set( vtk_test_srcs ${base_test_srcs}
    ${RDI_READER_SOURCE_DIR}/source/vtk/vtkVisualSonicsReader.cpp
    ${RDI_READER_SOURCE_DIR}/source/vtk/ImageViewer.cpp
    ${RDI_READER_SOURCE_DIR}/tests/vtk_test.cpp
    )

  add_executable( vtk_test ${vtk_test_srcs} )
  target_link_libraries( vtk_test vtkRendering vtkVolumeRendering boost_filesystem)
endif(USE_VTK)


add_subdirectory( common )
add_subdirectory( xml )

if( USE_ITK )
  add_subdirectory( itk )
endif()
