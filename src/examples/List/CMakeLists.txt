cmake_minimum_required(VERSION 2.6)
project(list)

add_executable(list main.cpp)
target_link_libraries (${PROJECT_NAME} TemplateEngine)

install(TARGETS list RUNTIME DESTINATION bin)
