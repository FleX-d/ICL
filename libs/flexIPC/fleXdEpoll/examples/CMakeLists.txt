cmake_minimum_required(VERSION 3.0)
project(fleXdEpollExamples VERSION "0.0.1")
set(PROJECT_DESCRIPTION "Epoll examples")

message(STATUS "***" )
message(STATUS "* " ${PROJECT_NAME})
message(STATUS "***")

set(Timer "${PROJECT_NAME}Timer")
set(Event "${PROJECT_NAME}Event")

message(STATUS "Timer: " ${Timer})
message(STATUS "Event: " ${Event})

message(STATUS "fleXdEpollPublicInc: " ${fleXdEpollPublicInc})
message(STATUS "fleXdEpollLib: " ${fleXdEpollLib})

include(GNUInstallDirs)

SET(publicHeaders
)
SET(privateHeaders
)
SET(privateSources
)

add_executable(${Timer} Timer.cpp)
target_include_directories(${Timer} PRIVATE ${fleXdEpollPublicInc})
target_compile_options(${Timer} PUBLIC -std=c++14 -Wall -Wfloat-conversion)
target_link_libraries(${Timer}
    ${fleXdEpollLib}
)
add_dependencies(${Timer} fleXdEpoll)

add_executable(${Event} Event.cpp)
target_include_directories(${Event} PRIVATE ${fleXdEpollPublicInc})
target_compile_options(${Event} PUBLIC -std=c++14 -Wall -Wfloat-conversion)
target_link_libraries(${Event}
    ${fleXdEpollLib}
)
add_dependencies(${Event} fleXdEpoll)
