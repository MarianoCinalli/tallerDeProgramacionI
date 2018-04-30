#include "Coordinates.h"
#include <unordered_map>
#include <vector>
#include <list>

using namespace std;

Coordinates arquero (120, 500 - SPRITE_SIZE/2);
Coordinates defensa1 (400 - SPRITE_SIZE/2, 500 - SPRITE_SIZE/2);
Coordinates defensa2 (400 - SPRITE_SIZE/2, 250 - SPRITE_SIZE/2);
Coordinates defensa3 (400 - SPRITE_SIZE/2, 750 - SPRITE_SIZE/2);
Coordinates medio1 (800 - SPRITE_SIZE/2, 400 - SPRITE_SIZE/2);
Coordinates medio2 (800 - SPRITE_SIZE/2, 500 - SPRITE_SIZE/2);
Coordinates medio3 (800 - SPRITE_SIZE/2, 600 - SPRITE_SIZE/2);
Coordinates delantero1 (1200 - SPRITE_SIZE/2, 350 - SPRITE_SIZE/2);
Coordinates delantero2 (1200 - SPRITE_SIZE/2, 500 - SPRITE_SIZE/2);
Coordinates delantero3 (1200 - SPRITE_SIZE/2, 750 - SPRITE_SIZE/2);

std::vector<Coordinates> formacion33 = {arquero, defensa1, defensa2, defensa3, medio1, medio2, medio3};
std::vector<Coordinates> formacion321 = {arquero, defensa1, defensa2, defensa3, medio1, medio3, delantero2};
std::vector<Coordinates> formacion312 = {arquero, defensa1, defensa2, defensa3, medio2, delantero1, delantero3};

std::vector<Coordinates> formacion1 = {defensa1};

std::unordered_map<int, vector<Coordinates>> formaciones = {
        {33,formacion33},
        {321,formacion321},
        {312,formacion312},
        {1,formacion1}
    };
