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

std::vector<glm::vec3> handCardPositions = {glm::vec3(1.2, 0, 0.2), glm::vec3(1.2, 0, -0.2)};
std::vector<glm::vec3> ComCardPositions = {glm::vec3(0.8, 0, -0.6), glm::vec3(0.8, 0, -0.2), glm::vec3(0.8, 0, 0.2), glm::vec3(0.8, 0, 0.6), glm::vec3(0.8, 0, 1.0)};

PokerClientStateMachine::PokerClientStateMachine(Window* window, Scene* scene){
    this->window = window;
    this->scene = scene;

    window->gui->addElement(execute_path + "/ui/stats.rml", "setStats");
    window->gui->initElement("setStats");
    window->gui->showElement("setStats", true);

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
        case PokerClientState::GetChips:
            getChips();
            break;
        case PokerClientState::GetLastBet:
            getLastBet();
            break;
        case PokerClientState::GetBet:
            getBet();
            break;
        case PokerClientState::GetComCards:
            getComCards();
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
        case PokerClientState::GetChips:
            initGetChips();
            break;
        case PokerClientState::GetLastBet:
            initGetLastBet();
            break;
        case PokerClientState::GetBet:
            initGetBet();
            break;
        case PokerClientState::GetComCards:
            initGetComCards();
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
    window->gui->bindButtonToElement("setStart", "manageClick",&pressed);
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
        } else if(msg == "chips"){
            state = PokerClientState::GetChips;
            initState();
            return;
        } else if(msg == "lastbet"){
            state = PokerClientState::GetLastBet;
            initState();
            return;
        } else if(msg == "gbet"){
            state = PokerClientState::GetBet;
            initState();
            return;
        } else if(msg == "comcards"){
            state = PokerClientState::GetComCards;
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

            handCardsModel.clear();
            while (std::getline(ss, card_str, ',')) {
                Card card;
                card.card_id = atoi(card_str.c_str());
                CardModel* cardModel = new CardModel(scene, card.card_id);
                cardModel->setPosition(handCardPositions[index%2]);
                cardModel->setRotation(glm::pi<float>()/2, glm::pi<float>()/2, glm::pi<float>()/2);

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

void PokerClientStateMachine::initGetChips(){
    cout << "POKER-STATE: GetChips" << endl;
    string id = "chips ok";  
    send_all(socket, id.c_str(), id.size());  
}

void PokerClientStateMachine::getChips(){
    std::string ok_msg = "chips ok";
    sf::Socket::Status status = socket.receive(buffer, sizeof(buffer), received);    
    if(status == sf::Socket::Status::Done){
        string chipsStrIn = string(buffer, received);
        chips = atoi(chipsStrIn.c_str());
        window->gui->SetElementText("setStats", "coins", "COINS: "+to_string(chips));
        send_all(socket, ok_msg.c_str(), ok_msg.size());  
        std::cout << "current amount of chips: " << chips << std::endl;
        state = PokerClientState::InGame;
        initState();
        return;
    } else if (status == sf::Socket::Status::Disconnected) {
        std::cerr << "Disconnected from server.\n";
        state = PokerClientState::Disconnected;
        initState();
        return;
    }  
}

void PokerClientStateMachine::initGetLastBet(){
    cout << "POKER-STATE: GetLastBet" << endl;
    string id = "lastbet ok";  
    send_all(socket, id.c_str(), id.size());  
}

void PokerClientStateMachine::getLastBet(){
    std::string ok_msg = "lastbet ok";
    sf::Socket::Status status = socket.receive(buffer, sizeof(buffer), received);    
    if(status == sf::Socket::Status::Done){
        string lastbetStr = string(buffer, received);
        lastbet = atoi(lastbetStr.c_str());
        send_all(socket, ok_msg.c_str(), ok_msg.size());  
        std::cout << "lastbet amount: " << lastbet << std::endl;
        window->gui->SetElementText("setStats", "lastbet", "LASTBET: "+to_string(lastbet));
        state = PokerClientState::InGame;
        initState();
        return;
    } else if (status == sf::Socket::Status::Disconnected) {
        std::cerr << "Disconnected from server.\n";
        state = PokerClientState::Disconnected;
        initState();
        return;
    }      
}

void PokerClientStateMachine::initGetBet(){
    cout << "POKER-STATE: GetBet" << endl;
    string id = "gbet ok";  
    send_all(socket, id.c_str(), id.size());  

    window->gui->addElement(execute_path + "/ui/gbet.rml", "gbet");
    window->gui->bindStringToElement("gbet", "raiseAmount", &raiseAmount);
    window->gui->initElement("gbet");
    pressed = false;
    window->gui->bindButtonToElement("gbet", "raise", &pressed);
    window->gui->bindButtonToElement("gbet", "call", &pressed1);
    window->gui->bindButtonToElement("gbet", "fold", &pressed2);
    window->gui->showElement("gbet", true);
}

void PokerClientStateMachine::getBet(){
    if(!has_recv_cf){
        sf::Socket::Status status = socket.receive(buffer, sizeof(buffer), received);  
        if(status == sf::Socket::Status::Done){
            has_recv_cf = true;
        }
        return;
    }
    if(pressed){
        std::cout << "raise" << endl;
        pressed = false;
        std::cout << raiseAmount << std::endl;
        string raiseStr = to_string(stoi(raiseAmount)+3);
        send_all(socket, raiseStr.c_str(), raiseStr.size());  
        window->gui->showElement("gbet", false);
        window->gui->deleteElement("gbet");
        state = PokerClientState::InGame;
        initState();
    } else if(pressed1){
        cout << "call" << endl;
        pressed1 = false;
        string callStr = "2";
        send_all(socket, callStr.c_str(), callStr.size());  
        window->gui->showElement("gbet", false);
        window->gui->deleteElement("gbet");
        state = PokerClientState::InGame;
        initState();
    } else if(pressed2){
        cout << "folded" << endl;
        folded = true;
        pressed2=false;
        string foldStr = "1";
        send_all(socket, foldStr.c_str(), foldStr.size());
        window->gui->showElement("gbet", false);
        window->gui->deleteElement("gbet");  
        state = PokerClientState::InGame;
        initState();
    }
}

void PokerClientStateMachine::initGetComCards(){
    cout << "POKER-STATE: GetComCards" << endl;
    string id = "comcards ok";
    send_all(socket, id.c_str(), id.size());
    getComReceiveState = 0;
    communityCards.clear();
}

void PokerClientStateMachine::getComCards(){
    std::string ok_msg = "cards ok";

    if(getComReceiveState == 0){
        sf::Socket::Status status = socket.receive(buffer, sizeof(buffer), received);
        if(status == sf::Socket::Status::Done){
            string msg = string(buffer, received);
            numCards = atoi(msg.c_str());
            send_all(socket, ok_msg.c_str(), ok_msg.size());
            getComReceiveState = 1;
        } else if (status == sf::Socket::Status::Disconnected) {
            std::cerr << "Disconnected from server.\n";
            state = PokerClientState::Disconnected;
            initState();
            return;
        }  
    } else if(getComReceiveState == 1){
        sf::Socket::Status status = socket.receive(buffer, sizeof(buffer), received);
        if(status == sf::Socket::Status::Done){
            string msg = string(buffer, received);
            send_all(socket, ok_msg.c_str(), ok_msg.size());
            
            std::string card_str;
            std::stringstream ss(msg);

            int index = 0;

            ComCardsModel.clear();
            while (std::getline(ss, card_str, ',')) {
                Card card;
                card.card_id = atoi(card_str.c_str());
                
                CardModel* cardModel = new CardModel(scene, card.card_id);
                cardModel->setPosition(ComCardPositions[index%5]);
                cardModel->setRotation(glm::pi<float>()/2, glm::pi<float>()/2, glm::pi<float>()/2);

                communityCards.add_cards(card);
                ComCardsModel.push_back(cardModel);
                index++;
            }

            communityCards.print_deck();
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