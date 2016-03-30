link_directories(
  ${GAZEBO_LIBRARY_DIRS}
  ${CMAKE_CURRENT_BINARY_DIR}
  )

set (plugins
  GearboxPlugin
  MotorPlugin
  PowerSourcePlugin
  SwitchPlugin
  WheelPlugin
  SimpleConnectionsPlugin
  TugOfWarPlugin
)

include_directories(
  ${GAZEBO_INCLUDE_DIRS}
  ${CMAKE_CURRENT_BINARY_DIR}/../msgs
)

foreach (src ${plugins})
  add_library(${src} SHARED ${src}.cc SimpleModelPlugin.cc)
  target_link_libraries(${src} ${GAZEBO_libraries} SimpleMsgs)
  install (TARGETS ${src} DESTINATION ${SIMPLEGDK_PLUGIN_INSTALL_DIR})
  #gz_install_includes("plugins" ${src}.hh)
endforeach (src ${plugins})
