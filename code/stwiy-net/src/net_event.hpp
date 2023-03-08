#ifndef SF_NET_EVENT_H
#define SF_NET_EVENT_H


#include <cstdint>
#include <string>

namespace gangsta::sf_net {

    enum NetEventType : int {
        CharacterSpawn = 0,
    };

    struct NetEvent {
        uint32_t conv;
        uint32_t ip;
        uint32_t port;
        NetEventType type;

        std::string to_string() {
            return
                    "Conv:" + std::to_string(conv) + ", " +
                    "Ip:" + std::to_string(ip) + ", " +
                    "Port:" + std::to_string(port) + ", " +
                    "Type:" + std::to_string(type) + "\r\n";
        }
    };

}

#endif