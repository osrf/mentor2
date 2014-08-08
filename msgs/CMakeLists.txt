
find_package(Protobuf REQUIRED)

set (msgs
  SimpleModel.proto
  SimpleMOOC.proto
)

PROTOBUF_GENERATE_CPP(PROTO_SRCS PROTO_HDRS ${msgs})

add_library(SimpleMsgs SHARED ${PROTO_SRCS})
target_link_libraries(SimpleMsgs ${PROTOBUF_LIBRARY})

