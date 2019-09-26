cmake_minimum_required(VERSION 3.10)
project(fsa_lib)

SET (CMAKE_LIBRARY_OUTPUT_DIRECTORY
        ${PROJECT_BINARY_DIR}/bin
        CACHE PATH
        "Single Directory for all"
        )
SET (CMAKE_RUNTIME_OUTPUT_DIRECTORY
        ${PROJECT_BINARY_DIR}/bin
        CACHE PATH
        "Single Directory for all"
        )

set(CMAKE_C_STANDARD 99)

enable_testing()

include_directories(include)
add_subdirectory(src)
add_subdirectory(TEST)
