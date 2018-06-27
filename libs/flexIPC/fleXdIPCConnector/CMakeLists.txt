cmake_minimum_required(VERSION 3.0)
project(fleXdIPCConnector VERSION "0.1.1")
set(PROJECT_DESCRIPTION "FleX-d Inter Process Comunication connector library")

message(STATUS "***" )
message(STATUS "* " ${PROJECT_NAME})
message(STATUS "***")

include(GNUInstallDirs)

set(sources_cpp
    src/FleXdIPCConnector.cpp
)

set(public_headers
    inc/public/FleXdIPCConnector.h
)

add_library(${PROJECT_NAME} SHARED ${sources_cpp})
set_target_properties(${PROJECT_NAME} PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION 1
    PUBLIC_HEADER "${public_headers}")
configure_file(${PROJECT_NAME}.pc.in ${PROJECT_NAME}.pc @ONLY)

target_include_directories(${PROJECT_NAME} PUBLIC inc/public/)
target_include_directories(${PROJECT_NAME} PRIVATE inc/ ${fleXdEpollPublicInc} ${fleXdIPCCommonPublicInc} $(fleXdUDSPublicInc))
target_compile_options(${PROJECT_NAME} PUBLIC -std=c++14 -Wall -Wfloat-conversion)

target_link_libraries(${PROJECT_NAME}
    ${fleXdEpollLib}
    ${fleXdUDSLib}
)

install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(FILES ${CMAKE_BINARY_DIR}/libs/flexIPC/fleXdIPCConnector/${PROJECT_NAME}.pc
    DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)

SET(fleXdIPCConnectorPublicInc "${fleXdIPCConnector_SOURCE_DIR}/inc/public" CACHE STRING "")
SET(fleXdIPCConnectorLib "${PROJECT_NAME}" CACHE STRING "")
message(STATUS "*Export fleXdIPCConnectorPublicInc: " ${fleXdUDSPublicInc})
message(STATUS "*Export fleXdIPCConnectorLib: " ${fleXdUDSLib})

if(${ENABLE_EXAMPLES})
    add_subdirectory(examples)
endif(${ENABLE_EXAMPLES})

if(${ENABLE_TESTS})
    add_subdirectory(gtest)
endif(${ENABLE_TESTS})

add_dependencies(${PROJECT_NAME} fleXdUDS)
