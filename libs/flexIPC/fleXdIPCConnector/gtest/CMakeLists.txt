cmake_minimum_required(VERSION 3.0)
project(IPCConnectorTests VERSION "0.0.1")

message(STATUS "***" )
message(STATUS "* " ${PROJECT_NAME})
message(STATUS "***")

find_package(PkgConfig)
pkg_check_modules(GTEST REQUIRED gtest>=1.7.0)
pkg_check_modules(GMOCK REQUIRED gmock>=1.7.0)

message(STATUS "GTEST_LIBRARIES: " ${GTEST_LIBRARIES})
message(STATUS "GTEST_LIBRARIES: " ${GMOCK_LIBRARIES})
message(STATUS "GTEST_LIBRARIES: " ${GTEST_INCLUDE_DIRS})
message(STATUS "GTEST_LIBRARIES: " ${GMOCK_INCLUDE_DIRS})

message(STATUS "fleXdIPCConnectorPublicInc: " ${fleXdIPCConnectorPublicInc})
message(STATUS "BINSTREAM_LIBRARIES: " ${BITSTREAM_INCLUDE_DIR})
message(STATUS "FLEXDUDS_INCLUDE_DIRS: "  ${FLEXDUDS_INCLUDE_DIRS})

SET(privateSources
    ${fleXdIPCConnector_SOURCE_DIR}/src/FleXdIPCConnector.cpp
    FleXdIPCConnectorTest.cpp
)

include_directories(
    ${GTEST_INCLUDE_DIRS}
    ${GMOCK_INCLUDE_DIRS}
    ${fleXdIPCConnectorPublicInc}
)

add_executable(${PROJECT_NAME} main.cpp ${privateSources})

target_compile_options(${PROJECT_NAME} PUBLIC -std=c++14 -Wall -Wfloat-conversion)
target_link_libraries( ${PROJECT_NAME}
    ${GTEST_LIBRARIES}
    ${GMOCK_LIBRARIES}
    ${fleXdIPCConnectorLib}
)

add_dependencies(${PROJECT_NAME} fleXdIPCConnector bitstream)
