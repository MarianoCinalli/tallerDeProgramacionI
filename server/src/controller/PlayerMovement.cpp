#include "controller/PlayerMovement.h"

PlayerMovement::PlayerMovement() {
    log("PlayerMovement: Creando movimientos para los jugadores. Si no se especifica una formacion, no voy a tener areas...", LOG_DEBUG);
    this->isLeftsideTeam = true;
}

void PlayerMovement::setFormation(int formation) {
    if (this->playerAreas != NULL) {
        log("PlayerMovement: Habia areas especificadas previamente. Borrandolas...", LOG_INFO);
        delete(this->playerAreas);
    }
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

PlayerMovement::~PlayerMovement() {
    log("PlayerMovement: Borrando PlayerMovement...", LOG_DEBUG);
}
