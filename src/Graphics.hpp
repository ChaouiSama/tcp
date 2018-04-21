#ifndef GRAPHICS_HPP_
#define GRAPHICS_HPP_

#include <SFML/Graphics.hpp>
#include <iostream>

class Graphics
{
public:
    Graphics();
    ~Graphics();

    void loadTexture(std::string path);
    void makeGridSprite(int type);
    void makeHitSprite(int x, int y, bool hit);
    void makeShipSprite(int x, int y);
    void makeTurnSprite(int turn);
    void removeShip(int counter);
    sf::Vector2f getSpritePos(int type);
    void setSpritePos(int type, sf::Vector2f pos);
    std::map<int, sf::Sprite> *getSpritesList();
    void draw(sf::RenderWindow *window);

private:
    sf::Texture m_texture;
    sf::Sprite m_sprite;

    std::map<int, sf::Sprite> m_sprites;
};

#endif /* GRAPHICS_HPP_ */
