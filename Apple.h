#pragma once
#include <SFML/Graphics.hpp>

class Apple : public sf::Drawable {
private:
    sf::Sprite appleSprite;
    bool collected = false;

public:
    Apple(const sf::Texture& texture, sf::Vector2f position) {
        appleSprite.setTexture(texture);
        appleSprite.setPosition(position);
    }

    void collect() {
        collected = true;
    }

    bool isCollected() const {
        return collected;
    }

    sf::FloatRect getBounds() const {
        return appleSprite.getGlobalBounds();
    }


    sf::Vector2f getPosition() const {
        return appleSprite.getPosition();
    }

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
        if (!collected) {
            target.draw(appleSprite, states);
        }
    }

    void setPosition(sf::Vector2f position) {
        appleSprite.setPosition(position);
    }
};
