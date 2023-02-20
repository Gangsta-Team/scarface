#ifndef SF_MOD_NET_MANAGER_H
#define SF_MOD_NET_MANAGER_H

#include "net_event.hpp"
#include "net_kcp_socket.hpp"
#include "net_buffer.hpp"

#include <concurrentqueue.h>

namespace gangsta::net {

    class Manager {

    private:
        moodycamel::ConcurrentQueue<NetEvent> *in_events;
        moodycamel::ConcurrentQueue<NetEvent> *out_events;
        KcpSocket *socket;

        static void serialize_net_event(Buffer &buffer, NetEvent &event);
        static void deserialize_net_event(Buffer &buffer, NetEvent &event);

        static constexpr int MAX_EVENT_SIZE = KcpSocket::MAX_UDP_SIZE;

    public:
        Manager();

        /**
         * Update network state
         */
        void update();

        /**
         * notify an event occurred to remote.
         * @param p_event
         */
        void notify_event(NetEvent p_event);

        /**
         * check if an event is available from remote
         * @param event
         * @return
         */
        bool check_event(NetEvent &event);
    };

    inline Manager g_Manager;
}

#endif