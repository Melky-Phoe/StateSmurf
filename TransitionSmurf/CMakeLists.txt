CMAKE_MINIMUM_REQUIRED(VERSION 3.21)
PROJECT(TransitionSmurf CXX)

IF (TARGET transition_smurf_lib)
	return()
ENDIF ()

SET(CMAKE_CXX_STANDARD 20)

SET(CMAKE_POSITION_INDEPENDENT_CODE  ON)

OPTION(BRINGAUTO_TESTS "Enable tests" OFF)
OPTION(BRINGAUTO_SAMPLES "Enable build of sample applications" OFF)
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

FIND_PACKAGE(libbringauto_logger REQUIRED COMPONENTS bringauto_logger_spdlog)

FILE(GLOB_RECURSE source_files "source/*")
ADD_LIBRARY(transition_smurf_lib STATIC ${source_files})
TARGET_INCLUDE_DIRECTORIES(transition_smurf_lib PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>")
TARGET_LINK_LIBRARIES(transition_smurf_lib PUBLIC bringauto_logger::bringauto_logger_spdlog diagram_smurf_lib)

IF (BRINGAUTO_SAMPLES)
	ADD_EXECUTABLE(transition_smurf example/main.cpp)
	TARGET_LINK_LIBRARIES(transition_smurf PUBLIC transition_smurf_lib)
ENDIF (BRINGAUTO_SAMPLES)

IF (BRINGAUTO_TESTS)
	ENABLE_TESTING()
	ADD_SUBDIRECTORY("test")
ENDIF ()

IF (BRINGAUTO_INSTALL)
	INCLUDE(GNUInstallDirs)

	TARGET_INCLUDE_DIRECTORIES(transition_smurf_lib
			PUBLIC
			"$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
			)
	INSTALL(TARGETS transition_smurf_lib
			EXPORT StateSmurfTargets
			LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
			ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
			RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
			INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
			)

	INSTALL(
			DIRECTORY
			${CMAKE_CURRENT_SOURCE_DIR}/include/
			DESTINATION
			${CMAKE_INSTALL_INCLUDEDIR}/state_smurf
	)

ENDIF ()