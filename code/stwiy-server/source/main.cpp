#include "main.hpp"

#include "enet/network_server.hpp"

Server* server = nullptr;

class ServerEvents : public ServerInterface {
private:
    std::vector<ENetPeer*> clients_;
    void Broadcast(const std::string& message){
        for (auto& client : clients_) {
            server->Send(client, message);
        }
    }
public:
    void onClientConnect(ENetEvent event) override {
        clients_.push_back(event.peer);
        std::cout << "Client connected: " << event.peer->address.host << ":" << event.peer->address.port << std::endl;
    }

    void onClientDisconnect(ENetEvent event) override {
        std::cout << "Client disconnected: " << event.peer->address.host << ":" << event.peer->address.port << std::endl;
        clients_.erase(std::remove(clients_.begin(), clients_.end(), event.peer), clients_.end());
        std::cout << "Total clients: " << clients_.size() << std::endl;
    }

    void onPacketReceived(ENetEvent event) override {
        std::cout << "Packet received from client: " << event.peer->address.host << ":" << event.peer->address.port << std::endl;
        std::cout << "Data: " << event.packet->data << std::endl;
        std::string message(reinterpret_cast<const char*>(event.packet->data), event.packet->dataLength);
        std::cout << "Received message from client: " << message << std::endl;
        Broadcast(message);
    }
};

int main() {
    std::cout << "stwiy-server" << std::endl;
    server = new Server(1234, 10);
    ServerEvents se;
    server->SetInterface(&se);

    while (true) {
        server->Update();
    }

    return 0;
}

