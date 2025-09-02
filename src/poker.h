#pragma once
#include "engine/windowManager.h"
#include <SFML/Network/TcpSocket.hpp>

enum class PokerClientState {
    Disconnected,
    WaitForStart,
    WaitForSetStart,
    SetStart
};

class PokerClientStateMachine {
public:
    PokerClientStateMachine(Window* window);
    void update();
private:
    void initState();

    void initDisconnected();
    void disconnected();
    void initWaitForStart();
    void waitForStart();
    void initWaitForSetStart();
    void waitForSetStart();
    void initSetStart();
    void setStart();

    PokerClientState state;

    //global
    Window* window;
    sf::TcpSocket socket;
    bool isMaster = false;
    char buffer[1024];
    std::size_t received;

    //Disconnected state
    string ip = "";
    double prev_time;

    //WaitForSetStart state
    bool pressed = false;
};