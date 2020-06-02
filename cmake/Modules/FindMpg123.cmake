find_package(PkgConfig)
pkg_check_modules(PC_MPG123 QUIET libmpg123)
set(MPG123_DEFINITIONS ${PC_MPG123_CFLAGS_OTHER})

find_path(LIBMPG123_INCLUDE_DIR mpg123.h
          HINTS ${PC_MPG123_INCLUDEDIR} ${PC_MPG123_INCLUDE_DIRS})

find_library(LIBMPG123_LIBRARY NAMES mpg123 libmpg123
             HINTS ${PC_MPG123_LIBDIR} ${PC_MPG123_LIBRARY_DIRS})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Mpg123 DEFAULT_MSG
		LIBMPG123_LIBRARY LIBMPG123_INCLUDE_DIR)

mark_as_advanced(LIBMPG123_LIBRARY LIBMPG123_INCLUDE_DIR)

set(LIBMPG123_LIBRARIES ${LIBMPG123_LIBRARY})
set(LIBMPG123_INCLUDE_DIRS ${LIBMPG123_INCLUDE_DIR})
