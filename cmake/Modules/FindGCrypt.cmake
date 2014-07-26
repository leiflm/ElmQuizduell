# - Try to find GCrypt
# Once done this will define
#  GCRYPT_FOUND - System has GCrypt
#  GCRYPT_INCLUDE_DIRS - The GCrypt include directories
#  GCRYPT_LIBRARIES - The libraries needed to use GCrypt
#  GCRYPT_DEFINITIONS - Compiler switches required for using GCrypt

find_package(PkgConfig)
pkg_check_modules(PC_GCRYPT QUIET gcrypt)
set(GCRYPT_DEFINITIONS ${PC_GCRYPT_CFLAGS_OTHER})

find_path(GCRYPT_INCLUDE_DIR gcrypt.h
    HINTS ${PC_GCRYPT_INCLUDEDIR} ${PC_GCRYPT_INCLUDE_DIRS} )

find_library(GCRYPT_LIBRARY NAMES gcrypt
    HINTS ${PC_GCRYPT_LIBDIR} ${PC_GCRYPT_LIBRARY_DIRS} )

set(GCRYPT_LIBRARIES ${GCRYPT_LIBRARY} )
set(GCRYPT_INCLUDE_DIRS ${GCRYPT_INCLUDE_DIR} )

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set GCRYPT_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(GCrypt  DEFAULT_MSG
    GCRYPT_LIBRARY GCRYPT_INCLUDE_DIR)

mark_as_advanced(GCRYPT_INCLUDE_DIR GCRYPT_LIBRARY )
