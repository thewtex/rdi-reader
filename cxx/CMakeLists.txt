SET(SPIRIT_PARSE_SOURCE_DIR ${CONVERT_VISUALSONICS_2_MAT_SOURCE_DIR}/spirit_parse)

INCLUDE_DIRECTORIES(${SPIRIT_PARSE_SOURCE_DIR} )

SET(convert_visualsonics_srcs  ${SPIRIT_PARSE_SRCS} 
                                    ${CONVERT_VISUALSONICS_2_MAT_SOURCE_DIR}/cxx/convert_visualsonics.cpp
                                    ${CONVERT_VISUALSONICS_2_MAT_SOURCE_DIR}/spirit_parse/rdiParser.cpp
                                    ${CONVERT_VISUALSONICS_2_MAT_SOURCE_DIR}/spirit_parse/rdiParserData.cpp
                                    ${CONVERT_VISUALSONICS_2_MAT_SOURCE_DIR}/spirit_parse/BoostSpiritInputFile.cpp
                                    )

ADD_EXECUTABLE( convert_visualsonics ${convert_visualsonics_srcs} )
