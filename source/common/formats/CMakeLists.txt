cmake_minimum_required(VERSION 2.6)
project( hello)
set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${hello_SOURCE_DIR})
find_package( Xsd REQUIRED )
find_package( XercesC REQUIRED )

set(XSD_ARGS "cxx-tree" "--generate-ostream" "--generate-serialization")

add_custom_command(
  #TARGET hello_driver PRE_BUILD
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/hello.cxx
  COMMAND ${XSD_EXECUTABLE} ${XSD_ARGS} ${CMAKE_CURRENT_SOURCE_DIR}/hello.xsd
  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/hello.xsd
  )

include_directories( ${CMAKE_CURRENT_BINARY_DIR} )
add_executable( hello_driver hello.cxx
  driver.cxx )
target_link_libraries( hello_driver ${XERCESC_LIBRARIES} )


