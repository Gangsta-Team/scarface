#include <iostream>
#include <thread>
#include <vector>
#include <string>
#include <chrono>

#include "net_client.hpp"
#include "net_server.hpp"

// state
std::atomic<bool> is_running;
gangsta::sf_net::Client *client;

// config
std::string server_ip = "127.0.0.1";
uint16_t server_auth_port = 13399;
uint16_t server_data_port = 1337;

void client_net_thread_fn() {

    std::this_thread::sleep_for(std::chrono::seconds(2));

    // connect over UDP
    client = new gangsta::sf_net::Client();
    client->connect(
            "user",
            "password",
            server_ip,
            server_auth_port,
            server_data_port
    );

    while (is_running) {
        // update state
        client->update();

        // receive event
        gangsta::sf_net::NetEvent receive_event{};
        if (client->receive_event(receive_event)) {
            std::cout << "Client Received Event: " + receive_event.to_string();
        }

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}


void server_net_thread_fn() {

    gangsta::sf_net::Server *server = new gangsta::sf_net::Server();
    server->start(server_auth_port, server_data_port);

    gangsta::sf_net::NetEvent receive_event{};
    while (is_running) {
        // echo server
        server->update();
        if (server->receive_event(receive_event)) {
            std::cout << "Server Received Event: " + receive_event.to_string();
            server->send_event(receive_event);
            std::cout << "Server ECHO'ed Event\r\n";
        }

        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    server->stop();
}

void run() {
    is_running = true;

    std::thread server_thread(server_net_thread_fn);
    std::thread client_thread(client_net_thread_fn);

    std::cout << "Press any e to exit..\n";
    while (is_running) {
        std::string user_input;
        std::getline(std::cin, user_input);
        if (user_input == "e") {
            is_running = false;
        } else if (user_input == "s") {
            gangsta::sf_net::NetEvent event{};
            event.type = gangsta::sf_net::NetEventType::CharacterSpawn;
            client->send_event(event);
        }
    }

    server_thread.join();
    client_thread.join();
}

int main(int argc, char **argv) {
    run();
    std::cout << "Done\n";
    return 0;
}