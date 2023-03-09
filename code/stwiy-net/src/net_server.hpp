#ifndef SF_NET_SERVER_H
#define SF_NET_SERVER_H

#include "net_event.hpp"
#include "net_socket.hpp"

#include "../third_party/kcp/ikcp.h"
#include "../third_party/concurrentqueue/concurrentqueue.h"

#include <vector>

namespace gangsta::sf_net {

    class Server {

    private:
        struct AuthRequest {
            uint32_t conv = 0;
            uint32_t key = 0;
        };

        struct Connection {
            ikcpcb *kcp = nullptr;
            uint32_t conv = 0;
            uint32_t ip = 0;
            uint16_t port = 0;
        };

        uint32_t conv_id_counter;
        std::mutex clients_mutex;
        std::mutex auth_mutex;
        std::vector<Connection> clients;
        std::vector<AuthRequest> auth_requests;
        NetSocket *socket;
        moodycamel::ConcurrentQueue<NetEvent> *in_events;
        moodycamel::ConcurrentQueue<NetEvent> *out_events;
        std::atomic<bool> is_tcp_auth_running;
        std::thread *tcp_auth_thread;

        void authentication_thread(uint16_t p_port);

        void provide_authentication(uint16_t p_port);

        void stop_provide_authentication();

        bool udp_input();

    public:
        struct KcpUser {
            Server *server = nullptr;
            Connection *connection = nullptr;
        };

        int udp_output(uint32_t p_conv,
                       uint32_t p_ip,
                       uint16_t p_port,
                       const char *p_buffer,
                       int p_buffer_length
        );

        Server();

        ~Server();

        void start(uint16_t p_auth_port, uint16_t p_data_port);

        void stop();

        void update();

        void send_event(NetEvent p_event);

        bool receive_event(NetEvent &event);
    };

}

#endif