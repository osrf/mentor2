add_definitions(-DWITH_CGRAPH)

include ( ${QT_USE_FILE} )

SET(sources
  private/QGVCore.cpp
  private/QGVGraphPrivate.cpp
  private/QGVEdgePrivate.cpp
  private/QGVGvcPrivate.cpp
  private/QGVNodePrivate.cpp
  QGVEdge.cpp
  QGVNode.cpp
  QGVScene.cpp
  QGVSubGraph.cpp
)

SET(qt_headers
  QGVScene.h
)

SET(headers
  qgv.h
  QGVEdge.h
  QGVNode.h
  QGVSubGraph.h
)

add_definitions(${QT_DEFINITIONS})

QT4_WRAP_CPP(headers_MOC ${qt_headers})

include_directories(${GRAPHVIZ_GVC_PKG_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/private)

add_library(qgvcore SHARED ${sources} ${headers_MOC} ${headers} ${resources_RCC})

target_link_libraries(qgvcore
  ${QT_LIBRARIES}
  ${GRAPHVIZ_GVC_PKG_LIBRARIES}
  ${GRAPHVIZ_GVC_LIBRARY}
  ${GRAPHVIZ_CGRAPH_LIBRARY}
  ${GRAPHVIZ_CDT_LIBRARY}
)
