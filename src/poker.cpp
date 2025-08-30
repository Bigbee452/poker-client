#include "poker.h"
#include <GLFW/glfw3.h>
#include <SFML/Network/IpAddress.hpp>
#include <SFML/Network/Socket.hpp>
#include <iostream>
#include <string>

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
        if(glfwGetTime()-prev_time < 0.1){
            return;
        }
        prev_time = glfwGetTime();
        cout << "Connecting to: " << ip << endl;

        auto ipAddr = sf::IpAddress::resolve(ip);
        if(!ipAddr.has_value()){
            cout << "Invallid ip." << endl;
            return;
        }
        if (socket.connect(ipAddr.value(), 9603) != sf::Socket::Status::Done) {
            std::cerr << "Error: Failed to connect to server.\n";
            return;
        }
        std::cout << "Connected to server!" << std::endl;
        socket.setBlocking(false);

        window->gui->showElement("getIp", false);
        window->gui->deleteElement("getIp");

        state = PokerClientState::WaitForStart;
        initState();
        return;
    }
}

void PokerClientStateMachine::initWaitForStart(){

}

void PokerClientStateMachine::waitForStart(){
    sf::Socket::Status status = socket.receive(buffer, sizeof(buffer), received);
    if(status == sf::Socket::Status::Done){
        string msg_in = string(buffer, received);
        if(msg_in == "id master"){
            state = PokerClientState::WaitForSetStart;
            initState();
            return;
        } else if(msg_in == "id normal"){
            cout << "normal" << endl;
        } else {
            std::cout << "unexpected packet: " << msg_in << endl;
        }
    }
}

void PokerClientStateMachine::initWaitForSetStart(){

}

void PokerClientStateMachine::waitForSetStart(){

}