#
# Build the actual library
#
cmake_minimum_required(VERSION 2.6)
project(TemplateEngine)



include_directories("include")

add_library(${PROJECT_NAME} STATIC src/Context.cpp 
  src/Dictionary.cpp 
  src/DictionaryList.cpp
  src/ExpansionTemplate.cpp
  src/Lexer.cpp
  src/LookaheadScanner.cpp
  src/RepeatTemplate.cpp
  src/SemanticVersion.cpp
  src/SimpleTemplate.cpp
  src/StringScanner.cpp
  src/Template.cpp
  src/TemplateList.cpp
  src/Types.cpp
  src/Version.cpp
  include/Context.hpp
  include/Dictionary.hpp
  include/DictionaryList.hpp
  include/Exception.hpp
  include/ExpansionTemplate.hpp
  include/Lexer.hpp
  include/LookaheadScanner.hpp
  include/RepeatTemplate.hpp
  include/ReverseIterator.hpp
  include/Scanner.hpp
  include/SemanticVersion.hpp
  include/SimpleTemplate.hpp
  include/stdafx.h
  include/StringScanner.hpp
  include/Template.hpp
  include/TemplateEngine.hpp
  include/TemplateList.hpp
  include/Types.hpp
  include/Version.hpp
)

set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX "d")

# Required on Unix OS family to be able to be linked into shared libraries.
set_target_properties(${PROJECT_NAME} PROPERTIES POSITION_INDEPENDENT_CODE ON)  

install(TARGETS ${PROJECT_NAME} 
		ARCHIVE DESTINATION lib)
install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/ DESTINATION libTemplateEngine
        FILES_MATCHING PATTERN "*.hpp")
		
set(${PROJECT_NAME}_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/include
  CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE)