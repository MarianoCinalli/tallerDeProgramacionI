#include "model/Formation.h"

Formation::Formation(int formation) {
    this->formation = formation;
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
            coordinates = new Coordinates(326, 501);
            break;
        case 3:
            coordinates = new Coordinates(410, 69);
            break;
        case 4:
            coordinates = new Coordinates(410, 880);
            break;
        case 5:
            coordinates = new Coordinates(798, 344);
            break;
        case 6:
            coordinates = new Coordinates(798, 663);
            break;
        case 7:
            coordinates = new Coordinates(1232, 501);
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
            coordinates = new Coordinates(326, 501);
            break;
        case 3:
            coordinates = new Coordinates(410, 100);
            break;
        case 4:
            coordinates = new Coordinates(410, 880);
            break;
        case 5:
            coordinates = new Coordinates(1076, 501);
            break;
        case 6:
            coordinates = new Coordinates(1258, 898);
            break;
        case 7:
            coordinates = new Coordinates(1260, 100);
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
            coordinates = new Coordinates(326, 515);
            break;
        case 3:
            coordinates = new Coordinates(410, 69);
            break;
        case 4:
            coordinates = new Coordinates(410, 880);
            break;
        case 5:
            coordinates = new Coordinates(777, 490);
            break;
        case 6:
            coordinates = new Coordinates(1232, 323);
            break;
        case 7:
            coordinates = new Coordinates(1232, 674);
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
            coordinates = new Coordinates(1288, 520);
            break;
        case 3:
            coordinates = new Coordinates(1245, 69);
            break;
        case 4:
            coordinates = new Coordinates(1245, 910);
            break;
        case 5:
            coordinates = new Coordinates(800, 336);
            break;
        case 6:
            coordinates = new Coordinates(800, 640);
            break;
        case 7:
            coordinates = new Coordinates(360, 490);
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
            coordinates = new Coordinates(1076, 533);
            break;
        case 3:
            coordinates = new Coordinates(1130, 890);
            break;
        case 4:
            coordinates = new Coordinates(1130, 105);
            break;
        case 5:
            coordinates = new Coordinates(400, 511);
            break;
        case 6:
            coordinates = new Coordinates(410, 100);
            break;
        case 7:
            coordinates = new Coordinates(410, 900);
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
            coordinates = new Coordinates(1288, 480);
            break;
        case 3:
            coordinates = new Coordinates(1245, 69);
            break;
        case 4:
            coordinates = new Coordinates(1245, 880);
            break;
        case 5:
            coordinates = new Coordinates(798, 344);
            break;
        case 6:
            coordinates = new Coordinates(798, 660);
            break;
        case 7:
            coordinates = new Coordinates(350, 514);
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
            coordinates = new Coordinates(300, 501);
            break;
        case 3:
            coordinates = new Coordinates(350, 156);
            break;
        case 4:
            coordinates = new Coordinates(350, 800);
            break;
        case 5:
            coordinates = new Coordinates(600, 200);
            break;
        case 6:
            coordinates = new Coordinates(600, 650);
            break;
        case 7:
            coordinates = new Coordinates(750, 470);
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
            coordinates = new Coordinates(1200, 501);
            break;
        case 3:
            coordinates = new Coordinates(1150, 156);
            break;
        case 4:
            coordinates = new Coordinates(1150, 800);
            break;
        case 5:
            coordinates = new Coordinates(900, 200);
            break;
        case 6:
            coordinates = new Coordinates(900, 650);
            break;
        case 7:
            coordinates = new Coordinates(830, 470);
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
