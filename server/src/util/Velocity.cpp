#include "util/Velocity.h"

Velocity::Velocity(int x, int y) {
    this->x = x;
    this->y = y;
}

int Velocity::getComponentX() {
    return this->x;
}

int Velocity::getComponentY() {
    return this->y;
}

void Velocity::setComponentX(int x) {
    this->x = x;
}

void Velocity::set(Velocity* vel) {
  this->x = vel -> x;
  this->y = vel -> y;
}

void Velocity::setComponentY(int y) {
    this->y = y;
}

void Velocity::accelerate(int direction, int amount) {
    this->changeOnDirection(direction, amount);
}

void Velocity::accelerate(int direction) {
    this->changeOnDirection(direction);
}

void Velocity::decelerate(int direction, int amount) {
    if (!this->isZero()) {
        this->changeOnDirection(direction, (-1) * amount);
    }
}

int sign(int num){
  if (num>0)
  return 1;
  else if (num<0)
  return -1;
  else return 0;
}

void Velocity::decelerate(int amount) {
    if (!this->isZero()) {
        if(this->x!=0){
          this->x-= sign(this->x)*amount;
        }
        if(this->y!=0){
          this->y-= sign(this->y)*amount;
        }
    }
}

void Velocity::scale(int num){
  this->x *= num;
  this->y *= num;
}

void Velocity::scaleY(int num){
  this->y *= num;
}

void Velocity::stop(){
  this->x = 0;
  this->y = 0;
}

void Velocity::stopDirection(int direction) {
    if (direction == PLAYER_ORIENTATION_LEFT || direction == PLAYER_ORIENTATION_RIGHT) {
        this->x = 0;
    } else {
        this->y = 0;
    }
}

bool Velocity::isZero() {
    return ((this->x == 0) && (this->y == 0));
}

// Como el origen esta arriba a la izquierda.
// Para la derecha y abajo suma si acelera, resta si desacelera.
// Para la izquierda y arriba resta si acelera, suma si desacelera.
void Velocity::changeOnDirection(int direction, int amount) {
    switch(direction) {
        case PLAYER_ORIENTATION_LEFT:
            this->x -= amount;
            break;
        case PLAYER_ORIENTATION_UP:
            this->y -= amount;
            break;
        case PLAYER_ORIENTATION_RIGHT:
            this->x += amount;
            break;
        case PLAYER_ORIENTATION_DOWN:
            this->y += amount;
            break;
    }
}

void Velocity::changeOnDirection(int direction) {

    int newx = 0;
    int newy = 0;
    switch(direction) {
        case PLAYER_ORIENTATION_LEFT:
            newx = -1;
            break;
        case PLAYER_ORIENTATION_UP:
            newy = -1;
            break;
        case PLAYER_ORIENTATION_RIGHT:
            newx = 1;
            break;
        case PLAYER_ORIENTATION_DOWN:
            newy = 1;
            break;
    }
    if (newx) {
      if(this->x + newx){
        this->x = newx;
      } else{
        this->x = 0;
      }
    } else {
    if(this->y + newy){
      this->y = newy ;
    } else {
      this->y = 0;
    }
  }
}

Velocity::~Velocity() {
}

int Velocity::getAsOrientation() {
  if (this->x > 0) {
    return PLAYER_ORIENTATION_RIGHT;
  }
  else if (this->x < 0) {
    return PLAYER_ORIENTATION_LEFT;
  }
  else if (this->y > 0) {
    return PLAYER_ORIENTATION_DOWN;
  }
  else if (this->y < 0) {
    return PLAYER_ORIENTATION_UP;
  }
  else
    return PLAYER_ORIENTATION_INVALID;
}

std::string Velocity::toString() {
    return "x: " + std::to_string(this->x) + " y: " + std::to_string(this->y);
}
