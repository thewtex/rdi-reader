set(spirit_parse_source_dir ${read_visualsonics_digital_rf_SOURCE_DIR}/source/common/spirit_parse)

set(base_srcs  ${spirit_parse_source_dir}/rdiParser.cpp
                                    ${spirit_parse_source_dir}/BoostSpiritInputFile.cpp
                                    ${read_visualsonics_digital_rf_SOURCE_DIR}/source/common/interpolation/Interpolation.cpp
                                    ${read_visualsonics_digital_rf_SOURCE_DIR}/source/common/interpolation/NearestNeighbor.cpp
                                    ${read_visualsonics_digital_rf_SOURCE_DIR}/source/common/interpolation/Bilinear.cpp
                                    ${read_visualsonics_digital_rf_SOURCE_DIR}/source/common/ImageReader.cpp
                                    ${read_visualsonics_digital_rf_SOURCE_DIR}/source/common/ImageReaderBase.cpp
                                    ${read_visualsonics_digital_rf_SOURCE_DIR}/source/common/MetadataReaderBase.cpp
                                    ${read_visualsonics_digital_rf_SOURCE_DIR}/source/common/VisualSonicsTransform.cpp
				    )


# include vtk
find_package(VTK REQUIRED)
if(NOT VTK_USE_RENDERING)
  message(FATAL_ERROR "${PROJECT_NAME} requires VTK_USE_RENDERING.")
endif(NOT VTK_USE_RENDERING)

include_directories( ${read_visualsonics_digital_rf_SOURCE_DIR}/source
  ${VTK_INCLUDE_DIRS}
  )
link_directories( ${VTK_LIBRARY_DIRS} )


# vs-vtk-viewer, a simple cli viewer for examining Visual Sonics
# .rdi/.rdb files
set( vs-vtk-viewer_srcs ${base_srcs}
  ${read_visualsonics_digital_rf_SOURCE_DIR}/source/vtk/vtkVisualSonicsReader.cpp
  ${read_visualsonics_digital_rf_SOURCE_DIR}/source/vtk/vtkUnsafeStructuredGridToImage.cpp
  ${read_visualsonics_digital_rf_SOURCE_DIR}/source/vtk/ImageViewer.cpp
  ${read_visualsonics_digital_rf_SOURCE_DIR}/source/vtk/vs-vtk-viewer.cpp
  )

add_executable( vs-vtk-viewer ${vs-vtk-viewer_srcs} )
target_link_libraries( vs-vtk-viewer vtkRendering vtkVolumeRendering boost_filesystem)



# vs-convert, a simple cli app to convert Visual Sonics .rdi/.rdb files to
# other image formats
set( vs-convert_srcs ${base_srcs}
  ${read_visualsonics_digital_rf_SOURCE_DIR}/source/vtk/vtkVisualSonicsReader.cpp
  ${read_visualsonics_digital_rf_SOURCE_DIR}/source/vtk/vs-convert.cpp
  )

add_executable( vs-convert ${vs-convert_srcs} )
target_link_libraries( vs-convert boost_filesystem vtkCommon vtkIO)


install( TARGETS vs-vtk-viewer vs-convert
  DESTINATION bin
  )


