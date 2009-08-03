#set(spirit_parse_source_dir ${read_visualsonics_digital_rf_SOURCE_DIR}/source/common/spirit_parse)


include_directories( ${read_visualsonics_digital_rf_SOURCE_DIR}/source
  #${spirit_parse_source_dir}
  ${read_visualsonics_digital_rf_SOURCE_DIR}/source/common
  )


#set(parse_vs_rdi_lib_srcs  ${spirit_parse_source_dir}/rdiParser.cpp
                                    #${spirit_parse_source_dir}/BoostSpiritInputFile.cpp
                                    #${read_visualsonics_digital_rf_SOURCE_DIR}/source/common/ImageReader.cpp
                                    #${read_visualsonics_digital_rf_SOURCE_DIR}/source/common/ImageReaderBase.cpp
                                    #${read_visualsonics_digital_rf_SOURCE_DIR}/source/common/MetadataReaderBase.cpp
				    #${read_visualsonics_digital_rf_SOURCE_DIR}/source/common/parse_vs_rdi.cpp
                                    #${read_visualsonics_digital_rf_SOURCE_DIR}/source/common/VisualSonicsTransform.cpp
				    #${read_visualsonics_digital_rf_SOURCE_DIR}/source/common/interpolation/Interpolation.cpp
				    #${read_visualsonics_digital_rf_SOURCE_DIR}/source/common/interpolation/NearestNeighbor.cpp
				    #${read_visualsonics_digital_rf_SOURCE_DIR}/source/common/interpolation/Bilinear.cpp
				    #)


#add_library( parse_vs_rdi SHARED ${parse_vs_rdi_lib_srcs} )
add_executable(rdi2xml
  rdi2xml.cpp
  rdiReader.cpp
  formats/rdi.cxx
  )
target_link_libraries(rdi2xml ${XERCESC_LIBRARIES})

set(ANALYZE_RDI_FORMAT OFF CACHE BOOL
  "Analyze rdi format from example sources to generate parsing code.")
if(ANALYZE_RDI_FORMAT)
  add_subdirectory(formats)
endif(ANALYZE_RDI_FORMAT)
