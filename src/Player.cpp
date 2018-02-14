#include "Player.hpp"

Player::Player()
{
    
}

void Player::addPlayer(int client_id)
{
    m_shape.setRadius(100);
    m_shape.setFillColor(sf::Color(0, 0, 0, 50));
    m_shape.setOutlineThickness(2);
    m_shape.setOutlineColor(sf::Color(0, 0, 0, 255));

    m_players.emplace(std::pair<int, sf::CircleShape>(client_id, m_shape));
}

void Player::setPosition(int client_id, float x, float y)
{
    m_players.find(client_id)->second.setPosition(x, y);
}

sf::Vector2f Player::getPosition(int client_id)
{
    m_pos = m_players.find(client_id)->second.getPosition();

    return m_pos;
}

void Player::move(int client_id, float x, float y)
{
    m_players.find(client_id)->second.move(x, y);
}

std::map<int, sf::CircleShape>* Player::getPlayerList()
{
    return &m_players;
}