#include "Coordinates.h"
#include <unordered_map>
#include <vector>

using namespace std;

Coordinates defensa1 (0,0);
Coordinates defensa2 (0,40);
Coordinates defensa3 (0,100);
Coordinates defensa4 (0,140);
Coordinates medio1 (40,0);
Coordinates medio2 (80,0);
Coordinates medio3 (180,0);

std::vector<Coordinates> formacion33 = {defensa1, defensa2, defensa3, medio1, medio2, medio3};
std::vector<Coordinates> formacion1 = {defensa1};


std::unordered_map<int, vector<Coordinates>> formaciones = {
        {33,formacion33},
//        {321,formacion321},
//        {312,formacion312},
        {1,formacion1}
    };
