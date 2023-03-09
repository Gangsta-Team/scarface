#include "net_event_serializer.hpp"

void gangsta::sf_net::NetEventSerializer::deserialize_net_event(
        gangsta::sf_net::Buffer &buffer,
        gangsta::sf_net::NetEvent &event
) {
    event.type = (NetEventType) buffer.read_u8();
}

void gangsta::sf_net::NetEventSerializer::serialize_net_event(
        gangsta::sf_net::Buffer &buffer,
        gangsta::sf_net::NetEvent &event
) {
    buffer.write_u8(event.type);
}

