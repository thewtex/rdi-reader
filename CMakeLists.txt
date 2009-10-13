cmake_minimum_required(VERSION 2.8)
if(COMMAND cmake_policy)
  cmake_policy(VERSION 2.8)
endif(COMMAND cmake_policy)


project(read_visualsonics_digital_rf)

set(ANALYZE_RDI_FORMAT OFF CACHE BOOL
  "Analyze rdi format from example sources to generate parsing code.")

set(BUILD_RDI2HTML ON CACHE BOOL
  "Build program that converts rdi files to a human readable html file.")

include(CTest)
if(BUILD_TESTING)
  if(NOT EXISTS ${read_visualsonics_digital_rf_SOURCE_DIR}/data_for_tests/.git)
    message(SEND_ERROR "In order to run tests, the data_for_tests must be
    downloaded.  Try 'git submodule init && git submodule update' in the source
    directory.  If you do not want to run tests, turn BUILD_TESTING OFF.")
  endif()
  enable_testing()
  add_subdirectory(tests)
endif(BUILD_TESTING)


#INCLUDE(CMakePrintSystemInformation)
IF(WIN32)
SET(CMAKE_C_OUTPUT_EXTENSION ".obj")
SET(CMAKE_CXX_OUTPUT_EXTENSION ".obj")
SET(CMAKE_CXX_COMPILE_OBJECT
		"mkdir -p <OBJECT_DIR>; <CMAKE_CXX_COMPILER>  -outdir <OBJECT_DIR> <FLAGS> -c <SOURCE> "
		)
SET(CMAKE_C_COMPILE_OBJECT
		"mkdir -p <OBJECT_DIR>; <CMAKE_C_COMPILER> -outdir <OBJECT_DIR> <FLAGS> -c <SOURCE> "
		)
SET(CMAKE_CXX_CREATE_SHARED_MODULE
	"<CMAKE_CXX_COMPILER> <LANGUAGE_COMPILE_FLAGS> <LINK_FLAGS> -output <TARGET> <OBJECTS> <LINK_LIBRARIES>"
	)
SET(CMAKE_C_CREATE_SHARED_MODULE
	"<CMAKE_C_COMPILER> <LANGUAGE_COMPILE_FLAGS> <LINK_FLAGS> -output <TARGET> <OBJECTS> <LINK_LIBRARIES>"
	)

INCLUDE(CMakePrintSystemInformation)
ENDIF(WIN32)

############## Compiling flags ##########

IF(CMAKE_COMPILER_IS_GNUCXX)

  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall" )
  SET(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   -Wall" )

  SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -ggdb3" )
  SET(CMAKE_C_FLAGS_DEBUG   "${CMAKE_C_FLAGS_DEBUG}   -ggdb3" )

  SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2 -DNDEBUG -ffast-math -fvisibility=hidden")
  SET(CMAKE_C_FLAGS_RELEASE   "${CMAKE_C_FLAGS_RELEASE}   -O2 -DNDEBUG -ffast-math -fvisibility=hidden" )


  SET(CMAKE_SHARED_LINKER_FLAGS_DEBUG "${CMAKE_SHARED_LINKER_FLAGS_DEBUG} -g" )
  SET(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} -O")

ENDIF(CMAKE_COMPILER_IS_GNUCXX)



SET_DIRECTORY_PROPERTIES(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "*.mex*")
SET_DIRECTORY_PROPERTIES(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "*.obj")

#MARK_AS_ADVANCED()

########### options #########################################
set( USE_VTK ON CACHE BOOL "Build capabilities related to the Visualization
Toolkit www.vtk.org" )
# where all the executables go
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY
  ${read_visualsonics_digital_rf_BINARY_DIR}/bin )

add_subdirectory(source)

