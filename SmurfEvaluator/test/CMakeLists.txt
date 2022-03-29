CMAKE_MINIMUM_REQUIRED(VERSION 3.20)
PROJECT(StateTransitionLogEvaluator)

SET(CMAKE_CXX_STANDARD 20)

FIND_PACKAGE(GTest REQUIRED)

INCLUDE_DIRECTORIES(${GTEST_INCLUDE_DIRS} "include")
FILE(GLOB_RECURSE source_test_files "${CMAKE_CURRENT_LIST_DIR}/source/*" )
ADD_EXECUTABLE(smurf_evaluator_tests ${source_test_files} ${CMAKE_CURRENT_LIST_DIR}/mainTests.cpp)
TARGET_INCLUDE_DIRECTORIES(smurf_evaluator_tests PUBLIC "${CMAKE_CURRENT_LIST_DIR}/include/")
TARGET_LINK_LIBRARIES(smurf_evaluator_tests PUBLIC ${GTEST_LIBRARIES} pthread smurf_evaluator_lib)
INCLUDE(GoogleTest)
GTEST_DISCOVER_TESTS(smurf_evaluator_tests)

ADD_CUSTOM_COMMAND(TARGET smurf_evaluator_tests PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_LIST_DIR}/testLogs ${CMAKE_CURRENT_BINARY_DIR}/testLogs)
