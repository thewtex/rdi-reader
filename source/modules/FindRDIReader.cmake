# Locate rdiReader
# Installed by the rdi-reader project.
# rdi-reader can found on the Ultrasound group git server at
# University of Wisconsin-Madsion
# Written by Matt McCormick <matt@mmmccormick.com>

# This module defines
# RDIREADER_INCLUDE_DIR, rdiReader.h, etc
# RDIREADER_LIBRARIES, the libraries to link against to use the C++ library
# RDIREADER_ITK_LIBRARIES, the libraries to link against to use the ITK ImageIO
# RDIREADER_FOUND, If false, don't try to use rdiReader.

FIND_PATH(RDIREADER_INCLUDE_DIR rdiReader.h
  $ENV{RDIREADERROOT}/source/common
  /usr/local/include/rdiReader
  /usr/include/rdiReader
)

FIND_LIBRARY(RDIREADER_LIBRARIES
  NAMES
    rdiReader
  PATHS
    $ENV{RDIREADERROOT}/build/lib
    /usr/local/lib
    /usr/lib
)

FIND_LIBRARY( RDIREADER_ITK_LIBRARIES
  NAMES
    ITKVisualSonicsImageIO
  PATHS
    $ENV{RDIREADERROOT}/build/lib
    /usr/local/lib
    /usr/lib
)

# if the include a the library are found then we have it
IF(RDIREADER_INCLUDE_DIR)
  IF(RDIREADER_LIBRARIES)
    SET( RDIREADER_FOUND "YES" )
  ENDIF(RDIREADER_LIBRARIES)
ENDIF(RDIREADER_INCLUDE_DIR)



MARK_AS_ADVANCED(
  RDIREADER_INCLUDE_DIR
  RDIREADER_LIBRARIES
  RDIREADER_ITK_LIBRARIES
)
