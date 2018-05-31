cmake_minimum_required(VERSION 3.0)
project(fleXdIPCConnector VERSION "0.0.1")
set(PROJECT_DESCRIPTION "FleX-d Inter Process Comunication connector library")

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
target_include_directories(${PROJECT_NAME} PRIVATE inc/ ${fleXdEpollPublicInc} ${fleXdIPCBufferPublicInc} $(fleXdUDSPublicInc))
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

add_dependencies(${PROJECT_NAME} fleXdUDS)