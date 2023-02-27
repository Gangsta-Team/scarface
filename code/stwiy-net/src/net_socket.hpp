#ifndef SF_NET_SOCKET_H
#define SF_NET_SOCKET_H

#include "net_buffer.hpp"

#include <cstdint>
#include <string>
#include <functional>

namespace gangsta::sf_net {

    class NetSocket {

    public:
        static constexpr int MAX_UDP_SIZE = 1024;

        struct Status {
            bool accept_would_block = false;
            bool error = false;

            void reset() {
                accept_would_block = false;
                error = false;
            }
        };

        struct Client {
            uintptr_t socket = 0;
            Status status = {};
        };

        static std::string ipv4_int_to_string(uint32_t in, bool *const success = nullptr);

        static uint32_t ipv4_string_to_int(const std::string &in, bool *const success = nullptr);

    private:
        void *user;

    public:
        NetSocket::Status status;

        NetSocket();

        ~NetSocket();

        bool initialize_tcp();

        bool initialize_udp();

        bool set_non_blocking(bool p_value);

        bool set_keep_alive(bool p_value);

        bool set_re_use_address(bool p_value);

        bool set_option(int p_level, int p_option, bool p_value);

        bool bind(uint16_t p_port);

        bool listen();

        NetSocket::Client accept();

        NetSocket::Client self();

        bool connect(const std::string &p_ip, uint16_t p_port);

        bool connect(uint32_t p_ip, uint16_t p_port);

        int receive(NetSocket::Client p_client, char *p_buffer, int p_buffer_length);

        int send(NetSocket::Client p_client, const char *p_buffer, int p_buffer_length);

        int receive(char *p_buffer, int p_buffer_length);

        int send(const char *p_buffer, int p_buffer_length);

        bool close();

        int send_to(const char *p_buffer, int p_buffer_length, uint32_t p_to_ip, uint16_t p_to_port);

        int receive_from(char *p_buffer, int p_len, uint32_t &out_from_ip, uint16_t &out_from_port);

    public:
        static bool receive_proto(gangsta::sf_net::NetSocket &p_socket,
                                  gangsta::sf_net::NetSocket::Client &p_client,
                                  gangsta::sf_net::Buffer &p_buffer
        ) {

            p_buffer.set_length(1024);
            p_buffer.seek(0);
            char *receive_buffer = reinterpret_cast<char *>(p_buffer.get_ptr());
            int receive_buffer_length = p_buffer.get_length();
            int current_length = 0;
            uint32_t receive_data_length = 0;
            bool read_header = false;
            while (true) {
                int ret = p_socket.receive(
                        p_client,
                        &receive_buffer[current_length],
                        receive_buffer_length - current_length
                );
                if (p_client.status.accept_would_block) {
                    continue;
                }
                if (p_client.status.error) {
                    return false;
                }
                if (ret < 0) {
                    return false;
                }
                current_length += ret;
                if (!read_header && current_length >= 4) {
                    receive_data_length = p_buffer.read_u32();
                    read_header = true;
                }
                if (read_header && current_length >= receive_data_length) {
                    return true;
                }
            }
        }

        static bool send_proto(gangsta::sf_net::NetSocket &p_socket,
                               gangsta::sf_net::NetSocket::Client &p_client,
                               gangsta::sf_net::Buffer &p_buffer,
                               const std::function<void(const gangsta::sf_net::Buffer &p_buffer)> &write
        ) {
            p_buffer.set_length(0);
            p_buffer.seek(0);
            p_buffer.write_u32(0);
            write(p_buffer);
            uint32_t send_data_length = p_buffer.get_length();
            p_buffer.seek(0);
            p_buffer.write_u32(send_data_length);
            p_buffer.seek(0);
            p_buffer.set_length(send_data_length);
            const char *send_buffer = reinterpret_cast<char *>(p_buffer.get_ptr());
            int total = 0;
            while (total < send_data_length) {
                int ret = p_socket.send(
                        p_client,
                        &send_buffer[total],
                        send_data_length - total
                );
                if (p_client.status.accept_would_block) {
                    continue;
                }
                if (p_client.status.error) {
                    return false;
                }
                if (ret < 0) {
                    return false;
                }
                total += ret;
            }
            return true;
        }
    };

}


#endif
