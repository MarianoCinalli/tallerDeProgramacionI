#include "controller/PlayerMovement.h"

PlayerMovement::PlayerMovement() {
    log("PlayerMovement: Creando movimientos para los jugadores. Si no se especifica una formacion, no voy a tener areas...", LOG_DEBUG);

    this->playerAreas = NULL;
    this->isLeftsideTeam = true;
    this->formation = 0;
}

void PlayerMovement::setFormation(int formation) {
    if (this->playerAreas != NULL) {
        log("PlayerMovement: Habia areas especificadas previamente. Borrandolas...", LOG_INFO);
        delete(this->playerAreas);
    }
    this->formation = formation;
    log("PlayerMovement: Creando areas para formacion: ", formation, LOG_DEBUG);
    this->playerAreas = new PlayerAreas(formation);
}

void PlayerMovement::setSide(bool isLeftsideTeam) {
    this->isLeftsideTeam = isLeftsideTeam;
}

bool PlayerMovement::isInsideArea(Coordinates* coordinates, int playerNumber) {
    bool canMove = true;
    if (this->playerAreas == NULL) {
        log("PlayerMovement: Las areas son nulas.", LOG_ERROR);
        return false;
    }
    Rectangle* rectangle = this->playerAreas->getForPlayer(playerNumber, this->isLeftsideTeam);
    if (rectangle == NULL) {
        log("PlayerMovement: El area es nula, para el jugador: ", playerNumber, LOG_ERROR);
        canMove = false;
    } else {
        canMove = rectangle->isInside(coordinates->getX(), coordinates->getY());
    }
    return canMove;
}

// Pone en 0 la componente, a la que le corresponde una coordenada de posicion cercana a los bordes.
void PlayerMovement::cleanVelocity(Velocity* velocity, Coordinates* coordinates, int playerNumber) {
    if (this->playerAreas == NULL) {
        log("PlayerMovement: Las areas son nulas.", LOG_ERROR);
    }
    Rectangle* rectangle = this->playerAreas->getForPlayer(playerNumber, this->isLeftsideTeam);
    if (rectangle == NULL) {
        log("PlayerMovement: El area es nula, para el jugador: ", playerNumber, LOG_ERROR);
    } else {
        // Me fijo si la posicion actual es cercana a los bordes del area de movimiento.
        // Si lo estoy solo me muevo en la direccion que no estoy cerca de los bordes.
        if (!rectangle->isVelocityPointingInside(velocity, coordinates)) {
            if (rectangle->isCloseToBordersInX(coordinates->getX())) {
                velocity->setComponentX(0);
            } else if (rectangle->isCloseToBordersInY(coordinates->getY())) {
                velocity->setComponentY(0);
            }
        }
    }
}

std::string PlayerMovement::getDebugLines() {
    std::string message = "";
    if (this->playerAreas != NULL) {
        message += this->playerAreas->getDebugLines();
    }
    return message;
}

Coordinates* PlayerMovement::getCoordinatesToFollow(Coordinates* ballPosition, Coordinates* basePosition, int playerNumber, bool isAttacking) {
    Coordinates* coordinates = new Coordinates(ballPosition->getX(), ballPosition->getY());
    int number = 0;
    if (playerNumber > 7) {
        number = playerNumber - 7;
    } else {
        number = playerNumber;
    }
    if (this->isDefender(number)) {
        if (isAttacking) {
            coordinates->setX(basePosition->getX());
            coordinates->setY(basePosition->getY());
        }
    } else if (this->isAttacker(number)) {
        if (!isAttacking) {
            coordinates->setX(basePosition->getX());
            coordinates->setY(basePosition->getY());
        }
    }
    return coordinates;
}

bool PlayerMovement::isDefender(int playerNumber) {
    bool isDefencePlayer = false;
    if (this->formation == 33) {
        // Para la formacion 33 considero a los tres como defensores.
        isDefencePlayer = (playerNumber <= 4);
    } else {
        // Para las demas solo el dos es defensor.
        isDefencePlayer = (playerNumber == 2);
    }
    return isDefencePlayer;
}

bool PlayerMovement::isAttacker(int playerNumber) {
    bool isAttackPlayer = false;
    if (this->formation == 33) {
        // Para la formacion 33 considero solo el 5 delantero.
        isAttackPlayer = (playerNumber == 5);
    } else if (this->formation == 312) {
        isAttackPlayer = (playerNumber == 6 || playerNumber == 7);
    } else if (this->formation == 321) {
        isAttackPlayer = (playerNumber == 7);
    }
    return isAttackPlayer;
}

PlayerMovement::~PlayerMovement() {
    log("PlayerMovement: Borrando PlayerMovement...", LOG_DEBUG);
}
