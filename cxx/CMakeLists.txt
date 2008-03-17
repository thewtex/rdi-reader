set(SPIRIT_PARSE_SOURCE_DIR ${read_visualsonics_digital_rf_SOURCE_DIR}/spirit_parse)

include_directories(${SPIRIT_PARSE_SOURCE_DIR}
		        ${read_visualsonics_digital_rf_SOURCE_DIR}/cxx  
			)
			
set(convert_visualsonics_srcs  ${SPIRIT_PARSE_SRCS} 
                                    ${read_visualsonics_digital_rf_SOURCE_DIR}/spirit_parse/rdiParser.cpp
                                    ${read_visualsonics_digital_rf_SOURCE_DIR}/spirit_parse/rdiParserData.cpp
                                    ${read_visualsonics_digital_rf_SOURCE_DIR}/spirit_parse/BoostSpiritInputFile.cpp
                                    ${read_visualsonics_digital_rf_SOURCE_DIR}/cxx/ExtractImage.cpp 
                                    ${read_visualsonics_digital_rf_SOURCE_DIR}/spirit_parse/ExtractMetadataBase.cpp 
                                    )

set(convert_visualsonics_bin_srcs  ${convert_visualsonics_srcs} 
                                    ${read_visualsonics_digital_rf_SOURCE_DIR}/cxx/convert_visualsonics.cpp
				    )

set(convert_visualsonics_lib_srcs  ${convert_visualsonics_srcs} 
				    ${read_visualsonics_digital_rf_SOURCE_DIR}/cxx/parse_vs_rdi.cpp
				    )

add_library( parse_vs_rdi SHARED ${convert_visualsonics_lib_srcs} )

add_executable( convert_visualsonics_test ${convert_visualsonics_bin_srcs} )
target_link_libraries( convert_visualsonics_test parse_vs_rdi )


