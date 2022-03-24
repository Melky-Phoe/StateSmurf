CMAKE_MINIMUM_REQUIRED(VERSION 3.20)
PROJECT(TransitionSmurf)

SET(CMAKE_CXX_STANDARD 20)

FIND_PACKAGE(GTest REQUIRED)

INCLUDE_DIRECTORIES(${GTEST_INCLUDE_DIRS} "include")
FILE(GLOB_RECURSE source_test_files "${CMAKE_CURRENT_LIST_DIR}/source/*" )
ADD_EXECUTABLE(transition_smurf_tests ${source_test_files} ${CMAKE_CURRENT_LIST_DIR}/mainTests.cpp)
TARGET_INCLUDE_DIRECTORIES(transition_smurf_tests PUBLIC "${CMAKE_CURRENT_LIST_DIR}/include")
TARGET_LINK_LIBRARIES(transition_smurf_tests PUBLIC ${GTEST_LIBRARIES} pthread transition_smurf_lib)
INCLUDE(GoogleTest)
GTEST_DISCOVER_TESTS(transition_smurf_tests)
