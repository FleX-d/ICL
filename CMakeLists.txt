cmake_minimum_required(VERSION 3.0)

if(NOT CMAKE_BUILD_TYPE)
    message(STATUS "setting default value 'Debug' for CMAKE_BUILD_TYPE")
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Build type" FORCE)
endif()

set_property(
    CACHE
    CMAKE_BUILD_TYPE
    PROPERTY STRINGS
    "Debug"
    "Release"
)

option(
    ENABLE_TESTS
    "Build tests?"
    OFF
)
option(
    ENABLE_EXAMPLES
    "Build examples?"
    OFF
)

message(STATUS "option ENABLE_TESTS=" ${ENABLE_TESTS})
message(STATUS "option ENABLE_EXAMPLES=" ${ENABLE_EXAMPLES})

add_subdirectory(libs)
