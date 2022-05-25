# Locate the vioarr/asgaard library
#
# This module defines the following variables:
#
# VIOARR_LIBRARY the name of the library;
# VIOARR_INCLUDE_DIR where to find vioarr/asgaard include files.
# VIOARR_FOUND true if both the VIOARR_LIBRARY and VIOARR_INCLUDE_DIR have been found.
#
# To help locate the library and include file, you can define a
# variable called VIOARR_ROOT which points to the root of the vioarr/asgaard library
# installation.
#

set( _vioarr_HEADER_SEARCH_DIRS
    "$ENV{CROSS}/include"
    "${CMAKE_SOURCE_DIR}/includes"
    "${CMAKE_SOURCE_DIR}/vioarr/libs/asgaard/include"
)
set( _vioarr_LIB_SEARCH_DIRS
    "$ENV{CROSS}/lib"
    "${CMAKE_SOURCE_DIR}/lib"
    "${CMAKE_SOURCE_DIR}/bin"
)

# Check environment for root search directory
set( _vioarr_ENV_ROOT $ENV{VIOARR_ROOT} )
if( NOT VIOARR_ROOT AND _vioarr_ENV_ROOT )
	set(VIOARR_ROOT ${_vioarr_ENV_ROOT} )
endif()

# Put user specified location at beginning of search
if( VIOARR_ROOT )
	list( INSERT _vioarr_HEADER_SEARCH_DIRS 0 "${VIOARR_ROOT}/include" )
	list( INSERT _vioarr_LIB_SEARCH_DIRS 0 "${VIOARR_ROOT}/lib" )
endif()

# Search for the header
FIND_PATH(VIOARR_INCLUDE_DIR "asgaard/application.hpp"
    PATHS ${_vioarr_HEADER_SEARCH_DIRS}
)
mark_as_advanced(VIOARR_INCLUDE_DIR)

# Search for the library
FIND_LIBRARY(VIOARR_LIBRARY NAMES asgaard
    PATHS ${_vioarr_LIB_SEARCH_DIRS}
)
mark_as_advanced(VIOARR_LIBRARY)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(VIOARR DEFAULT_MSG
    VIOARR_LIBRARY VIOARR_INCLUDE_DIR
)

if (VIOARR_FOUND)
  set(VIOARR_INCLUDE_DIR "${VIOARR_INCLUDE_DIR}")
  set(VIOARR_LIBRARIES "${VIOARR_LIBRARY}")

  if (NOT TARGET VIOARR::Asgaard)
    add_library(VIOARR::Asgaard UNKNOWN IMPORTED)
    set_target_properties(VIOARR::Asgaard PROPERTIES
      IMPORTED_LOCATION "${VIOARR_LIBRARY}"
      INTERFACE_INCLUDE_DIRECTORIES "${VIOARR_INCLUDE_DIR}")
  endif ()
endif ()
