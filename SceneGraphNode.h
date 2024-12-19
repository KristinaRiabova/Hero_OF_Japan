#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#pragma once

class SceneGraphNode {
public:


    sf::Drawable* drawable = nullptr;
    sf::Transformable* transformable = nullptr;
    std::vector<std::shared_ptr<SceneGraphNode>> children;

    SceneGraphNode(sf::Drawable* drawable = nullptr, sf::Transformable* transformable = nullptr)
        : drawable(drawable), transformable(transformable) {}

    void addChild(std::shared_ptr<SceneGraphNode> child) {
        children.push_back(child);
    }

    void draw(sf::RenderTarget& target, sf::RenderStates states) {
        if (drawable) {
            states.transform *= transformable->getTransform();
            target.draw(*drawable, states);
        }
        for (auto& child : children) {
            child->draw(target, states);
        }
    }

    sf::FloatRect getGlobalBounds() const {
        if (drawable) {
            auto sprite = dynamic_cast<sf::Sprite*>(drawable);
            if (sprite) return sprite->getGlobalBounds();
        }
        return {};
    }

    bool checkCollision(const SceneGraphNode& other) const {
        return getGlobalBounds().intersects(other.getGlobalBounds());
    }

    
    
};

