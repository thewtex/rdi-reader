SET(SPIRIT_PARSE_SOURCE_DIR ${CONVERT_VISUALSONICS_2_MAT_SOURCE_DIR}/spirit_parse)

INCLUDE_DIRECTORIES(${SPIRIT_PARSE_SOURCE_DIR}
		        ${CONVERT_VISUALSONICS_2_MAT_SOURCE_DIR}/cxx  
			)
			
link_directories(  /usr/local/lib  )

SET(convert_visualsonics_srcs  ${SPIRIT_PARSE_SRCS} 
                                    ${CONVERT_VISUALSONICS_2_MAT_SOURCE_DIR}/spirit_parse/rdiParser.cpp
                                    ${CONVERT_VISUALSONICS_2_MAT_SOURCE_DIR}/spirit_parse/rdiParserData.cpp
                                    ${CONVERT_VISUALSONICS_2_MAT_SOURCE_DIR}/spirit_parse/BoostSpiritInputFile.cpp
                                    )

SET(convert_visualsonics_bin_srcs  ${convert_visualsonics_srcs} 
                                    ${CONVERT_VISUALSONICS_2_MAT_SOURCE_DIR}/cxx/convert_visualsonics.cpp
				    )

SET(convert_visualsonics_lib_srcs  ${convert_visualsonics_srcs} 
				    ${CONVERT_VISUALSONICS_2_MAT_SOURCE_DIR}/cxx/parse_vs_rdi.cpp
				    )

add_library( parse_vs_rdi SHARED ${convert_visualsonics_lib_srcs} )

ADD_EXECUTABLE( convert_visualsonics_test ${convert_visualsonics_bin_srcs} )
target_link_libraries( convert_visualsonics_test parse_vs_rdi )


