cmake_minimum_required(VERSION 3.0)
project(fleXdUDSExamples VERSION "0.0.1")
set(PROJECT_DESCRIPTION "IPC unix domain socket examples")

message(STATUS "***" )
message(STATUS "* " ${PROJECT_NAME})
message(STATUS "***")

set(UDSClient "${PROJECT_NAME}Client")
set(UDSServer "${PROJECT_NAME}Server")

message(STATUS "UDSClient: " ${UDSClient})
message(STATUS "UDSServer: " ${UDSServer})

message(STATUS "fleXdEpollPublicInc: " ${fleXdEpollPublicInc})
message(STATUS "fleXdEpollLib: " ${fleXdEpollLib})
message(STATUS "fleXdIPCCommonPublicInc: " ${fleXdIPCCommonPublicInc})
message(STATUS "fleXdIPCCommonLib: " ${fleXdIPCCommonLib})
message(STATUS "fleXdUDSPublicInc: " ${fleXdUDSPublicInc})
message(STATUS "fleXdUDSLib: " ${fleXdUDSLib})

include(GNUInstallDirs)

SET(publicHeaders
)
SET(privateHeaders
)
SET(privateSources
)

add_executable(${UDSClient} Client.cpp)
target_include_directories(${UDSClient} PRIVATE ${fleXdEpollPublicInc} ${fleXdIPCCommonPublicInc} ${fleXdUDSPublicInc})
target_compile_options(${UDSClient} PUBLIC -std=c++14 -Wall -Wfloat-conversion)
target_link_libraries(${UDSClient}
    ${fleXdEpollLib}
    ${fleXdIPCCommonLib}
    ${fleXdUDSLib}
)
add_dependencies(${UDSClient} fleXdEpoll fleXdIPCCommon fleXdUDS)

add_executable(${UDSServer} Server.cpp)
target_include_directories(${UDSServer} PRIVATE ${fleXdEpollPublicInc} ${fleXdIPCCommonPublicInc} ${fleXdUDSPublicInc})
target_compile_options(${UDSServer} PUBLIC -std=c++14 -Wall -Wfloat-conversion)
target_link_libraries(${UDSServer}
    ${fleXdEpollLib}
    ${fleXdIPCCommonLib}
    ${fleXdUDSLib}
)
add_dependencies(${UDSServer} fleXdEpoll fleXdIPCCommon fleXdUDS)    
