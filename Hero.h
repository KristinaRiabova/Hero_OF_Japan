    #pragma once
    #include <SFML/Graphics.hpp>
    #include <iostream>
    #include <fstream> 
    #include "TextureManager.h"

    class Hero : public sf::Drawable, public sf::Transformable {
    private:
        enum Direction {
            left, right
        };

        sf::Texture heroTexture;
        sf::Sprite heroSprite;
        Direction direct = Direction::right;
        int numTexture = 0;
        int countTextures = 9;
        bool collectedAppleThisQuest = false;

        bool hasApple = false;

        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
            states.transform *= getTransform();
            states.texture = &heroTexture;
            target.draw(heroSprite, states);
        }

        void NextTexture() {
            heroSprite.setTextureRect(sf::IntRect(numTexture * 128, 0, 128, 128));
            numTexture++;
            if (numTexture == countTextures) numTexture = 0;
        }

    public:
        Hero() {}

        bool load(const sf::Texture& texture, unsigned int width, unsigned int height) {
            heroTexture = texture;
            heroSprite.setTexture(heroTexture);
            heroSprite.setTextureRect(sf::IntRect(0, 0, width, height));
            heroSprite.setOrigin(heroSprite.getLocalBounds().width / 2, heroSprite.getLocalBounds().height / 2);
            heroSprite.setPosition(100, 500);
            return true;
        }

        void move(int dx, int dy) {
            NextTexture();
            int newPositionX = heroSprite.getPosition().x + dx;
            int newPositionY = heroSprite.getPosition().y - dy;

            newPositionX = std::max(0, std::min(newPositionX, 800));  
            newPositionY = std::max(500, std::min(newPositionY, 530));  

            int wHero = abs(heroSprite.getLocalBounds().width * heroSprite.getScale().x);
            int hHero = abs(heroSprite.getLocalBounds().height * heroSprite.getScale().y);

            if (dx > 0) {
                if (direct == left) heroSprite.setScale(1.f, 1.f);
                direct = right;
            }
            else if (dx < 0) {
                if (direct == right) heroSprite.setScale(-1.f, 1.f);
                direct = left;
            }

            heroSprite.setPosition(newPositionX, newPositionY);

            
            
        }

        void serialize(std::ofstream& outFile, const sf::Time& elapsedTime) const {
            sf::Vector2f position = heroSprite.getPosition();
            outFile.write(reinterpret_cast<const char*>(&position), sizeof(position));

            outFile.write(reinterpret_cast<const char*>(&numTexture), sizeof(numTexture));

          
            float elapsedSeconds = elapsedTime.asSeconds();
            outFile.write(reinterpret_cast<const char*>(&elapsedSeconds), sizeof(elapsedSeconds));
        }

        void deserialize(std::ifstream& inFile, sf::Time& elapsedTime) {
            sf::Vector2f position;
            inFile.read(reinterpret_cast<char*>(&position), sizeof(position));
            heroSprite.setPosition(position);

            inFile.read(reinterpret_cast<char*>(&numTexture), sizeof(numTexture));
            NextTexture();


            float elapsedSeconds;
            inFile.read(reinterpret_cast<char*>(&elapsedSeconds), sizeof(elapsedSeconds));
            elapsedTime = sf::seconds(elapsedSeconds);
        }
        sf::FloatRect getBounds() const {
                return heroSprite.getGlobalBounds();
        }




        void collectApple() {
            hasApple = true;
            std::cout << "Hero has collected an apple!" << std::endl;
        }

        bool getHasApple() const {
            return hasApple;
        }

        void resetApple() {
            hasApple = false;
        }
        bool hasCollectedAppleThisQuest() const {
            return collectedAppleThisQuest;
        }

        void setCollectedAppleThisQuest(bool value) {
            collectedAppleThisQuest = value;
        }
    };
