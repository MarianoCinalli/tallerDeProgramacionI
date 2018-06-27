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
    if (validatePositionInBoundsY()){
      return -1;
    }
    return 0;

}

int Coordinates::setY(int otherY) {
    this->y = otherY;
    if(validatePositionInBoundsY()){
    return -1;
  }
  return 0;
}

int Coordinates::addX(int amount) {
  this->x += amount;
  int validation = validatePositionInBoundsX();
  if (validation == POST_BOUNCE_VALIDATE){
    this->x -= amount;
    return -1;
  }else if(validation == OUT_FIELD_VALIDATE){
    return -1;
  }
  return 0;
}

void Coordinates::subtractX(int amount) {
    this->x -= amount;
    validatePositionInBoundsX();
}

int Coordinates::addY(int amount) {
    this->y += amount;
    int validation = validatePositionInBoundsY();
    if (validation == POST_BOUNCE_VALIDATE){
      this->y -= amount;
      return -1;
    }else if(validation == OUT_FIELD_VALIDATE){
      return -1;
    }
    return 0;
}

void Coordinates::subtractY(int amount) {
    this->y -= amount;
    validatePositionInBoundsY();
}

void Coordinates::set(Coordinates other){
    this->x = other.x;
    this->y = other.y;
    validatePositionInBoundsY();
}

int Coordinates::distanceTo(Coordinates* other){
    return sqrt(pow(this->x-other->x,2) + pow(this->y-other->y,2));
}

int Coordinates::validatePositionInBoundsY() {
    // log("Coordinates: Validate in bounds", LOG_DEBUG);
    //Keep in bounds
    if (this->x < 0) {
        this->x = 0;
        return OUT_FIELD_VALIDATE;
    }
    if (this->x + SPRITE_SIZE > LEVEL_WIDTH) {
        this->x = LEVEL_WIDTH - SPRITE_SIZE;
        return OUT_FIELD_VALIDATE;
    }
    if (this->y < 0) {
        this->y = 0;
        return OUT_FIELD_VALIDATE;
    }
    if (this->y + SPRITE_SIZE > LEVEL_HEIGHT) {
        this->y = LEVEL_HEIGHT - SPRITE_SIZE;
        return OUT_FIELD_VALIDATE;
    }
    return false;
}


int Coordinates::validatePositionInBoundsX() {
    // log("Coordinates: Validate in bounds", LOG_DEBUG);
    //Keep in bounds
    int x = this->x;
    int y = this->y;
    if (x < 0) {
        x = 0;
        return OUT_FIELD_VALIDATE;
    }
    if (x + SPRITE_SIZE > LEVEL_WIDTH) {
        x = LEVEL_WIDTH - SPRITE_SIZE;
        return OUT_FIELD_VALIDATE;
    }
    //check post
    if ((y > TOP_POST_POSITION-25) && (y < TOP_POST_POSITION+10)) {
        if ((x < 70) || (x > 1480))
          return POST_BOUNCE_VALIDATE;
    }
    if ((y > BOTTOM_POST_POSITION-10) && (y < BOTTOM_POST_POSITION+25)) {
        if ((x < 70) || (x > 1480))
          return POST_BOUNCE_VALIDATE;
    }
    return false;
}

std::string Coordinates::toString() {
    return "x: " + std::to_string(this->x) + " y: " + std::to_string(this->y);
}
