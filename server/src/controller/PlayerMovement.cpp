#include "controller/PlayerMovement.h"

PlayerMovement::PlayerMovement() {
    log("PlayerMovement: Creando movimientos para los jugadores. Si no se especifica una formacion, no voy a tener areas...", LOG_DEBUG);
    this->playerAreas = NULL;
}

void PlayerMovement::setFormation(int formation) {
    if (this->playerAreas != NULL) {
        log("PlayerMovement: Habia areas especificadas previamente. Borrandolas...", LOG_INFO);
        delete(this->playerAreas);
    }
    log("PlayerMovement: Creando areas para formacion: ", formation, LOG_DEBUG);
    this->playerAreas = new PlayerAreas(formation);
}

bool PlayerMovement::canMoveTo(int x, int y, int playerNumber, bool isSelected) {
    bool canMove = true;
    if (this->playerAreas == NULL) {
        log("PlayerMovement: Las areas son nulas.", LOG_ERROR);
        return false;
    }
    if (!isSelected) {
        Rectangle* rectangle = this->playerAreas->getForPlayer(playerNumber);
        if (rectangle == NULL) {
            log("PlayerMovement: No se permite que el jugador que se mueva. Porque el area es nula, para el jugador: ", playerNumber, LOG_ERROR);
            canMove = false;
        } else {
            canMove = rectangle->isInside(x, y);
        }
    }
    return canMove;
}

PlayerMovement::~PlayerMovement() {
    log("PlayerMovement: Borrando PlayerMovement...", LOG_DEBUG);
}
