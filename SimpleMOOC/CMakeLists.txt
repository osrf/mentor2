include_directories(${GAZEBO_INCLUDE_DIRS})
link_directories(${GAZEBO_LIBRARY_DIRS})

add_library(SimpleMOOCPlugin SHARED SimpleMOOCPlugin.cc)
target_link_libraries(SimpleMOOCPlugin ${GAZEBO_libraries})
