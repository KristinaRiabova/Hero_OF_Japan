#ifndef CLOUD_H
#define CLOUD_H

#include <SFML/Graphics.hpp>
#include <fstream>

class Cloud : public sf::Drawable, public sf::Transformable
{
private:
    sf::Sprite sprite;
    sf::Vector2f velocity;

public:
    Cloud(const sf::Texture &texture, const sf::Vector2f &position, const sf::Vector2f &velocity)
        : velocity(velocity)
    {
        sprite.setTexture(texture);
        sprite.setPosition(position);
    }

    void update()
    {
        sprite.move(velocity);

        if (sprite.getPosition().x < -sprite.getGlobalBounds().width)
        {
            sprite.setPosition(800.f, sprite.getPosition().y); 
        }
        else if (sprite.getPosition().x > 800.f)
        {
            sprite.setPosition(-sprite.getGlobalBounds().width, sprite.getPosition().y);
        }
    }

    sf::FloatRect getBounds() const
    {
        return sprite.getGlobalBounds();
    }

    void resolveCollision()
    {
        velocity.x = -velocity.x;
    }

 
    void serialize(std::ofstream &outFile) const
    {
    
        outFile.write(reinterpret_cast<const char*>(&sprite.getPosition()), sizeof(sf::Vector2f));
        outFile.write(reinterpret_cast<const char*>(&velocity), sizeof(sf::Vector2f));
    }

  
    void deserialize(std::ifstream &inFile)
    {

        sf::Vector2f position;
        inFile.read(reinterpret_cast<char*>(&position), sizeof(sf::Vector2f));
        inFile.read(reinterpret_cast<char*>(&velocity), sizeof(sf::Vector2f));

        sprite.setPosition(position);
    }

    

private:
    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override
    {
        target.draw(sprite, states);
    }
};

#endif
