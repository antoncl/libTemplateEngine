#
# Project build
#

cmake_minimum_required(VERSION 2.6)

set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)
set(CMAKE_DISABLE_SOURCE_CHANGES  ON)

project(TemplateEngine)

option(USE_UNITTEST "Compile and run unit tests" OFF)
option(BUILD_DOCUMENTATION "Generate documenation" OFF)

if ("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
  message(SEND_ERROR "In-source builds are not allowed.")
endif ()

set(CMAKE_VERBOSE_MAKEFILE OFF)
set(CMAKE_COLOR_MAKEFILE   ON)

# Remove 'lib' prefix for shared libraries on Windows
if (WIN32)
  set(CMAKE_SHARED_LIBRARY_PREFIX "")
endif ()

if (WIN32)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc -D_CRT_SECURE_NO_WARNINGS")
elseif (UNIX)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall")
endif ()

#set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/../lib)
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/../bin)
add_subdirectory(TemplateEngine)

if (BUILD_DOCUMENTATION)
	add_subdirectory(doc)
endif (BUILD_DOCUMENTATION)

if (USE_UNITTEST)
  add_subdirectory(test)
endif (USE_UNITTEST)
