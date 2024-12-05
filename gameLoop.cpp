#include "gameProject.h"
#include "TextureManager.h"
#include "AudioManager.h"
#include "Hero.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <fstream>
#include <iostream>
#include <sstream>
#include "Utils.h"
#include "Npc.h"
#include "Apple.h"
#include "TextureManager.h"
#include "Panda.h"

void runGameLoop(sf::RenderWindow &window, TextureManager &textureManager, AudioManager &audioManager)
{
    sf::Time TimeOnPause;
    sf::Font font;
    if (!font.loadFromFile("./font/PressStart2P-Regular.ttf"))
    {
        std::cerr << "Failed to load font\n";
        return;
    }

    sf::Music *backgroundMusic = audioManager.getMusic("background_music");
    backgroundMusic->setLoop(true);
    backgroundMusic->play();

    sf::Sprite background(textureManager.getTexture("background"));
    sf::Sprite wall(textureManager.getTexture("wall"));
    wall.setPosition(200, 550);

    Hero hero;
    if (!hero.load(textureManager.getTexture("hero"), 128, 128))
    {
        return;
    }

    Npc npc;
    if (!npc.load(textureManager.getTexture("npc"), font))
    {
        return;
    }

    Panda panda;
    if (!panda.load(textureManager.getTexture("panda"), font))
    {
        return;
    }

    Apple apple(textureManager.getTexture("apple"), sf::Vector2f(100, 560));
    bool isPause = false;
    bool isWinInactive = false;
    bool npcIdle = true;

    bool questAccepted = false;

    std::vector<sf::Vector2f> apples;

    sf::Clock clock;
    sf::Clock pauseClock;

    sf::Text pause = sf::Text("", font, 14);
    pause.setColor(sf::Color(0, 0, 0));
    pause.setPosition(720, 10);

    sf::Text time("Time game: 00:00", font, 14);
    time.setFillColor(sf::Color::Black);
    time.setPosition(10, 10);

    sf::Text questMessage("Press E", font, 10);
    questMessage.setFillColor(sf::Color::Black);
    questMessage.setPosition(550, 450);

    while (window.isOpen())
    {
        sf::Event event;

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)

                window.close();

            if (window.hasFocus())
            {
                if (isWinInactive)
                {
                    isWinInactive = false;
                    TimeOnPause += pauseClock.getElapsedTime();
                }

                if (event.type == sf::Event::KeyPressed)
                {

                    if (event.key.code == sf::Keyboard::E)
                    {
                        if (hero.getBounds().intersects(npc.getBounds()))
                        {
                            if (npc.isQuestWindowOpen())
                            {
                                npc.setCanTalkAfterQuest(true);
                                std::cout << "You can talk to the NPC again!" << std::endl;
                            }

                            if (!questAccepted)
                            {
                                npc.openQuestWindow([&](bool accepted)
                                                    {
                                                        questAccepted = accepted;
                                                        if (accepted) {
                                                            std::cout << "Quest accepted!\n";
                                                        } else {
                                                            std::cout << "Quest rejected!\n";
                                                        } });
                            }
                            else
                            {
                                std::cout << "Quest already accepted!\n";
                            }
                        }
                    }

                    if (!isPause)
                    {
                        if (event.key.code == sf::Keyboard::Left)
                            hero.move(-10, 0);
                        else if (event.key.code == sf::Keyboard::Right)
                            hero.move(10, 0);
                        else if (event.key.code == sf::Keyboard::Up)
                            hero.move(0, 10);
                        else if (event.key.code == sf::Keyboard::Down)
                            hero.move(0, -10);
                    }

                    if (event.key.code == sf::Keyboard::P)
                    {
                        if (isPause)
                        {
                            isPause = false;
                            TimeOnPause += pauseClock.getElapsedTime();
                            pauseClock.restart();
                            pause.setString("");
                            backgroundMusic->play();
                            npc.setPaused(false);
                            panda.setPaused(false);
                        }
                        else
                        {
                            isPause = true;
                            pauseClock.restart();
                            pause.setString("Pause");
                            backgroundMusic->pause();
                            npc.setPaused(true);
                            panda.setPaused(true);
                        }
                    }

                    if (event.key.code == sf::Keyboard::S)
                    {
                        std::ofstream outFile("game_data.dat", std::ios::binary);
                        if (outFile.is_open())
                        {
                            hero.serialize(outFile, clock.getElapsedTime() - TimeOnPause);
                            npc.serialize(outFile);
                            panda.serialize(outFile);
                   
                            audioManager.saveMusicProgress("background_music", outFile);

                            outFile.close();
                            std::cout << "Game progress saved.\n";
                        }
                        else
                        {
                            std::cerr << "Failed to open file for saving game data.\n";
                        }
                    }

                    if (event.key.code == sf::Keyboard::L)
                    {
                        std::ifstream inFile("game_data.dat", std::ios::binary);
                        if (inFile.is_open())
                        {
                            sf::Time loadedTime;
                            hero.deserialize(inFile, loadedTime);
                            npc.deserialize(inFile);
                            panda.deserialize(inFile);
                            TimeOnPause = clock.getElapsedTime() - loadedTime;

                           
                            audioManager.loadMusicProgress("background_music", inFile);

                            inFile.close();
                            std::cout << "Game progress loaded.\n";
                        }
                        else
                        {
                            std::cerr << "Failed to open file for loading game data.\n";
                        }
                    }
                }

                npc.handleInput(event);
                if (hero.getBounds().intersects(apple.getBounds()) && !hero.getHasApple() && questAccepted && !hero.hasCollectedAppleThisQuest())
                {
                    hero.collectApple();
                    apple = Apple(textureManager.getTexture("apple"), sf::Vector2f(rand() % 800, 550));
                    hero.setCollectedAppleThisQuest(true);
                    npc.checkQuestCompletion(hero);
                }
            }
            else
            {
                if (!isWinInactive)
                {
                    isWinInactive = true;
                    pauseClock.restart();
                }
            }
        }

        if (!isPause)
        {
            time.setString("Time game: " + GetTime(clock.getElapsedTime() - TimeOnPause));
        }

        if (npcIdle)
        {
            npc.idleAnimation(clock.getElapsedTime());
            panda.idleAnimation(clock.getElapsedTime());
        }

        
        window.clear();
        window.draw(background);
        window.draw(time);
        window.draw(pause);
        window.draw(panda);
        window.draw(npc);
        window.draw(hero);
        window.draw(wall);
        

        window.draw(apple);

        if (questAccepted)
        {
            questMessage.setString("Find an apple");
            npc.checkQuestCompletion(hero);
            if (npc.isQuestCompleted())
            {
                questMessage.setString("You have completed the quest!");
                questMessage.setPosition(250, 100);
                
            }
        }

        if (hero.getBounds().left + hero.getBounds().width >= 520 && hero.getBounds().left <= 660)
        {
            if (npc.isQuestCompleted())
            {
                questMessage.setString("You have completed the quest!");
                questMessage.setPosition(250, 100);
                window.draw(questMessage);
                npc.setupThankYouText();
                window.draw(npc.getThankYouText());
            }
            else if (!npc.isQuestWindowOpen())
            {

                window.draw(questMessage);
            }
        }

        window.display();

        if ((clock.getElapsedTime() - TimeOnPause - pauseClock.getElapsedTime()).asSeconds() > 600)
        {
            break;
        }
    }
}
