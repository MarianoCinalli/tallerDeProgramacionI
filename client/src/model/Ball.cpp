#include "model/Ball.h"

Ball::Ball(Coordinates* position) {
    log("Pelota: Creando pelota...", LOG_INFO);
    this->position = position;
    this->still = true;
    this->orientation = 0;
    log("Pelota: Pelota creada...", LOG_INFO);
}

Coordinates* Ball::getPosition() {
    return this->position;
}

int Ball::isStill(){
  return this->still;
}

int Ball::getOrientation() {
  return this->orientation;
}


void Ball::parseYaml(YAML::Node node){
  if (node["cx"]){
    this->position->setX(node["cx"].as<int>());
  }
  if (node["cy"]){
    this->position->setY(node["cy"].as<int>());
  }
  if (node["st"]){
    this->still = node["st"].as<int>();
  }
  if (node["sd"]){
    this->orientation = node["sd"].as<int>();
  }
}


Ball::~Ball() {
    delete(this->position);
}
