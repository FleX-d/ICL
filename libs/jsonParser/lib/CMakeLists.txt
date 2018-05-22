cmake_minimum_required(VERSION 3.0)
project(jsonParser VERSION "0.0.1")
set(PROJECT_DESCRIPTION "fasade for json parser")

include(GNUInstallDirs)

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

find_package(nlohmann_json REQUIRED)
if (nlohmann_json_FOUND)
    message(STATUS "nlohmann_json was found with version: " ${nlohmann_json_VERSION})
endif (nlohmann_json_FOUND)

target_include_directories(${PROJECT_NAME} PUBLIC inc/public)
target_include_directories(${PROJECT_NAME} PRIVATE inc/)
target_compile_options(${PROJECT_NAME} PUBLIC -std=c++14 -Wall -Wfloat-conversion)

target_link_libraries(${PROJECT_NAME}
    nlohmann_json
)

SET(jsonParserPublicInc "${publicHeaders}" PARENT_SCOPE)
SET(jsonParserLib "${PROJECT_NAME}" PARENT_SCOPE)

install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.pc
    DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)
