cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_COMPILER g++)
set(CMAKE_CXX_FLAGS "-g -Wall -Wextra -Werror -Wpedantic")

project(TKOM VERSION 1.0
        DESCRIPTION "Very nice project"
        LANGUAGES CXX)

set(EXECUTABLE_OUTPUT_PATH  ${PROJECT_SOURCE_DIR}/bin/)
set(HEADER_DIRECTORY ${PROJECT_SOURCE_DIR}/include/)
set(SOURCE_DIRECTORY ${PROJECT_SOURCE_DIR}/src/)
set(LEXICAL_ANALYZER_DIRECTORY ${PROJECT_SOURCE_DIR}/src/lexicalAnalyzer/)
set(PARSER_DIRECTORY ${PROJECT_SOURCE_DIR}/src/parser/)
set(INTERPRETER_DIRECTORY ${PROJECT_SOURCE_DIR}/src/interpreter/)
set(HELPERS_DIRECTORY /helpers/)

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${EXECUTABLE_OUTPUT_PATH})

include_directories(${HEADER_DIRECTORY})

add_subdirectory(tests)

set(SOURCES 
        ${SOURCE_DIRECTORY}/main.cpp
        ${SOURCE_DIRECTORY}/program.cpp
        ${SOURCE_DIRECTORY}/source.cpp
        ${SOURCE_DIRECTORY}/matrix.cpp
        ${SOURCE_DIRECTORY}/matrixHelpers.cpp
        ${SOURCE_DIRECTORY}/matrixOperators.cpp
        ${SOURCE_DIRECTORY}/${HELPERS_DIRECTORY}/sourceFactory.cpp
        ${PARSER_DIRECTORY}/parser.cpp
        ${PARSER_DIRECTORY}/node.cpp
        ${PARSER_DIRECTORY}/statementNode.cpp
        ${PARSER_DIRECTORY}/expressionNode.cpp
        ${LEXICAL_ANALYZER_DIRECTORY}/lexicalAnalyzer.cpp
        ${INTERPRETER_DIRECTORY}/interpreter.cpp
        ${INTERPRETER_DIRECTORY}/evaluator.cpp
        ${INTERPRETER_DIRECTORY}/semanticAnalyzer.cpp
        ${INTERPRETER_DIRECTORY}/interpreterHelpers.cpp

)

add_executable(TKOM ${SOURCES})