cmake_minimum_required(VERSION 2.6)
project(simple)

add_executable(simple main.cpp)
target_link_libraries (${PROJECT_NAME} TemplateEngine)

install(TARGETS simple RUNTIME DESTINATION bin)
