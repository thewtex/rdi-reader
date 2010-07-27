cmake_minimum_required(VERSION 2.8)
if(COMMAND cmake_policy)
  cmake_policy(VERSION 2.8)
endif(COMMAND cmake_policy)


project(RDI_READER)

set( BUILD_SHARED_LIBS OFF CACHE BOOL
  "Build static or shared libraries.  Shared libraries might not work on Windows at the moment." )

set(ANALYZE_RDI_FORMAT OFF CACHE BOOL
  "Analyze rdi format from example sources to generate parsing code.")

set(BUILD_RDI2HTML ON CACHE BOOL
  "Build program that converts rdi files to a human readable html file.")

set( USE_VTK OFF CACHE BOOL
  "Build capabilities related to the Visualization Toolkit www.vtk.org" )

set( USE_ITK ON CACHE BOOL
  "Build capabilities related to the Insight Toolkit www.itk.org" )
if( USE_ITK )
  find_package( ITK REQUIRED )
endif()


if(CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
  set(MAKE_C_FLAGS   "${CMAKE_C_FLAGS}   -Wall")

  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -ggdb3 -fno-inline")
  set(CMAKE_C_FLAGS_DEBUG   "${CMAKE_C_FLAGS_DEBUG}   -ggdb3")

  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2 -DNDEBUG -ffast-math")
  set(CMAKE_C_FLAGS_RELEASE   "${CMAKE_C_FLAGS_RELEASE}   -O2 -DNDEBUG -ffast-math")

  set(CMAKE_SHARED_LINKER_FLAGS_DEBUG "${CMAKE_SHARED_LINKER_FLAGS_DEBUG} -g")
  set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} -O")
endif(CMAKE_COMPILER_IS_GNUCXX)

# where all the executables go
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY
  ${RDI_READER_BINARY_DIR}/bin )
# where all the libraries go
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY
  ${RDI_READER_BINARY_DIR}/lib )


add_subdirectory(source)

include(CTest)
if(BUILD_TESTING)
  if(NOT EXISTS ${RDI_READER_SOURCE_DIR}/tests/data/input/.git)
    message(SEND_ERROR "In order to run tests, the tests/data/input must be
    downloaded.  Try 'git submodule init && git submodule update' in the source
    directory.  If you do not want to run tests, turn BUILD_TESTING OFF.")
  endif()
  enable_testing()
  add_subdirectory(tests)
endif(BUILD_TESTING)
