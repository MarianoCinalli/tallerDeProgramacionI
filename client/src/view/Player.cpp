#include "view/Player.h"

Player::Player(int orientation, Coordinates* position) {
    this->orientation = orientation;
    this->coordinates = position;
}

Coordinates* Player::getPosition() {
    return this->coordinates;
}

int Player::getOrientation() {
    return this->orientation;
}

void Player::moveUp() {
    this->coordinates->decrementY();
}

void Player::moveDown() {
    this->coordinates->incrementY();
}

void Player::moveRight() {
    this->coordinates->decrementX();
}

void Player::moveLeft() {
    this->coordinates->incrementX();
}

void Player::setOrientation(int orientation) {
    this->orientation = orientation;
}

void Player::render(SDL_Renderer* screen, Coordinates* position) {
    SDL_RenderCopy(screen, this->sprite->getCurrent(), , );
}

Player::~Player() {
}
