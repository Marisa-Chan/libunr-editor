cmake_minimum_required (VERSION 3.10)
project(UNRED CXX)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_MODULE_PATH cmake/Modules ${CMAKE_MODULE_PATH})

set(UNR_VER_MAJOR 0)
set(UNR_VER_MINOR 2)
set(UNR_VER_PATCH 1)

set(UNR_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
set(UNR_INC ${UNR_ROOT}/include)
set(UNR_SRC ${UNR_ROOT}/src)

option(BUILD_CYGWIN "Build_Cygwin" OFF )
option(BUILD_DEUXEX "DeusEx support" OFF)
option(BUILD_RUNE "Rune support" OFF)

add_definitions("-DINSTALL_PREFIX=\"${CMAKE_INSTALL_PREFIX}\"")
message(STATUS "Install prefix: " ${CMAKE_INSTALL_PREFIX})

if (WIN32)
	add_definitions("-Wno-invalid-offsetof")
endif()

find_package(Dumb REQUIRED)
find_package(Gme REQUIRED)
find_package(Mpg123 REQUIRED)
find_package(Vorbisfile REQUIRED)
find_package(X11)
find_package(wxWidgets REQUIRED base core adv)

find_package(Libunr REQUIRED)

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
	${LIBUNR_LIBRARIES}
	${OPENGL_LIBRARIES}
	${X11_LIBRARIES}
	${LIBGME_LIBRARIES}
	${LIBDUMB_LIBRARIES}
	${wxWidgets_LIBRARIES}
	${LIBMPG123_LIBRARIES}
	${LIBVORBISFILE_LIBRARIES}
)

install(TARGETS libunr-editor RUNTIME
		DESTINATION bin)
