SET(spirit_parse_source_dir ${CONVERT_VISUALSONICS_2_MAT_SOURCE_DIR}/spirit_parse)
SET(my_matlab_source_dir ${CONVERT_VISUALSONICS_2_MAT_SOURCE_DIR}/matlab)

INCLUDE_DIRECTORIES(${spirit_parse_source_dir} ${my_matlab_source_dir})


SET(SPIRIT_PARSE_SRCS ${spirit_parse_source_dir}/BoostSpiritInputFile.cpp
                      ${spirit_parse_source_dir}/rdiParser.cpp
                      ${spirit_parse_source_dir}/rdiParserData.cpp
                           )

SET(CONVERT_VISUALSONICS_2_MAT_SRCS ${SPIRIT_PARSE_SRCS}
                                    ${CONVERT_VISUALSONICS_2_MAT_SOURCE_DIR}/matlab/vs2m_gateway.cpp
                                    ${CONVERT_VISUALSONICS_2_MAT_SOURCE_DIR}/matlab/vs2m_gateway_helper_functions.cpp
                                    ${CONVERT_VISUALSONICS_2_MAT_SOURCE_DIR}/matlab/vs2m_gateway_Input.cpp
                                    ${CONVERT_VISUALSONICS_2_MAT_SOURCE_DIR}/matlab/vs2m_gateway_Output.cpp
                                    ${CONVERT_VISUALSONICS_2_MAT_SOURCE_DIR}/matlab/vs2m_gateway_OutputFile.cpp
                                    )


IF(NOT WIN32)
	SET(CONVERT_VISUALSONICS_2_MAT_SRCS ${CONVERT_VISUALSONICS_2_MAT_SRCS}
										${MEXVERSION_DIR}/mexversion.c
										)
ENDIF(NOT WIN32)

ADD_LIBRARY(convert_visualsonics_2_mat MODULE ${CONVERT_VISUALSONICS_2_MAT_SRCS})

# set appropriate suffix and prefix as dictated by MatLab
IF(UNIX)
  SET_TARGET_PROPERTIES(convert_visualsonics_2_mat PROPERTIES
      PREFIX ""
      SUFFIX ".mexglx"
      )
ENDIF(UNIX)
IF(WIN32)
  SET_TARGET_PROPERTIES(convert_visualsonics_2_mat PROPERTIES
	PREFIX ""
#	SUFFIX ".mex.dll" #older versions, e.g. R14
	SUFFIX ".mexw32"
	)
ENDIF(WIN32)

TARGET_LINK_LIBRARIES(convert_visualsonics_2_mat boost_filesystem)

INSTALL(TARGETS convert_visualsonics_2_mat
            LIBRARY DESTINATION . COMPONENT runtime
            )
