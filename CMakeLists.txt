cmake_minimum_required(VERSION 3.20)
project(State_Graph)

set(CMAKE_CXX_STANDARD 20)


FILE(GLOB_RECURSE source_files "source/*")
ADD_LIBRARY(bringautolib STATIC ${source_files} source/StateGraph.cpp include/StateGraph.h source/Vertex.cpp include/Vertex.h source/Edge.cpp include/Edge.h)
TARGET_INCLUDE_DIRECTORIES(bringautolib PUBLIC "include/")
#TARGET_LINK_LIBRARIES(bringautolib PUBLIC cxxopts::cxxopts protobuff_lib ${OSMIUM_XML_LIBRARIES} bringauto_logger::bringauto_logger)


ADD_EXECUTABLE(StateGraph main.cpp ${source_files})
TARGET_LINK_LIBRARIES(StateGraph PUBLIC bringautolib)

