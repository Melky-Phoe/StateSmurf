CMAKE_MINIMUM_REQUIRED(VERSION 3.20)
PROJECT(SmurfExampleApp CXX)
SET(CMAKE_CXX_STANDARD 20)

OPTION(BRINGAUTO_SYSTEM_DEP "Use system dependencies" OFF)

IF(NOT BRINGAUTO_SYSTEM_DEP)
	FIND_PACKAGE(CMLIB
				COMPONENTS CMDEF CMUTIL STORAGE
				REQUIRED
				)
	INCLUDE(cmake/Dependencies.cmake)
ENDIF()

FIND_PACKAGE(libbringauto_logger REQUIRED)

FIND_PACKAGE(state_smurf REQUIRED)

FILE(GLOB_RECURSE source_files "source/*")
ADD_LIBRARY(smurf_example_app_lib SHARED ${source_files})
TARGET_INCLUDE_DIRECTORIES(smurf_example_app_lib PUBLIC "include")
TARGET_LINK_LIBRARIES(smurf_example_app_lib PUBLIC bringauto_logger::bringauto_logger transition_smurf_lib diagram_smurf_lib)

ADD_EXECUTABLE(smurf_example_app main.cpp)
TARGET_LINK_LIBRARIES(smurf_example_app PUBLIC smurf_example_app_lib)