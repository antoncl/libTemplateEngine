#
# Build the unit tests
#

cmake_minimum_required(VERSION 2.6)

project(tests)

set(Boost_USE_STATIC_LIBS        ON)
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME    OFF)
find_package(Boost COMPONENTS unit_test_framework REQUIRED)


if(NOT Boost_FOUND)
	if (WIN32)
		message(STATUS, "Try setting the envirement variables BOOST_INCLUDEDIR and BOOST_LIBRARYDIR")
	endif ()
    message(SEND_ERROR "Failed to locate boost libraries, the unit tests cannot be built")
else()
	include_directories(${Boost_INCLUDE_DIRS} ${TemplateEngine_INCLUDE_DIRS})

	add_executable(${PROJECT_NAME} src/Dictionary.cpp
		src/Lexer.cpp
		src/LookaheadScanner.cpp
		src/Parser.cpp
		src/StringScanner.cpp
		src/run.cpp)

	target_link_libraries (${PROJECT_NAME} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY} TemplateEngine)

	install(TARGETS ${PROJECT_NAME} 
			RUNTIME DESTINATION bin)
endif()
