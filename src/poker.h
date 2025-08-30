#pragma once
#include "engine/windowManager.h"
#include <SFML/Network/TcpSocket.hpp>

enum class PokerClientState {
    Disconnected,
    WaitForStart,
    WaitForSetStart
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

    PokerClientState state;

    //global
    Window* window;
    sf::TcpSocket socket;

    //Disconnected state
    string ip = "";
    double prev_time;
};