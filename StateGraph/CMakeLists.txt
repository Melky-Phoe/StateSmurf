CMAKE_MINIMUM_REQUIRED(VERSION 3.20)
PROJECT(State_Graph)

SET(CMAKE_CXX_STANDARD 20)

OPTION(BRINGAUTO_TESTS "Enable tests" ON)
#TARGET_LINK_LIBRARIES(bringautolib PUBLIC cxxopts::cxxopts protobuff_lib ${OSMIUM_XML_LIBRARIES} bringauto_logger::bringauto_logger)

IF (BRINGAUTO_TESTS)
	ENABLE_TESTING()
	ADD_SUBDIRECTORY("test")
ENDIF(BRINGAUTO_TESTS)

FILE(GLOB_RECURSE source_files "source/*")
ADD_LIBRARY(bringautolib STATIC ${source_files})
TARGET_INCLUDE_DIRECTORIES(bringautolib PUBLIC "include")

ADD_EXECUTABLE(. main.cpp)
TARGET_LINK_LIBRARIES(. PUBLIC bringautolib)

