CMAKE_MINIMUM_REQUIRED(VERSION 3.21)
PROJECT(StateSmurf CXX)

SET(CMAKE_CXX_STANDARD 20)

SET(state_smurf_VERSION 2.2.0)
INCLUDE(GNUInstallDirs)

OPTION(BRINGAUTO_SAMPLES "Enable build of sample application" OFF)
OPTION(BRINGAUTO_TESTS "Enable tests" OFF)
OPTION(BRINGAUTO_INSTALL "Enable install" OFF)
OPTION(BRINGAUTO_PACKAGE "Enable package generation" OFF)
OPTION(BRINGAUTO_SYSTEM_DEP "Enable system dependencies" OFF)

IF (BRINGAUTO_PACKAGE)
    IF(NOT BRINGAUTO_INSTALL)
        SET(BRINGAUTO_INSTALL ON CACHE BOOL "Forced install due to BRINGAUTO_PACKAGE=ON" FORCE)
        MESSAGE(WARNING "BRINGAUTO_INSTALL is switched to on because of BRINGAUTO_PACKAGE=ON")
    ENDIF()
ENDIF()

SET(CMAKE_INSTALL_RPATH $ORIGIN/../lib/)

ADD_SUBDIRECTORY(DiagramSmurf)
ADD_SUBDIRECTORY(TransitionSmurf)
ADD_SUBDIRECTORY(SmurfEvaluator)
# ExampleApp cannot be added to build by BRINGAUTO_SAMPLES, because it uses FIND_PACKAGE to find StateSmurf,
# which serves its main purpose to show how StateSmurf can be used

IF (BRINGAUTO_TESTS)
    ENABLE_TESTING()
ENDIF()

IF (BRINGAUTO_INSTALL)

    INSTALL(EXPORT StateSmurfTargets
            FILE state_smurfTargets.cmake
            DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/state_smurf
            )
    INCLUDE(CMakePackageConfigHelpers)

    CONFIGURE_PACKAGE_CONFIG_FILE(
            ${CMAKE_CURRENT_SOURCE_DIR}/cmake/state_smurfConfig.cmake.in
            "${CMAKE_CURRENT_BINARY_DIR}/state_smurf/state_smurfConfig.cmake"
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
            DESTINATION
            ${ConfigPackageLocation}
            )

    INSTALL(FILES
            "${CMAKE_CURRENT_BINARY_DIR}/state_smurf/state_smurfConfig.cmake"
            "${CMAKE_CURRENT_BINARY_DIR}/state_smurf/state_smurfConfigVersion.cmake"
            DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/state_smurf
            )

    INSTALL(FILES
            "${CMAKE_SOURCE_DIR}/SmurfTestingScripts/smurf_compare_scenarios.py"
            DESTINATION bin)

ENDIF()

IF (BRINGAUTO_PACKAGE)
    SET(CPACK_GENERATOR ZIP)
    SET(CPACK_PACKAGE_CONTACT "Josef Melkus <josef.melkus@bringauto.com>")
    SET(CPACK_DEBIAN_PACKAGE_MAINTAINER "Josef Melkus <josef.melkus@bringauto.com>")
    SET(CPACK_PACKAGE_VERSION ${state_smurf_VERSION})
    INCLUDE(CPack)
ENDIF()
