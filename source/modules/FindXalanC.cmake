# Locate Xalan-C include paths and libraries
# Xalan-C can be found at http://xml.apache.org/xalan-c/
# Adapted from FindXercesC.cmake written by Frederic Heem, frederic.heem _at_ telsey.it
# Matt McCormick (thewtex) <matt@mmmccormick.com>

# This module defines
# XALANC_INCLUDE_DIR, where to find xalan-c headers
# XALANC_LIBRARIES, the libraries to link against to use xalan-c.
# XALANC_FOUND, If false, don't try to use xalanc.

FIND_PATH(XALANC_INCLUDE_DIR xalanc/DOMSupport/DOMServices.hpp
  "[HKEY_CURRENT_USER\\software\\xalan-c\\src]"
  "[HKEY_CURRENT_USER\\xalan-c\\src]"
  $ENV{XALANCROOT}/src/
  /usr/local/include
  /usr/include
)

FIND_LIBRARY(XALANC_LIBRARIES
  NAMES
    xalan-c
  PATHS
    "[HKEY_CURRENT_USER\\software\\xalan-c\\lib]"
    "[HKEY_CURRENT_USER\\xalan-c\\lib]"
    $ENV{XALANCROOT}/lib
    /usr/local/lib
    /usr/lib
)

# if the include a the library are found then we have it
IF(XALANC_INCLUDE_DIR)
  IF(XALANC_LIBRARIES)
    SET( XALANC_FOUND "YES" )
  ENDIF(XALANC_LIBRARIES)
ENDIF(XALANC_INCLUDE_DIR)

MARK_AS_ADVANCED(
  XALANC_INCLUDE_DIR
  XALANC_LIBRARIES
)
