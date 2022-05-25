# Locate the osmesa library
# sudo apt-get install libosmesa6-dev
#
# This module defines the following variables:
#
# OSMESA_LIBRARY      - The name of the library;
# OSMESA_INCLUDE_DIR  - Where to find osmesa include files.
# OSMESA_FOUND        - True if both the OSMESA_LIBRARY and OSMESA_INCLUDE_DIR have been found.
# OSMESA_VERSION      - Version of OSMesa found
# OSMESA::OSMesa      - Imported target
#
# To help locate the library and include file, you can define a
# variable called OSMESA_ROOT which points to the root of the osmesa library
# installation.
#

set( _osmesa_HEADER_SEARCH_DIRS
    "/usr/include/"
    "$ENV{CROSS}/include"
    "${CMAKE_SOURCE_DIR}/includes"
    "${CMAKE_SOURCE_DIR}/mesa/include"
)

set( _osmesa_LIB_SEARCH_DIRS
    "/usr/lib/x86_64-linux-gnu/"
    "$ENV{CROSS}/lib"
    "${CMAKE_SOURCE_DIR}/lib"
    "${CMAKE_SOURCE_DIR}/bin"
    "${CMAKE_SOURCE_DIR}/mesa/build/vali-$ENV{VALI_ARCH}"
)

# Check environment for root search directory
set( _osmesa_ENV_ROOT $ENV{OSMESA_ROOT} )
if( NOT OSMESA_ROOT AND _osmesa_ENV_ROOT )
	set(OSMESA_ROOT ${_osmesa_ENV_ROOT} )
endif()

# Put user specified location at beginning of search
if( OSMESA_ROOT )
	list( INSERT _osmesa_HEADER_SEARCH_DIRS 0 "${OSMESA_ROOT}/include" )
	list( INSERT _osmesa_LIB_SEARCH_DIRS 0 "${OSMESA_ROOT}/lib" )
endif()

# Search for the header
FIND_PATH(OSMESA_INCLUDE_DIR "GL/osmesa.h"
    PATHS ${_vioarr_HEADER_SEARCH_DIRS}
)
mark_as_advanced(OSMESA_INCLUDE_DIR)

# Search for the library
FIND_LIBRARY(OSMESA_LIBRARY 
    NAMES OSMesa OSMesa16 OSMesa32 gallium-osmesa
    PATHS ${_vioarr_LIB_SEARCH_DIRS}
)
mark_as_advanced(OSMESA_LIBRARY)

if (OSMESA_INCLUDE_DIR AND EXISTS "${OSMESA_INCLUDE_DIR}/GL/osmesa.h")
    file(STRINGS "${OSMESA_INCLUDE_DIR}/GL/osmesa.h" _OSMesa_version_lines 
          REGEX "OSMESA_[A-Z]+_VERSION"
    )
    string(REGEX REPLACE ".*# *define +OSMESA_MAJOR_VERSION +([0-9]+).*" "\\1" _OSMesa_version_major "${_OSMesa_version_lines}")
    string(REGEX REPLACE ".*# *define +OSMESA_MINOR_VERSION +([0-9]+).*" "\\1" _OSMesa_version_minor "${_OSMesa_version_lines}")
    string(REGEX REPLACE ".*# *define +OSMESA_PATCH_VERSION +([0-9]+).*" "\\1" _OSMesa_version_patch "${_OSMesa_version_lines}")
    set(OSMESA_VERSION "${_OSMesa_version_major}.${_OSMesa_version_minor}.${_OSMesa_version_patch}")
    unset(_OSMesa_version_major)
    unset(_OSMesa_version_minor)
    unset(_OSMesa_version_patch)
    unset(_OSMesa_version_lines)
endif ()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(OSMESA
  REQUIRED_VARS OSMESA_INCLUDE_DIR OSMESA_LIBRARY
  VERSION_VAR OSMESA_VERSION
)

if (OSMESA_FOUND)
  set(OSMESA_INCLUDE_DIRS "${OSMESA_INCLUDE_DIR}")
  set(OSMESA_LIBRARIES "${OSMESA_LIBRARY}")

  if (NOT TARGET OSMESA::OSMesa)
    add_library(OSMESA::OSMesa UNKNOWN IMPORTED)
    set_target_properties(OSMESA::OSMesa PROPERTIES
      IMPORTED_LOCATION "${OSMESA_LIBRARY}"
      INTERFACE_INCLUDE_DIRECTORIES "${OSMESA_INCLUDE_DIR}")
  endif ()
endif ()
