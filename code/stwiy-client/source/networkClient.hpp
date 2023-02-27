#ifndef _NETWORK_CLIENT_HPP
#define _NETWORK_CLIENT_HPP

#include "net_client.hpp"

namespace gangsta {
    class NetworkClient {

    private:
        gangsta::sf_net::Client *client;

    public:
        void Connect();
        void Update();
    };

    inline NetworkClient g_networkClient;

}


#endif
