cmake_minimum_required(VERSION 3.25)
project("Building Permit Parser")

find_package(wxWidgets REQUIRED)
find_package(OpenXLSX REQUIRED)

include_directories(src)
include_directories(include)

add_executable(${PROJECT_NAME} main.cpp gui.cpp parser.cpp)

target_link_libraries(${PROJECT_NAME} wx::wxWidgets OpenXLSX::OpenXLSX)

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Werror -pedantic)
