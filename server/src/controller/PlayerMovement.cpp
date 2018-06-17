#include "controller/PlayerMovement.h"

PlayerMovement::PlayerMovement() {
    this->defenders = 0;
    this->midfielders = 0;
    this->attackers = 0;
}

void PlayerMovement::setFormation(int formation) {
    this->defenders = (formation / 100) % 10;
    this->midfielders = (formation / 10) % 10;
    this->attackers = formation % 10;
}

bool PlayerMovement::canMoveTo(int x, int y, int playerNumber, bool isSelected) {
    bool canMove = false;
    if (!isSelected) {
        /*
        Rectangle* rectangle = this->getAreaForPlayer(playerNumber);
        canMove = rectangle->isInside(x, y);
        */
    }
    return canMove;
}
/*
Rectangle* PlayerMovement::getAreaForPlayer(int playerNumber) {
}
*/
PlayerMovement::~PlayerMovement() {
}


