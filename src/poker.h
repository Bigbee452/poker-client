#pragma once
#include "cards.h"
#include "windowManager.h"
#include <SFML/Network/TcpSocket.hpp>
#include <string>

enum class PokerClientState {
    Disconnected,
    WaitForStart,
    WaitForSetStart,
    SetStart,
    InGame,
    SendInt,
    GetHand,
    GetChips,
    GetLastBet,
    GetBet,
    GetComCards
};

class PokerClientStateMachine {
public:
    PokerClientStateMachine(Window* window, Scene* scene);
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
    void initInGame();
    void inGame();
    void initSendInt();
    void sendInt();
    void initGetHand();
    void getHand();
    void initGetChips();
    void getChips();
    void initGetLastBet();
    void getLastBet();
    void initGetBet();
    void getBet();
    void initGetComCards();
    void getComCards();

    PokerClientState state;

    Scene* scene;
    std::vector<CardModel*> handCardsModel = {};
    std::vector<CardModel*> ComCardsModel = {};

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

    //setStart state
    string startCoins = "";

    //getHand state
    int getHandReceiveState = 0; //0: get number of cards 1: receive cards
    int numCards = 0;
    Deck handCards;

    //getComCards state
    int getComReceiveState = 0; //0: get number of cards 1: receive cards
    Deck communityCards;

    //getBet
    bool pressed1 = false;
    bool pressed2 = false;
    bool has_recv_cf = false;
    string raiseAmount = "";

    int chips = -1;
    int lastbet = -1;

    bool folded = false;

};