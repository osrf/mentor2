cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
enable_testing()

project (gdk)
include(ExternalProject)

set (GDK_MAJOR_VERSION 0)
set (GDK_MINOR_VERSION 1)
set (GDK_PATCH_VERSION 0)
set (GDK_VERSION ${GDK_MAJOR_VERSION}.${GDK_MINOR_VERSION})
set (GDK_VERSION_FULL ${GDK_MAJOR_VERSION}.${GDK_MINOR_VERSION}.${GDK_PATCH_VERSION})
message (STATUS "${PROJECT_NAME} version ${GDK_VERSION_FULL}")

set (gdk_cmake_dir ${PROJECT_SOURCE_DIR}/cmake CACHE PATH "Location of CMake scripts")

# Packaging configuration
set (CPACK_PACKAGE_VERSION "${GDK_VERSION_FULL}")
set (CPACK_PACKAGE_VERSION_MAJOR "${GDK_MAJOR_VERSION}")
set (CPACK_PACKAGE_VERSION_MINOR "${GDK_MINOR_VERSION}")
set (CPACK_PACKAGE_VERSION_PATCH "${GDK_PATCH_VERSION}")
list (APPEND CPACK_SOURCE_GENERATOR "TBZ2")
list (APPEND CPACK_SOURCE_IGNORE_FILES ";TODO;/.hg/;.hgignore;.swp$;/build/")
set (CPACK_SOURCE_PACKAGE_FILE_NAME "@PROJECT_NAME@-@GDK_VERSION_FULL@")
include (CPack)


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

else()
  message(FATAL_ERROR "pkg-config is required; please install it")
endif()

include(FindBoost)
find_package(Boost ${MIN_BOOST_VERSION} REQUIRED system filesystem regex thread)
find_package(Protobuf REQUIRED)
find_package (Qt4)
if (NOT QT4_FOUND)
  BUILD_ERROR("Missing: Qt4")
endif()

add_subdirectory(CMLEditor)
add_subdirectory(SimpleMOOC)
add_subdirectory(MOOCUI)


########### Add uninstall target ###############
configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/cmake/cmake_uninstall.cmake"
  IMMEDIATE @ONLY)
add_custom_target(uninstall
  "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake/cmake_uninstall.cmake")
