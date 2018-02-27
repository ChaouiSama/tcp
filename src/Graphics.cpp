#include "Graphics.hpp"

Graphics::Graphics()
{

}

Graphics::~Graphics()
{

}

void Graphics::loadTexture(std::string path)
{
    if (!mTexture.loadFromFile(path))
        std::cout << "unable to load texture" << std::endl;
}

void Graphics::makeSprite(int type)
{
    mSprite.setTexture(mTexture);

    switch (type)
    {
    case 0:
        mSprite.setTextureRect(sf::IntRect(0, 0, 382, 382));
        mSprite.setPosition(0, 0);
        mSprites.emplace(type, mSprite);
        break;

    case 1:
        mSprite.setTextureRect(sf::IntRect(0, 0, 382, 382));
        mSprite.setPosition(380, 380);
        mSprites.emplace(type, mSprite);
        break;

    case 2:
        mSprite.setTextureRect(sf::IntRect(382, 36, 36, 188));
        mSprite.setPosition(4, 762 - 188);
        mSprites.emplace(type, mSprite);
        break;

    case 3:
        mSprite.setTextureRect(sf::IntRect(382, 36, 36, 150));
        mSprite.setPosition(8 + 36, 762 - 150);
        mSprites.emplace(type, mSprite);
        break;

    case 4:
        mSprite.setTextureRect(sf::IntRect(382, 36, 36, 112));
        mSprite.setPosition(12 + 2 * 36, 762 - 112);
        mSprites.emplace(type, mSprite);
        break;

    case 5:
        mSprite.setTextureRect(sf::IntRect(382, 36, 36, 74));
        mSprite.setPosition(16 + 3 * 36, 762 - 74);
        mSprites.emplace(type, mSprite);
        break;

    case 6:
        mSprite.setTextureRect(sf::IntRect(382, 36, 36, 188));
        mSprite.setPosition(762 - 4 - 36, 0);
        mSprites.emplace(type, mSprite);
        break;

    case 7:
        mSprite.setTextureRect(sf::IntRect(382, 36, 36, 150));
        mSprite.setPosition(762 - 8 - 2 * 36, 0);
        mSprites.emplace(type, mSprite);
        break;

    case 8:
        mSprite.setTextureRect(sf::IntRect(382, 36, 36, 112));
        mSprite.setPosition(762 - 12 - 3 * 36, 0);
        mSprites.emplace(type, mSprite);
        break;

    case 9:
        mSprite.setTextureRect(sf::IntRect(382, 36, 36, 74));
        mSprite.setPosition(762 - 16 - 4 * 36, 0);
        mSprites.emplace(type, mSprite);
        break;

    default:
        break;
    }
}

void Graphics::makeHitSprite(int x, int y)
{
    mSprite.setTextureRect(sf::IntRect(382, 0, 36, 36));
    mSprite.setPosition(x * 2 + (x - 1) * 36, y * 2 + (y - 1) * 36);
    mSprites.emplace(mSprites.size(), mSprite);
}

void Graphics::draw(sf::RenderWindow *window)
{
    for (std::map<int, sf::Sprite>::iterator iter(mSprites.begin()); iter != mSprites.end(); ++iter)
    {
        window->draw(iter->second);
    }
}