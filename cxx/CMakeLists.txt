set(spirit_parse_source_dir ${read_visualsonics_digital_rf_SOURCE_DIR}/common/spirit_parse)


include_directories( ${read_visualsonics_digital_rf_SOURCE_DIR} 
  ${spirit_parse_source_dir} 
  ${read_visualsonics_digital_rf_SOURCE_DIR}/common
  )


set(convert_visualsonics_test_srcs  ${spirit_parse_source_dir}/rdiParser.cpp
                                    ${spirit_parse_source_dir}/BoostSpiritInputFile.cpp
                                    ${read_visualsonics_digital_rf_SOURCE_DIR}/cxx/ReadImage.cpp 
                                    ${read_visualsonics_digital_rf_SOURCE_DIR}/common/ReadImageBase.cpp 
                                    ${read_visualsonics_digital_rf_SOURCE_DIR}/common/ReadMetadataBase.cpp 
                                    )



set(convert_visualsonics_bin_srcs  ${convert_visualsonics_test_srcs} 
                                    ${read_visualsonics_digital_rf_SOURCE_DIR}/cxx/convert_visualsonics_test.cpp
				    )

set(convert_visualsonics_lib_srcs  ${convert_visualsonics_test_srcs} 
				    ${read_visualsonics_digital_rf_SOURCE_DIR}/cxx/parse_vs_rdi.cpp
				    )
				  

add_library( parse_vs_rdi SHARED ${convert_visualsonics_lib_srcs} )

add_executable( convert_visualsonics_test ${convert_visualsonics_bin_srcs} )
target_link_libraries( convert_visualsonics_test parse_vs_rdi )


