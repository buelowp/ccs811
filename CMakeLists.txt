cmake_minimum_required (VERSION 3.2.0)
project (ccs811)

set (CMAKE_INCLUDE_CURRENT_DIR ON)
FILE (GLOB SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
FILE (GLOB HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/*.h")

add_library (${PROJECT_NAME} SHARED ${SOURCES})

