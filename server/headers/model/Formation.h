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
        // Para que el juego en marcha.
        Coordinates* getCoordinatesForPlayer(int playerNumber, bool leftsideOfThePitch);
        Coordinates* getCoordinatesForPlayer321(int playerNumber);
        Coordinates* getCoordinatesForPlayer33(int playerNumber);
        Coordinates* getCoordinatesForPlayer312(int playerNumber);
        Coordinates* getCoordinatesForPlayerOnTheRightside321(int playerNumber);
        Coordinates* getCoordinatesForPlayerOnTheRightside33(int playerNumber);
        Coordinates* getCoordinatesForPlayerOnTheRightside312(int playerNumber);
        // Para que esten atras de la mitad.
        Coordinates* getCoordinatesForPlayerBehindMiddle(int playerNumber, bool leftsideOfThePitch);
        Coordinates* getCoordinatesForPlayerOnTheLeftBehindMiddle(int playerNumber);
        Coordinates* getCoordinatesForPlayerOnTheRightsideBehindMiddle(int playerNumber);
        Coordinates* getDefaultCoordinates();
        ~Formation();
};
#endif // FORMATION_H
