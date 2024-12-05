#include "gameProject.h"
#include "TextureManager.h"
#include "AudioManager.h"
#include "LoadResources.h"  
#include "Utils.h"

using namespace std;

string GetTime(sf::Time t);
void runGameLoop(sf::RenderWindow& window, TextureManager& textureManager, AudioManager& audioManager);

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML + CMake Example");

    TextureManager textureManager;
    AudioManager audioManager;

   
    if (!loadResources(textureManager, audioManager)) {
        return -1;
    }

   
    runGameLoop(window, textureManager, audioManager);

    return 0;
}

string GetTime(sf::Time elapsed) {
    int minutes = static_cast<int>(elapsed.asSeconds()) / 60;
    int seconds = static_cast<int>(elapsed.asSeconds()) % 60;

    std::ostringstream timeStream;
    timeStream << std::setfill('0') << std::setw(2) << minutes
        << ":" << std::setfill('0') << std::setw(2) << seconds;
    return timeStream.str();
}
