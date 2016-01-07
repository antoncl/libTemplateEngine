#
# Project build
#
cmake_minimum_required(VERSION 3.2)

set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)
set(CMAKE_DISABLE_SOURCE_CHANGES  ON)

project(TemplateEngine)
include(CMakeDependentOption)

set(Boost_USE_STATIC_LIBS        ON)
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME     ON)
find_package(Boost COMPONENTS unit_test_framework REQUIRED)
if(Boost_FOUND)
	option(USE_UNITTEST "Compile and run unit tests" ON)
else()
	option(USE_UNITTEST "Compile and run unit tests" OFF)
endif(Boost_FOUND)

find_package(Doxygen)
if(DOXYGEN_FOUND)
	option(BUILD_DOCUMENTATION "Generate documenation" ON)
else()
	option(BUILD_DOCUMENTATION "Generate documenation" OFF)
endif(DOXYGEN_FOUND)

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
