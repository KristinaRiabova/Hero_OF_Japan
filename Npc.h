#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

class Npc : public sf::Drawable, public sf::Transformable {
private:
    enum Direction {
        left, right
    };

    sf::Texture npcTexture;
    sf::Sprite npcSprite;
    Direction direction = Direction::right;
    int numTexture = 0;
    int countTextures = 9;

    float animationSpeed = 0.1f; 
    sf::Time lastUpdateTime;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
        states.transform *= getTransform();
        states.texture = &npcTexture;
        target.draw(npcSprite, states);
    }

    void NextTexture(sf::Time deltaTime) {
       
        lastUpdateTime += deltaTime;

        
        if (lastUpdateTime.asSeconds() >= animationSpeed) {
            numTexture++;  
            if (numTexture >= countTextures) {
                numTexture = 0; 
            }

          
            npcSprite.setTextureRect(sf::IntRect(numTexture * 128, 0, 128, 128));
            lastUpdateTime = sf::Time::Zero;  
        }
    }

public:
    bool load(const std::string& textureFile) {
        if (!npcTexture.loadFromFile(textureFile)) {
            std::cerr << "Error: Failed to load NPC texture from file: " << textureFile << "\n";
            return false;
        }

        npcSprite.setTexture(npcTexture);
        npcSprite.setTextureRect(sf::IntRect(0, 0, 128, 128));  
        npcSprite.setOrigin(npcSprite.getLocalBounds().width / 2, npcSprite.getLocalBounds().height / 2);

        npcSprite.setPosition(600, 500);  

        std::cout << "NPC texture loaded successfully from " << textureFile << "\n";

        return true;
    }

    void idleAnimation(sf::Time deltaTime) {
        NextTexture(deltaTime);  
    }

    sf::Vector2f getPosition() const {
        return npcSprite.getPosition();
    }
};
