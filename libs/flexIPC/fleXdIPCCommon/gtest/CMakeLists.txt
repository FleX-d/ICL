cmake_minimum_required(VERSION 3.0)
project(fleXdIPCCommonTests VERSION "0.0.1")


message(STATUS "***" )
message(STATUS "* " ${PROJECT_NAME})
message(STATUS "***")

message(STATUS "fleXdIPCCommonPublicInc: " ${fleXdIPCCommonPublicInc})
message(STATUS "fleXdIPCCommonLib: " ${fleXdIPCCommonLib})

find_package(PkgConfig)
pkg_check_modules(GTEST REQUIRED gtest>=1.7.0)
pkg_check_modules(GMOCK REQUIRED gmock>=1.7.0)

message(STATUS "GTEST_LIBRARIES: " ${GTEST_LIBRARIES})
message(STATUS "GTEST_LIBRARIES: " ${GMOCK_LIBRARIES})
message(STATUS "GTEST_INCLUDE_DIRS: " ${GTEST_INCLUDE_DIRS})
message(STATUS "GMOCK_INCLUDE_DIRS: " ${GMOCK_INCLUDE_DIRS})

message(STATUS "CRCPP_LIBRARIES: " ${CRCPP_INCLUDE_DIR})
message(STATUS "BITSTREAM_LIBRARIES: " ${BITSTREAM_INCLUDE_DIR})


include_directories(
    ${CRCPP_INCLUDE_DIR}
    ${BITSTREAM_INCLUDE_DIR}
    ${GTEST_INCLUDE_DIRS}
    ${GMOCK_INCLUDE_DIRS}
    ${fleXdIPCCommonPublicInc}
)

add_executable(${PROJECT_NAME} main.cpp FleXdIPCBufferTest.cpp FleXdIPCMsgTest.cpp)
target_compile_options(${PROJECT_NAME} PUBLIC -std=c++14 -Wall -Wfloat-conversion)
target_link_libraries( ${PROJECT_NAME}
    ${fleXdIPCCommonLib}
    ${GTEST_LIBRARIES}
    ${GMOCK_LIBRARIES}
)
add_dependencies(${PROJECT_NAME} fleXdIPCCommon crcpp bitstream )


