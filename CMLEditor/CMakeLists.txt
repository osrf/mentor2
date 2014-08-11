#add_subdirectory(QGVCore)

include ( ${QT_USE_FILE} )

link_directories(
  ${Boost_LIBRARY_DIRS}
)

set (sources
  CMLEdge.cc
  CMLEditor.cc
  CMLEditorItem.cc
  CMLEditorPalette.cc
  CMLEditorPlugin.cc
  CMLEditorScene.cc
  CMLEditorView.cc
  CMLEditorWidget.cc
  CMLManager.cc
  CMLNode.cc
  CMLPortInspector.cc
  CMLRender.cc
  GrabberHandle.cc
  RectItem.cc
  RotateHandle.cc
)

set (qt_headers
  CMLEditor.hh
  CMLEditorItem.hh
  CMLEditorPalette.hh
  CMLEditorScene.hh
  CMLEditorView.hh
  CMLPortInspector.hh
  RectItem.hh
)

set (headers
  CMLEdge.hh
  CMLEditorPlugin.hh
  CMLEditorWidget.hh
  CMLManager.hh
  CMLNode.hh
  CMLRender.hh
  GrabberHandle.hh
  RotateHandle.hh
)

set (resources resources.qrc)

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
#    qgvcore
    SimpleMsgs
    ${GAZEBO_libraries}
    ${OGRE_LIBRARIES})
