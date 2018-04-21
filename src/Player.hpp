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
    std::vector<int> *getPlayerList();
    
private:
    sf::CircleShape m_shape;
    
    sf::Vector2f m_pos;
    
    std::vector<int> m_players;
};

#endif /* PLAYER_HPP */