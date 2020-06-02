find_path(LIBUNR_INCLUDE_DIR Util/TQueue.h REQUIRED
          PATHS ${CMAKE_CURRENT_SOURCE_DIR}/../libunr/include/ )

find_library(LIBUNR_LIBRARY NAMES unr libunr REQUIRED
             PATHS ${CMAKE_CURRENT_SOURCE_DIR}/../libunr/ )

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(LibUnr DEFAULT_MSG
		LIBUNR_LIBRARY LIBUNR_INCLUDE_DIR)

mark_as_advanced(LIBUNR_LIBRARY LIBUNR_INCLUDE_DIR)

set(LIBUNR_LIBRARIES ${LIBUNR_LIBRARY})
set(LIBUNR_INCLUDE_DIRS ${LIBUNR_INCLUDE_DIR})
