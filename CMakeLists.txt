cmake_minimum_required(VERSION 3.14)
project(fsa C)

set(CMAKE_C_STANDARD 99)
include_directories(include)

add_library(fsa SHARED src/fsa.c include/fsa/fsa.h)
