set(SPIRIT_PARSE_SOURCE_DIR ${CONVERT_VISUALSONICS_2_MAT_SOURCE_DIR}/spirit_parse)

include_directories(${SPIRIT_PARSE_SOURCE_DIR}
		        ${CONVERT_VISUALSONICS_2_MAT_SOURCE_DIR}/cxx  
			)
			
link_directories(  /usr/local/lib  )

set(convert_visualsonics_srcs  ${SPIRIT_PARSE_SRCS} 
                                    ${CONVERT_VISUALSONICS_2_MAT_SOURCE_DIR}/spirit_parse/rdiParser.cpp
                                    ${CONVERT_VISUALSONICS_2_MAT_SOURCE_DIR}/spirit_parse/rdiParserData.cpp
                                    ${CONVERT_VISUALSONICS_2_MAT_SOURCE_DIR}/spirit_parse/BoostSpiritInputFile.cpp
                                    )

set(convert_visualsonics_bin_srcs  ${convert_visualsonics_srcs} 
                                    ${CONVERT_VISUALSONICS_2_MAT_SOURCE_DIR}/cxx/convert_visualsonics.cpp
				    )

set(convert_visualsonics_lib_srcs  ${convert_visualsonics_srcs} 
				    ${CONVERT_VISUALSONICS_2_MAT_SOURCE_DIR}/cxx/parse_vs_rdi.cpp
				    ${CONVERT_VISUALSONICS_2_MAT_SOURCE_DIR}/spirit_parse/InputBase.cpp 
				    ${CONVERT_VISUALSONICS_2_MAT_SOURCE_DIR}/spirit_parse/OutputBase.cpp 
				    )

add_library( parse_vs_rdi SHARED ${convert_visualsonics_lib_srcs} )

add_executable( convert_visualsonics_test ${convert_visualsonics_bin_srcs} )
target_link_libraries( convert_visualsonics_test parse_vs_rdi )


