find_package(Xsd REQUIRED)

set(XSD_ARGS "cxx-tree" "--generate-ostream" "--generate-serialization")

set(MODEL_RDI
  ${read_visualsonics_digital_rf_SOURCE_DIR}/data_for_tests/machine/visualsonics/vevo770/rmv710b/pat143_seg2.rdi
  CACHE PATH
  "The *rdi that is used as a model to define the rdi format/content.")
mark_as_advanced(MODEL_RDI)

add_custom_target(format_evaluator ALL
  ${CMAKE_CURRENT_SOURCE_DIR}/format_evaluator.py
  ${MODEL_RDI}
  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/format_evaluator.py
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  COMMENT "Generating rdi format schema from ${MODEL_RDI}."
  )

add_custom_command(
  TARGET format_evaluator
  POST_BUILD
  COMMAND ${XSD_EXECUTABLE} ${XSD_ARGS} ${CMAKE_CURRENT_SOURCE_DIR}/rdi.xsd
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  COMMENT "Translating rdi.xsd into rdi.cxx/rdi.hxx."
  )

