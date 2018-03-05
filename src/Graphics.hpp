#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#include <SFML/Graphics.hpp>
#include <iostream>

class Graphics
{
public:
    Graphics();
    ~Graphics();

    void loadTexture(std::string path);
    void makeGridSprite(int type);
    void makeHitSprite(int x, int y);
    void makeShipSprite(int x, int y);
    sf::Vector2f getSpritePos(int type);
    void setSpritePos(int type, sf::Vector2f pos);
    std::map<int, sf::Sprite>* getSpritesList();
    void draw(sf::RenderWindow *window);

private:
    sf::Texture mTexture;
    sf::Sprite mSprite;

    //std::map<int, std::pair<sf::Sprite, sf::Transform>> mSprites;
    std::map<int, sf::Sprite> mSprites;
};

#endif /* GRAPHICS_HPP */