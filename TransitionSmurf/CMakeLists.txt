CMAKE_MINIMUM_REQUIRED(VERSION 3.20)
PROJECT(TransitionSmurf CXX)

IF(TARGET TransitionSmurfLib)
	return()
ENDIF()

SET(CMAKE_CXX_STANDARD 20)

FIND_PACKAGE(CMLIB REQUIRED)

OPTION(BRINGAUTO_TESTS "Enable tests" OFF)
OPTION(STATESMURF_TESTS "Enable StateSmurf tests" ON)
OPTION(BRINGAUTO_SAMPLES "Enable build of sample applications" OFF)
OPTION(BRINGAUTO_INSTALL "Enable install feature" OFF)
OPTION(BRINGAUTO_PACKAGE "Enable package creation" OFF)
OPTION(BRINGAUTO_SYSTEM_DEP "Use system dependencies" OFF)

IF(NOT BRINGAUTO_SYSTEM_DEP)
	INCLUDE(cmake/Dependencies.cmake)
ENDIF()

FIND_PACKAGE(libbringauto_logger)

ADD_SUBDIRECTORY("../DiagramSmurf/" "DiagramSmurf/")

FILE(GLOB_RECURSE source_files "source/*")
ADD_LIBRARY(TransitionSmurfLib SHARED ${source_files})
TARGET_INCLUDE_DIRECTORIES(TransitionSmurfLib PUBLIC "include" )
TARGET_LINK_LIBRARIES(TransitionSmurfLib PUBLIC bringauto_logger::bringauto_logger DiagramSmurfLib)

IF (BRINGAUTO_SAMPLES)
	ADD_EXECUTABLE(transitionSmurf example/main.cpp)
	TARGET_LINK_LIBRARIES(transitionSmurf PUBLIC TransitionSmurfLib)
ENDIF(BRINGAUTO_SAMPLES)

IF (BRINGAUTO_TESTS AND STATESMURF_TESTS)
	ENABLE_TESTING()
	ADD_SUBDIRECTORY("test")
ENDIF(BRINGAUTO_TESTS)
