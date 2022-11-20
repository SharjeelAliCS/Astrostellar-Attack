# COMP 3501 Project: Astrostellar Attack
Authors: SHARJEEL ALI \#101070889, DAVID NEUDORF \#101029913

## Video Demo

The assignment contains a video demo as well, so please watch that. It's a bit long but it covers the  gist of the game's features. https://youtu.be/gBNgNmY3Q50

## HOW TO BUILD

To build, simply run cmake using the cmakelists.txt file, and maks sure to set the correct libraries in the cmake folder.

To also add, you will require changing 3 paths defined in the path_config header file located in include/setup/path_config.h

Once the game is running, it takes 40 seconds to load everything, so just wait.

## LIBRARIES

The codebase uses two libraries, being the freetype library for text rendering as well as nlohmann json for json reading and writing. These are both included in the libraries/ folder, so there should hopefully be no need for extra downloads.

## CODE STRUCTURE

As seen, the code is organized into various sections. This is mainly described in the writeup report, but for the most important parts, such as shaders, keep this in mind:
resource/shader: stores the object shaders
resource/screen: stores screen space effects
resource/particle: stores particle effects

## DOCUMENTATION

Everything aside from the files found in the /setup have been commented (the /setup includes the resource classes, which werent really modified at all). Each function is commented in the header file, while only important or complex things are commented in the source file to make things easier.

Whenever an external source was used, it is also commented (Source: Link).

## MAKE YOUR LIFE EASY

In order to make testing around with features easier, you can modify the save file located in saves/game_state.json. DO NOT MODIFY THE _DEFAULT.JSON FILE AT ALL COSTS. This file stores all of the save data such as player inventory, etc. so if you want to buy everything to test it, change the values here.

Another thing to test, is the bounties. Go into src/world/game.cpp and around 650 there will be the following code:
"else if (btn.compare("startButton")==0) {"
In here, you should see a "num" attribute that is being assigned using if statements to compare bounty string values. Change the number to something small such as 5 for the asteroid or enemies if statements. This means instead of destroying 60 or 40 enemies/asteroids, you only need to destroy 5.
