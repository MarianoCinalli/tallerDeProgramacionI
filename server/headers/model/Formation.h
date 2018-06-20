#include "util/Constants.h"
#include "util/Coordinates.h"
#include "util/logger.h"

#ifndef FORMATION_H
#define FORMATION_H

class Formation {
    private:
        int formation;
        bool isLocal;
    public:
        Formation(int formation, bool isLocal);
        void setFormation(int formation);
        Coordinates* getCoordinatesForPlayer(int playerNumber);
        Coordinates* getCoordinatesForPlayer321(int playerNumber);
        Coordinates* getCoordinatesForPlayer33(int playerNumber);
        Coordinates* getCoordinatesForPlayer312(int playerNumber);
        Coordinates* getDefaultCoordinates();
        ~Formation();
};
#endif // FORMATION_H