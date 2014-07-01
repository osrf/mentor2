include ( ${QT_USE_FILE} )

#include_directories(${OGRE-Terrain_INCLUDE_DIRS})
include_directories(${GAZEBO_INCLUDE_DIRS})
link_directories(${GAZEBO_LIBRARY_DIRS})

add_library(MOOCUIPlugin SHARED MOOCUIPlugin.cc)
target_link_libraries(MOOCUIPlugin ${GAZEBO_libraries})
#target_link_libraries(MOOCUIPlugin ${GAZEBO_libraries} ${OGRE_LIBRARIES})
