#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <string>
#include <iostream>
#include <stdexcept>

class TextureManager {
private:
    std::unordered_map<std::string, sf::Texture> textures;

public:
 
    void loadTexture(const std::string& key, const std::string& filename) {
        sf::Texture texture;
        if (!texture.loadFromFile(filename)) {
            std::cerr << "Failed to load texture: " << filename << "\n";
            throw std::runtime_error("Texture loading failed");
        }


        textures[key] = std::move(texture);
        std::cout << "Texture loaded successfully: " << key << " from " << filename << "\n";
    }

   
    const sf::Texture& getTexture(const std::string& key) const {
        auto it = textures.find(key);
        
        if (it == textures.end()) {
            std::cerr << "Texture not found: " << key << "\n";
            throw std::runtime_error("Texture not found");
        }
        return it->second;
    }

   
    void clear() {
        textures.clear();
        std::cout << "All textures cleared.\n";
    }
};
