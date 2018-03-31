#include "view/Player.h"

Player::Player(int orientation, Coordinates* position) {
    this->orientation = orientation;
    this->coordinates = position;
    this->maxVelocity = 10; // TODO: Probar si va muy rapido.
    this->velocity = new Velocity(0, 0); // Empieza quieto.
    this->spriteManager = new SpriteManager();
}

Coordinates* Player::getPosition() {
    return this->coordinates;
}

int Player::getOrientation() {
    return this->orientation;
}

void Player::setOrientation(int orientation) {
    this->orientation = orientation;
}

void Player::accelerate(int direction) {
    this->velocity->accelerate(direction, this->maxVelocity);
    // modify sprite.
}

void Player::decelerate(int direction) {
    this->velocity->deaccelerate(direction, this->maxVelocity);
    // modify sprite.
}

void Player::render(SDL_Renderer* screen, Coordinates* position) {
    this->spriteManager->render(screen, position);
}

Player::~Player() {
    delete(this->coordinates);
    delete(this->velocity);
}
