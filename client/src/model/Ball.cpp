#include "model/Ball.h"

Ball::Ball(Coordinates* position) {
    log("Pelota: Creando pelota...", LOG_INFO);
    this->position = position;
    this->still = true;
    log("Pelota: Pelota creada...", LOG_INFO);
}

Coordinates* Ball::getPosition() {
    return this->position;
}

int Ball::isStill(){
  return this->still;
}


void Ball::parseYaml(YAML::Node node){
  if (node["cx"]){
    this->position->setX(node["cx"].as<int>());
  }
  if (node["cy"]){
    // this->position->setY(node["cy"].as<int>() + CAMERA_OFFSET);
    this->position->setY(node["cy"].as<int>()); //offset cuando dibuja

  }
  if (node["st"]){
    this->still = node["st"].as<int>();
  }
}


Ball::~Ball() {
    delete(this->position);
}
