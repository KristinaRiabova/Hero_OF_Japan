
#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>

class Npc : public sf::Drawable, public sf::Transformable
{
private:


    sf::Texture npcTexture;
    sf::Sprite npcSprite;

    int numTexture = 0;
    int countTextures = 6;
    float animationSpeed = 0.1f;
    sf::Clock animationClock;
    bool isPaused = false;

    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override
    {
        states.transform *= getTransform();
        states.texture = &npcTexture;
        target.draw(npcSprite, states);
    }

    void NextTexture(sf::Time deltaTime)
    {
        if (isPaused)
            return;

        if (animationClock.getElapsedTime().asSeconds() >= animationSpeed)
        {
            numTexture++;
            if (numTexture >= countTextures)
            {
                numTexture = 0;
            }
            npcSprite.setTextureRect(sf::IntRect(numTexture * 128, 0, 128, 128));
            animationClock.restart();
        }
    }

public:
    bool load(const sf::Texture &texture)
    {
        npcTexture = texture;
        npcSprite.setTexture(npcTexture);
        npcSprite.setTextureRect(sf::IntRect(0, 0, 128, 128));
        
        npcSprite.setPosition(700, 450);  
        return true;
    }

    void setPaused(bool paused)
    {
        isPaused = paused;
    }

    void idleAnimation(sf::Time deltaTime)
    {
        NextTexture(deltaTime);
    }

    sf::Vector2f getPosition() const
    {
        return npcSprite.getPosition();
    }

    void update(sf::Time deltaTime)
    {
        idleAnimation(deltaTime);
    }

    sf::FloatRect getBounds() const
    {
        return npcSprite.getGlobalBounds();
    }

    void serialize(std::ofstream &outFile) const
    {
        sf::Vector2f position = npcSprite.getPosition();
        outFile.write(reinterpret_cast<const char *>(&position), sizeof(position));
        outFile.write(reinterpret_cast<const char *>(&numTexture), sizeof(numTexture));
    }

    void deserialize(std::ifstream &inFile)
    {
        sf::Vector2f position;
        inFile.read(reinterpret_cast<char *>(&position), sizeof(position));
        npcSprite.setPosition(position);

        inFile.read(reinterpret_cast<char *>(&numTexture), sizeof(numTexture));
        npcSprite.setTextureRect(sf::IntRect(numTexture * 128, 0, 128, 128));
    }
};
