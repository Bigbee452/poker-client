#pragma once
#include <SFML/Network.hpp>
#include <SFML/Network/TcpSocket.hpp>

bool send_all(sf::TcpSocket& socket, const void* data, std::size_t totalSize);
bool receive_with_timeout(sf::TcpSocket& socket, std::string& outMessage, sf::Time timeout);