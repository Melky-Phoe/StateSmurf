CMAKE_MINIMUM_REQUIRED(VERSION 3.21)
PROJECT(SmurfEvaluator CXX)

SET(CMAKE_CXX_STANDARD 20)

OPTION(BRINGAUTO_TESTS "Enable tests" OFF)
OPTION(BRINGAUTO_INSTALL "Enable install feature" OFF)
OPTION(BRINGAUTO_PACKAGE "Enable package creation" OFF)
OPTION(BRINGAUTO_SYSTEM_DEP "Use system dependencies" OFF)

IF (NOT BRINGAUTO_SYSTEM_DEP)
	FIND_PACKAGE(CMLIB
			COMPONENTS CMDEF CMUTIL STORAGE
			REQUIRED
			)
	INCLUDE(cmake/Dependencies.cmake)
ENDIF ()

FIND_PACKAGE(cxxopts 3.0.0 REQUIRED)

FIND_PACKAGE(libbringauto_logger)

FILE(GLOB_RECURSE source_files "source/*")
ADD_LIBRARY(smurf_evaluator_lib STATIC ${source_files})
TARGET_INCLUDE_DIRECTORIES(smurf_evaluator_lib PUBLIC "include")
TARGET_LINK_LIBRARIES(smurf_evaluator_lib PUBLIC cxxopts::cxxopts)

ADD_EXECUTABLE(smurf_evaluator main.cpp)
TARGET_LINK_LIBRARIES(smurf_evaluator PUBLIC smurf_evaluator_lib)

IF (BRINGAUTO_TESTS)
	ENABLE_TESTING()
	ADD_SUBDIRECTORY("test")
ENDIF ()

IF (BRINGAUTO_INSTALL)
	INSTALL(TARGETS smurf_evaluator
			DESTINATION ${CMAKE_INSTALL_BINDIR}
			EXPORT StateSmurfTargets
			)
ENDIF ()