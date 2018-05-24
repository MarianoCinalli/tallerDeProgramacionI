#include "model/Ball.h"

Ball::Ball(Coordinates* position) {
    log("Pelota: Creando pelota...", LOG_INFO);
    this->position = position;
    log("Pelota: Pelota creada...", LOG_INFO);
}

Coordinates* Ball::getPosition() {
    return this->position;
}


void Ball::parseYaml(YAML::Node node){
  if (node["cx"]){
    this->position->setX(node["cx"].as<int>());
  }
  if (node["cy"]){
    this->position->setY(node["cy"].as<int>());
  }
}


Ball::~Ball() {
    delete(this->position);
}
