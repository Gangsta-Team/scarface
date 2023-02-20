#include "net_kcp_socket.hpp"

#include <cstdint>
#include <chrono>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

int udp_output_cb(const char *buf, int len, ikcpcb *kcp, void *user) {
    gangsta::net::KcpSocket *server = (gangsta::net::KcpSocket *) user;
    return server->udp_output(buf, len);
}

int gangsta::net::KcpSocket::udp_output(const char *buf, int len) {
    // TODO write to udp socket
    return 0;
}

void gangsta::net::KcpSocket::update() {
    uint64_t now_ms = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::steady_clock::now().time_since_epoch()).count();
    ikcp_update(kcp, now_ms);
}

int gangsta::net::KcpSocket::udp_input(const char *data, long size) {
    // TODO read from udp socket
    return ikcp_input(kcp, data, size);
}

int gangsta::net::KcpSocket::send(const char *buffer, int len) {
    return ikcp_send(kcp, buffer, len);
}

int gangsta::net::KcpSocket::receive(char *buffer, int len) {
    return ikcp_recv(kcp, buffer, len);
}


SOCKET server_socket; // TODO
struct sockaddr_in server_addr; // TODO

void gangsta::net::KcpSocket::update_server() {
    // I'm a server and recv data from multiple clients
    // TODO
}


void gangsta::net::KcpSocket::update_client() {
    // I'm a client and only recv data from server
    // TODO

    sockaddr_in addr{};

    int buffer_length = MAX_UDP_SIZE;
    char buffer[MAX_UDP_SIZE];
    int from_length = sizeof(sockaddr);
    int result = recvfrom(server_socket, buffer, buffer_length, 0, (sockaddr*)&addr, &from_length);
    if (result == SOCKET_ERROR) {
        fprintf(stderr, "result == SOCKET_ERROR\n");
    }
}




void initialize_socket(SOCKET* p_socket, u_short p_port, struct sockaddr_in *p_addr, bool p_send) {

    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    *p_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    p_addr->sin_family = AF_INET;
    p_addr->sin_port = htons(p_port);
    if (send) {
        p_addr->sin_addr.s_addr = htonl(INADDR_LOOPBACK);
        return;
    }
    p_addr->sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    if (bind(*p_socket, (sockaddr*)p_addr, sizeof(sockaddr)) < 0) {
        fprintf(stderr, "error binding socket\n");
        closesocket(*p_socket);
        return;
    }
}


gangsta::net::KcpSocket::KcpSocket() {
    kcp = ikcp_create(1, this);
    kcp->output = udp_output_cb;

    initialize_socket(&server_socket, 0, &server_addr, true);
}
