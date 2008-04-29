cmake_minimum_required(VERSION 2.6)
cmake_policy(SET CMP0003 NEW)

project(read_visualsonics_digital_rf)


###########  Install Prefix ############
SET(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/.. 
      CACHE PATH "in the private directory of the VisualSonics2mat Matlab class, set in CMakeLists.txt" 
      FORCE
      )

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


########### output directories #################
if(NOT LIBRARY_OUTPUT_PATH)
	set(LIBRARY_OUTPUT_PATH ${read_visualsonics_digital_rf_BINARY_DIR}/lib
		CACHE INTERNAL "single output directory for building all libraries.")
endif(NOT LIBRARY_OUTPUT_PATH)
if(NOT EXECUTABLE_OUTPUT_PATH)
	set(EXECUTABLE_OUTPUT_PATH ${read_visualsonics_digital_rf_BINARY_DIR}/lib
		CACHE INTERNAL "single output directory for building all executables.")
endif(NOT EXECUTABLE_OUTPUT_PATH)


########### set up addition CMakeLists.txt locations #########
#ADD_SUBDIRECTORY(matlab)
add_subdirectory(source)

########### testing ############################

include(CTest)
if(BUILD_TESTING)
	enable_testing()
    add_subdirectory(tests)
    if(NOT TEST_OUTPUT_PATH)
		set(TEST_OUTPUT_PATH ${read_visualsonics_digital_rf_BINARY_DIR}/bin
			CACHE INTERNAL "output directory for testing executables")
	endif(NOT TEST_OUTPUT_PATH)
endif(BUILD_TESTING)

