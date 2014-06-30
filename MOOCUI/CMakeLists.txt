cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

include (FindPkgConfig)
if (PKG_CONFIG_FOUND)
  pkg_check_modules(GAZEBO gazebo)
  pkg_check_modules(OGRE OGRE)
  pkg_check_modules(OGRE-Terrain OGRE-Terrain)    
endif()   

find_package (Qt4)
if (NOT QT4_FOUND)
  BUILD_ERROR("Missing: Qt4")
endif()

include ( ${QT_USE_FILE} )
add_definitions(${QT_DEFINITIONS})


include_directories(${OGRE-Terrain_INCLUDE_DIRS})  
include_directories(${GAZEBO_INCLUDE_DIRS} ${OGRE_INCLUDE_DIRS})
link_directories(${GAZEBO_LIBRARY_DIRS} ${OGRE_LIBRARY_DIRS})

add_library(MOOCUIPlugin SHARED MOOCUIPlugin.cc)
target_link_libraries(MOOCUIPlugin ${GAZEBO_libraries} ${OGRE_LIBRARIES})
