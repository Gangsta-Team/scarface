#include "main.hpp"

#include "net_server.hpp"

#include <iostream>
#include <chrono>
#include <thread>

std::atomic<bool> is_running = false;
uint16_t server_auth_port = 13399;
uint16_t server_data_port = 13377;

void server_net_thread_fn() {
    std::cout << "Server thread started\n";
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

int main() {

    printf("stwiy-server\n");
    printf("Server listening auth_port: %d\n", server_auth_port);
    printf("Server listening data_port: %d\n", server_data_port);

    is_running = true;
    std::thread server_thread(server_net_thread_fn);

    std::cout << "Press any e to exit..\n";
    while (is_running) {
        std::string user_input;
        std::getline(std::cin, user_input);
        if (user_input == "e") {
            is_running = false;
        }
    }
    server_thread.join();

    return 0;
}

