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
            int margin = 0;
            int number = 0;
            if (playerNumber > 7) {
                number = playerNumber - 7;
            } else {
                number = playerNumber;
            }
            if (this->isDefender(number)) {
                margin = DEFENDER_MARGIN;
            } else if (this->isAttacker(number)) {
                margin = ATTACKER_MARGIN;
            } else {
                margin = MIDFILDER_MARGIN_CLOSE;
            }
            if (rectangle->isCloseToBordersInX(coordinates->getX() + margin)) {
                velocity->setComponentX(0);
            } else if (rectangle->isCloseToBordersInY(coordinates->getY() + margin)) {
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

Coordinates* PlayerMovement::getCoordinatesToFollow(Coordinates* ballPosition, Coordinates* currentPosition, Coordinates* basePosition, int playerNumber, bool isAttacking) {
    int x = ballPosition->getX();
    int y = ballPosition->getY();
    int number = 0;
    if (playerNumber > 7) {
        number = playerNumber - 7;
    } else {
        number = playerNumber;
    }
    Rectangle* rectangle = this->playerAreas->getForPlayer(playerNumber, this->isLeftsideTeam);
    if (this->isDefender(number)) {
        if (isAttacking) {
            // Se acerca al arco.
            Coordinates* furthestPoint = rectangle->getFurthestMiddleVertex(ballPosition);
            x = furthestPoint->getX();
            y = furthestPoint->getY();
            delete(furthestPoint);
            /*
            // Se queda quieto.
            x = basePosition->getX();
            y = basePosition->getY();
            */
        }
        // Ahora corre a la pelota.
        // Dejo lo de abajo por si queremos restaurar la distancia del defensor a la pelota.
        /*else {
            x += this->getAmountToModifyPosition(ballPosition->getX(), currentPosition->getX(), DEFENDER_MARGIN);
            y += this->getAmountToModifyPosition(ballPosition->getY(), currentPosition->getY(), DEFENDER_MARGIN);
        }*/
    } else if (this->isAttacker(number)) {
        if (isAttacking) {
            if (rectangle->isInside(ballPosition->getX(), ballPosition->getY())) {
                // Si la pelota esta dentro del cuadrado corro al punto
                // mas lejano en Y, el X es el mas cercano al arco.
                Coordinates* furthestPoint = rectangle->getFurthestVertex(ballPosition, this->isLeftsideTeam);
                x = furthestPoint->getX();
                y = furthestPoint->getY();
                delete(furthestPoint);
            }
            // Sino corre hacia la pelota. Hasta el borde de su area.
        } else {
            // Se acerca al arco.
            Coordinates* furthestPoint = rectangle->getFurthestMiddleVertex(ballPosition);
            x = furthestPoint->getX();
            y = furthestPoint->getY();
            delete(furthestPoint);
            /*
            // Con esto se queda quieto.
            x = basePosition->getX();
            y = basePosition->getY();
            */
        }
    } else {
        if (isAttacking) {
            // Este se va siempre al punto mas lejano.
            // Si el jugador de su equipo con la pelota pasa la mitad
            // del area del mediocampista va atras de la pelota.
            Coordinates* furthestPoint = rectangle->getFurthestVertex(ballPosition);
            x = furthestPoint->getX();
            y = furthestPoint->getY();
            delete(furthestPoint);
        }
        // Ahora corre a la pelota.
        // Dejo lo de abajo por si queremos restaurar la distancia del mediocampista a la pelota.
        /*else {
            x += this->getAmountToModifyPosition(ballPosition->getX(), currentPosition->getX(), MIDFILDER_MARGIN_CLOSE);
            y += this->getAmountToModifyPosition(ballPosition->getY(), currentPosition->getY(), MIDFILDER_MARGIN_CLOSE);
        }*/
    }
    Coordinates* coordinates = new Coordinates(x, y);
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

// Devuelve el margen que mantienen los jugadores a la pelota.
int PlayerMovement::getAmountToModifyPosition(int destination, int origin, int margin) {
    int value = 0;
    if (origin < destination) {
        // Si estoy atras de la pelota el margen es negativo.
        // Porque se lo resto a la posicion final.
        value = -1 * margin;
    } else {
        value = margin;
    }
    return value;
}

PlayerMovement::~PlayerMovement() {
    log("PlayerMovement: Borrando PlayerMovement...", LOG_DEBUG);
}
