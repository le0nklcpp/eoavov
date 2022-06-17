# DESCRIPTION
eoavov is a **game engine based on Tesseract *game engine* which is based on Cube 2** game engine.
I am working on it hoping to build something good.I know I won't finish it.
## Features:
- **Other** dynamic entitiy system.Easy to modify and add your own entity type
- **Other** health system(Not ready yet)
- Inventory system
- Scripting *enchancement*
   - In-game events handling
   - Game objects control
   - Disappearing text messages and textures on HUD
## Being completed:
- Literally any thing listed above

## Coming soon:
- Acoustic system
- Combat AI
- Contents
- Annoying cutscenes rendering
- Gameplay
- Remove the network functionality
- Visual Studio build files(MinGW is fine yet)

So I am making a game engine to create a cool game using scripts.Just for fun.It is single player only.
## Current tasks:
- cmath command
- save/load entities
- weapons
- inventory system enchancement: add containers(or adapt inventory entity)
- weapons scripting enchancement
- physics - physicstrigger must check the water level
- projectile entity
- scripted particles
- breakable material
- camera entity
- acoustic system
- light beams
- hitscan
- hitboxes

# LICENSING INFO
This project is licensed under zlib license.See LICENSE.


# BUILDING
You can build this project using g++ on GNU/Linux and with MinGW+MSYS on Windows,
but you will have to install libsdl2,libsdl2-mixer,libsdl2-image and zlib development files first.
If you're using MinGW, then use version 7.0 or higher, because older versions of MinGW are a bit buggy.

# CODE STYLE
Read the readme_sauerbraten.txt section LICENSE NOTES first.
I decided to use my own code style for files that were completely or mostly written by myself.
This is why a few files in src/game directory contain different indentation.
