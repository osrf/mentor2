include ( ${QT_USE_FILE} )

link_directories(
  ${Boost_LIBRARY_DIRS}
)

set (sources
  CMLComponentInspector.cc
  CMLConnectionMaker.cc
  CMLEditor.cc
  CMLEditorPlugin.cc
  CMLEvents.cc
  CMLManager.cc
  CMLPortInspector.cc
  CMLPropertyManager.cc
  CMLRender.cc
)

set (qt_headers
  CMLComponentInspector.hh
  CMLEditor.hh
  CMLPortInspector.hh
  CMLPropertyManager.hh
  CMLRender.hh
)

set (headers
  CMLConnectionMaker.hh
  CMLEditorPlugin.hh
  CMLEvents.hh
  CMLManager.hh
)

set (resources cmlresources.qrc)

add_definitions(${QT_DEFINITIONS})

QT4_WRAP_CPP(headers_MOC ${qt_headers})
QT4_ADD_RESOURCES(resources_RCC ${resources})

include_directories(
  ${GAZEBO_INCLUDE_DIRS}
  ${OGRE_INCLUDE_DIRS}
  ${OGRE-Terrain_INCLUDE_DIRS}
  ${CMAKE_CURRENT_BINARY_DIR}/../msgs
)

#include_directories(${CMAKE_CURRENT_SOURCE_DIR}/QGVCore)

link_directories(${GAZEBO_LIBRARY_DIRS} ${OGRE_LIBRARY_DIRS})

add_library(CMLEditorPlugin SHARED ${sources} ${headers_MOC} ${headers} ${resources_RCC})
target_link_libraries(CMLEditorPlugin
    SimpleMsgs
    ${GAZEBO_LIBRARIES}
    ${OGRE_LIBRARIES})
install (TARGETS CMLEditorPlugin DESTINATION ${SIMPLEGDK_PLUGIN_INSTALL_DIR})
