cmake_minimum_required (VERSION 3.10)
project(UNRED CXX)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules" ${CMAKE_MODULE_PATH})

set(UNR_VER_MAJOR 0)
set(UNR_VER_MINOR 2)
set(UNR_VER_PATCH 1)

set(UNR_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
set(UNR_INC ${UNR_ROOT}/include)
set(UNR_SRC ${UNR_ROOT}/src)

add_definitions("-DINSTALL_PREFIX=\"${CMAKE_INSTALL_PREFIX}\"")
message(STATUS "Install prefix: " ${CMAKE_INSTALL_PREFIX})

if (WIN32)
	add_definitions("-Wno-invalid-offsetof")
endif()

find_package(wxWidgets REQUIRED base core adv)

# When doing a top level build we don't want to do find_package
if (NOT TARGET Unr::Unr)
	find_package(Unr REQUIRED)
endif()

include(${wxWidgets_USE_FILE})

include_directories(
	${LIBUNR_INCLUDE_DIRS}
	${UNR_INC}
	${X11_INCLUDE_DIR}
	${VORBISFILE_INCLUDE_DIRS}
	${LIBGME_INCLUDE_DIRS}
	${LIBDUMB_INCLUDE_DIRS}
	${LIBMPG123_INCLUDE_DIRS}
	${LIBMPG123_INCLUDE_DIRS}
)

set(LIBUNR_EDITOR_SOURCES
	${UNR_SRC}/EdBrowser.cpp
	${UNR_SRC}/EdClassBrowser.cpp
	${UNR_SRC}/EdConfigFrame.cpp
	${UNR_SRC}/EdEditor.cpp
	${UNR_SRC}/EdGamePrompt.cpp
	${UNR_SRC}/EdMain.cpp
	${UNR_SRC}/EdMasterBrowser.cpp
	${UNR_SRC}/EdObjectBrowser.cpp
	${UNR_SRC}/EdPackageBrowser.cpp
	${UNR_SRC}/EdToolFrame.cpp
)

add_executable(libunr-editor
	${LIBUNR_EDITOR_SOURCES}
)

target_link_libraries(libunr-editor
	dl
	Unr::Unr
	${wxWidgets_LIBRARIES}
)

install(TARGETS libunr-editor RUNTIME
		DESTINATION bin)
