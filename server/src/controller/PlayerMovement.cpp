#include "controller/PlayerMovement.h"

PlayerMovement::PlayerMovement() {
    this->playerAreas = new PlayerAreas();
}

void PlayerMovement::setFormation(int formation) {
    this->playerAreas = new PlayerAreas(formation);
}

bool PlayerMovement::canMoveTo(int x, int y, int playerNumber, bool isSelected) {
    bool canMove = false;
    if (!isSelected) {
        Rectangle* rectangle = this->playerAreas->getForPlayer(playerNumber);
        canMove = rectangle->isInside(x, y);
    }
    return canMove;
}

PlayerMovement::~PlayerMovement() {
}
