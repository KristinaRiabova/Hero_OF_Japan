#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>

class Panda : public sf::Drawable, public sf::Transformable
{
private:
    enum Direction
    {
        left,
        right
    };

    sf::Texture npcTexture;
    sf::Sprite npcSprite;
    Direction direction = Direction::right;
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
    bool load(const sf::Texture &texture, const sf::Font &fnt)
    {
        npcTexture = texture;
        npcSprite.setTexture(npcTexture);
        npcSprite.setTextureRect(sf::IntRect(0, 0, 128, 128));
        npcSprite.setOrigin(npcSprite.getLocalBounds().width / 2, npcSprite.getLocalBounds().height / 2);
        npcSprite.setPosition(100, 600);

        return true;
    }

    void idleAnimation(sf::Time deltaTime)
    {
        NextTexture(deltaTime);
    }

    sf::Vector2f getPosition() const
    {
        return npcSprite.getPosition();
    }

    void setPaused(bool paused)
    {
        isPaused = paused;
    }

    void update(sf::Time deltaTime)
    {
        if (!isPaused)
        {
            idleAnimation(deltaTime);
        }
    }

    sf::FloatRect getBounds() const
    {
        return npcSprite.getGlobalBounds();
    }

    void serialize(std::ofstream& outFile) const
    {
        sf::Vector2f position = npcSprite.getPosition();
        outFile.write(reinterpret_cast<const char*>(&position.x), sizeof(position.x));
        outFile.write(reinterpret_cast<const char*>(&position.y), sizeof(position.y));
        outFile.write(reinterpret_cast<const char*>(&numTexture), sizeof(numTexture));
        outFile.write(reinterpret_cast<const char*>(&direction), sizeof(direction));
        outFile.write(reinterpret_cast<const char*>(&isPaused), sizeof(isPaused));
    }

    void deserialize(std::ifstream& inFile)
    {
        sf::Vector2f position;
        inFile.read(reinterpret_cast<char*>(&position.x), sizeof(position.x));
        inFile.read(reinterpret_cast<char*>(&position.y), sizeof(position.y));
        npcSprite.setPosition(position);
        inFile.read(reinterpret_cast<char*>(&numTexture), sizeof(numTexture));
        npcSprite.setTextureRect(sf::IntRect(numTexture * 128, 0, 128, 128));
        inFile.read(reinterpret_cast<char*>(&direction), sizeof(direction));
        inFile.read(reinterpret_cast<char*>(&isPaused), sizeof(isPaused));
    }
};
