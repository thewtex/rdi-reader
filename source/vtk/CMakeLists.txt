set(spirit_parse_source_dir ${read_visualsonics_digital_rf_SOURCE_DIR}/source/common/spirit_parse)


include_directories( ${read_visualsonics_digital_rf_SOURCE_DIR}/source )


set(base_srcs  ${spirit_parse_source_dir}/rdiParser.cpp
                                    ${spirit_parse_source_dir}/BoostSpiritInputFile.cpp
                                    ${read_visualsonics_digital_rf_SOURCE_DIR}/source/common/interpolation/Interpolation.cpp
                                    ${read_visualsonics_digital_rf_SOURCE_DIR}/source/common/interpolation/NearestNeighbor.cpp
                                    ${read_visualsonics_digital_rf_SOURCE_DIR}/source/common/interpolation/Bilinear.cpp
                                    ${read_visualsonics_digital_rf_SOURCE_DIR}/source/cxx/ImageReader.cpp
                                    ${read_visualsonics_digital_rf_SOURCE_DIR}/source/common/ImageReaderBase.cpp
                                    ${read_visualsonics_digital_rf_SOURCE_DIR}/source/common/MetadataReaderBase.cpp
                                    ${read_visualsonics_digital_rf_SOURCE_DIR}/source/common/VisualSonicsTransform.cpp
				    )



find_package(VTK REQUIRED)
if(NOT VTK_USE_RENDERING)
  message(FATAL_ERROR "${PROJECT_NAME} requires VTK_USE_RENDERING.")
endif(NOT VTK_USE_RENDERING)
include_directories( ${VTK_INCLUDE_DIRS} )
link_directories( ${VTK_LIBRARY_DIRS} )



set( vs-vtk-viewer_srcs ${base_srcs}
  ${read_visualsonics_digital_rf_SOURCE_DIR}/source/vtk/vtkVisualSonicsReader.cpp
  ${read_visualsonics_digital_rf_SOURCE_DIR}/source/vtk/ImageViewer.cpp
  ${read_visualsonics_digital_rf_SOURCE_DIR}/source/vtk/vs-vtk-viewer.cpp
  )

add_executable( vs-vtk-viewer ${vs-vtk-viewer_srcs} )
target_link_libraries( vs-vtk-viewer vtkRendering boost_filesystem )

