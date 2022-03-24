CMAKE_MINIMUM_REQUIRED(VERSION 3.20)
PROJECT(StateSmurf CXX)

SET(CMAKE_CXX_STANDARD 20)

SET(state_smurf_VERSION 0.2.0)
INCLUDE(GNUInstallDirs)

ADD_SUBDIRECTORY(DiagramSmurf)
ADD_SUBDIRECTORY(TransitionSmurf)
ADD_SUBDIRECTORY(SmurfEvaluator)
IF(BRINGAUTO_SAMPLES)
    ADD_SUBDIRECTORY(SmurfExampleApp)
ENDIF()

IF (BRINGAUTO_INSTALL)
    INSTALL(EXPORT StateSmurfTargets
            FILE state_smurfTargets.cmake
            NAMESPACE state_smurf::
            DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/state_smurf
            )
    INCLUDE(CMakePackageConfigHelpers)

    CONFIGURE_PACKAGE_CONFIG_FILE(
            ${CMAKE_CURRENT_SOURCE_DIR}/cmake/state_smurfConfig.cmake.in
            "${CMAKE_CURRENT_BINARY_DIR}/state_smurfConfig.cmake"
            INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/state_smurf
    )
    WRITE_BASIC_PACKAGE_VERSION_FILE(
            "${CMAKE_CURRENT_BINARY_DIR}/state_smurf/state_smurfConfigVersion.cmake"
            VERSION ${state_smurf_VERSION}
            COMPATIBILITY SameMajorVersion
    )

    EXPORT(EXPORT StateSmurfTargets
            FILE "${CMAKE_CURRENT_BINARY_DIR}/state_smurf/state_smurfTargets.cmake"
            )

    SET(ConfigPackageLocation lib/cmake/state_smurf)
    INSTALL(EXPORT StateSmurfTargets
            FILE
            state_smurfTargets.cmake
            NAMESPACE
            state_smurf::
            DESTINATION
            ${ConfigPackageLocation}
            )

    INSTALL(FILES
            "${CMAKE_CURRENT_BINARY_DIR}/state_smurfConfig.cmake"
            "${CMAKE_CURRENT_BINARY_DIR}/state_smurfConfigVersion.cmake"
            DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/state_smurf
            )

ENDIF()