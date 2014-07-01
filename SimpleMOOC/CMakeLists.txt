#include_directories(${OGRE-Terrain_INCLUDE_DIRS})
#include_directories(${GAZEBO_INCLUDE_DIRS} ${OGRE_INCLUDE_DIRS})
#link_directories(${GAZEBO_LIBRARY_DIRS} ${OGRE_LIBRARY_DIRS})
include_directories(${GAZEBO_INCLUDE_DIRS})
link_directories(${GAZEBO_LIBRARY_DIRS})

add_library(SimpleMOOCPlugin SHARED SimpleMOOCPlugin.cc)
target_link_libraries(SimpleMOOCPlugin ${GAZEBO_libraries})
#target_link_libraries(SimpleMOOCPlugin ${GAZEBO_libraries} ${OGRE_LIBRARIES})
