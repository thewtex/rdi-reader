set(spirit_parse_source_dir ${RDI_READER_SOURCE_DIR}/source/common/spirit_parse)

set(base_srcs  ${spirit_parse_source_dir}/rdiParser.cpp
                                    ${spirit_parse_source_dir}/BoostSpiritInputFile.cpp
                                    ${RDI_READER_SOURCE_DIR}/source/common/interpolation/Interpolation.cpp
                                    ${RDI_READER_SOURCE_DIR}/source/common/interpolation/NearestNeighbor.cpp
                                    ${RDI_READER_SOURCE_DIR}/source/common/interpolation/Bilinear.cpp
                                    ${RDI_READER_SOURCE_DIR}/source/common/ImageReader.cpp
                                    ${RDI_READER_SOURCE_DIR}/source/common/ImageReaderBase.cpp
                                    ${RDI_READER_SOURCE_DIR}/source/common/MetadataReaderBase.cpp
                                    ${RDI_READER_SOURCE_DIR}/source/common/VisualSonicsTransform.cpp
				    )


# include vtk
find_package(VTK REQUIRED)
if(NOT VTK_USE_RENDERING)
  message(FATAL_ERROR "${PROJECT_NAME} requires VTK_USE_RENDERING.")
endif(NOT VTK_USE_RENDERING)

include_directories( ${RDI_READER_SOURCE_DIR}/source
  ${VTK_INCLUDE_DIRS}
  )
link_directories( ${VTK_LIBRARY_DIRS} )


# vs-vtk-viewer, a simple cli viewer for examining Visual Sonics
# .rdi/.rdb files
set( vs-vtk-viewer_srcs ${base_srcs}
  ${RDI_READER_SOURCE_DIR}/source/vtk/vtkVisualSonicsReader.cpp
  ${RDI_READER_SOURCE_DIR}/source/vtk/ImageViewer.cpp
  ${RDI_READER_SOURCE_DIR}/source/vtk/vs-vtk-viewer.cpp
  )

add_executable( vs-vtk-viewer ${vs-vtk-viewer_srcs} )
target_link_libraries( vs-vtk-viewer vtkRendering vtkVolumeRendering boost_filesystem)



# vs-vtk-convert, a simple cli app to convert Visual Sonics .rdi/.rdb files to
# other image formats
set( vs-vtk-convert_srcs ${base_srcs}
  ${RDI_READER_SOURCE_DIR}/source/vtk/vtkVisualSonicsReader.cpp
  ${RDI_READER_SOURCE_DIR}/source/vtk/vtkUnsafeStructuredGridToImage.cpp
  ${RDI_READER_SOURCE_DIR}/source/vtk/vs-vtk-convert.cpp
  )

add_executable( vs-vtk-convert ${vs-vtk-convert_srcs} )
target_link_libraries( vs-vtk-convert
  boost_filesystem
  vtkCommon
  vtkGraphics #StructuredGridClip
  vtkImaging
  vtkIO)


install( TARGETS vs-vtk-viewer vs-vtk-convert
  DESTINATION bin
  )


