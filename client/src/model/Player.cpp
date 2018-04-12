#include "model/Player.h"

Player::Player(int orientation, Coordinates* position) {
    log("Jugador: Creando jugador...", LOG_INFO);
    this->orientation = orientation;
    this->position = position;
    this->maxVelocity = 3; // TODO: Probar si va muy rapido.
    this->velocity = new Velocity(0, 0); // Empieza quieto.
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
    this->orientation = orientation;
}

void Player::toggleIsSelected() {
    this->isSelected = !this->isSelected;
}

bool Player::getIsSelected() {
    return this->isSelected;
}

void Player::accelerate(int direction) {
    this->velocity->accelerate(direction, this->maxVelocity);
    this->orientation = direction; // Para que quede mirando para donde venia corriendo.
    log("Jugador: El jugador esta acelerando, velocidad actual: ", this->velocity, LOG_DEBUG);
}

void Player::decelerate(int direction) {
    if (!this->velocity->isZero()){
      this->velocity->decelerate(direction, this->maxVelocity);
      this->orientation = direction;
      log("Jugador: El jugador esta frenando, velocidad actual: ", this->velocity, LOG_DEBUG);
    }
}

void Player::stop(int direction){
  this->velocity->stop();
  this->orientation = direction;
  log("Jugador: El jugador esta quieto, velocidad actual: ", this->velocity, LOG_DEBUG);

}

void Player::updatePosition() {
    this->position->addX(this->velocity->getComponentX());
    this->position->addY(this->velocity->getComponentY());
    log("Jugador: Actualizando la posicion del jugador, posicion actual: ", this->position, LOG_DEBUG);
}

void Player::setPosition(Coordinates pos) {
  this->position->setCoord(pos);
}

Player::~Player() {
    delete(this->position);
    delete(this->velocity);
}
