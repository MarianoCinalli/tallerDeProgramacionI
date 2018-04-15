#include "model/Player.h"

Player::Player(int orientation, Coordinates* position) {
    log("Jugador: Creando jugador...", LOG_INFO);
    this->orientation = orientation;
    this->position = position;
    this->maxVelocity = 3; // TODO: Probar si va muy rapido.
    this->velocity = new Velocity(0, 0); // Empieza quieto.
    this->sliding = false;
    this->wasSliding = false;   //Deberia estar en PlayerSpriteManager
    this->kicking = false;
    this->wasKicking = false;
    this->canMove = true;
    this->isSelected = false;
    log("Jugador: Jugador creado.", LOG_INFO);
}

Coordinates* Player::getPosition() {
    return this->position;
}

int Player::getOrientation() {
    return this->orientation;
}

Velocity* Player::getVelocity() {
    return this->velocity;
}

void Player::setOrientation(int orientation) {
    if (this->isRunningDiagonaly()) {
        if (this->velocity->getComponentX() > 0) {
            this->orientation = PLAYER_ORIENTATION_RIGHT;
        } else {
            this->orientation = PLAYER_ORIENTATION_LEFT;
        }
    } else {
        this->orientation = orientation;
    }
}

// Si corre en dos direcciones devuelve true.
bool Player::isRunningDiagonaly() {
    return ((this->velocity->getComponentX() != 0) && (this->velocity->getComponentY() != 0));
}

void Player::toggleIsSelected() {
    this->isSelected = !this->isSelected;
}

bool Player::getIsSelected() {
    return this->isSelected;
}

void Player::accelerate(int direction) {
    this->setOrientation(direction); // Para que quede mirando para donde venia corriendo.
    this->velocity->accelerate(direction, this->maxVelocity);
    log("Jugador: El jugador esta acelerando, velocidad actual: ", this->velocity, LOG_DEBUG);
}

void Player::decelerate(int direction) {
    this->setOrientation(direction);
    if (!this->velocity->isZero()) {
        this->velocity->decelerate(direction, this->maxVelocity);
        log("Jugador: El jugador esta frenando, velocidad actual: ", this->velocity, LOG_DEBUG);
    }
}

void Player::stop(int direction) {
    this->velocity->stop();
    this->orientation = direction;
    log("Jugador: El jugador esta quieto, velocidad actual: ", this->velocity, LOG_DEBUG);
}


void Player::updatePosition() {
    if (this->canMove) {
        this->position->addX(this->velocity->getComponentX());
        this->position->addY(this->velocity->getComponentY());
        log("Jugador: Actualizando la posicion del jugador, posicion actual: ", this->position, LOG_DEBUG);
    }
}

void Player::setPosition(Coordinates pos) {
    this->position->setCoord(pos);
}

Player::~Player() {
    delete(this->position);
    delete(this->velocity);
}

//SLIDE AND KICK FUNCTIONS

void Player::startsKicking() {
    if (!this->sliding) {
        this->kicking = true;
        this->canMove = false;
    }
}

void Player::stopKicking() {
    this->kicking = false;
    this->wasKicking = false;
    this->canMove = true;
}

bool Player::isKicking() {
    return this->kicking;
}

bool Player::wasKickingYet() {
    return this->wasKicking;
}

void Player::isAlreadyKicking() {
    this->wasKicking = true;
}
bool Player::isSliding() {
    return this->sliding;
}

bool Player::wasSlidingYet() {
    return this->wasSliding;
}

void Player::startsSliding() {
    if (!this->kicking)
        this->sliding = true;
    //this->velocity->accelerate(this->orientation, this->maxVelocity);
}

void Player::isAlreadySliding() {
    this->wasSliding = true;
}

void Player::stopSliding() {
    this->sliding = false;
    this->wasSliding = false;
}
