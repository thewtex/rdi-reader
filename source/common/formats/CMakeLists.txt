find_package(Xsd REQUIRED)

set(XSD_ARGS "cxx-tree" "--generate-ostream" "--generate-serialization")

#add_custom_command(
  #OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/rdi.xsd
add_custom_target(format_evaluator ALL
  ${CMAKE_CURRENT_SOURCE_DIR}/format_evaluator.py ${read_visualsonics_digital_rf_SOURCE_DIR}/test_data/machine/visualsonics/vevo770/rmv710b/pat143_seg2.rdi
  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/format_evaluator.py
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  )

add_custom_command(
  #OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/rdi.cxx
  TARGET format_evaluator
  COMMAND ${XSD_EXECUTABLE} ${XSD_ARGS} ${CMAKE_CURRENT_SOURCE_DIR}/rdi.xsd
  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/rdi.xsd
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  )

#include_directories( ${CMAKE_CURRENT_BINARY_DIR} )
#add_executable( hello_driver hello.cxx
  #driver.cxx )
#target_link_libraries( hello_driver ${XERCESC_LIBRARIES} )


