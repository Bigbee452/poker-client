#include "poker.h"
#include <GLFW/glfw3.h>
#include <SFML/Network/IpAddress.hpp>
#include <iostream>

PokerClientStateMachine::PokerClientStateMachine(Window* window){
    this->window = window;
    state = PokerClientState::Disconnected;
    initState();
}

void PokerClientStateMachine::update(){
    switch(state){
        case PokerClientState::Disconnected:
            disconnected();
        case PokerClientState::WaitForStart:
            waitForStart();
        case PokerClientState::WaitForSetStart:
            waitForSetStart();
    }
}

void PokerClientStateMachine::initState(){
    switch(state){
        case PokerClientState::Disconnected:
            initDisconnected();
        case PokerClientState::WaitForStart:
            initWaitForStart();
        case PokerClientState::WaitForSetStart:
            initWaitForSetStart();
    }
}

void PokerClientStateMachine::initDisconnected(){
    window->gui->addElement(execute_path + "/ui/ip.rml", "getIp");
    window->gui->bindStringToElement("getIp", "ipTextBox", &ip);
    window->gui->initElement("getIp");
    window->gui->showElement("getIp", true);

    prev_time = glfwGetTime();
}

void PokerClientStateMachine::disconnected(){
    if(window->getKey(257)){ //check if enter is pressed.
        if(glfwGetTime()-prev_time < 0.5){
            return;
        }
        prev_time = glfwGetTime();
        cout << "connecting to: " << ip << endl;

        sf::IpAddress ipAddr = sf::IpAddress::resolve(ip).value();
        if (socket.connect(ipAddr, 9603) != sf::Socket::Status::Done) {
            std::cerr << "Error: Failed to connect to server.\n";
            return;
        }
        std::cout << "Connected to server!" << std::endl;
        window->gui->showElement("getIp", false);
    }
}

void PokerClientStateMachine::initWaitForStart(){

}

void PokerClientStateMachine::waitForStart(){

}

void PokerClientStateMachine::initWaitForSetStart(){

}

void PokerClientStateMachine::waitForSetStart(){

}