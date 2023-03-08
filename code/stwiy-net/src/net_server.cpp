#include "net_server.hpp"

#include "net_buffer.hpp"
#include "net_event_serializer.hpp"

#include <chrono>

int udp_output_server_cb(const char *buf, int len, ikcpcb *kcp, void *user) {
    gangsta::sf_net::Server::KcpUser *kcp_user = (gangsta::sf_net::Server::KcpUser *) user;
    return kcp_user->server->udp_output(
            kcp_user->connection->conv,
            kcp_user->connection->ip,
            kcp_user->connection->port,
            buf,
            len
    );
}

gangsta::sf_net::Server::Server() {
    socket = nullptr;
    conv_id_counter = 1;
    clients = std::vector<Connection>();
    auth_requests = std::vector<AuthRequest>();
    in_events = new moodycamel::ConcurrentQueue<NetEvent>();
    out_events = new moodycamel::ConcurrentQueue<NetEvent>();
    tcp_auth_thread = nullptr;
    is_tcp_auth_running = false;
}

gangsta::sf_net::Server::~Server() {
    stop();
}

void gangsta::sf_net::Server::start(uint16_t p_auth_port, uint16_t p_data_port) {
    socket = new NetSocket();
    if (!socket->initialize_udp()) {

    }
    if (!socket->set_non_blocking(true)) {

    }
    if (!socket->set_re_use_address(true)) {

    }
    if (!socket->bind(p_data_port)) {
        socket->close();
        return;
    }
    provide_authentication(p_auth_port);
}

void gangsta::sf_net::Server::stop() {
    stop_provide_authentication();
}

void gangsta::sf_net::Server::update() {
    while (udp_input()) {
        // todo perhaps break at some point to allow processing when much data comes in
    }

    NetEvent event{};
    Buffer net_buffer = Buffer();

    // process writes
    while (out_events->try_dequeue(event)) {
        // NetEvent -> bytes -> ikcp_send -> udp_output

        Connection *client = nullptr;
        { // lock scope
            std::lock_guard<std::mutex> lockGuard(clients_mutex);
            for (Connection &c: clients) {
                if (c.ip == event.ip
                    && c.port == event.port) {
                    client = &c;
                    break;
                }
            }
        }

        if (client == nullptr) {
            continue;
        }

        net_buffer.set_length(0);
        net_buffer.seek(0);
        NetEventSerializer::serialize_net_event(net_buffer, event);
        const char *buffer = reinterpret_cast<char *>(net_buffer.get_ptr());
        int len = net_buffer.get_length();
        if (len > 1024) {
            // error
        }
        int ret = ikcp_send(client->kcp, buffer, len);
        if (ret == 0) {
            // good
        } else {
            // bad
        }
    }

    // process reads
    int current_client = 0;
    int max_clients = 1;
    while (current_client < max_clients) {
        Connection *client = nullptr;
        { // lock scope
            std::lock_guard<std::mutex> lockGuard(clients_mutex);
            max_clients = clients.size();
            if (current_client < max_clients) {
                client = &clients[current_client];
            } else {
                client = nullptr;
            }
            current_client++;
        }

        if (client == nullptr) {
            continue;
        }

        while (true) {
            // ikcp_recv -> bytes -> net event
            net_buffer.set_length(1024);
            net_buffer.seek(0);
            char *buffer = reinterpret_cast<char *>(net_buffer.get_ptr());
            int len = net_buffer.get_length();
            int ret = ikcp_recv(client->kcp, buffer, len);
            if (ret == -1) {
                // no data available
                break;
            } else if (ret == -2) {
                // error
                break;
            } else if (ret == -3) {
                // buffer to small
                break;
            } else if (ret < 0) {
                //  unknown
                break;
            } else if (ret == 0) {
                //  no data
                break;
            } else {
                net_buffer.set_length(ret);
            }
            event.port = client->port;
            event.ip = client->ip;
            event.conv = client->conv;
            NetEventSerializer::deserialize_net_event(net_buffer, event);
            in_events->enqueue(event);
        }

        uint64_t now_ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::steady_clock::now().time_since_epoch()).count();
        ikcp_update(client->kcp, now_ms);
    }
}

/**
 * return true if there might be more data
 */
bool gangsta::sf_net::Server::udp_input() {
    int buffer_length = NetSocket::MAX_UDP_SIZE;
    char *buffer = new char[buffer_length];
    uint32_t from_ip = 0;
    uint16_t from_port = 0;
    int socket_ret = socket->receive_from(
            buffer,
            buffer_length,
            from_ip,
            from_port
    );
    if (socket->status.accept_would_block) {
        return false;
    }
    if (socket->status.error) {
        return false;
    }
    if (socket_ret < 0) {
        return false;
    }

    Connection *client = nullptr;
    { // lock scope
        std::lock_guard<std::mutex> lockGuard(clients_mutex);
        for (Connection &c: clients) {
            if (c.ip == from_ip
                && c.port == from_port) {
                client = &c;
                break;
            }
        }
    }

    if (client == nullptr) {
        // https://github.com/skywind3000/kcp/wiki/EN_Cooperate-With-Tcp-Server
        // udp packet is not intended for any client
        // check if new client data is included
        // TODO

        if (socket_ret < 8) {
            // length to small for new client
            return true;
        }

        uint32_t conv = (uint32_t) (buffer[0] | (buffer[1] << 8) | (buffer[2] << 16) | (buffer[3] << 24));
        uint32_t key = (uint32_t) (buffer[4] | (buffer[5] << 8) | (buffer[6] << 16) | (buffer[7] << 24));

        AuthRequest *auth_request = nullptr;
        { // lock scope
            std::lock_guard<std::mutex> lockGuard(auth_mutex);
            for (AuthRequest &r: auth_requests) {
                if (r.conv == conv) {
                    auth_request = &r;
                    break;
                }
            }
            if (auth_request != nullptr) {
                auth_requests.erase(std::remove_if(
                                            auth_requests.begin(),
                                            auth_requests.end(),
                                            [&](AuthRequest const &r) { return r.conv == auth_request->conv; }),
                                    auth_requests.end());
            }
        }
        if (auth_request == nullptr) {
            return true;
        }
        if (auth_request->key != key) {
            return true;
        }

        KcpUser *kcp_user = new KcpUser();
        kcp_user->server = this;
        kcp_user->connection = new Connection();

        client = kcp_user->connection;
        client->ip = from_ip;
        client->port = from_port;
        client->conv = conv;
        client->kcp = ikcp_create(conv, kcp_user);
        ikcp_setoutput(client->kcp, &udp_output_server_cb);

        { // lock scope
            std::lock_guard<std::mutex> lockGuard(clients_mutex);
            clients.push_back(*client);
        }

        // TODO - UDP = unreliable
        // send tcp ack ?
        // TODO - UDP = unreliable

        return true;
    }

    int kcp_ret = ikcp_input(client->kcp, buffer, buffer_length);
    return true;
}

bool gangsta::sf_net::Server::receive_event(gangsta::sf_net::NetEvent &event) {
    return in_events->try_dequeue(event);
}

void gangsta::sf_net::Server::send_event(gangsta::sf_net::NetEvent p_event) {
    out_events->enqueue(p_event);
}

void gangsta::sf_net::Server::authentication_thread(uint16_t p_port) {

    NetSocket net_socket = NetSocket();
    if (!net_socket.initialize_tcp()) {

    }
    if (!net_socket.set_keep_alive(false)) {

    }
    if (!net_socket.set_non_blocking(true)) {

    }
    if (!net_socket.set_re_use_address(true)) {

    }
    if (!net_socket.bind(p_port)) {
        net_socket.close();
        return;
    }
    if (!net_socket.listen()) {
        net_socket.close();
        return;
    }

    Buffer net_buffer = Buffer();
    while (is_tcp_auth_running) {
        std::this_thread::sleep_for(std::chrono::seconds(1));

        NetSocket::Client client = net_socket.accept();
        if (client.status.accept_would_block) {
            continue;
        }

        if (!NetSocket::receive_proto(net_socket, client, net_buffer)) {
            continue;
        }

        std::string user = net_buffer.read_string();
        std::string password = net_buffer.read_string();

        AuthRequest auth_request{};
        { // lock scope
            std::lock_guard<std::mutex> lockGuard(auth_mutex);
            auth_request.conv = conv_id_counter;
            auth_request.key = 1;
            auth_requests.push_back(auth_request);
            conv_id_counter++;
        }

        std::function<void(const gangsta::sf_net::Buffer &)> write = [&net_buffer, &auth_request]
                (const gangsta::sf_net::Buffer &) {
            net_buffer.write_u32(auth_request.conv);
            net_buffer.write_u32(auth_request.key);
        };

        if (!NetSocket::send_proto(net_socket, client, net_buffer, write)) {
            continue;
        }
    }
}

void gangsta::sf_net::Server::provide_authentication(uint16_t p_port) {
    if (is_tcp_auth_running) {
        return;
    }
    is_tcp_auth_running = true;
    tcp_auth_thread = new std::thread(&Server::authentication_thread, this, p_port);
}

void gangsta::sf_net::Server::stop_provide_authentication() {
    if (!is_tcp_auth_running) {
        return;
    }
    is_tcp_auth_running = false;
    tcp_auth_thread->join();
}

int gangsta::sf_net::Server::udp_output(
        uint32_t p_conv,
        uint32_t p_ip,
        uint16_t p_port,
        const char *p_buffer,
        int p_buffer_length
) {
    int ret = socket->send_to(
            p_buffer,
            p_buffer_length,
            p_ip,
            p_port
    );
    if (socket->status.accept_would_block) {
        return -1;
    }
    if (socket->status.error) {
        return -1;
    }
    // return value of ikcp_output, seems not to be used
    return ret;
}


