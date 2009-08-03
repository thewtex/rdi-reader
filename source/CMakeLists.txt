set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/modules)
find_package(XercesC REQUIRED)

add_subdirectory( common )

if(USE_VTK)
  add_subdirectory( vtk )
endif(USE_VTK)

add_subdirectory( itk )
