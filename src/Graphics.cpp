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

void Graphics::makeGridSprite(int type)
{
    mSprite.setTexture(mTexture);
    //sf::Transform transform;

    switch (type)
    {
    case 0:
        mSprite.setTextureRect(sf::IntRect(0, 0, 382, 382));
        mSprite.setPosition(0, 0);
        //mSprites.emplace(type, std::make_pair(mSprite, transform));
        mSprites.emplace(type, mSprite);
        break;

    case 1:
        mSprite.setTextureRect(sf::IntRect(0, 0, 382, 382));
        mSprite.setPosition(380, 380);
        //mSprites.emplace(type, std::make_pair(mSprite, transform));
        mSprites.emplace(type, mSprite);
        break;

    /*case 2:
        mSprite.setTextureRect(sf::IntRect(382, 36, 36, 188));
        mSprite.setPosition(4, 762 - 188);
        mSprites.emplace(type, std::make_pair(mSprite, transform));
        break;

    case 3:
        mSprite.setTextureRect(sf::IntRect(382, 36, 36, 150));
        mSprite.setPosition(8 + 36, 762 - 150);
        mSprites.emplace(type, std::make_pair(mSprite, transform));
        break;

    case 4:
        mSprite.setTextureRect(sf::IntRect(382, 36, 36, 112));
        mSprite.setPosition(12 + 2 * 36, 762 - 112);
        mSprites.emplace(type, std::make_pair(mSprite, transform));
        break;

    case 5:
        mSprite.setTextureRect(sf::IntRect(382, 36, 36, 74));
        mSprite.setPosition(16 + 3 * 36, 762 - 74);
        mSprites.emplace(type, std::make_pair(mSprite, transform));
        break;*/

    default:
        break;
    }
}

void Graphics::makeHitSprite(int x, int y)
{
    mSprite.setTextureRect(sf::IntRect(382, 0, 36, 36));
    mSprite.setPosition(x * 2 + (x - 1) * 36, y * 2 + (y - 1) * 36);
    //sf::Transform transform;
    //mSprites.emplace(mSprites.size(), std::make_pair(mSprite, transform));
    mSprites.emplace(mSprites.size(), mSprite);
}

void Graphics::makeShipSprite(int x, int y)
{
    mSprite.setTextureRect(sf::IntRect(382, 36, 36, 36));
    mSprite.setPosition(x * 2 + (x - 1) * 36, y * 2 + (y - 1) * 36);
    //sf::Transform transform;
    //mSprites.emplace(mSprites.size(), std::make_pair(mSprite, transform));
    mSprites.emplace(mSprites.size(), mSprite);
}

sf::Vector2f Graphics::getSpritePos(int type)
{
    //return std::get<0>(mSprites.find(type)->second).getPosition();
    return mSprites.find(type)->second.getPosition();
}

void Graphics::setSpritePos(int type, sf::Vector2f pos)
{
    //std::get<0>(mSprites.find(type)->second).setPosition(pos);
    mSprites.find(type)->second.setPosition(pos);
}

std::map<int, sf::Sprite>* Graphics::getSpritesList()
{
    return &mSprites;
}

void Graphics::draw(sf::RenderWindow *window)
{
    for (auto iter(mSprites.begin()); iter != mSprites.end(); ++iter)
    {
        window->draw(iter->second);
    }
}