CMAKE_MINIMUM_REQUIRED(VERSION 3.20)
PROJECT(SmurfExampleApp CXX)

SET(CMAKE_CXX_STANDARD 20)

FIND_PACKAGE(CMLIB REQUIRED)

OPTION(BRINGAUTO_TESTS "Enable tests" OFF)
OPTION(STATESMURF_TESTS "Enable StateSmurf tests" ON)
OPTION(BRINGAUTO_INSTALL "Enable install feature" OFF)
OPTION(BRINGAUTO_PACKAGE "Enable package creation" OFF)
OPTION(BRINGAUTO_SYSTEM_DEP "Use system dependencies" OFF)

IF(NOT BRINGAUTO_SYSTEM_DEP)
	INCLUDE(cmake/Dependencies.cmake)
ENDIF()

FIND_PACKAGE(libbringauto_logger)

ADD_SUBDIRECTORY(../TransitionSmurf TransitionSmurf)
FILE(GLOB_RECURSE source_files "source/*")
ADD_LIBRARY(SmurfExampleAppLib SHARED ${source_files})
TARGET_INCLUDE_DIRECTORIES(SmurfExampleAppLib PUBLIC "include")
TARGET_LINK_LIBRARIES(SmurfExampleAppLib PUBLIC bringauto_logger::bringauto_logger TransitionSmurfLib DiagramSmurfLib)

ADD_EXECUTABLE(smurfExampleApp main.cpp)
TARGET_LINK_LIBRARIES(smurfExampleApp PUBLIC SmurfExampleAppLib)

IF(BRINGAUTO_TESTS AND STATESMURF_TESTS)
	ENABLE_TESTING()
	FIND_PACKAGE(Python3 COMPONENTS Interpreter Development)
	ADD_TEST(NAME automatedEvaluatorTests COMMAND ${Python3_EXECUTABLE} ${CMAKE_SOURCE_DIR}/../SmurfTestingScripts/CompareScenarios.py
			-s ${CMAKE_SOURCE_DIR}/test/smurf/scenarios.json
			-e ${CMAKE_BINARY_DIR}/smurfExampleApp
			--evaluator ${CMAKE_SOURCE_DIR}/../SmurfEvaluator/_build/smurfEvaluator
			--output ${CMAKE_SOURCE_DIR}/test/smurf/
			#-c
			)
ENDIF()