#include "Player.hpp"

Player::Player()
{
    
}

void Player::addPlayer(int client_id)
{
    m_players.push_back(client_id);
}

std::vector<int>* Player::getPlayerList()
{
    return &m_players;
}