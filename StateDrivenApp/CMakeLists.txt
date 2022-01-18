CMAKE_MINIMUM_REQUIRED(VERSION 3.20)
PROJECT(StateDrivenApp CXX)

FIND_PACKAGE(CMLIB REQUIRED)

OPTION(BRINGAUTO_TESTS "Enable tests" OFF)
OPTION(BRINGAUTO_INSTALL "Enable install feature" OFF)
OPTION(BRINGAUTO_PACKAGE "Enable package creation" OFF)
OPTION(BRINGAUTO_SYSTEM_DEP "Use system dependencies" OFF)

IF(NOT BRINGAUTO_SYSTEM_DEP)
	INCLUDE(cmake/Dependencies.cmake)
ENDIF()

SET(CMAKE_CXX_STANDARD 20)

FIND_PACKAGE(libbringauto_logger)

#ADD_SUBDIRECTORY("../StateGraph/" "StateGraph/") Can't be added, because StateTransition adds it too
ADD_SUBDIRECTORY("../StateTransition/" "StateTransition/")

FILE(GLOB_RECURSE source_files "source/*")
ADD_LIBRARY(stateDrivenAppLib SHARED ${source_files})
TARGET_INCLUDE_DIRECTORIES(stateDrivenAppLib PUBLIC "include")
TARGET_LINK_LIBRARIES(stateDrivenAppLib PUBLIC bringauto_logger::bringauto_logger stateTransitionLib)

ADD_EXECUTABLE(stateDrivenApp main.cpp)
TARGET_LINK_LIBRARIES(stateDrivenApp PUBLIC stateDrivenAppLib)

