cmake_minimum_required(VERSION 3.20)

project(MiniChronos
        VERSION 0.1
        DESCRIPTION "A simple profiler for simple needs"
        LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 20)

# Initialize tests with Google Test
enable_testing()

# Submodules
find_package(Git QUIET)
if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
    # Update submodules as needed
    option(GIT_SUBMODULE "Check submodules during build" ON)

    if(GIT_SUBMODULE)
        message(STATUS "Submodule update")
        execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                RESULT_VARIABLE GIT_SUBMOD_RESULT)
        if(NOT GIT_SUBMOD_RESULT EQUAL "0")
            message(FATAL_ERROR "git submodule update --init failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
        endif()
    endif()
endif()

if(NOT EXISTS "${PROJECT_SOURCE_DIR}/extern/googletest/CMakeLists.txt")
    message(FATAL_ERROR "The submodules were not downloaded! GIT_SUBMODULE was turned off or failed. Please update submodules and try again.")
endif()

add_subdirectory(extern/googletest)

# Library
set(MINI_CHRONOS_LIB_NAME mini_chronos)
set(MINI_CHRONOS_TEST_NAME ${MINI_CHRONOS_LIB_NAME}_tests)

set(MINI_CHRONOS_LIB_FILES
        src/mini_chronos.h src/mini_chronos.cpp
        src/database.h src/database.cpp
        src/timer.h src/timer.cpp
        src/error_handler.h src/error_handler.cpp)

add_library(${MINI_CHRONOS_LIB_NAME} ${MINI_CHRONOS_LIB_FILES})

set(MINI_CHRONOS_TEST_FILES
        tests/mini_chronos_tests.cpp)

add_executable(${MINI_CHRONOS_TEST_NAME} ${MINI_CHRONOS_TEST_FILES})
target_include_directories(${MINI_CHRONOS_TEST_NAME} PRIVATE src/)
target_link_libraries(${MINI_CHRONOS_TEST_NAME}
        PRIVATE
        ${MINI_CHRONOS_LIB_NAME}
        GTest::gtest
        GTest::gmock
        GTest::gtest_main)
