cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-g -Wall")

project(TKOM VERSION 1.0
        DESCRIPTION "Very nice project"
        LANGUAGES CXX)

set(EXECUTABLE_OUTPUT_PATH  ${PROJECT_SOURCE_DIR}/bin/)
set(HEADER_DIRECTORY ${PROJECT_SOURCE_DIR}/include/)
set(SOURCE_DIRECTORY ${PROJECT_SOURCE_DIR}/src/)

include_directories(${HEADER_DIRECTORY})

add_subdirectory(tests)

set(SOURCES 
        ${SOURCE_DIRECTORY}/main.cpp
        ${SOURCE_DIRECTORY}/program.cpp
        ${SOURCE_DIRECTORY}/source.cpp
        ${HEADER_DIRECTORY}/program.hpp
        ${HEADER_DIRECTORY}/source.hpp
)

add_executable(TKOM ${SOURCES})