cmake_minimum_required(VERSION 3.0)
project(jsonParser VERSION "0.0.1")
set(PROJECT_DESCRIPTION "fasade for json parser")

message(STATUS "***" )
message(STATUS "* " ${PROJECT_NAME})
message(STATUS "***")

include(GNUInstallDirs)

set(JSON_INCLUDE_DIR ${CMAKE_BINARY_DIR}/ext/json CACHE INTERNAL "Path to include folder for json")
message(STATUS "JSON_INCLUDE_DIR:" ${JSON_INCLUDE_DIR})
add_custom_command(OUTPUT ${JSON_INCLUDE_DIR}
  COMMAND mkdir -p ${CMAKE_BINARY_DIR}/ext/json
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
  COMMENT "Creating dirs for json"
  VERBATIM
)

set(JSON_HEADER ${JSON_INCLUDE_DIR}/json.hpp)
add_custom_command(OUTPUT ${JSON_HEADER}
  COMMAND ${CMAKE_COMMAND} -E chdir ${JSON_INCLUDE_DIR} wget https://raw.githubusercontent.com/nlohmann/json/develop/single_include/nlohmann/json.hpp
  COMMAND ${CMAKE_COMMAND} -E touch ${JSON_HEADER}
  WORKING_DIRECTORY ${JSON_INCLUDE_DIR}
  DEPENDS ${JSON_INCLUDE_DIR}
  COMMENT "Downloading json.hpp"
  VERBATIM
)
add_custom_target(json_download DEPENDS ${JSON_HEADER})

SET(publicHeaders
    inc/public/ParserTypes.h
    inc/public/JsonObj.h
    inc/public/Base64.h
)
SET(privateHeaders

)
SET(privateSources
    src/JsonObj.cpp
    src/Base64.cpp
)

add_library(${PROJECT_NAME} SHARED ${privateSources})
set_target_properties(${PROJECT_NAME} PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION 1
    PUBLIC_HEADER "${publicHeaders}")
configure_file(${PROJECT_NAME}.pc.in ${PROJECT_NAME}.pc @ONLY)

target_include_directories(${PROJECT_NAME} PUBLIC inc/public)
target_include_directories(${PROJECT_NAME} PRIVATE inc/
  ${JSON_INCLUDE_DIR}
)
target_compile_options(${PROJECT_NAME} PUBLIC -std=c++14 -Wall -Wfloat-conversion)

SET(jsonParserPublicInc "${jsonParser_SOURCE_DIR}/inc/public" CACHE STRING "")
SET(jsonParserLib "${PROJECT_NAME}" CACHE STRING "")
message(STATUS "*Export jsonParserPublicInc: " ${jsonParserPublicInc})
message(STATUS "*Export jsonParserLib: " ${jsonParserLib})

add_dependencies(${PROJECT_NAME} json_download)

install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(FILES ${CMAKE_BINARY_DIR}/libs/jsonParser/${PROJECT_NAME}.pc
    DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)

if(${ENABLE_TESTS})
    add_subdirectory(tests)
    add_subdirectory(Gtest)
endif(${ENABLE_TESTS})
