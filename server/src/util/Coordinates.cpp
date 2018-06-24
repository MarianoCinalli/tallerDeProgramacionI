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

int Coordinates::setX(int otherX) {
    this->x = otherX;
    if (validatePositionInBounds()){
      return -1;
    }
    return 0;
}

int Coordinates::setY(int otherY) {
    this->y = otherY;
    if(validatePositionInBounds()){
    return -1;
  }
  return 0;
}

int Coordinates::addX(int amount) {
    this->x += amount;
    if (validatePositionInBounds()){
      return -1;
    }
    return 0;
}

void Coordinates::subtractX(int amount) {
    this->x -= amount;
    validatePositionInBounds();
}

int Coordinates::addY(int amount) {
    this->y += amount;
    if (validatePositionInBounds()){
      return -1;
    }
    return 0;
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

void Coordinates::normalize(){
    log("Coordinates(joako): entro a normalizar ", LOG_DEBUG); //SACAR
    float div = (sqrt(pow(this->x,2) + pow(this->y,2)));
    float normalX = (x / div);
    float normalY = (y / div);
    this->x = (int)normalX;
    this->y = (int)normalY;
    log("Coordinates(joako):salgo de normalizar ", LOG_DEBUG); //SACAR
}

int Coordinates::distanceTo(Coordinates* other){
    return sqrt(pow(this->x-other->x,2) + pow(this->y-other->y,2));
}

int Coordinates::validatePositionInBounds() {
    // log("Coordinates: Validate in bounds", LOG_DEBUG);
    //Keep in bounds
    if (this->x < 0) {
        this->x = 0;
        return true;
    }
    if (this->x + SPRITE_SIZE > LEVEL_WIDTH) {
        this->x = LEVEL_WIDTH - SPRITE_SIZE;
        return true;
    }
    if (this->y < 0) {
        this->y = 0;
        return true;
    }
    if (this->y + SPRITE_SIZE > LEVEL_HEIGHT) {
        this->y = LEVEL_HEIGHT - SPRITE_SIZE;
        return true;
    }
    return false;
}

std::string Coordinates::toString() {
    return "x: " + std::to_string(this->x) + " y: " + std::to_string(this->y);
}
