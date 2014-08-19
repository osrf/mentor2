
find_package(Protobuf REQUIRED)

set (msgs
  SimpleConnection.proto
  SimpleModel.proto
  Event.proto
)

PROTOBUF_GENERATE_CPP(PROTO_SRCS PROTO_HDRS ${msgs})

add_library(SimpleMsgs SHARED ${PROTO_SRCS})
target_link_libraries(SimpleMsgs ${PROTOBUF_LIBRARY})
install (TARGETS SimpleMsgs DESTINATION ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR})
