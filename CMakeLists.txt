cmake_minimum_required(VERSION 3.25)
project(BuildingPermitParser)

find_package(wxWidgets REQUIRED)
find_package(OpenXLSX REQUIRED)
include(${wxWidgets_USE_FILE})

include_directories(src)
include_directories(include)

add_executable(${PROJECT_NAME} src/main.cpp src/gui.cpp src/parser.cpp)

target_link_libraries(${PROJECT_NAME} ${wxWidgets_LIBRARIES} OpenXLSX::OpenXLSX)
target_include_directories(${PROJECT_NAME} PRIVATE ${wxWidgets_INCLUDE_DIRS})

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -pedantic)
