# Hero of Japan

## Progress of the task:
 - The programming environment was used to create the project Visual Studio 2022 
 - In the programming environment created new project CMake
 - Local GitHub version control repository created
 - The SFML framework has been selected and installed for rendering the game.
 - To connect the SFML library, changes have been made to the CMakeList.txt settings file.

 ## Creating the main window:
 A window of a fixed size 800*600 is created and then filled with a background that I made in figma

 ## Creating a player

 It was decided to use a sprite as the player and set a texture for it in the form of a swordsman
 A separate Hero class has been created for the hero, which inherits from the Drawable and Transformable classes, which will then make it possible to display it on the screen and perform transformations.
 The texture for the hero is a png file with 9 images that simulate the hero's walk
 To simulate walking, replace the texture with the following one at each step of the hero

 ## Creating a npc

 It was created a npc with idle animation , u can speak with her and take a quest from her.
 A separate Npc class has been created for the npc, which inherits from the Drawable and Transformable classes, which will then make it possible to display it on the screen and perform transformations.
 The texture for the npc is a png file with 8 images that simulate the npc's idle

 ## Creating panadas
 It was created a panda with wave animation.
 A separate panda class has been created for the pand, which inherits from the Drawable and Transformable classes, which will then make it possible to display it on the screen and perform transformations.
 The texture for the panda is a png file with 12 images that simulate the panda's waving



 ## Event handling
 The cursor keys (arrows) are used to move the hero.
 For 4 arrows we check events, and for each of them we call methods for moving the hero.
 According to the logic of the game, the hero turns left and right, but does not rotate.
 The hero's movement is performed only within the game window, this is tracked in the hero's movement method
 Also now u can save and load the game state(music, hero, npc, panda ,time). U need to use S L.
 When u pause the game, npc and pandas also pause.
 Also u can interact with npc, for that u need to use E on keyboard. And then u need to choose accept or reject the quest from npc.

## Saving Game Progress

Press the **`S` key** to save your game progress. The game will store the following data in a binary file called `game_data.dat`:

- **Character Positions**: Saves the exact coordinates of the Panda and NPC on the screen.
- **Animation State**: Stores the current animation frame and texture for each character.
- **Game State**: Saves whether the game and animations are paused or active.
- **Background Music Progress**: Captures the current playback position of the background music.

The data is serialized into binary format for efficient storage, ensuring fast save and load operations.

---

## Loading Game Progress

Press the **`L` key** to load your previously saved game progress. The game will retrieve the following data from the `game_data.dat` file:

- **Character Positions**: Restores all characters to their saved locations.
- **Animation State**: Resumes animations from the exact frame at the time of saving.
- **Game State**: Restores the pause state and other gameplay settings.
- **Background Music Progress**: Continues the background music from where it was left off.

If the save file is missing or corrupted, the game will display an error message and proceed without loading.

---

This system ensures that players can pause and resume their adventures seamlessly, preserving all critical gameplay details.



 ## Game time
 Tracking of game time is done by the Clock object.
 To implement the pause of the game, another timer has been created, with the help of which the time spent on pause is measured.

In the game, a pause is activated in two cases:
pressing a key;
the window becomes inactive.

When the game window is inactive, in addition to pausing, key press events are no longer tracked



## Controls

| Key   | Action                            |
|-------|-----------------------------------|
| `S`   | Save the game                     |
| `L`   | Load the game                     |
| `P`   | Pause/Unpause the game            |
| `E`   | Interact with npc                 |

---


# Additional Resource Types and Their Usage

## 1. Textures
**Purpose**: Textures are used to render the visual appearance of characters, objects, and backgrounds.  
**Usage**:  
- **Character Animations**: The game uses sprite sheets to manage multiple frames for character animations, such as the Panda's idle animation.
- **Environment**: Textures are applied to backgrounds and objects to create an immersive game world.  

**Example**: A texture file is loaded into memory using `sf::Texture` and mapped to a sprite using `sf::Sprite`.

---

## 2. Fonts
**Purpose**: Fonts are used for displaying in-game text such as messages, and pause notifications.  
**Usage**:  
- Displaying the **Pause** text when the game is paused.  

---

**Example**: Fonts are managed using `sf::Font` and rendered using `sf::Text`.

## 3. Audio
**Purpose**: Adds depth to the gameplay experience through music and sound effects.  
**Usage**:  
- **Background Music**: Creates an ambient atmosphere during gameplay.  
- **Audio State Persistence**: The game's save and load system includes the current playback position of the background music.

**Example**: Audio is managed using an audio manager, which handles play, pause, and save/load operations.

---

## 4. Save Data
**Purpose**: Stores the game state to allow players to save and resume their progress.  
**Usage**:  
- Serialized data includes character positions, animation states, pause state, and music progress.  
- Data is saved in a binary file (`game_data.dat`) for efficient storage.

**Example**: Save data is written and read using `std::ofstream` and `std::ifstream`.

---

