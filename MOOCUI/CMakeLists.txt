include ( ${QT_USE_FILE} )

set (headers
  MOOCUIPlugin.hh
)

set (qt_headers
  MOOCUIWidget.hh
  MOOCLoginDialog.hh
)

set (sources
  MOOCUIWidget.cc
  MOOCUIPlugin.cc
  MOOCLoginDialog.cc
)

include_directories(
  ${GAZEBO_INCLUDE_DIRS}
  ${CMAKE_CURRENT_BINARY_DIR}/../msgs
  )

link_directories(${GAZEBO_LIBRARY_DIRS})

add_definitions(${QT_DEFINITIONS})
    

QT4_WRAP_CPP(headers_MOC ${qt_headers})
QT4_ADD_RESOURCES(resources_RCC ${resources})

add_library(MOOCUIPlugin SHARED ${sources} ${headers_MOC})
target_link_libraries(MOOCUIPlugin ${GAZEBO_libraries} SimpleMOOCMsgs)
