cmake_minimum_required(VERSION 3.0)
project(fleXdEpoll VERSION "0.1.0")
set(PROJECT_DESCRIPTION "Epoll library")

message(STATUS "***" )
message(STATUS "* " ${PROJECT_NAME})
message(STATUS "***")

include(GNUInstallDirs)

SET(publicHeaders
    inc/public/FleXdEpoll.h
    inc/public/FleXdEvent.h
    inc/public/FleXdIPCTypes.h
    inc/public/FleXdTimer.h
)
SET(privateHeaders
)
SET(privateSources
    src/FleXdEpoll.cpp
    src/FleXdEvent.cpp
    src/FleXdTimer.cpp
)

add_library(${PROJECT_NAME} SHARED ${privateSources})
set_target_properties(${PROJECT_NAME} PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION 1
    PUBLIC_HEADER "${publicHeaders}")
configure_file(${PROJECT_NAME}.pc.in ${PROJECT_NAME}.pc @ONLY)

target_include_directories(${PROJECT_NAME} PUBLIC inc/public)
target_include_directories(${PROJECT_NAME} PRIVATE inc/)
target_compile_options(${PROJECT_NAME} PUBLIC -std=c++14 -Wall -Wfloat-conversion)

target_link_libraries(${PROJECT_NAME}

)

install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
    install(FILES ${CMAKE_BINARY_DIR}/libs/flexIPC/fleXdEpoll/${PROJECT_NAME}.pc
    DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)

SET(fleXdEpollPublicInc "${fleXdEpoll_SOURCE_DIR}/inc/public" CACHE STRING "")
SET(fleXdEpollLib "${PROJECT_NAME}" CACHE STRING "")
message(STATUS "*Export fleXdEpollPublicInc: " ${fleXdEpollPublicInc})
message(STATUS "*Export fleXdEpollLib: " ${fleXdEpollLib})

if(${ENABLE_EXAMPLES})
    add_subdirectory(examples)
endif(${ENABLE_EXAMPLES})
