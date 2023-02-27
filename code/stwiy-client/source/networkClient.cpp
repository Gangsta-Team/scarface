#include "networkClient.hpp"

#include "logger.hpp"

std::string server_ip = "127.0.0.1";
uint16_t server_auth_port = 13399;
uint16_t server_data_port = 13377;

void gangsta::NetworkClient::Connect() {
    client = new gangsta::sf_net::Client();
    client->connect(
            "user",
            "password",
            server_ip,
            server_auth_port,
            server_data_port
    );
}

void gangsta::NetworkClient::Update() {
    if (!client->is_connected()) {
        return;
    }
    client->update();
    gangsta::sf_net::NetEvent receive_event{};
    if (client->receive_event(receive_event)) {
        gangsta::Logger::Info("Network Client Received Event: " + receive_event.to_string());
    }
}
