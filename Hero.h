#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

class Hero : public sf::Drawable, public sf::Transformable {
private:
    enum Direction {
        left, right
    };

    sf::Texture heroTexture;
    sf::Sprite heroSprite;
    Direction direct = Direction::right;
    int numTexture = 0;
    int countTextures = 9;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        states.transform *= getTransform();
        states.texture = &heroTexture;
        target.draw(heroSprite, states);
    }

    void NextTexture() {
        heroSprite.setTextureRect(sf::IntRect(numTexture * 128, 0, 128, 128));
        numTexture++;
        if (numTexture == countTextures) numTexture = 0;
    }

public:
    bool load(const std::string& bgset, unsigned int width, unsigned int height) {

        if (!heroTexture.loadFromFile(bgset)) {
            return false;
        }

        heroSprite.setTexture(heroTexture);
        heroSprite.setTextureRect(sf::IntRect(0, 0, 128, 128));
        heroSprite.setOrigin(heroSprite.getLocalBounds().width / 2, heroSprite.getLocalBounds().height / 2);

        int startX = 800 / 2 - heroSprite.getLocalBounds().width / 2;
        int startY = 600 / 2 - heroSprite.getLocalBounds().height / 2 - 5;

        heroSprite.setPosition(startX, startY);

        return true;
    }

    void move(int dx, int dy) {
        NextTexture();
        int newPositionX = heroSprite.getPosition().x + dx;
        int newPositionY = heroSprite.getPosition().y - dy;

        
        newPositionX = std::max(0, std::min(newPositionX, 800));  

        
        newPositionY = std::max(500, std::min(newPositionY, 530));  

        int wHero = abs(heroSprite.getLocalBounds().width * heroSprite.getScale().x);
        int hHero = abs(heroSprite.getLocalBounds().height * heroSprite.getScale().y);

        if (dx > 0) {
            if (direct == left) heroSprite.setScale(1.f, 1.f);
            direct = right;
        }
        else if (dx < 0) {
            if (direct == right) heroSprite.setScale(-1.f, 1.f);
            direct = left;
        }

        // Set the new position with the constraints applied
        heroSprite.setPosition(newPositionX, newPositionY);
    }
};
