cmake_minimum_required(VERSION 3.0)
project(jsonParserTests VERSION "0.0.1")

message(STATUS "jsonParserPublicInc: " ${jsonParserPublicInc})
message(STATUS "jsonParserLib: " ${jsonParserLib})
include_directories(${jsonParserPublicInc})

add_executable(example example.cpp)
target_link_libraries(example
    ${jsonParserLib}
)
add_dependencies(example jsonParser)


add_executable(example1 example1.cpp)
target_link_libraries(example1
    ${jsonParserLib}
)
add_dependencies(example1 jsonParser)
