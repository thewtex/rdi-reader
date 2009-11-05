set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/modules )
find_package( XercesC REQUIRED )

add_subdirectory( common )
add_subdirectory( modules )
add_subdirectory( xml )

if( USE_VTK )
  add_subdirectory(vtk)
endif()

if( USE_ITK )
  add_subdirectory(itk)
endif()
