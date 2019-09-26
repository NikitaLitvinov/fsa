project(fsa_test C)

set(TEST_LIB fsa)

include_directories(test_utils)
add_library(tap SHARED test_utils/tap.c)

file(GLOB TEST_SRCS *.c)
if(TEST_SRCS)
    foreach(file ${TEST_SRCS})
        get_filename_component(TEST_NAME ${file} NAME_WE)
        set(${TEST_NAME}_SRC ${TEST_NAME}.c)
        add_executable(${TEST_NAME}_test ${${TEST_NAME}_SRC})
        target_link_libraries(${TEST_NAME}_test ${TEST_LIB} tap)
        add_test(NAME ${TEST_NAME}_TEST COMMAND  ${TEST_NAME}_test )
    endforeach ()
endif()
