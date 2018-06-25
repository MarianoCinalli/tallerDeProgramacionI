#include "model/Formation.h"

Formation::Formation(int formation, bool isLocal) {
    this->formation = formation;
    this->isLocal = isLocal;
}

void Formation::setFormation(int formation) {
    this->formation = formation;
}

Coordinates* Formation::getCoordinatesForPlayer(int playerNumber, bool leftsideOfThePitch) {
    Coordinates* coordinates;
    int number = 0;
    if (playerNumber > 7) {
        number = playerNumber - 7;
    } else {
        number = playerNumber;
    }
    if (leftsideOfThePitch) {
        if (this->formation == 321) {
            coordinates = getCoordinatesForPlayer321(number);
        } else if (this->formation == 33) {
            coordinates = getCoordinatesForPlayer33(number);
        } else if (this->formation == 312) {
            coordinates = getCoordinatesForPlayer312(number);
        } else {
            log("Formation: Fomacion no reconocida: ", this->formation, LOG_ERROR);
        }
    } else {
        if (this->formation == 321) {
            coordinates = getCoordinatesForPlayerOnTheRightside321(number);
        } else if (this->formation == 33) {
            coordinates = getCoordinatesForPlayerOnTheRightside33(number);
        } else if (this->formation == 312) {
            coordinates = getCoordinatesForPlayerOnTheRightside312(number);
        } else {
            log("Formation: Fomacion no reconocida: ", this->formation, LOG_ERROR);
        }
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

Coordinates* Formation::getCoordinatesForPlayerOnTheRightside321(int playerNumber) {
    Coordinates* coordinates = NULL;
    switch(playerNumber) {
        case 1:
            coordinates = new Coordinates(1450, 500);
            break;
        case 2:
            coordinates = new Coordinates(802, 128);
            break;
        case 3:
            coordinates = new Coordinates(802, 875);
            break;
        case 4:
            coordinates = new Coordinates(1304, 501);
            break;
        case 5:
            coordinates = new Coordinates(811, 368);
            break;
        case 6:
            coordinates = new Coordinates(811, 632);
            break;
        case 7:
            coordinates = new Coordinates(298, 501);
            break;
        default:
            log("Formation: Numero de jugador no entendido: ", playerNumber, LOG_ERROR);
    }
    return coordinates;
}

Coordinates* Formation::getCoordinatesForPlayerOnTheRightside33(int playerNumber) {
    Coordinates* coordinates = NULL;
    switch(playerNumber) {
        case 1:
            coordinates = new Coordinates(1450, 500);
            break;
        case 2:
            coordinates = new Coordinates(1157, 181);
            break;
        case 3:
            coordinates = new Coordinates(1144, 502);
            break;
        case 4:
            coordinates = new Coordinates(1157, 821);
            break;
        case 5:
            coordinates = new Coordinates(442, 181);
            break;
        case 6:
            coordinates = new Coordinates(455, 502);
            break;
        case 7:
            coordinates = new Coordinates(442, 820);
            break;
        default:
            log("Formation: Numero de jugador no entendido: ", playerNumber, LOG_ERROR);
    }
    return coordinates;
}

Coordinates* Formation::getCoordinatesForPlayerOnTheRightside312(int playerNumber) {
    Coordinates* coordinates = NULL;
    switch(playerNumber) {
        case 1:
            coordinates = new Coordinates(1450, 500);
            break;
        case 2:
            coordinates = new Coordinates(800, 874);
            break;
        case 3:
            coordinates = new Coordinates(1254, 501);
            break;
        case 4:
            coordinates = new Coordinates(800, 129);
            break;
        case 5:
            coordinates = new Coordinates(797, 499);
            break;
        case 6:
            coordinates = new Coordinates(340, 631);
            break;
        case 7:
            coordinates = new Coordinates(340, 368);
            break;
        default:
            log("Formation: Numero de jugador no entendido: ", playerNumber, LOG_ERROR);
    }
    return coordinates;
}

// Coordenadas atras del medio.
// Para el equipo de la izquierda el jugador numero 7 esta en 800 x 600. Para que saque del medio.

Coordinates* Formation::getCoordinatesForPlayerBehindMiddle(int playerNumber, bool leftsideOfThePitch) {
    Coordinates* coordinates;
    int number = 0;
    if (playerNumber > 7) {
        number = playerNumber - 7;
    } else {
        number = playerNumber;
    }
    if (leftsideOfThePitch) {
        coordinates = getCoordinatesForPlayerOnTheLeftBehindMiddle(number);
    } else {
        coordinates = getCoordinatesForPlayerOnTheRightsideBehindMiddle(number);
    }

    if (coordinates == NULL) {
        log("Formation: Las coordenadas son nulas. Usando defaults.", LOG_ERROR);
        coordinates = getDefaultCoordinates();
    }
    return coordinates;
}

Coordinates* Formation::getCoordinatesForPlayerOnTheLeftBehindMiddle(int playerNumber) {
    Coordinates* coordinates = NULL;
    switch(playerNumber) {
        case 1:
            coordinates = new Coordinates(105, 501);
            break;
        case 2:
            coordinates = new Coordinates(350, 156);
            break;
        case 3:
            coordinates = new Coordinates(350, 800);
            break;
        case 4:
            coordinates = new Coordinates(300, 501);
            break;
        case 5:
            coordinates = new Coordinates(600, 200);
            break;
        case 6:
            coordinates = new Coordinates(600, 650);
            break;
        case 7:
            coordinates = new Coordinates(800, 600);
            break;
        default:
            log("Formation: Numero de jugador no entendido: ", playerNumber, LOG_ERROR);
    }
    return coordinates;
}

Coordinates* Formation::getCoordinatesForPlayerOnTheRightsideBehindMiddle(int playerNumber) {
    Coordinates* coordinates = NULL;
    switch(playerNumber) {
        case 1:
            coordinates = new Coordinates(1450, 500);
            break;
        case 2:
            coordinates = new Coordinates(1150, 156);
            break;
        case 3:
            coordinates = new Coordinates(1150, 800);
            break;
        case 4:
            coordinates = new Coordinates(1200, 501);
            break;
        case 5:
            coordinates = new Coordinates(900, 200);
            break;
        case 6:
            coordinates = new Coordinates(900, 650);
            break;
        case 7:
            coordinates = new Coordinates(700, 600);
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
