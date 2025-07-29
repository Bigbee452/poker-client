#include "network.h"
#include <SFML/System/Time.hpp>
#include <iostream>
#include <string>
#include <thread>

bool send_all(sf::TcpSocket& socket, const void* data, std::size_t totalSize) {
    const char* buffer = static_cast<const char*>(data);
    std::size_t sentTotal = 0;

    while (sentTotal < totalSize) {
        std::size_t sent = 0;
        sf::Socket::Status status = socket.send(buffer + sentTotal, totalSize - sentTotal, sent);
        if (status != sf::Socket::Status::Done) {
            return false; // Error or disconnected
        }
        sentTotal += sent;
    }

    return true;
}

bool receive_with_timeout(sf::TcpSocket& socket, std::string& outMessage, sf::Time timeout) {
    socket.setBlocking(false); // Make the socket non-blocking

    auto start = std::chrono::steady_clock::now();
    char buffer[1024];
    std::size_t received;

    while (true) {
        sf::Socket::Status status = socket.receive(buffer, sizeof(buffer), received);
        if (status == sf::Socket::Status::Done) {
            outMessage = std::string(buffer, received);
            return true; // Successfully received
        } else if (status == sf::Socket::Status::Disconnected) {
            std::cerr << "Disconnected from server.\n";
            return false;
        }

        // Check timeout
        auto now = std::chrono::steady_clock::now();
        if (now - start > std::chrono::milliseconds(timeout.asMilliseconds())) {
            std::cout << "Timeout reached. No data received.\n";
            return false;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(10)); // avoid busy waiting
    }
}