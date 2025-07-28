#pragma once
#include <glm/fwd.hpp>
#include <string>
#include <vector>
#include "engine/windowManager.h"

struct Card {
    int card_id; //spades 0=ace, 2=2, ..., 12=king, Hearts 13=1, ..., Diamonds, Cubs
    int suit() const {
        return card_id/13; //spades hearts diamonds cubs
    }
    int rank() const {
        return card_id%13;
    }
    std::string card_str(){
        std::string suits[] = {"♠Spades", "♥Hearts", "♦Diamonds", "♣Cubs "};
        std::string ranks[] = {"Jack", "Queen", "King"};
        std::string suit = "";
        std::string rank = "";

        if(card_id%13 < 10){
            rank = std::to_string(card_id%13 + 1);
        } else {
            rank = ranks[card_id%13-10];
        } 
        suit = suits[card_id/13];
        std::string return_string = suit + " " + rank;
        return return_string;
    }

    std::string card_str_short(){
        std::string suits[] = {"s", "h", "d", "c"};
        std::string ranks[] = {"A", "2", "3", "4", "5", "6", "7", "8", "9", "T", "J", "Q", "K"}; 
        std::string return_string = ranks[rank()]+suits[suit()];
        return return_string;      
    }
};

class CardModel {
public:
    CardModel(Scene* scene);
    void setRotation(glm::vec3 rotation);
    void setRotation(float x, float y, float z);
    void setPosition(glm::vec3 position);
    void setPosition(float x, float y, float z);
private:
    void setPreTransform();

    int card_id = 0;
    int model_id;
    Material* front_mat;
    Material* back_mat;

    Model* front;
    Model* back;

    Scene* scene;

    glm::vec3 position = glm::vec3(0.0f);
    glm::vec3 rotation = glm::vec3(0.0f);

    glm::mat4 pre_transform = glm::mat4(1.0f);
};

class Deck {
    public:
        Deck(bool start_full);
        Deck();
        void shuffle();
        void clear();
        void add_cards(std::vector<Card> in_cards);
        std::vector<Card> take_cards(int number);
        std::vector<Card> vieuw_cards(int number);
        void print_deck();
        int size();
    private:
        std::vector<Card> cards = {};

        void sort_rank();
        void sort_suit();
};