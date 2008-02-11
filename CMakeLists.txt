PROJECT(CONVERT_VISUALSONICS_2_MAT)



############ Matlab directories #########
#INCLUDE(FindMatlab) # bad bad gives invalid generator error

IF(WIN32)
  SET(MATLAB_DIR "/cygdrive/c/MATLAB/R2007b" CACHE FILEPATH "Path to the matlab directory")
ENDIF(WIN32)

IF(UNIX)
# SET(MATLAB_DIR /usr/local/matlabR2006a CACHE FILEPATH "Path to the matlab directory")
SET(MATLAB_DIR /home/matt/apps/matlab CACHE FILEPATH "Path to the matlab directory")
ENDIF(UNIX)

SET(MATLAB_INCLUDE_DIR ${MATLAB_DIR}/extern/include)
SET(MEXVERSION_DIR ${MATLAB_DIR}/extern/src)

INCLUDE_DIRECTORIES(${MATLAB_INCLUDE_DIR} ${MEXVERSION_DIR})
LINK_DIRECTORIES(${MEXVERSION_DIR} ${MATLAB_LIBRARIES}) 




####### Set up compiler ########
IF(UNIX)
#cannot be changed after the cmake or ccmake run-- must change it and start over
# see how to install another version of g++ here:
# http://www.tellurian.com.au/whitepapers/multiplegcc.php
# should use the same version of gcc that Matlab was compiled with, see Mathworks' website
#
# SET(CMAKE_CXX_COMPILER
#   /usr/local/gcc/4.1.1/bin/g++  # for Matlab version 7.4
# #   /usr/local/gcc/3.4.4/bin/g++ # for Matlab version 7.2
#   CACHE FILEPATH "Should use the compiler that was used to compile Matlab (see Mathworks website)"
#   FORCE
# )
# SET(CMAKE_C_COMPILER
#   /usr/local/gcc/4.1.1/bin/gcc # for Matlab version 7.4
# #   /usr/local/gcc/3.4.4/bin/g++ # for Matlab version 7.2
#   CACHE FILEPATH "Should use the compiler that was used to compile Matlab (see Mathworks website)"
#   FORCE
# )
ENDIF(UNIX)

IF(WIN32)
# IF(MINGW)
# note: "MinGW Makefiles" if using MinGW
# see http://www.cmake.org/Wiki/CMake_Generator_Specific_Information
# you must also download a special version of make.exe from 
# http://unxutils.sourceforge.net/
SET(CMAKE_MAKE_PROGRAM
	C:/vs/unxutils/usr/local/wbin/make.exe
	CACHE FILEPATH
	FORCE
	"See notes in root CMakeLists.txt"
	)
	
##ENDIF(MINGW)

# the MinGW compiler + extras
# http://nuwen.net/mingw.html
# then set up mex with gnumex
# http://gnumex.sourceforge.net/
SET(CMAKE_CXX_COMPILER
	"${MATLAB_DIR}/bin/mex.bat"
	CACHE FILEPATH 
	"Use Matlab's mex.bat directly, set up compiler and options there"
	FORCE
	)
SET(CMAKE_C_COMPILER
	"${MATLAB_DIR}/bin/mex.bat"
	CACHE FILEPATH 
	"Use Matlab's mex.bat directly, set up compiler and options there"
	FORCE
	)
ENDIF(WIN32)



###########  Install Prefix ############
SET(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/.. 
      CACHE PATH "in the private directory of the VisualSonics2mat Matlab class, set in CMakeLists.txt" 
      FORCE
      )

SET(CMAKE_VERBOSE_MAKEFILE ON CACHE BOOL "Whether or not to spit out all
compiler commands during build" )
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
# hints for values by issueing 'mex -v' in Matlab

IF(UNIX) # all unix-like O/S's including Apple OS X and MINGW
  SET(CMAKE_COMPILER_IS_GNUCXX 1)
  SET(CMAKE_COMPILER_IS_GNUCC 1)
  # 32bit Linux
  SET(MATLAB_LIBRARIES ${MATLAB_DIR}/bin/glnx86)
  
  SET(CMAKE_CXX_FLAGS
#     "-fPIC -ansi -D_GNU_SOURCE -DMATLAB_MEX_FILE -pthread -march=pentium4 -Wall -Wno-deprecated -D_FILE_OFFSET_BITS=64"
    #"-fPIC -ansi -D_GNU_SOURCE -DMATLAB_MEX_FILE -pthread -m32 -Wall -Wno-deprecated -D_FILE_OFFSET_BITS=64"  
    "-fPIC -ansi -D_GNU_SOURCE -pthread -m32 -Wall -Wno-deprecated -D_FILE_OFFSET_BITS=64"  
    CACHE STRING "options output with mex -v CHANGE IN CMakeLists.txt NOT HERE!"
    FORCE
  )
  SET(CMAKE_C_FLAGS
#     "-fPIC -ansi -D_GNU_SOURCE -DMATLAB_MEX_FILE -pthread -march=pentium4 -Wall -fexceptions -D_FILE_OFFSET_BITS=64"
    #"-fPIC -ansi -D_GNU_SOURCE -DMATLAB_MEX_FILE -pthread -m32 -Wall -fexceptions -D_FILE_OFFSET_BITS=64 "  
    "-fPIC -ansi -D_GNU_SOURCE -pthread -m32 -Wall -fexceptions -D_FILE_OFFSET_BITS=64 "  
    CACHE STRING "options output with mex -v CHANGE IN CMakeLists.txt NOT HERE!"
    FORCE
  )
  SET(CMAKE_CXX_FLAGS_DEBUG
    "-g -ggdb3 -pg" 
    CACHE STRING "options output with mex -v CHANGE IN CMakeLists.txt NOT HERE!"
    FORCE
  )
  
  SET(CMAKE_C_FLAGS_DEBUG
    "-g -ggdb3 -pg" 
    CACHE STRING "options output with mex -v CHANGE IN CMakeLists.txt NOT HERE!"
    FORCE
  )
    
  SET(CMAKE_CXX_FLAGS_RELEASE 
    "-O2 -DNDEBUG -malign-double -ffast-math -fvisibility=hidden" 
    CACHE STRING "CHANGE IN CMakeLists.txt NOT HERE"
    FORCE
  )
  SET(CMAKE_C_FLAGS_RELEASE 
    "-O2 -DNDEBUG -malign-double -ffast-math -fvisibility=hidden" 
    CACHE STRING "CHANGE IN CMakeLists.txt NOT HERE"
    FORCE
  )
  SET(CMAKE_SHARED_LINKER_FLAGS
    #    "-Wl,-rpath-link,${MATLAB_LIBRARIES} -L${MATLAB_LIBRARIES} -lmx -lmex -lmat -lm -Wl,--version-script,${MATLAB_DIR}/extern/lib/glnx86/mexFunction.map -pthread -Wl,--no-undefined"
    ""
    CACHE STRING "CHANGE IN CMakeLists.txt NOT HERE"
    FORCE
  )
  SET(CMAKE_SHARED_LINKER_FLAGS_DEBUG
    "-g"
    CACHE STRING "CHANGE IN CMakeLists.txt NOT HERE"
    FORCE
  )
  SET(CMAKE_SHARED_LINKER_FLAGS_RELEASE
    "-O"
    CACHE STRING "CHANGE IN CMakeLists.txt NOT HERE"
    FORCE
  )
ENDIF(UNIX)

IF(WIN32)
  # 32bit MS Windows
  SET(MATLAB_LIBRARIES ${MATLAB_DIR}/bin/mexw32)
  
  SET(CMAKE_CXX_FLAGS
	""
	CACHE STRING "Used with Matlab's mex, change options within mexopts file"
	FORCE
	)
	
  SET(CMAKE_C_FLAGS
	""
	CACHE STRING "Used with Matlab's mex, change options within mexopts file"
	FORCE
	)  
  SET(CMAKE_CXX_FLAGS_DEBUG
	"-g"
	CACHE STRING "Used with Matlab's mex, change options within mexopts file"
	FORCE
	)	
  SET(CMAKE_C_FLAGS_DEBUG
	"-g"
	CACHE STRING "Used with Matlab's mex, change options within mexopts file"
	FORCE
	)
  SET(CMAKE_CXX_FLAGS_RELEASE
	"-O"
	CACHE STRING "Used with Matlab's mex, change options within mexopts file"
	FORCE
	)
  SET(CMAKE_C_FLAGS_RELEASE
	"-O"
	CACHE STRING "Used with Matlab's mex, change options within mexopts file"
	FORCE
	)
#  SET(CMAKE_C_LINK_EXECUTABLE
##	CACHE FILEPATH
#	FORCE
#	)
#  SET(CMAKE_C_LINK_EXECUTABLE
#	"${MATLAB_DIR}/bin/mex.bat"
#	CACHE FILEPATH
#	FORCE
#	)
  SET(CMAKE_SHARED_LINKER_FLAGS
    ""
    CACHE STRING "CHANGE IN CMakeLists.txt NOT HERE"
    FORCE
  )
   SET(CMAKE_SHARED_LINKER_FLAGS_DEBUG
    "-g"
    CACHE STRING "CHANGE IN CMakeLists.txt NOT HERE"
    FORCE
  )
  SET(CMAKE_SHARED_LINKER_FLAGS_RELEASE
    ""
    CACHE STRING "CHANGE IN CMakeLists.txt NOT HERE"
    FORCE
  )
	
ENDIF(WIN32)


SET_DIRECTORY_PROPERTIES(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "*.mex*")
SET_DIRECTORY_PROPERTIES(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "*.obj")

#MARK_AS_ADVANCED()


########### set up addition CMakeLists.txt locations #########
ADD_SUBDIRECTORY(matlab)
add_subdirectory(cxx)
ADD_SUBDIRECTORY(spirit_parse)

