cmake_minimum_required(VERSION 3.0)
project(jsonParserTests VERSION "0.0.1")

message(STATUS "jsonParserPublicInc: " ${jsonParserPublicInc})
message(STATUS "jsonParserLib: " ${jsonParserLib})
include_directories(${jsonParserPublicInc})

find_package(PkgConfig)
pkg_check_modules(GTEST REQUIRED gtest>=1.7.0)
pkg_check_modules(GMOCK REQUIRED gmock>=1.7.0)

message(STATUS "GTEST_LIBRARIES: " ${GTEST_LIBRARIES})
message(STATUS "GTEST_LIBRARIES: " ${GMOCK_LIBRARIES})
message(STATUS "GTEST_LIBRARIES: " ${GTEST_INCLUDE_DIRS})
message(STATUS "GTEST_LIBRARIES: " ${GMOCK_INCLUDE_DIRS})

include_directories(
    ${GTEST_INCLUDE_DIRS}
    ${GMOCK_INCLUDE_DIRS}
)

add_executable(UnitTests main.cpp JsonParserUnitTest.cpp BinStreamUnitTest.cpp)
target_link_libraries(UnitTests
    ${jsonParserLib}
    ${GTEST_LIBRARIES}
    ${GMOCK_LIBRARIES}
)
add_dependencies(UnitTests jsonParser)


#add_executable(BinStreamUnitTest main.cpp BinStreamUnitTest.cpp)
#target_link_libraries(BinStreamUnitTest
#    ${jsonParserLib}
#    ${GTEST_LIBRARIES}
#    ${GMOCK_LIBRARIES}
#)
#add_dependencies(BinStreamUnitTest jsonParser)
