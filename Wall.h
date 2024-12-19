#pragma once
#include <SFML/Graphics.hpp>

class Wall : public sf::Drawable, public sf::Transformable {
private:
    sf::RectangleShape shape;

public:
    Wall(float width, float height, float posX, float posY) {
        shape.setSize({width, height});
        shape.setFillColor(sf::Color::Black);
        shape.setPosition(posX, posY);
    }

   
    sf::FloatRect getBounds() const {
        return shape.getGlobalBounds();
    }

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
        target.draw(shape, states);
    }
};
