#ifndef LOADRESOURCES_H
#define LOADRESOURCES_H

#include "TextureManager.h"
#include "AudioManager.h"
#include <string>
#include <iostream>

bool loadResources(TextureManager& textureManager, AudioManager& audioManager) {
    try {
        textureManager.loadTexture("background", "./images/background.png");
        textureManager.loadTexture("wall", "./images/bambu.png");
        textureManager.loadTexture("hero", "./images/Walk.png");
        textureManager.loadTexture("tree", "./images/tree.png");  
        textureManager.loadTexture("npc", "./images/Idle.png");
        textureManager.loadTexture("apple", "./images/apple_regular_30_30px.png"); 
        textureManager.loadTexture("panda", "./images/PandaWave.png"); 

     
        audioManager.loadMusic("background_music", "./audio/21. Tsushima Suite - IV. Kodoku.mp3");
    } catch (const std::exception& e) {
        std::cerr << e.what() << "\n";
        return false;
    }
    return true;
}


#endif
