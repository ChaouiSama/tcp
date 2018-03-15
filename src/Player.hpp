#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <iostream>

class Player
{
public:
    Player();

    void addPlayer(int client_id);

    //void setPosition(int client_id, float x, float y);

    //sf::Vector2f getPosition(int client_id);

    //void move(int client_id, float x, float y);

    //std::map<int, sf::CircleShape>* getPlayerList();
    std::vector<int> *getPlayerList();
    
private:
    sf::CircleShape m_shape;
    
    sf::Vector2f m_pos;
    
    //std::map<int, sf::CircleShape> m_players;
    std::vector<int> m_players;
};

#endif /* PLAYER_HPP */