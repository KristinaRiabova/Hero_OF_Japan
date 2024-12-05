#pragma once
#include <SFML/Audio.hpp>
#include <unordered_map>
#include <string>
#include <iostream>
#include <stdexcept>
#include <memory> 

class AudioManager {
private:
    std::unordered_map<std::string, std::unique_ptr<sf::Music>> musicFiles; 

public:

    void loadMusic(const std::string& key, const std::string& filename) {
        auto music = std::make_unique<sf::Music>(); 
        if (!music->openFromFile(filename)) {
            std::cerr << "Failed to load music: " << filename << "\n";
            throw std::runtime_error("Music loading failed");
        }

        musicFiles[key] = std::move(music);  
        std::cout << "Music loaded successfully: " << key << " from " << filename << "\n";
    }


    sf::Music* getMusic(const std::string& key) {
        auto it = musicFiles.find(key);
        if (it == musicFiles.end()) {
            std::cerr << "Music not found: " << key << "\n";
            throw std::runtime_error("Music not found");
        }
        return it->second.get();  
    }

};
