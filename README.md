# Hero of Japan

## Progress of the task:
 - The programming environment was used to create the project Visual Studio 2022 
 - In the programming environment created new project CMake
 - Local GitHub version control repository created
 - The SFML framework has been selected and installed for rendering the game.
 - To connect the SFML library, changes have been made to the CMakeList.txt settings file.

 ## Creating the main window:
 A window of a fixed size 800�600 is created and then filled with a specific color sf::Color(138,236,209)

 ## Creating a player

 It was decided to use a sprite as the player and set a texture for it in the form of a swordsman
 A separate Hero class has been created for the hero, which inherits from the Drawable and Transformable classes, which will then make it possible to display it on the screen and perform transformations.
 The texture for the hero is a png file with 9 images that simulate the hero's walk
 To simulate walking, replace the texture with the following one at each step of the hero

 ## Event handling
 The cursor keys (arrows) are used to move the hero.
 For 4 arrows we check events, and for each of them we call methods for moving the hero.
 According to the logic of the game, the hero turns left and right, but does not rotate.
 The hero's movement is performed only within the game window, this is tracked in the hero's movement method




 ## Game time
 Tracking of game time is done by the Clock object.
 To implement the pause of the game, another timer has been created, with the help of which the time spent on pause is measured.

In the game, a pause is activated in two cases:
pressing a key;
the window becomes inactive.

When the game window is inactive, in addition to pausing, key press events are no longer tracked




