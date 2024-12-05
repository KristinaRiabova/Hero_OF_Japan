#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>

class Npc : public sf::Drawable, public sf::Transformable
{
private:
    enum Direction
    {
        left,
        right
    };

    sf::Texture npcTexture;
    sf::Sprite npcSprite;
    Direction direction = Direction::right;
    int numTexture = 0;
    int countTextures = 6;

    float animationSpeed = 0.1f;
    sf::Clock animationClock;
    bool isPaused = false;
    sf::Font font;
    sf::Text questText;

    sf::Text thankYouText;

    sf::Text questWindowText;
    sf::RectangleShape questWindowBackground;
    bool showQuestWindow = false;
    bool hasApple = false;
    bool questAlreadyAccepted = false;
    bool questCompleted = false;
    bool canTalkAfterQuest = false;
    bool hasBeenThanked = false;

    std::function<void(bool)> questCallback;

    virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override
    {
        states.transform *= getTransform();
        states.texture = &npcTexture;
        target.draw(npcSprite, states);

        if (showQuestWindow)
        {
            target.draw(questWindowBackground, states);
            target.draw(questWindowText, states);
        }

        if (questCompleted)
        {
            target.draw(thankYouText, states);
        }
     
        if (canTalkAfterQuest && !questCompleted)
        {
            target.draw(questText, states);
        }
    }

    void NextTexture(sf::Time deltaTime)
    {
        if (isPaused)
            return;

        if (animationClock.getElapsedTime().asSeconds() >= animationSpeed)
        {
            numTexture++;
            if (numTexture >= countTextures)
            {
                numTexture = 0;
            }
            npcSprite.setTextureRect(sf::IntRect(numTexture * 128, 0, 128, 128));
            animationClock.restart();
        }
    }

public:
    bool load(const sf::Texture &texture, const sf::Font &fnt)
    {
        npcTexture = texture;
        npcSprite.setTexture(npcTexture);
        npcSprite.setTextureRect(sf::IntRect(0, 0, 128, 128));
        npcSprite.setOrigin(npcSprite.getLocalBounds().width / 2, npcSprite.getLocalBounds().height / 2);
        npcSprite.setPosition(600, 500);

        font = fnt;

        questWindowText.setFont(font);
        questWindowText.setString("OH! Are u new here?\nI need your help stranger.\nI feel ill,\ncan u bring me an apple?\nDo you want to take the quest?\nYes: [Y] No: [N]");
        questWindowText.setCharacterSize(12);
        questWindowText.setFillColor(sf::Color::White);

        questWindowBackground.setSize(sf::Vector2f(400, 150));
        questWindowBackground.setFillColor(sf::Color(0, 0, 0, 150));
        questWindowBackground.setPosition(400, 300);

        questWindowText.setPosition(420, 320);

        return true;
    }
    const sf::Text &getThankYouText() const
    {
        return thankYouText;
    }

    void setCanTalkAfterQuest(bool value) { canTalkAfterQuest = value; }

    void completeQuest()
    {
        questCompleted = true;
        setCanTalkAfterQuest(true);
        setupThankYouText(); 
    }

    void setupThankYouText()
    {
       if (!hasBeenThanked)
        {
            thankYouText.setFont(font);
            thankYouText.setString("Thank you hero for helping me!");
            thankYouText.setCharacterSize(12);
            thankYouText.setFillColor(sf::Color::Green);
            thankYouText.setPosition(500, 450);
            hasBeenThanked = true; 
        }
    }

    bool isQuestCompleted() const
    {
        return questCompleted;
    }

    bool isQuestAlreadyAccepted() const
    {
        return questAlreadyAccepted;
    }

    void idleAnimation(sf::Time deltaTime)
    {
        NextTexture(deltaTime);
    }

    sf::Vector2f getPosition() const
    {
        return npcSprite.getPosition();
    }

    void openQuestWindow(std::function<void(bool)> callback)
    {
        if (questAlreadyAccepted)
            return;

        showQuestWindow = true;
        questCallback = callback;
    }

    void closeQuestWindow()
    {
        showQuestWindow = false;
    }

    bool isQuestWindowOpen() const
    {
        return showQuestWindow;
    }

    void handleInput(const sf::Event &event)
    {
        if (showQuestWindow)
        {
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Y)
                {
                    questCallback(true);
                    questAlreadyAccepted = true;

                    closeQuestWindow();
                }
                else if (event.key.code == sf::Keyboard::N)
                {
                    questCallback(false);
                    closeQuestWindow();
                }
            }
        }
    }

    void setPaused(bool paused)
    {
        isPaused = paused;
    }

    void serialize(std::ofstream &outFile) const
    {
        sf::Vector2f position = npcSprite.getPosition();
        outFile.write(reinterpret_cast<const char *>(&position), sizeof(position));
        outFile.write(reinterpret_cast<const char *>(&numTexture), sizeof(numTexture));
        outFile.write(reinterpret_cast<const char *>(&questAlreadyAccepted), sizeof(questAlreadyAccepted));
        outFile.write(reinterpret_cast<const char *>(&questCompleted), sizeof(questCompleted));
        outFile.write(reinterpret_cast<const char *>(&canTalkAfterQuest), sizeof(canTalkAfterQuest));
        outFile.write(reinterpret_cast<const char *>(&hasBeenThanked), sizeof(hasBeenThanked));
    }

   void deserialize(std::ifstream &inFile)
    {
        sf::Vector2f position;
        inFile.read(reinterpret_cast<char *>(&position), sizeof(position));
        npcSprite.setPosition(position);

        inFile.read(reinterpret_cast<char *>(&numTexture), sizeof(numTexture));
        npcSprite.setTextureRect(sf::IntRect(numTexture * 128, 0, 128, 128));

        inFile.read(reinterpret_cast<char *>(&questAlreadyAccepted), sizeof(questAlreadyAccepted));
        inFile.read(reinterpret_cast<char *>(&questCompleted), sizeof(questCompleted));
        inFile.read(reinterpret_cast<char *>(&canTalkAfterQuest), sizeof(canTalkAfterQuest));
        inFile.read(reinterpret_cast<char *>(&hasBeenThanked), sizeof(hasBeenThanked));
    }


    void update(sf::Time deltaTime)
    {
        idleAnimation(deltaTime);
    }

    sf::FloatRect getBounds() const
    {
        return npcSprite.getGlobalBounds();
    }

    void checkQuestCompletion(Hero &hero)
    {
        if (hero.getHasApple() && !questCompleted)
        {
            hasApple = true;
            questCompleted = true;
            hero.resetApple();
            questCallback(true);
            setupThankYouText();
            std::cout << "Quest completed! You can return to the NPC." << std::endl;
        }
    }

    
};
