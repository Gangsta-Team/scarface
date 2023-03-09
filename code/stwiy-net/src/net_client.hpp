#ifndef SF_NET_CLIENT_H
#define SF_NET_CLIENT_H

#include "net_event.hpp"
#include "net_socket.hpp"

#include "../third_party/kcp/ikcp.h"
#include "../third_party/concurrentqueue/concurrentqueue.h"

#include <string>
#include <cstdint>

namespace gangsta::sf_net {

    class Client {

    private:
        ikcpcb *kcp;
        uint32_t conv;
        uint32_t key;
        uint32_t ip;
        uint16_t auth_port;
        uint16_t data_port;
        NetSocket *socket;
        moodycamel::ConcurrentQueue<NetEvent> *in_events;
        moodycamel::ConcurrentQueue<NetEvent> *out_events;
        bool connected;

        void udp_input();

        bool get_authentication(const std::string &p_user,
                                const std::string &p_password,
                                const std::string &p_ip,
                                uint16_t p_port
        );

    public:
        Client();

        bool is_connected();

        void connect(const std::string &p_user,
                     const std::string &p_password,
                     const std::string &p_ip,
                     uint16_t p_auth_port,
                     uint16_t p_data_port
        );

        void update();


        int udp_output(const char *p_buffer, int p_buffer_length);

        /**
        * notify an event occurred to remote.
        * @param p_event
        */
        void send_event(NetEvent p_event);

        /**
         * check if an event is available from remote
         * @param event
         * @return
         */
        bool receive_event(NetEvent &event);
    };

}

#endif