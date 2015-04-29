cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
enable_testing()

project (simplegdk)
include(ExternalProject)

set (SIMPLEGDK_MAJOR_VERSION 0)
set (SIMPLEGDK_MINOR_VERSION 1)
set (SIMPLEGDK_PATCH_VERSION 0)
set (SIMPLEGDK_VERSION ${SIMPLEGDK_MAJOR_VERSION}.${SIMPLEGDK_MINOR_VERSION})
set (SIMPLEGDK_VERSION_FULL ${SIMPLEGDK_MAJOR_VERSION}.${SIMPLEGDK_MINOR_VERSION}.${SIMPLEGDK_PATCH_VERSION})
message (STATUS "${PROJECT_NAME} version ${SIMPLEGDK_VERSION_FULL}")

set (simplegdk_cmake_dir ${PROJECT_SOURCE_DIR}/cmake CACHE PATH "Location of CMake scripts")

# Packaging configuration
set (CPACK_PACKAGE_VERSION "${SIMPLEGDK_VERSION_FULL}")
set (CPACK_PACKAGE_VERSION_MAJOR "${SIMPLEGDK_MAJOR_VERSION}")
set (CPACK_PACKAGE_VERSION_MINOR "${SIMPLEGDK_MINOR_VERSION}")
set (CPACK_PACKAGE_VERSION_PATCH "${SIMPLEGDK_PATCH_VERSION}")
list (APPEND CPACK_SOURCE_GENERATOR "TBZ2")
list (APPEND CPACK_SOURCE_IGNORE_FILES ";TODO;/.hg/;.hgignore;.swp$;/build/")
set (CPACK_SOURCE_PACKAGE_FILE_NAME "@PROJECT_NAME@-@SIMPLEGDK_VERSION_FULL@")
include (CPack)

# Extending the Module path with the local module path of desuto
SET(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

#####################################
# Set the default build type
if (NOT CMAKE_BUILD_TYPE)
  set (CMAKE_BUILD_TYPE "Release" CACHE STRING
    "Choose the type of build, options are: Debug Release RelWithDebInfo" FORCE)
  message(STATUS "Build type not selected: Release selected by default")
endif (NOT CMAKE_BUILD_TYPE)

# find gazebo
include (FindPkgConfig)
if (PKG_CONFIG_FOUND)
  pkg_check_modules(GAZEBO gazebo>=3.0.0 REQUIRED)
  pkg_check_modules(OGRE OGRE)
  pkg_check_modules(OGRE-Terrain OGRE-Terrain)
  #pkg_check_modules(GVC libgvc)
else()
  message(FATAL_ERROR "pkg-config is required; please install it")
endif()

include(FindBoost)
find_package(Boost ${MIN_BOOST_VERSION} REQUIRED system filesystem regex thread)
find_package(Protobuf REQUIRED)
find_package (Qt4)
if (NOT QT4_FOUND)
  MESSAGE("Qt4 not found")
endif()


include(GNUInstallDirs)

if(NOT DEFINED CMAKE_INSTALL_LIBDIR)
  set(CMAKE_INSTALL_LIBDIR lib)
endif(NOT DEFINED CMAKE_INSTALL_LIBDIR)
set(SIMPLEGDK_PLUGIN_INSTALL_DIR
  ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}/simplegdk-${SIMPLEGDK_VERSION}/plugins
)

# Generate the setup.sh file
configure_file(${CMAKE_SOURCE_DIR}/cmake/setup.sh.in ${PROJECT_BINARY_DIR}/setup.sh @ONLY)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/setup.sh DESTINATION ${CMAKE_INSTALL_PREFIX}/share/simplegdk-${SIMPLEGDK_VERSION}/)
# Also install the setup.sh in an unversioned location
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/setup.sh DESTINATION ${CMAKE_INSTALL_PREFIX}/share/simplegdk/)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GAZEBO_CXX_FLAGS} -std=c++11")

add_subdirectory(msgs)
add_subdirectory(CMLEditor)
add_subdirectory(scoring)
add_subdirectory(model_plugins)
add_subdirectory(models)

########### Add uninstall target ###############
configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/cmake/cmake_uninstall.cmake"
  IMMEDIATE @ONLY)
add_custom_target(uninstall
  "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake/cmake_uninstall.cmake")
