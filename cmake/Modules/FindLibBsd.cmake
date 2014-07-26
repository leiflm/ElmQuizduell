# - Try to find LibBsd
# Once done this will define
#  LIBBSD_FOUND - System has LibBsd
#  LIBBSD_INCLUDE_DIRS - The LibBsd include directories
#  LIBBSD_LIBRARIES - The libraries needed to use LibBsd
#  LIBBSD_DEFINITIONS - Compiler switches required for using LibBsd

find_package(PkgConfig)
pkg_check_modules(PC_LIBBSD QUIET libbsd)
set(LIBBSD_DEFINITIONS ${PC_LIBBSD_CFLAGS_OTHER})

find_path(LIBBSD_INCLUDE_DIR bsd/string.h
    HINTS ${PC_LIBBSD_INCLUDEDIR} ${PC_LIBBSD_INCLUDE_DIRS}
    PATH_SUFFIXES libbsd )

find_library(LIBBSD_LIBRARY NAMES bsd libbsd
    HINTS ${PC_LIBBSD_LIBDIR} ${PC_LIBBSD_LIBRARY_DIRS} )

set(LIBBSD_LIBRARIES ${LIBBSD_LIBRARY} )
set(LIBBSD_INCLUDE_DIRS ${LIBBSD_INCLUDE_DIR} )

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set LIBBSD_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(LibBsd  DEFAULT_MSG
    LIBBSD_LIBRARY LIBBSD_INCLUDE_DIR)

mark_as_advanced(LIBBSD_INCLUDE_DIR LIBBSD_LIBRARY )
