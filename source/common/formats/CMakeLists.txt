find_package(Xsd REQUIRED)
include_directories( ${XSD_INCLUDE_DIR} )

set(XSD_ARGS "cxx-tree" "--generate-ostream" "--generate-serialization")

set(MODEL_RDI
  ${CMAKE_CURRENT_SOURCE_DIR}/model_rdi/710B/1090-400_seg1.rdi
  CACHE PATH
  "The *rdi that is used as a model to define the rdi format/content.")
mark_as_advanced(MODEL_RDI)

add_custom_target(format-evaluator ALL
  ${CMAKE_CURRENT_SOURCE_DIR}/format-evaluator.py
  ${MODEL_RDI}
  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/format-evaluator.py
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  COMMENT "Generating rdi format schema from ${MODEL_RDI}."
  )

add_custom_command(
  TARGET format-evaluator
  POST_BUILD
  COMMAND ${XSD_EXECUTABLE} ${XSD_ARGS} ${CMAKE_CURRENT_SOURCE_DIR}/rdi.xsd
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  COMMENT "Translating rdi.xsd into rdi.cxx/rdi.hxx."
  )

add_dependencies(rdiReader format-evaluator)
