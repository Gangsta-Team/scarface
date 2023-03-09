#include "net_socket.hpp"

#include <WinSock2.h>
#include <ws2tcpip.h>
#include <stdexcept>

#pragma comment(lib, "ws2_32.lib")

struct UserDataWin {
    WORD version_requested = MAKEWORD(2, 2);
    WSADATA wsa_data = {};
    bool wsa_initialized = false;
    SOCKET socket = 0;
};

gangsta::sf_net::NetSocket::NetSocket() {
    // The WSAStartup function typically leads to protocol-specific helper DLLs being loaded.
    // As a result, the WSAStartup function should not be called from the DllMain function in a application DLL.
    // This can potentially cause deadlocks. For more information, please see the DLL Main Function.
    status = {};
    user = new UserDataWin();
    UserDataWin *usr = static_cast<UserDataWin *>(user);
    int ret = WSAStartup(
            usr->version_requested,
            &usr->wsa_data
    );
    if (ret == 0) {
        usr->wsa_initialized = true;
    } else {
        // error
    }
}

gangsta::sf_net::NetSocket::~NetSocket() {
    // An application must call the WSACleanup function for every successful time the WSAStartup function is called.
    // This means, for example, that if an application calls WSAStartup three times, it must call WSACleanup three times.
    // The first two calls to WSACleanup do nothing except decrement an internal counter; the final WSACleanup call for the task does all necessary resource deallocation for the task.
    UserDataWin *usr = static_cast<UserDataWin *>(user);
    if (usr->wsa_initialized) {
        WSACleanup();
    }
}

bool gangsta::sf_net::NetSocket::set_option(int p_level, int p_option, bool p_value) {
    UserDataWin *usr = static_cast<UserDataWin *>(user);
    BOOL opt_val = p_value;
    int opt_val_len = sizeof(BOOL);
    int ret = setsockopt(usr->socket, p_level, p_option, (char *) &opt_val, opt_val_len);
    if (ret < 0) {
        return false;
    }
    return true;
}

bool gangsta::sf_net::NetSocket::set_keep_alive(bool p_value) {
    return set_option(SOL_SOCKET, SO_KEEPALIVE, p_value);
}

bool gangsta::sf_net::NetSocket::set_re_use_address(bool p_value) {
    return set_option(SOL_SOCKET, SO_REUSEADDR, p_value);
}

bool gangsta::sf_net::NetSocket::set_non_blocking(bool p_value) {
    UserDataWin *usr = static_cast<UserDataWin *>(user);
    unsigned long ul = p_value ? 1 : 0;
    int ret = ioctlsocket(usr->socket, FIONBIO, &ul);
    if (ret == SOCKET_ERROR) {
        return false;
    }
    return true;
}

bool gangsta::sf_net::NetSocket::initialize_tcp() {
    UserDataWin *usr = static_cast<UserDataWin *>(user);
    usr->socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (usr->socket < 0) {
        // err
        return false;
    }
    return true;
}

bool gangsta::sf_net::NetSocket::initialize_udp() {
    UserDataWin *usr = static_cast<UserDataWin *>(user);
    usr->socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (usr->socket < 0) {
        // err
        return false;
    }
    return true;
}

bool gangsta::sf_net::NetSocket::bind(uint16_t p_port) {
    UserDataWin *usr = static_cast<UserDataWin *>(user);
    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(p_port);
    int ret = ::bind(usr->socket,
                     (struct sockaddr *) &server_addr,
                     sizeof(server_addr)
    );
    if (ret < 0) {
        return false;
    }
    return true;
}

bool gangsta::sf_net::NetSocket::listen() {
    UserDataWin *usr = static_cast<UserDataWin *>(user);
    int ret = ::listen(usr->socket, SOMAXCONN);
    if (ret < 0) {
        return false;
    }
    return true;
}

gangsta::sf_net::NetSocket::Client gangsta::sf_net::NetSocket::accept() {
    UserDataWin *usr = static_cast<UserDataWin *>(user);
    sockaddr_in client_addr{};
    socklen_t client_addr_len = sizeof(client_addr);
    SOCKET client_socket = ::accept(
            usr->socket,
            (struct sockaddr *) &client_addr,
            &client_addr_len
    );
    NetSocket::Client client{};
    if (client_socket == INVALID_SOCKET) {
        int err = WSAGetLastError();
        if (err == WSAEWOULDBLOCK) {
            client.status.accept_would_block = true;
        } else {
            client.status.error = true;
        }
    } else {
        client.socket = client_socket;
    }
    return client;
}

bool gangsta::sf_net::NetSocket::connect(const std::string &p_ip, uint16_t p_port) {
    bool success = false;
    uint32_t ip = ipv4_string_to_int(p_ip, &success);
    return connect(ip, p_port);
}

bool gangsta::sf_net::NetSocket::connect(uint32_t p_ip, uint16_t p_port) {
    UserDataWin *usr = static_cast<UserDataWin *>(user);
    sockaddr_in socket_addr{};
    socket_addr.sin_family = AF_INET;
    socket_addr.sin_addr.s_addr = htonl(p_ip);
    socket_addr.sin_port = htons(p_port);
    int ret = ::connect(usr->socket, (sockaddr *) &socket_addr, sizeof(sockaddr));
    if (ret < 0) {
        return false;
    }
    return true;
}

int gangsta::sf_net::NetSocket::receive(
        gangsta::sf_net::NetSocket::Client p_client,
        char *p_buffer,
        int p_buffer_length) {
    p_client.status.reset();
    int ret = recv(p_client.socket, p_buffer, p_buffer_length, 0);
    if (ret == SOCKET_ERROR) {
        int err = WSAGetLastError();
        if (err == WSAEWOULDBLOCK) {
            p_client.status.accept_would_block = true;
        } else {
            p_client.status.error = true;
        }
    }
    return ret;
}

int gangsta::sf_net::NetSocket::send(
        gangsta::sf_net::NetSocket::Client p_client,
        const char *p_buffer,
        int p_buffer_length) {
    p_client.status.reset();
    int ret = ::send(p_client.socket, p_buffer, p_buffer_length, 0);
    if (ret == SOCKET_ERROR) {
        int err = WSAGetLastError();
        if (err == WSAEWOULDBLOCK) {
            p_client.status.accept_would_block = true;
        } else {
            p_client.status.error = true;
        }
    }
    return ret;
}

int gangsta::sf_net::NetSocket::receive(
        char *p_buffer,
        int p_buffer_length) {
    UserDataWin *usr = static_cast<UserDataWin *>(user);
    status.reset();
    int ret = recv(usr->socket, p_buffer, p_buffer_length, 0);
    if (ret == SOCKET_ERROR) {
        int err = WSAGetLastError();
        if (err == WSAEWOULDBLOCK) {
            status.accept_would_block = true;
        } else {
            status.error = true;
        }
    }
    return ret;
}

int gangsta::sf_net::NetSocket::send(
        const char *p_buffer,
        int p_buffer_length) {
    UserDataWin *usr = static_cast<UserDataWin *>(user);
    status.reset();
    int ret = ::send(usr->socket, p_buffer, p_buffer_length, 0);
    if (ret == SOCKET_ERROR) {
        int err = WSAGetLastError();
        if (err == WSAEWOULDBLOCK) {
            status.accept_would_block = true;
        } else {
            status.error = true;
        }
    }
    return ret;
}

bool gangsta::sf_net::NetSocket::close() {
    UserDataWin *usr = static_cast<UserDataWin *>(user);
    int ret = closesocket(usr->socket);
    if (ret < 0) {
        return false;
    }
    return true;
}

int gangsta::sf_net::NetSocket::send_to(
        const char *p_buffer,
        int p_buffer_length,
        uint32_t p_to_ip,
        uint16_t p_to_port
) {
    UserDataWin *usr = static_cast<UserDataWin *>(user);
    status.reset();
    sockaddr_in to_socket_addr{};
    to_socket_addr.sin_family = AF_INET;
    to_socket_addr.sin_addr.s_addr = htonl(p_to_ip);
    to_socket_addr.sin_port = htons(p_to_port);
    int ret = sendto(
            usr->socket,
            p_buffer,
            p_buffer_length,
            0,
            (sockaddr *) &to_socket_addr,
            sizeof(to_socket_addr)
    );
    if (ret == SOCKET_ERROR) {
        int err = WSAGetLastError();
        if (err == WSAEWOULDBLOCK) {
            status.accept_would_block = true;
        } else {
            status.error = true;
        }
    }
    return ret;
}

int gangsta::sf_net::NetSocket::receive_from(
        char *p_buffer,
        int p_len,
        uint32_t &out_from_ip,
        uint16_t &out_from_port
) {
    UserDataWin *usr = static_cast<UserDataWin *>(user);
    status.reset();
    sockaddr_in from_socket_addr{};
    int from_length = sizeof(sockaddr);
    int ret = recvfrom(
            usr->socket,
            p_buffer,
            p_len,
            0,
            (sockaddr *) &from_socket_addr,
            &from_length
    );
    if (ret == SOCKET_ERROR) {
        int err = WSAGetLastError();
        if (err == WSAEWOULDBLOCK) {
            status.accept_would_block = true;
        } else {
            status.error = true;
        }
    }
    out_from_ip = ntohl(static_cast<uint32_t>(from_socket_addr.sin_addr.s_addr));
    out_from_port = ntohs(from_socket_addr.sin_port);
    return ret;
}

std::string gangsta::sf_net::NetSocket::ipv4_int_to_string(uint32_t in, bool *const success) {
    std::string ret(INET_ADDRSTRLEN, '\0');
    in = htonl(in);
    const bool _success = (NULL != inet_ntop(AF_INET, &in, &ret[0], ret.size()));
    if (success) {
        *success = _success;
    }
    if (_success) {
        ret.pop_back(); // remove null-terminator required by inet_ntop
    } else if (!success) {
        char buf[200] = {0};
        //std::strerror_r(errno, buf, sizeof(buf));
        throw std::runtime_error(
                std::string("error converting ipv4 int to string ") +
                std::to_string(errno) +
                std::string(": ") +
                std::string(buf)
        );
    }
    return ret;
}

uint32_t gangsta::sf_net::NetSocket::ipv4_string_to_int(const std::string &in, bool *const success) {
    uint32_t ret;
    const bool _success = (1 == inet_pton(AF_INET, in.c_str(), &ret));
    ret = ntohl(ret);
    if (success) {
        *success = _success;
    } else if (!_success) {
        char buf[200] = {0};
        //strerror_r(errno, buf, sizeof(buf));
        throw std::runtime_error(
                std::string("error converting ipv4 string to int ") +
                std::to_string(errno) +
                std::string(": ") +
                std::string(buf)
        );
    }
    return ret;
}

gangsta::sf_net::NetSocket::Client gangsta::sf_net::NetSocket::self() {
    UserDataWin *usr = static_cast<UserDataWin *>(user);
    gangsta::sf_net::NetSocket::Client me = gangsta::sf_net::NetSocket::Client();
    me.socket = usr->socket;
    return me;
}

