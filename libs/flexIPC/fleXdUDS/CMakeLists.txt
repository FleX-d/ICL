cmake_minimum_required(VERSION 3.0)
project(fleXdUDS VERSION "0.1.0")
set(PROJECT_DESCRIPTION "IPC unix domain socket library")

message(STATUS "***" )
message(STATUS "* " ${PROJECT_NAME})
message(STATUS "***")

message(STATUS "fleXdEpollPublicInc: " ${fleXdEpollPublicInc})
message(STATUS "fleXdEpollLib: " ${fleXdEpollLib})
message(STATUS "fleXdIPCCommonPublicInc: " ${fleXdIPCCommonPublicInc})
message(STATUS "fleXdIPCCommonLib: " ${fleXdIPCCommonLib})

include(GNUInstallDirs)

SET(publicHeaders
    inc/public/FleXdUDS.h
    inc/public/FleXdUDSClient.h
    inc/public/FleXdUDSServer.h
)
SET(privateHeaders
)
SET(privateSources
    src/FleXdUDS.cpp
    src/FleXdUDSClient.cpp
    src/FleXdUDSServer.cpp
)

add_library(${PROJECT_NAME} SHARED ${privateSources})
set_target_properties(${PROJECT_NAME} PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION 1
    PUBLIC_HEADER "${publicHeaders}")
configure_file(${PROJECT_NAME}.pc.in ${PROJECT_NAME}.pc @ONLY)

target_include_directories(${PROJECT_NAME} PUBLIC inc/public)
target_include_directories(${PROJECT_NAME} PRIVATE inc/ ${fleXdEpollPublicInc} ${fleXdIPCCommonPublicInc})
target_compile_options(${PROJECT_NAME} PUBLIC -std=c++14 -Wall -Wfloat-conversion)

target_link_libraries(${PROJECT_NAME}
    ${fleXdEpollLib}
    ${fleXdIPCCommonLib}
)

install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(FILES ${CMAKE_BINARY_DIR}/libs/flexIPC/fleXdUDS/${PROJECT_NAME}.pc
    DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)

SET(fleXdUDSPublicInc "${fleXdUDS_SOURCE_DIR}/inc/public" CACHE STRING "")
SET(fleXdUDSLib "${PROJECT_NAME}" CACHE STRING "")

if(${ENABLE_EXAMPLES})
    add_subdirectory(examples)
endif(${ENABLE_EXAMPLES})

if(${ENABLE_TESTS})
#    add_subdirectory(gtest)
endif(${ENABLE_TESTS})

add_dependencies(${PROJECT_NAME} fleXdEpoll fleXdIPCCommon)
