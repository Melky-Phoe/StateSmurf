CMAKE_MINIMUM_REQUIRED(VERSION 3.20)
PROJECT(StateSmurf CXX)

SET(CMAKE_CXX_STANDARD 20)

INCLUDE(GNUInstallDirs)

ADD_SUBDIRECTORY(DiagramSmurf)
ADD_SUBDIRECTORY(TransitionSmurf)
ADD_SUBDIRECTORY(SmurfEvaluator)
IF(BRINGAUTO_SAMPLES)
    ADD_SUBDIRECTORY(SmurfExampleApp)
ENDIF()

IF (BRINGAUTO_INSTALL)
    INSTALL(EXPORT StateSmurfTargets
            FILE StateSmurfTargets.cmake
            NAMESPACE StateSmurf::
            DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/StateSmurf
            )
    ## TODO Package here or in BRINGAUTO_PACKAGES??
    INCLUDE(CMakePackageConfigHelpers)

    CONFIGURE_PACKAGE_CONFIG_FILE(
            ${CMAKE_CURRENT_SOURCE_DIR}/cmake/StateSmurfConfig.cmake.in
            "${CMAKE_CURRENT_BINARY_DIR}/StateSmurfConfig.cmake"
            INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/StateSmurf
    )

    INSTALL(FILES
            "${CMAKE_CURRENT_BINARY_DIR}/StateSmurfConfig.cmake"
            "${CMAKE_CURRENT_BINARY_DIR}/StateSmurfConfigVersion.cmake"
            DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/StateSmurf
            )
ENDIF()