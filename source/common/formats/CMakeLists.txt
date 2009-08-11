find_package(Xsd REQUIRED)

set(XSD_ARGS "cxx-tree" "--generate-ostream" "--generate-serialization")

set(MODEL_RDI
  ${read_visualsonics_digital_rf_SOURCE_DIR}/data_for_tests/machine/visualsonics/vevo770/rmv710b/pat143_seg2.rdi
  CACHE PATH
  "The *rdi that is used as a model to define the rdi format/content.")
mark_as_advanced(MODEL_RDI)

#add_custom_command(
  #OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/rdi.xsd
add_custom_target(format_evaluator ALL
  ${CMAKE_CURRENT_SOURCE_DIR}/format_evaluator.py
  ${MODEL_RDI}
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


