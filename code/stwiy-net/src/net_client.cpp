#include "net_client.hpp"

#include "net_buffer.hpp"
#include "net_event_serializer.hpp"

#include <chrono>

int udp_output_client_cb(const char *buf, int len, ikcpcb *kcp, void *user) {
    gangsta::sf_net::Client *client = (gangsta::sf_net::Client *) user;
    return client->udp_output(buf, len);
}

gangsta::sf_net::Client::Client() {
    kcp = nullptr;
    socket = nullptr;
    conv = 0;
    ip = 0;
    auth_port = 0;
    data_port = 0;
    key = 0;
    in_events = new moodycamel::ConcurrentQueue<NetEvent>();
    out_events = new moodycamel::ConcurrentQueue<NetEvent>();
    connected = false;
}

void gangsta::sf_net::Client::connect(
        const std::string &p_user,
        const std::string &p_password,
        const std::string &p_ip,
        uint16_t p_auth_port,
        uint16_t p_data_port
) {
    ip = NetSocket::ipv4_string_to_int(p_ip);
    auth_port = p_auth_port;
    data_port = p_data_port;

    // TODO hash password
    if (!get_authentication(p_user, p_password, p_ip, p_auth_port)) {
        return;
    }


    socket = new NetSocket();
    if (!socket->initialize_udp()) {
        return;
    }
    if (!socket->set_re_use_address(true)) {
        return;
    }
    if (!socket->set_non_blocking(true)) {
        return;
    }
    if (!socket->connect(ip, data_port)) {
        return;
    }

    // TODO - UDP = unreliable
    char *buffer = new char[8];
    buffer[0] = conv;
    buffer[1] = conv << 8;
    buffer[2] = conv << 16;
    buffer[3] = conv << 24;
    buffer[4] = key;
    buffer[5] = key << 8;
    buffer[6] = key << 16;
    buffer[7] = key << 24;
    int ret = socket->send_to(
            buffer,
            8,
            ip,
            data_port
    );
    if (socket->status.accept_would_block) {
        return;
    }
    if (socket->status.error) {
        return;
    }
    delete[] buffer;
    // TODO - UDP = unreliable

    kcp = ikcp_create(conv, this);
    ikcp_setoutput(kcp, &udp_output_client_cb);

    // todo should get ack before allowing to use kcp
    connected = true;
}

void gangsta::sf_net::Client::update() {

    // TODO unsure about ordering of events
    // udp_input = read new udp data
    // ikcp_send -> stage data to be send
    // ikcp_recv -> check received data
    // ikcp_update -> flushes available data -> udp_output

    udp_input();

    NetEvent event{};
    Buffer net_buffer = Buffer();
    while (out_events->try_dequeue(event)) {
        // NetEvent -> bytes -> ikcp_send -> udp_output
        net_buffer.set_length(0);
        net_buffer.seek(0);
        NetEventSerializer::serialize_net_event(net_buffer, event);
        const char *buffer = reinterpret_cast<char *>(net_buffer.get_ptr());
        int len = net_buffer.get_length();
        if (len > 1024) {
            // error
        }
        int ret = ikcp_send(kcp, buffer, len);
        if (ret == 0) {
            // good
        } else {
            // bad
        }
    }

    while (true) {
        // ikcp_recv -> bytes -> net event
        net_buffer.set_length(1024);
        net_buffer.seek(0);
        char *buffer = reinterpret_cast<char *>(net_buffer.get_ptr());
        int len = net_buffer.get_length();
        int ret = ikcp_recv(kcp, buffer, len);
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
        NetEventSerializer::deserialize_net_event(net_buffer, event);
        in_events->enqueue(event);
    }

    uint64_t now_ms = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::steady_clock::now().time_since_epoch()).count();
    ikcp_update(kcp, now_ms);
}

void gangsta::sf_net::Client::udp_input() {
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
        return;
    }
    if (socket->status.error) {
        return;
    }
    if (socket_ret < 0) {
        return;
    }
    int kcp_ret = ikcp_input(kcp, buffer, buffer_length);
    // TODO check return
}

int gangsta::sf_net::Client::udp_output(const char *p_buffer, int p_buffer_length) {
    int ret = socket->send_to(
            p_buffer,
            p_buffer_length,
            ip,
            data_port
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

void gangsta::sf_net::Client::send_event(gangsta::sf_net::NetEvent p_event) {
    out_events->enqueue(p_event);
}

bool gangsta::sf_net::Client::receive_event(gangsta::sf_net::NetEvent &event) {
    return in_events->try_dequeue(event);
}

bool gangsta::sf_net::Client::get_authentication(const std::string &p_user, const std::string &p_password,
                                                 const std::string &p_ip, uint16_t p_port) {
    NetSocket net_socket = NetSocket();
    if (!net_socket.initialize_tcp()) {
        return false;
    }
    if (!net_socket.set_keep_alive(false)) {
        return false;
    }
    if (!net_socket.set_re_use_address(true)) {
        return false;
    }
    if (!net_socket.connect(p_ip, p_port)) {
        net_socket.close();
        return false;
    }


    Buffer net_buffer = Buffer();

    std::function<void(const gangsta::sf_net::Buffer &)> write = [&net_buffer, p_user, p_password]
            (const gangsta::sf_net::Buffer &) {
        net_buffer.write_string(p_user);
        net_buffer.write_string(p_password);
    };

    gangsta::sf_net::NetSocket::Client client = net_socket.self();
    if (!NetSocket::send_proto(net_socket, client, net_buffer, write)) {
        return false;
    }
    if (!NetSocket::receive_proto(net_socket, client, net_buffer)) {
        return false;
    }
    conv = net_buffer.read_u32();
    key = net_buffer.read_u32();
    net_socket.close();

    return true;
}

bool gangsta::sf_net::Client::is_connected() {
    return connected;
}
