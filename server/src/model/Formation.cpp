#include "model/Formation.h"

Formation::Formation(int formation, bool isLocal) {
    this->formation = formation;
    this->isLocal = isLocal;
}

void Formation::setFormation(int formation) {
    this->formation = formation;
}

Coordinates* Formation::getCoordinatesForPlayer(int playerNumber) {
    Coordinates* coordinates;
    if (this->formation == 321) {
        coordinates = getCoordinatesForPlayer321(playerNumber);
    } else if (this->formation == 33) {
        coordinates = getCoordinatesForPlayer33(playerNumber);
    } else if (this->formation == 312) {
        coordinates = getCoordinatesForPlayer312(playerNumber);
    } else {
        log("Formation: Fomacion no reconocida: ", this->formation, LOG_ERROR);
    }
    if (coordinates == NULL) {
        log("Formation: Las coordenadas son nulas. Usando defaults.", LOG_ERROR);
        coordinates = getDefaultCoordinates();
    }
    return coordinates;
}

Coordinates* Formation::getCoordinatesForPlayer321(int playerNumber) {
    Coordinates* coordinates = NULL;
    switch(playerNumber) {
        case 1:
            coordinates = new Coordinates(105, 501);
            break;
        case 2:
            coordinates = new Coordinates(799, 129);
            break;
        case 3:
            coordinates = new Coordinates(799, 873);
            break;
        case 4:
            coordinates = new Coordinates(296, 501);
            break;
        case 5:
            coordinates = new Coordinates(809, 368);
            break;
        case 6:
            coordinates = new Coordinates(809, 632);
            break;
        case 7:
            coordinates = new Coordinates(1322, 501);
            break;
        default:
            log("Formation: Numero de jugador no entendido: ", playerNumber, LOG_ERROR);
    }
    return coordinates;
}

Coordinates* Formation::getCoordinatesForPlayer33(int playerNumber) {
    Coordinates* coordinates = NULL;
    switch(playerNumber) {
        case 1:
            coordinates = new Coordinates(105, 501);
            break;
        case 2:
            coordinates = new Coordinates(442, 181);
            break;
        case 3:
            coordinates = new Coordinates(455, 502);
            break;
        case 4:
            coordinates = new Coordinates(442, 820);
            break;
        case 5:
            coordinates = new Coordinates(1157, 181);
            break;
        case 6:
            coordinates = new Coordinates(1144, 502);
            break;
        case 7:
            coordinates = new Coordinates(1157, 821);
            break;
        default:
            log("Formation: Numero de jugador no entendido: ", playerNumber, LOG_ERROR);
    }
    return coordinates;
}

Coordinates* Formation::getCoordinatesForPlayer312(int playerNumber) {
    Coordinates* coordinates = NULL;
    switch(playerNumber) {
        case 1:
            coordinates = new Coordinates(105, 501);
            break;
        case 2:
            coordinates = new Coordinates(802, 873);
            break;
        case 3:
            coordinates = new Coordinates(345, 501);
            break;
        case 4:
            coordinates = new Coordinates(802, 129);
            break;
        case 5:
            coordinates = new Coordinates(802, 501);
            break;
        case 6:
            coordinates = new Coordinates(1258, 632);
            break;
        case 7:
            coordinates = new Coordinates(1258, 368);
            break;
        default:
            log("Formation: Numero de jugador no entendido: ", playerNumber, LOG_ERROR);
    }
    return coordinates;
}

Coordinates* Formation::getDefaultCoordinates() {
    return new Coordinates(800, 600);
}

Formation::~Formation() {
}
