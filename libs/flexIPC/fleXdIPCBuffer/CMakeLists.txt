cmake_minimum_required(VERSION 3.0)
project(fleXdIPCBuffer VERSION "0.0.1")
set(PROJECT_DESCRIPTION "IPC buffer library")

message(STATUS "***" )
message(STATUS "* " ${PROJECT_NAME})
message(STATUS "***")

include(ExternalProject)
find_package(Git REQUIRED)

ExternalProject_Add(
    crcpp
    PREFIX ${CMAKE_BINARY_DIR}/ext
    GIT_REPOSITORY https://github.com/d-bahr/CRCpp.git
    TIMEOUT 10
    #UPDATE_COMMAND ${GIT_EXECUTABLE} pull
    UPDATE_COMMAND ""
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
    LOG_DOWNLOAD ON
)

ExternalProject_Get_Property(crcpp source_dir)
set(CRCPP_INCLUDE_DIR ${source_dir}/inc CACHE INTERNAL "Path to include folder for CRCpp")
message(STATUS "CRCPP_INCLUDE_DIR:" ${CRCPP_INCLUDE_DIR})

ExternalProject_Add(
    bitstream
    PREFIX ${CMAKE_BINARY_DIR}/ext
    GIT_REPOSITORY https://github.com/AdrianPeniak/bitstream.git
    TIMEOUT 10
    #UPDATE_COMMAND ${GIT_EXECUTABLE} pull
    UPDATE_COMMAND ""
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
    LOG_DOWNLOAD ON
)

ExternalProject_Get_Property(bitstream source_dir)
set(BITSTREAM_INCLUDE_DIR ${source_dir}/lib CACHE INTERNAL "Path to include folder for bitstream")
message(STATUS "BITSTREAM_INCLUDE_DIR:" ${BITSTREAM_INCLUDE_DIR})


include(GNUInstallDirs)

SET(publicHeaders
    inc/public/FleXdIPCBuffer.h
    inc/public/FleXdIPCMsg.h
    inc/public/FleXdIPCBufferTypes.h
)
SET(privateHeaders
    inc/FleXdIPCFactory.h
)
SET(privateSources
    src/FleXdIPCBuffer.cpp
    src/FleXdIPCFactory.cpp
    src/FleXdIPCMsg.cpp
)

add_library(${PROJECT_NAME} SHARED ${privateSources})
set_target_properties(${PROJECT_NAME} PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION 1
    PUBLIC_HEADER "${publicHeaders}")
configure_file(${PROJECT_NAME}.pc.in ${PROJECT_NAME}.pc @ONLY)

target_include_directories(${PROJECT_NAME} PUBLIC inc/public)
target_include_directories(${PROJECT_NAME} PRIVATE inc/ ${CRCPP_INCLUDE_DIR} ${BITSTREAM_INCLUDE_DIR} )
target_compile_options(${PROJECT_NAME} PUBLIC -std=c++14 -Wall -Wfloat-conversion)

target_link_libraries(${PROJECT_NAME}

)

install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(FILES ${CMAKE_BINARY_DIR}/libs/flexIPC/fleXdIPCBuffer/${PROJECT_NAME}.pc
    DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)
    
SET(fleXdIPCBufferPublicInc "${fleXdIPCBuffer_SOURCE_DIR}/inc/public" PARENT_SCOPE)
SET(fleXdIPCBufferLib "${PROJECT_NAME}" PARENT_SCOPE)    
message(STATUS "*Export fleXdIPCBufferPublicInc: " ${fleXdIPCBufferPublicInc})
message(STATUS "*Export fleXdIPCBufferLib: " ${fleXdIPCBufferLib})

add_dependencies(${PROJECT_NAME} crcpp bitstream)

if(${ENABLE_TESTS})
    add_subdirectory(gtest)
endif(${ENABLE_TESTS})
