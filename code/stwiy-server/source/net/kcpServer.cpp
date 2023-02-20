/*#include "kcpServer.hpp"

#include <cstdint>
#include <chrono>

int udp_output_cb(const char *buf, int len, ikcpcb *kcp, void *user) {
    gangsta::net::KcpServer *server = (gangsta::net::KcpServer *) user;
    return server->udp_output(buf, len);
}

gangsta::net::KcpServer::KcpServer() {
    kcp = ikcp_create(1, this);
    kcp->output = udp_output_cb;
}

int gangsta::net::KcpServer::udp_output(const char *buf, int len) {
    // TODO write to udp socket
    return 0;
}

void gangsta::net::KcpServer::update() {
    uint64_t now_ms = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::steady_clock::now().time_since_epoch()).count();
    ikcp_update(kcp, now_ms);
}

int gangsta::net::KcpServer::udp_input(const char *data, long size) {
    // TODO read from udp socket
    return ikcp_input(kcp, data, size);
}

*/