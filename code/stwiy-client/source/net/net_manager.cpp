#include "net_manager.hpp"

gangsta::net::Manager::Manager() {
    in_events = new moodycamel::ConcurrentQueue<NetEvent>();
    out_events = new moodycamel::ConcurrentQueue<NetEvent>();
    socket = new KcpSocket();
}

void gangsta::net::Manager::update() {

    NetEvent event{};
    Buffer net_buffer = Buffer();
    while (out_events->try_dequeue(event)) {
        net_buffer.set_length(0);
        net_buffer.seek(0);
        serialize_net_event(net_buffer, event);
        const char *buffer = reinterpret_cast<char *>(net_buffer.get_ptr());
        int len = net_buffer.get_length();
        if(len > MAX_EVENT_SIZE){
            // error
        }
        int ret = socket->send(buffer, len);
        if (ret == 0) {
            // good
        } else {
            // bad
        }
    }

    while (true) {
        net_buffer.set_length(MAX_EVENT_SIZE);
        net_buffer.seek(0);
        char *buffer = reinterpret_cast<char *>(net_buffer.get_ptr());
        int len = net_buffer.get_length();
        int ret = socket->receive(buffer, len);
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
        deserialize_net_event(net_buffer, event);
        in_events->enqueue(event);
    }

    socket->update();
}

void gangsta::net::Manager::notify_event(gangsta::net::NetEvent p_event) {
    out_events->enqueue(p_event);
}

bool gangsta::net::Manager::check_event(gangsta::net::NetEvent &event) {
    return in_events->try_dequeue(event);
}

void gangsta::net::Manager::deserialize_net_event(gangsta::net::Buffer &buffer, gangsta::net::NetEvent &event) {
    buffer.write_u8(event.type);
}

void gangsta::net::Manager::serialize_net_event(gangsta::net::Buffer &buffer, gangsta::net::NetEvent &event) {
    event.type = (NetEventType)buffer.read_u8();
}
