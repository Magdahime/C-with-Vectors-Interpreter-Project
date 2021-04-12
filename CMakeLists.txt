cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-g -Wall")

project(TKOM VERSION 1.0
        DESCRIPTION "Very nice project"
        LANGUAGES CXX)

set(EXECUTABLE_OUTPUT_PATH  ${PROJECT_SOURCE_DIR}/bin/)
add_subdirectory(tests)
