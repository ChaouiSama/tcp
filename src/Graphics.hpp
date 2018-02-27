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
    void makeSprite(int type);
    void makeHitSprite(int x, int y);
    void draw(sf::RenderWindow *window);

private:
    sf::Texture mTexture;
    sf::Sprite mSprite;

    std::map<int, sf::Sprite> mSprites;
};

#endif /* GRAPHICS_HPP */