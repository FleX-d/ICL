cmake_minimum_required(VERSION 3.0)
add_subdirectory(lib)

if(${ENABLE_TESTS})
    add_subdirectory(tests)
    add_subdirectory(Gtest)
endif(${ENABLE_TESTS})

