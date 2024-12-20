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
#include "Panda.h"
#include "SceneGraphNode.h"
#include "SpatialPartition.h"

#include "Wall.h"
#include "Cloud.h"

void runGameLoop(sf::RenderWindow &window, TextureManager &textureManager, AudioManager &audioManager)
{
    SpatialPartition spatialPartition(128, 800, 600);
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



    Npc npc;
    if (!npc.load(textureManager.getTexture("npc")))
    {
        return;
    }

    Hero hero;
    if (!hero.load(textureManager.getTexture("hero"), 128, 128))
    {
        return;
    }

    Cloud cloud1(textureManager.getTexture("cloud"), sf::Vector2f(100.f, 50.f), sf::Vector2f(0.1f, 0.f));
    Cloud cloud2(textureManager.getTexture("cloud"), sf::Vector2f(400.f, 100.f), sf::Vector2f(-0.08f, 0.f));

    Wall wall1(10, 100, 500, 550);

    Wall wall2(10, 100, 10, 550);

    auto rootNode = std::make_shared<SceneGraphNode>(nullptr, nullptr);
    auto backgroundNode = std::make_shared<SceneGraphNode>(&background, &background);

    auto heroNode = std::make_shared<SceneGraphNode>(&hero, &hero);
    auto npcNode = std::make_shared<SceneGraphNode>(&npc, &npc);
    auto wall1Node = std::make_shared<SceneGraphNode>(&wall1, &wall1);
    auto wall2Node = std::make_shared<SceneGraphNode>(&wall2, &wall2);
    auto cloud1Node = std::make_shared<SceneGraphNode>(&cloud1, &cloud1);
    auto cloud2Node = std::make_shared<SceneGraphNode>(&cloud2, &cloud2);

    rootNode->addChild(backgroundNode);
    rootNode->addChild(heroNode);
    rootNode->addChild(npcNode);
    rootNode->addChild(wall1Node);
    rootNode->addChild(wall2Node);
    rootNode->addChild(cloud1Node);
    rootNode->addChild(cloud2Node);

    bool isPause = false;
    bool isWinInactive = false;
    bool npcIdle = true;
    bool heroIdle = true;

    sf::Clock clock;
    sf::Clock pauseClock;

    sf::Text pause = sf::Text("", font, 14);
    pause.setFillColor(sf::Color(0, 0, 0));
    pause.setPosition(720, 10);

    sf::Text time("Time game: 00:00", font, 14);
    time.setFillColor(sf::Color::Black);
    time.setPosition(10, 10);

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

                    if (!isPause)
                    {
                        if (event.key.code == sf::Keyboard::Left)
                        {
                            hero.move(-10, 0, wall1, wall2);
                        }
                        else if (event.key.code == sf::Keyboard::Right)
                        {
                            hero.move(10, 0, wall1, wall2);
                        }
                        else if (event.key.code == sf::Keyboard::Up)
                        {
                            hero.move(0, 10, wall1, wall2);
                        }
                        else if (event.key.code == sf::Keyboard::Down)
                        {
                            hero.move(0, -10, wall1, wall2);
                        }
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

                        }
                        else
                        {
                            isPause = true;
                            pauseClock.restart();
                            pause.setString("Pause");
                            backgroundMusic->pause();
                            npc.setPaused(true);
                        }
                    }

                    if (event.key.code == sf::Keyboard::S)
                    {
                        std::ofstream outFile("game_data.dat", std::ios::binary);
                        if (outFile.is_open())
                        {
                            hero.serialize(outFile, clock.getElapsedTime() - TimeOnPause);
                            npc.serialize(outFile);
                            cloud1.serialize(outFile); 
                            cloud2.serialize(outFile);

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
                            cloud1.deserialize(inFile); // Load cloud1 data
                            cloud2.deserialize(inFile); // Load cloud2 data

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
        }

        cloud1.update();
        cloud2.update();

        if (cloud1.getBounds().intersects(cloud2.getBounds()))
    {
        cloud1.resolveCollision();
        cloud2.resolveCollision();

    }


        spatialPartition.clear();
        sf::FloatRect heroBounds = hero.getBounds();
        spatialPartition.insert(&heroBounds);

        sf::FloatRect npcBounds = npc.getBounds();
        spatialPartition.insert(&npcBounds);

        sf::FloatRect cloud1Bounds = cloud1.getBounds();
        spatialPartition.insert(&cloud1Bounds);

        sf::FloatRect cloud2Bounds = cloud2.getBounds();
        spatialPartition.insert(&cloud2Bounds);

        sf::FloatRect wall1Bounds = wall1.getBounds();
        spatialPartition.insert(&wall1Bounds);

        sf::FloatRect wall2Bounds = wall2.getBounds();
        spatialPartition.insert(&wall2Bounds);

        auto nearbyObjects = spatialPartition.query(hero.getBounds());

        for (const auto &obj : nearbyObjects)
        {


             if (hero.getBounds().intersects(*obj))
            {

                
            else if (cloud2.getBounds().intersects(*obj))
            {

                std::cout << "Collision detected with cloud2 object.\n";
            }
            }


        }

        window.clear();

        rootNode->draw(window, sf::RenderStates::Default);
        window.draw(time);
        window.draw(pause);

        window.display();

        if ((clock.getElapsedTime() - TimeOnPause - pauseClock.getElapsedTime()).asSeconds() > 600)
        {
            break;
        }
    }
}
