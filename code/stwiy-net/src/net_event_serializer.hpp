#ifndef SF_NET_EVENT_SERIALIZER_H
#define SF_NET_EVENT_SERIALIZER_H

#include "net_event.hpp"
#include "net_buffer.hpp"

namespace gangsta::sf_net {

    class NetEventSerializer {

    public:
        static void serialize_net_event(Buffer &buffer, NetEvent &event);

        static void deserialize_net_event(Buffer &buffer, NetEvent &event);
    };

}

#endif