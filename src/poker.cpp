#include "poker.h"
#include "network.h"
#include <GLFW/glfw3.h>
#include <SFML/Network/IpAddress.hpp>
#include <SFML/Network/Socket.hpp>
#include <algorithm>
#include <cctype>
#include <glm/fwd.hpp>
#include <iostream>
#include <string>
#include <vector>
#include "cards.h"

std::vector<glm::vec3> handCardPositions = {glm::vec3(0.1, 0, 0), glm::vec3(0.4, 0, 0)};

PokerClientStateMachine::PokerClientStateMachine(Window* window, Scene* scene){
    this->window = window;
    this->scene = scene;

    state = PokerClientState::Disconnected;
    initState();
}

void PokerClientStateMachine::update(){
    switch(state){
        case PokerClientState::Disconnected:
            disconnected();
            break;
        case PokerClientState::WaitForStart:
            waitForStart();
            break;
        case PokerClientState::WaitForSetStart:
            waitForSetStart();
            break;
        case PokerClientState::SetStart:
            setStart();
            break;
        case PokerClientState::InGame:
            inGame();
            break;
        case PokerClientState::SendInt:
            sendInt();
            break;
        case PokerClientState::GetHand:
            getHand();
            break;
    }
}

void PokerClientStateMachine::initState(){
    switch(state){
        case PokerClientState::Disconnected:
            initDisconnected();
            break;
        case PokerClientState::WaitForStart:
            initWaitForStart();
            break;
        case PokerClientState::WaitForSetStart:
            initWaitForSetStart();
            break;
        case PokerClientState::SetStart:
            initSetStart();
            break;
        case PokerClientState::InGame:
            initInGame();
            break;
        case PokerClientState::SendInt:
            initSendInt();
            break;
        case PokerClientState::GetHand:
            initGetHand();
            break;
    }
}

void PokerClientStateMachine::initDisconnected(){
    cout << "POKER-STATE: Disconnected" << endl;
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
    cout << "POKER-STATE: WaitForStart" << endl;
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
            cout << "Normal identifier received" << endl;
        } else if(msg_in == "start"){
            state = PokerClientState::InGame;
            initState();
            return;
        } else if(msg_in == "hand"){
            state = PokerClientState::InGame;
            initState();
            return;
        } else {
            std::cout << "unexpected packet: " << msg_in << endl;
        }
    }
}

void PokerClientStateMachine::initWaitForSetStart(){
    cout << "POKER-STATE: WaitForSetStart" << endl;

    window->gui->addElement(execute_path + "/ui/start.rml", "setStart");
    window->gui->bindStringToElement("setStart", "startCoins", &startCoins);
    window->gui->initElement("setStart");
    pressed = false;
    window->gui->bindButtonToElement("setStart", &pressed);
    window->gui->showElement("setStart", true);
}

void PokerClientStateMachine::waitForSetStart(){
    if(pressed){
        pressed = false;

        if(startCoins == ""){
            return;
        }
        if(!std::all_of(startCoins.begin(), startCoins.end(), ::isdigit)){
            return;
        }

        window->gui->showElement("setStart", false);
        window->gui->deleteElement("setStart");
        
        state = PokerClientState::SetStart;
        initState();
        return;
    }
}

void PokerClientStateMachine::initSetStart(){
    cout << "POKER-STATE: SetStart" << endl;
    string startStr = "start";
    if(!send_all(socket, startStr.c_str(), startStr.size())){
        std::cout << "failed to send start message, Disconnecting" << std::endl;
        socket.disconnect();
        state = PokerClientState::Disconnected;
        initState();
        return;
    }
}

void PokerClientStateMachine::setStart(){
    state = PokerClientState::InGame;
    initState();
}

void PokerClientStateMachine::initInGame(){
    cout << "POKER-STATE: InGame" << endl;
}

void PokerClientStateMachine::inGame(){
    sf::Socket::Status status = socket.receive(buffer, sizeof(buffer), received);
    if (status == sf::Socket::Status::Done) {
        string msg = std::string(buffer, received);
        if(msg == "aint"){
            state = PokerClientState::SendInt;
            initState();
            return;
        } else if(msg == "hand"){
            state = PokerClientState::GetHand;
            initState();
            return;
        } else {
            cout << "unknown message: " << msg << endl;
        }
    } else if (status == sf::Socket::Status::Disconnected) {
        std::cerr << "Disconnected from server.\n";
        state = PokerClientState::Disconnected;
        initState();
        return;
    }
}

void PokerClientStateMachine::initSendInt(){
    cout << "POKER-STATE: SendInt" << endl;
    string id = "aint ok";
    send_all(socket, id.c_str(), id.size());
}

void PokerClientStateMachine::sendInt(){
    sf::Socket::Status status = socket.receive(buffer, sizeof(buffer), received);
    if (status == sf::Socket::Status::Done) {
        string msg = std::string(buffer, received);
        if(msg == "enter start chips: "){
            send_all(socket, startCoins.c_str(), startCoins.size());
            state = PokerClientState::InGame;
            initState();
            return;
        } else {
            cout << "aint unknown message disconnecting" << endl;
            state = PokerClientState::Disconnected;
            initState();
            return;
        }
    } else if (status == sf::Socket::Status::Disconnected) {
        std::cerr << "Disconnected from server.\n";
        state = PokerClientState::Disconnected;
        initState();
        return;
    }    
}

void PokerClientStateMachine::initGetHand(){
    cout << "POKER-STATE: GetHand" << endl;
    string id = "hand ok";
    send_all(socket, id.c_str(), id.size());
    getHandReceiveState = 0;
    handCards.clear();
}

void PokerClientStateMachine::getHand(){
    std::string ok_msg = "cards ok";

    if(getHandReceiveState == 0){
        sf::Socket::Status status = socket.receive(buffer, sizeof(buffer), received);
        if(status == sf::Socket::Status::Done){
            string msg = string(buffer, received);
            numCards = atoi(msg.c_str());
            send_all(socket, ok_msg.c_str(), ok_msg.size());
            getHandReceiveState = 1;
        } else if (status == sf::Socket::Status::Disconnected) {
            std::cerr << "Disconnected from server.\n";
            state = PokerClientState::Disconnected;
            initState();
            return;
        }  
    } else if(getHandReceiveState == 1){
        sf::Socket::Status status = socket.receive(buffer, sizeof(buffer), received);
        if(status == sf::Socket::Status::Done){
            string msg = string(buffer, received);
            send_all(socket, ok_msg.c_str(), ok_msg.size());
            
            std::string card_str;
            std::stringstream ss(msg);

            int index = 0;

            //handCardsModel.clear();
            while (std::getline(ss, card_str, ',')) {
                Card card;
                card.card_id = atoi(card_str.c_str());
                CardModel cardModel(scene, card.card_id);
                cardModel.setPosition(handCardPositions[index%2]);
                cardModel.setRotation(glm::pi<float>(), glm::pi<float>(), 0);

                handCards.add_cards(card);
                handCardsModel.push_back(cardModel);
                index++;
            }

            handCards.print_deck();
            state = PokerClientState::InGame;
            initState();
            return;
        }
    } else {
        cout << "Unknown getHand message, Disconnecting" << endl;
        socket.disconnect();
        state = PokerClientState::Disconnected;
        initState();
        return;
    }
}