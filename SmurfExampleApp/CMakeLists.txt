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
ADD_LIBRARY(smurf_example_app_lib SHARED ${source_files})
TARGET_INCLUDE_DIRECTORIES(smurf_example_app_lib PUBLIC "include")
TARGET_LINK_LIBRARIES(smurf_example_app_lib PUBLIC bringauto_logger::bringauto_logger transition_smurf_lib diagram_smurf_lib)

ADD_EXECUTABLE(smurf_example_app main.cpp)
TARGET_LINK_LIBRARIES(smurf_example_app PUBLIC smurf_example_app_lib)

IF(BRINGAUTO_TESTS AND STATESMURF_TESTS)
	ENABLE_TESTING()
	FIND_PACKAGE(Python3 COMPONENTS Interpreter Development)
	SET(STATESMURF_DIR ${CMAKE_SOURCE_DIR}/..)
	ADD_TEST(NAME automatedEvaluatorTests COMMAND ${Python3_EXECUTABLE} ${STATESMURF_DIR}/SmurfTestingScripts/CompareScenarios.py
			--scenario ${CMAKE_SOURCE_DIR}/test/smurf/scenarios.json
			--executable ${CMAKE_BINARY_DIR}/smurf_example_app
			--evaluator ${STATESMURF_DIR}/SmurfEvaluator/_build/smurf_evaluator
			--output ${CMAKE_SOURCE_DIR}/test/smurf/
			#--create-etalons
			)
ENDIF()
