# - Try to find Json-C
# Once done this will define
#  JSON-C_FOUND - System has Json-C
#  JSON-C_INCLUDE_DIRS - The Json-C include directories
#  JSON-C_LIBRARIES - The libraries needed to use Json-C
#  JSON-C_DEFINITIONS - Compiler switches required for using Json-C

find_package(PkgConfig)
pkg_check_modules(PC_JSON-C QUIET json-c)
set(JSON-C_DEFINITIONS ${PC_JSON-C_CFLAGS_OTHER})

find_path(JSON-C_INCLUDE_DIR json-c/json.h
    HINTS ${PC_JSON-C_INCLUDEDIR} ${PC_JSON-C_INCLUDE_DIRS}
    PATH_SUFFIXES json-c )

find_library(JSON-C_LIBRARY NAMES json-c
    HINTS ${PC_JSON-C_LIBDIR} ${PC_JSON-C_LIBRARY_DIRS} )

set(JSON-C_LIBRARIES ${JSON-C_LIBRARY} )
set(JSON-C_INCLUDE_DIRS ${JSON-C_INCLUDE_DIR} )

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set JSON-C_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(Json-C  DEFAULT_MSG
    JSON-C_LIBRARY JSON-C_INCLUDE_DIR)

mark_as_advanced(JSON-C_INCLUDE_DIR JSON-C_LIBRARY )
