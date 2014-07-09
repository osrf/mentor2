
find_package(Protobuf REQUIRED)

set (msgs SimpleMOOC.proto)

PROTOBUF_GENERATE_CPP(PROTO_SRCS PROTO_HDRS ${msgs})

add_library(SimpleMOOCMsgs SHARED ${PROTO_SRCS})
target_link_libraries(SimpleMOOCMsgs ${PROTOBUF_LIBRARY})

