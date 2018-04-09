#include "Coordinates.h"
#include <unordered_map>
#include <vector>
#include <list>

using namespace std;

Coordinates arquero (0,500);
Coordinates defensa1 (400,500);
Coordinates defensa2 (400,250);
Coordinates defensa3 (400,750);
Coordinates defensa4 (0,500);
Coordinates medio1 (800,400);
Coordinates medio2 (800,500);
Coordinates medio3 (800,600);

std::vector<Coordinates> formacion33 = {arquero, defensa1, defensa2, defensa3, medio1, medio2, medio3};
std::vector<Coordinates> formacion1 = {defensa1};


std::unordered_map<int, vector<Coordinates>> formaciones = {
        {33,formacion33},
//        {321,formacion321},
//        {312,formacion312},
        {1,formacion1}
    };
