cmake_minimum_required(VERSION 3.0)
project(fleXdIPCConnectorExamples VERSION "0.0.1")
set(PROJECT_DESCRIPTION "IPC Connector examples")

message(STATUS "***" )
message(STATUS "* " ${PROJECT_NAME})
message(STATUS "***")

set(APP1 "${PROJECT_NAME}App1")
set(APP2 "${PROJECT_NAME}App2")
set(APP3 "${PROJECT_NAME}App3")
set(APPGEN "${PROJECT_NAME}AppGen")

message(STATUS "APP1: " ${APP1})
message(STATUS "APP2: " ${APP2})
message(STATUS "APP3: " ${APP3})
message(STATUS "APPGEN: " ${APPGEN})

message(STATUS "fleXdEpollPublicInc: " ${fleXdEpollPublicInc})
message(STATUS "fleXdEpollLib: " ${fleXdEpollLib})
message(STATUS "fleXdIPCCommonPublicInc: " ${fleXdIPCCommonPublicInc})
message(STATUS "fleXdIPCCommonLib: " ${fleXdIPCCommonLib})
message(STATUS "fleXdUDSPublicInc: " ${fleXdUDSPublicInc})
message(STATUS "fleXdUDSLib: " ${fleXdUDSLib})
message(STATUS "fleXdIPCConnectorPublicInc: " ${fleXdIPCConnectorPublicInc})
message(STATUS "fleXdIPCConnectorLib: " ${fleXdIPCConnectorLib})

include(GNUInstallDirs)

SET(publicHeaders
)
SET(privateHeaders
)
SET(privateSources
)

add_executable(${APP1} App1.cpp)
target_include_directories(${APP1} PRIVATE ${fleXdEpollPublicInc} ${fleXdIPCCommonPublicInc} ${fleXdUDSPublicInc} ${fleXdIPCConnectorPublicInc})
target_compile_options(${APP1} PUBLIC -std=c++14 -Wall -Wfloat-conversion)
target_link_libraries(${APP1}
    ${fleXdEpollLib}
    ${fleXdIPCCommonLib}
    ${fleXdUDSLib}
    ${fleXdIPCConnectorLib}
)
add_dependencies(${APP1} fleXdEpoll fleXdIPCCommon fleXdUDS fleXdIPCConnector)

add_executable(${APP2} App2.cpp)
target_include_directories(${APP2} PRIVATE ${fleXdEpollPublicInc} ${fleXdIPCCommonPublicInc} ${fleXdUDSPublicInc} ${fleXdIPCConnectorPublicInc})
target_compile_options(${APP2} PUBLIC -std=c++14 -Wall -Wfloat-conversion)
target_link_libraries(${APP2}
    ${fleXdEpollLib}
    ${fleXdIPCCommonLib}
    ${fleXdUDSLib}
    ${fleXdIPCConnectorLib}
)
add_dependencies(${APP2} fleXdEpoll fleXdIPCCommon fleXdUDS fleXdIPCConnector)

add_executable(${APP3} App3.cpp)
target_include_directories(${APP3} PRIVATE ${fleXdEpollPublicInc} ${fleXdIPCCommonPublicInc} ${fleXdUDSPublicInc} ${fleXdIPCConnectorPublicInc})
target_compile_options(${APP3} PUBLIC -std=c++14 -Wall -Wfloat-conversion)
target_link_libraries(${APP3}
    ${fleXdEpollLib}
    ${fleXdIPCCommonLib}
    ${fleXdUDSLib}
    ${fleXdIPCConnectorLib}
)
add_dependencies(${APP3} fleXdEpoll fleXdIPCCommon fleXdUDS fleXdIPCConnector)

add_executable(${APPGEN} AppGen.cpp)
target_include_directories(${APPGEN} PRIVATE ${fleXdEpollPublicInc} ${fleXdIPCCommonPublicInc} ${fleXdUDSPublicInc} ${fleXdIPCConnectorPublicInc})
target_compile_options(${APPGEN} PUBLIC -std=c++14 -Wall -Wfloat-conversion)
target_link_libraries(${APPGEN}
    ${fleXdEpollLib}
    ${fleXdIPCCommonLib}
    ${fleXdUDSLib}
    ${fleXdIPCConnectorLib}
)
add_dependencies(${APPGEN} fleXdEpoll fleXdIPCCommon fleXdUDS fleXdIPCConnector)
