#include "Graphics.hpp"

Graphics::Graphics()
{
    m_sprite.setTexture(m_texture);
}

Graphics::~Graphics()
{

}

void Graphics::loadTexture(std::string path)
{
    if (!m_texture.loadFromFile(path))
        std::cout << "unable to load texture" << std::endl;
}

void Graphics::makeGridSprite(int type)
{
    switch (type)
    {
    case 0:
        m_sprite.setTextureRect(sf::IntRect(0, 0, 382, 382));
        m_sprite.setPosition(0, 0);
        m_sprites.emplace(type, m_sprite);
        break;

    case 1:
        m_sprite.setTextureRect(sf::IntRect(0, 0, 382, 382));
        m_sprite.setPosition(382 + 76, 0);
        m_sprites.emplace(type, m_sprite);
        break;

    default:
        break;
    }

    m_sprite.setTextureRect(sf::IntRect(794, 0, 76, 382));
    m_sprite.setPosition(382, 0);
    m_sprites.emplace(-1, m_sprite);
}

void Graphics::makeHitSprite(int x, int y, bool hit)
{
    int delta;
    if (x > 12)
        delta = 4;
    else
        delta = 2;

    if (hit)
    {
        m_sprite.setTextureRect(sf::IntRect(382, 72, 36, 36));
        m_sprite.setPosition((x - 1) * 2 + delta + (x - 1) * 36, y * 2 + (y - 1) * 36);
        m_sprites.emplace(m_sprites.size(), m_sprite);
    }
    else
    {
        m_sprite.setTextureRect(sf::IntRect(382, 0, 36, 36));
        m_sprite.setPosition((x - 1) * 2 + delta + (x - 1) * 36, y * 2 + (y - 1) * 36);
        m_sprites.emplace(m_sprites.size(), m_sprite);
    }
}

void Graphics::makeShipSprite(int x, int y)
{
    m_sprite.setTextureRect(sf::IntRect(382, 36, 36, 36));
    m_sprite.setPosition((x - 1) * 2 + 4 + (x - 1) * 36, y * 2 + (y - 1) * 36);
    m_sprites.emplace(m_sprites.size(), m_sprite);
}

void Graphics::makeTurnSprite(int turn)
{
    switch (turn)
    {
    case 0:
        m_sprites.find(-1)->second.setTextureRect(sf::IntRect(418, 0, 76, 382));
        m_sprites.find(-1)->second.setPosition(382, 0);
        break;

    case 1:
        m_sprites.find(-1)->second.setTextureRect(sf::IntRect(796, 0, 76, 382));
        m_sprites.find(-1)->second.setPosition(382, 0);
        break;

    default:
        break;
    }
}

void Graphics::removeShip(int counter)
{
    m_sprites.erase(m_sprites.find(m_sprites.size() - counter), m_sprites.end());
}

sf::Vector2f Graphics::getSpritePos(int type)
{
    return m_sprites.find(type)->second.getPosition();
}

void Graphics::setSpritePos(int type, sf::Vector2f pos)
{
    m_sprites.find(type)->second.setPosition(pos);
}

std::map<int, sf::Sprite> *Graphics::getSpritesList()
{
    return &m_sprites;
}

void Graphics::draw(sf::RenderWindow *window)
{
    for (auto iter(m_sprites.begin()); iter != m_sprites.end(); ++iter)
    {
        window->draw(iter->second);
    }
}
