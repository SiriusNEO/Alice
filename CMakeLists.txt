cmake_minimum_required(VERSION 3.18)
project(alice CXX)

set(CMAKE_CXX_STANDARD 14)

######################## File Globbin ########################

set(SRC_DIR src)
set(ENTRY_FILE ${SRC_DIR}/alice.cpp)
set(TEST_FILE ${SRC_DIR}/alice_test.cpp)

file(GLOB_RECURSE SOURCES ${SRC_DIR}/ltl/*.cpp
                          ${SRC_DIR}/frontend/*.cpp
                          ${SRC_DIR}/ts/*.cpp)

######################## Target ########################

add_executable(${PROJECT_NAME} ${SOURCES} ${ENTRY_FILE})
target_compile_options(${PROJECT_NAME} PUBLIC -O2)
target_include_directories(${PROJECT_NAME} PUBLIC ${SRC_DIR})

######################## Test ########################

set(TEST_PROJECT_NAME alice_test)

add_executable(${TEST_PROJECT_NAME} ${SOURCES} ${TEST_FILE})
target_compile_options(${TEST_PROJECT_NAME} PUBLIC -O2)
target_include_directories(${TEST_PROJECT_NAME} PUBLIC ${SRC_DIR})
