#include "gameProject.h"
#include "Npc.h"
using namespace std;

string GetTime(sf::Time t);

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML + CMake Example");
    sf::Time TimeOnPause;

    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("./images/background.png")) {
        std::cerr << "Failed to load background image\n";
        return -1;
    }

    sf::Sprite background(backgroundTexture);

    Hero hero;
    if (!hero.load("./images/Walk.png", 128, 128)) {
        return -1;
    }
    Npc npc;
    if (!npc.load("./images/Idle.png")) {  
        return -1;
    }

    bool isPause = false;
    bool isWinInactive = false;

    sf::Clock clock;
    sf::Clock pauseClock;
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        std::cerr << "Failed to load font\n";
        return -1;
    }

    sf::Text pause = sf::Text("", font, 14);
    pause.setColor(sf::Color(0, 0, 0));
    pause.setPosition(720, 10);
    sf::Text time = sf::Text("Time game: 00:00", font, 14);
    time.setColor(sf::Color(0, 0, 0));
    time.setPosition(10, 10);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (window.hasFocus()) {
                if (isWinInactive) {
                    isWinInactive = false;
                    TimeOnPause += pauseClock.getElapsedTime();
                }
                if (event.type == sf::Event::KeyPressed) {
                    if (!isPause) {
                        if (event.key.code == sf::Keyboard::Left) hero.move(-10, 0);
                        else if (event.key.code == sf::Keyboard::Right) hero.move(10, 0);
                        else if (event.key.code == sf::Keyboard::Up) hero.move(0, 10);
                        else if (event.key.code == sf::Keyboard::Down) hero.move(0, -10);
                    }
                    if (event.key.code == sf::Keyboard::P) {
                        if (isPause) {
                            isPause = false;
                            TimeOnPause += pauseClock.getElapsedTime();
                            pauseClock.restart();
                            pause.setString("");
                        }
                        else {
                            isPause = true;
                            pauseClock.restart();
                            pause.setString("Pause");
                        }
                    }
                }
            }
            else {
                if (!isWinInactive) {
                    isWinInactive = true;
                    pauseClock.restart();
                }
            }
        }

        if (!isPause) {
            time.setString("Time game: " + GetTime(clock.getElapsedTime() - TimeOnPause));
        }

        // Update NPC animation
        npc.idleAnimation(clock.getElapsedTime());

        window.clear();
        window.draw(background);
        window.draw(time);
        window.draw(pause);
        window.draw(hero);
        window.draw(npc);

        window.display();

        if ((clock.getElapsedTime() - TimeOnPause - pauseClock.getElapsedTime()).asSeconds() > 10) break;
    }
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
