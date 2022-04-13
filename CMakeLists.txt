cmake_minimum_required(VERSION 3.22)
project(project CXX)

set(CMAKE_CXX_STANDARD 23)

# Source directories and files
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(BIN_DIR ${CMAKE_SOURCE_DIR}/bin)

# Executable output directory
set(EXECUTABLE_OUTPUT_PATH ${BIN_DIR})

# All needed source files
file(GLOB_RECURSE SOURCES ${SRC_DIR}/*.hpp ${SRC_DIR}/*.cpp)
message(STATUS "Found ${SOURCES} source files")
add_definitions("-fconcepts")

find_package(MPI REQUIRED)

include_directories(${MPI_INCLUDE_PATH})
add_executable(task ${SRC_DIR}/main.cpp ${SOURCES})
target_link_libraries(task PUBLIC ${MPI_LIBRARIES})

if(MPI_COMPILE_FLAGS)
    set_target_properties(task PROPERTIES COMPILE_FLAGS "${MPI_COMPILE_FLAGS}")
endif()

if(MPI_LINK_FLAGS)
    set_target_properties(task PROPERTIES LINK_FLAGS "${MPI_LINK_FLAGS}")
endif()
