#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/quaternion_transform.hpp>
#include <glm/ext/scalar_constants.hpp>
#include <glm/fwd.hpp>
#include <glm/trigonometric.hpp>
#include <random>
#include <iostream>
#include <algorithm>
#include <string>
#include <vector>

#include "cards.h"
#include "engine/mesh.h"

Deck::Deck(bool start_full){
    if(start_full){
        for(int i = 0; i < 52;  i++){
            Card card;
            card.card_id = i;
            cards.push_back(card);
        }
    }
}

Deck::Deck(){

}

int initialized_cards = 0;

CardModel::CardModel(Scene* scene){
    this->scene = scene;
    model_id = initialized_cards;
    initialized_cards++;

    front_mat = new Material;
    front_mat->ambient = glm::vec3(1.0f);
    front_mat->diffuse = glm::vec3(1.0f);
    front_mat->specular = glm::vec3(1.0f);
    front_mat->shininess = 32.0f;

    Texture front_texture;
    std::string texturePath = "textures/cards/";
    texturePath += "card";
    texturePath += std::to_string(card_id) + ".png";
    front_texture.id = TextureFromFile(texturePath.c_str(), execute_path);
    front_texture.path = texturePath;
    front_texture.type = "texture_diffuse";
    std::vector<Texture> textures = {front_texture};
    front_mat->textures = textures;

    std::string name = "card_front";
    name += std::to_string(model_id);
    scene->add_model("models/card.fbx", name, front_mat);
    front = scene->get_model(name);

    back_mat = new Material;
    back_mat->ambient = glm::vec3(1.0f);
    back_mat->diffuse = glm::vec3(1.0f);
    back_mat->specular = glm::vec3(1.0f);
    back_mat->shininess = 32.0f;

    Texture back_texture;
    texturePath = "textures/cards/back.png";
    back_texture.id = TextureFromFile(texturePath.c_str(), execute_path);
    back_texture.path = texturePath;
    back_texture.type = "texture_diffuse";
    std::vector<Texture> back_textures = {back_texture};
    back_mat->textures = back_textures;

    name = "card_back";
    name += std::to_string(model_id);
    scene->add_model("models/card.fbx", name, back_mat);

    back = scene->get_model(name);

    front->setRotation(glm::pi<float>(), 0.0f, 0.0f);
}

void CardModel::setRotation(glm::vec3 rotation){
    this->rotation = rotation;   

    setPreTransform();
}

void CardModel::setPosition(glm::vec3 position){
    this->position = position;

    setPreTransform();
}

void CardModel::setPosition(float x, float y, float z){
    setPosition(glm::vec3(x, y, z));
}

void CardModel::setPreTransform(){
    pre_transform = glm::mat4(1.0f);
    pre_transform = glm::translate(pre_transform, position);
    pre_transform = glm::rotate(pre_transform, rotation.x, glm::vec3(1.0, 0.0, 0.0));
    pre_transform = glm::rotate(pre_transform, rotation.y, glm::vec3(0.0, 1.0, 0.0));
    pre_transform = glm::rotate(pre_transform, rotation.z, glm::vec3(0.0, 0.0, 1.0));    

    front->setPreTransform(pre_transform);
    back->setPreTransform(pre_transform); 
}

void CardModel::setRotation(float x, float y, float z){
    setRotation(glm::vec3(x, y, z));
}

void Deck::shuffle(){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::vector<Card> shuffled_cards = {};
    int deck_size = cards.size();
    for(int i = 0; i < deck_size; i++){
        std::uniform_int_distribution<> dist(0, cards.size()-1);
        int random_index = dist(gen);
        Card card = cards[random_index];
        cards.erase(cards.begin()+random_index);
        shuffled_cards.push_back(card);
    }
    cards.clear();
    cards = std::move(shuffled_cards);
}

void Deck::clear(){
    cards.clear();
}

std::vector<Card> Deck::take_cards(int number){
    std::vector<Card> top_cards = {};
    for(int i = 0; i < number; i++){
        if(!cards.empty()){
            top_cards.push_back(cards.back());
            cards.pop_back();
        } else {
            std::cerr << "trying to take cards out of an empty deck" << std::endl;
        }
    }
    return top_cards;
}

std::vector<Card> Deck::vieuw_cards(int number){
    std::vector<Card> top_cards = {};
    for(int i = 1; i <= number; i++){
        if(i >= 0){
            top_cards.push_back(cards[cards.size()-i]);
        } else {
            std::cerr << "trying to take cards out of an empty deck" << std::endl;
        }
    }
    return top_cards;
}

void Deck::add_cards(std::vector<Card> in_cards){
    for(Card card : in_cards){
        cards.push_back(card);
    }
}

void Deck::print_deck(){
    for(Card card : cards){
        std::cout << card.card_str() << std::endl;
    }
}

void Deck::sort_rank(){
    std::sort(cards.begin(), cards.end(), [](const Card& a, const Card& b){return a.rank() < b.rank();});
}

void Deck::sort_suit(){
    std::sort(cards.begin(), cards.end(), [](const Card& a, const Card& b){return a.suit() < b.suit();});
}

int Deck::size(){
    return cards.size();
}