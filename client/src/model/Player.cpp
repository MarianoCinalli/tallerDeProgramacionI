#include "model/Player.h"

Player::Player(int orientation, Coordinates* position) {
    log("Jugador: Creando jugador.", LOG_INFO);
    this->orientation = orientation;
    this->position = position;
    this->maxVelocity = 3; // TODO: Probar si va muy rapido.
    this->velocity = new Velocity(0, 0); // Empieza quieto.
    this->sliding = false;
    this->wasSliding = false;   //Deberia estar en PlayerSpriteManager
    this->kicking = false;
    this->wasKicking = false;
    this->canMove = true;
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
    this->orientation = orientation;
}

void Player::accelerate(int direction) {
    this->velocity->accelerate(direction, this->maxVelocity);
    this->orientation = direction; // Para que quede mirando para donde venia corriendo.
    log("Jugador: El jugador esta acelerando, velocidad actual: ", this->velocity, LOG_DEBUG);
}

void Player::decelerate(int direction) {
    this->velocity->decelerate(direction, this->maxVelocity); //no uso getter? por si cambia algo
    this->orientation = direction;
    log("Jugador: El jugador esta frenando, velocidad actual: ", this->velocity, LOG_DEBUG);
}


void Player::updatePosition() {
    if(this->canMove){
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
    if (!this->sliding){
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

void Player::isAlreadySliding(){
    this->wasSliding = true;
}

void Player::stopSliding() {
    this->sliding = false;
    this->wasSliding = false;
}
