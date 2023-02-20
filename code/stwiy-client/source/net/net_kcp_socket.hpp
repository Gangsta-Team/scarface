#ifndef SF_MOD_KCP_SOCKET_H
#define SF_MOD_KCP_SOCKET_H

#include <ikcp.h>

namespace gangsta::net {

    class KcpSocket {

    private:
        ikcpcb *kcp;

        void update_server();
        void update_client();

    public:
        static constexpr int MAX_UDP_SIZE = 1024;

        KcpSocket();

        void update();

        int udp_output(const char *buf, int len);

        int udp_input(const char *data, long size);

        int send(const char *buffer, int len);

        int receive(char *buffer, int len);
    };

}

#endif