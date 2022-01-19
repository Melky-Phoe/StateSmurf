CMAKE_MINIMUM_REQUIRED(VERSION 3.20)
PROJECT(StateTransitionLogEvaluator)

SET(CMAKE_CXX_STANDARD 20)

FIND_PACKAGE(GTest REQUIRED)

INCLUDE_DIRECTORIES(${GTEST_INCLUDE_DIRS} "include")
FILE(GLOB_RECURSE source_test_files "${CMAKE_CURRENT_LIST_DIR}/source/*" )
ADD_EXECUTABLE(state_transition_log_evaluator_test ${source_test_files} ${CMAKE_CURRENT_LIST_DIR}/mainTests.cpp)
TARGET_INCLUDE_DIRECTORIES(state_transition_log_evaluator_test PUBLIC "${CMAKE_CURRENT_LIST_DIR}/include/")
TARGET_LINK_LIBRARIES(state_transition_log_evaluator_test PUBLIC ${GTEST_LIBRARIES} pthread evaluatorLib)
INCLUDE(GoogleTest)
GTEST_DISCOVER_TESTS(state_transition_log_evaluator_test)
