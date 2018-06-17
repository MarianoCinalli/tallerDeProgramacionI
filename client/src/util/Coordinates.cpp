#include "util/Coordinates.h"
#include <cmath>

Coordinates::Coordinates(int x, int y) {
    this->x = x;
    this->y = y;
}

Coordinates::~Coordinates() {
}

int Coordinates::getX() {
    return this->x;
}

int Coordinates::getY() {
    return this->y;
}

void Coordinates::setX(int otherX) {
    this->x = otherX;
    validatePositionInBounds();
}

void Coordinates::setY(int otherY) {
    this->y = otherY;
    validatePositionInBounds();
}

void Coordinates::addX(int amount) {
    this->x += amount;
    validatePositionInBounds();
}

void Coordinates::subtractX(int amount) {
    this->x -= amount;
    validatePositionInBounds();
}

void Coordinates::addY(int amount) {
    this->y += amount;
    validatePositionInBounds();
}

void Coordinates::subtractY(int amount) {
    this->y -= amount;
    validatePositionInBounds();
}

void Coordinates::set(Coordinates other){
    this->x = other.x;
    this->y = other.y;
    validatePositionInBounds();
}

int Coordinates::distanceTo(Coordinates* other){
    return sqrt(pow(this->x-other->x,2) + pow(this->y-other->y,2));
}

void Coordinates::validatePositionInBounds() {
    // log("Coordinates: Validate in bounds", LOG_DEBUG);
    //Keep in bounds
    if (this->x < 0) {
        this->x = 0;
    }
    if (this->x + SPRITE_SIZE > LEVEL_WIDTH) {
        this->x = LEVEL_WIDTH - SPRITE_SIZE;
    }
    if (this->y < 0) {
        this->y = 0;
    }
    if (this->y + SPRITE_SIZE > LEVEL_HEIGHT) {
        this->y = LEVEL_HEIGHT - SPRITE_SIZE;
    }
}

std::string Coordinates::toString() {
    return "x: " + std::to_string(this->x) + " y: " + std::to_string(this->y);
}
