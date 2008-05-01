set(spirit_parse_source_dir ${read_visualsonics_digital_rf_SOURCE_DIR}/source/common/spirit_parse)


include_directories( ${read_visualsonics_digital_rf_SOURCE_DIR}/source
  ${spirit_parse_source_dir}
  ${read_visualsonics_digital_rf_SOURCE_DIR}/source/common
  )


set(parse_vs_rdi_lib_srcs  ${spirit_parse_source_dir}/rdiParser.cpp
                                    ${spirit_parse_source_dir}/BoostSpiritInputFile.cpp
                                    ${read_visualsonics_digital_rf_SOURCE_DIR}/source/cxx/ImageReader.cpp
                                    ${read_visualsonics_digital_rf_SOURCE_DIR}/source/common/ImageReaderBase.cpp
                                    ${read_visualsonics_digital_rf_SOURCE_DIR}/source/common/MetadataReaderBase.cpp
				    ${read_visualsonics_digital_rf_SOURCE_DIR}/source/cxx/parse_vs_rdi.cpp
				    )


add_library( parse_vs_rdi SHARED ${parse_vs_rdi_lib_srcs} )

