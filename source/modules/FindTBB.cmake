# - Find TBB
# Find the Intel Thread Building Blocks
# includes and library.  Set up project its
# use.
#
# This modules defines:
#   TBB_DEFINITIONS, -DTBB_DO_ASSERT, added to definitions with Debug BUILD_TYPE
#     -DTBB
#   TBB_INCLUDE_DIR, where to find tbb/task_scheduler_init.h, etc
#   TBB_LIBRARIES, the libraries to link against
# COMPONENTS can be specified.
# Valid COMPONENTS would be
#   tbb
#   tbbmalloc
# wherein
#   TBB_tbb_LIBRARY
#   TBB_tbbmalloc_LIBRARY
#   TBB_tbb_debug_LIBRARY
#   TBB_tbbmalloc_debug_LIBRARY
# are then defined.
# It is good to use TBB_LIBRARIES or TBB_tbb_VAR_LIBRARY, e.g.,
# because these will be tbb and change in a Debug build to tbb_debug.
# Also, -DTBB_DO_ASSERT is added to CMAKE_CXX_FLAGS_DEBUG.
# For more information on this, see the tutorial PDF.
#
# created on 24 June 2008 by thewtex <matt@mmmccormick.com>

set( all_tbb_libs tbb tbbmalloc )

# include path
find_path( TBB_INCLUDE_DIR tbb/task_scheduler_init.h )
mark_as_advanced( TBB_INCLUDE_DIR )
if(NOT QUIETLY)
  message(STATUS "TBB_INCLUDE_DIR: ${TBB_INCLUDE_DIR}" )
endif(NOT QUIETLY)

if( TBB_FIND_COMPONENTS )
  set( tbb_libs_to_find ${TBB_FIND_COMPONENTS} )
else( TBB_FIND_COMPONENTS )
  set( tbb_libs_to_find ${all_tbb_libs} )
endif( TBB_FIND_COMPONENTS )

# libraries
foreach(comp ${tbb_libs_to_find})
  find_library( TBB_${comp}_LIBRARY ${comp} )
  mark_as_advanced( TBB_${comp}_LIBRARY )
  find_library( TBB_${comp}_debug_LIBRARY ${comp}_debug )
  mark_as_advanced( TBB_${comp}_debug_LIBRARY )
  if( ${CMAKE_BUILD_TYPE} MATCHES "Debug" )
    set(TBB_${comp}_VAR_LIBRARY ${TBB_${comp}_debug_LIBRARY} )
    set(TBB_LIBRARIES ${TBB_LIBRARIES} ${TBB_${comp}_VAR_LIBRARY} )
  else( ${CMAKE_BUILD_TYPE} MATCHES "Debug" )
    set(TBB_${comp}_VAR_LIBRARY ${TBB_${comp}_LIBRARY} )
    set(TBB_LIBRARIES ${TBB_LIBRARIES} ${TBB_${comp}_VAR_LIBRARY} )
  endif( ${CMAKE_BUILD_TYPE} MATCHES "Debug" )
endforeach(comp)
if(NOT QUIETLY)
  message(STATUS "TBB_LIBRARIES: ${TBB_LIBRARIES}" )
endif(NOT QUIETLY)


# error handling
include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(TBB DEFAULT_MSG TBB_INCLUDE_DIR TBB_LIBRARIES)

# definitions
set( TBB_DEFINITIONS -DTBB )
if( ${CMAKE_BUILD_TYPE} MATCHES Debug )
  set( TBB_DEFINITIONS ${TBB_DEFINITIONS} -DTBB_DO_ASSERT )
endif( ${CMAKE_BUILD_TYPE} MATCHES Debug )
