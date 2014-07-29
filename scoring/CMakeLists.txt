link_directories(
  ${GAZEBO_LIBRARY_DIRS}
  ${CMAKE_CURRENT_BINARY_DIR}
  )

set (src
  ScoringPlugin.cc
  )

include_directories(
  ${GAZEBO_INCLUDE_DIRS}
  ${CMAKE_CURRENT_BINARY_DIR}/../msgs
  )

add_library(ScoringPlugin SHARED ${src} )
target_link_libraries(ScoringPlugin ${GAZEBO_libraries} SimpleMOOCMsgs)

