#include "model/Player.h"

Player::Player(int orientation, Coordinates* position, int teamNumber) {
    log("Jugador: Creando jugador...", LOG_INFO);
    this->orientation = orientation;
    this->position = position;
    this->team = team;
    this->sliding = false;
    this->kicking = false;
    this->isSelected = false;
    this->still = true;
    this->runningFast = false;
    log("Jugador: Jugador creado.", LOG_INFO);
}

void Player::parseYaml(YAML::Node node){
  if (node["te"]){
    this->team = node["te"].as<int>();
  }
  if (node["se"]){
    this->isSelected = node["se"].as<int>();
  }
  if (node["ru"]){
    this->runningFast = node["ru"].as<int>();
  }
  if (node["ki"]){
    this->kicking = node["ki"].as<int>();
  }
  if (node["sl"]){
    this->sliding = node["sl"].as<int>();
  }
  if (node["st"]){
    this->still = node["st"].as<int>();
  }
  if (node["or"]){
    this->orientation = node["or"].as<int>();
  }
  if (node["cx"]){
    this->position->setX(node["cx"].as<int>());
  }
  if (node["cy"]){
    this->position->setY(node["cy"].as<int>() + CAMERA_OFFSET);
  }
  if (node["nm"]){
    this->userName = node["nm"].as<std::string>();
  }
}

Coordinates* Player::getPosition() {
    return this->position;
}

int Player::getOrientation() {
    return this->orientation;
}

bool Player::getIsSelected() {
    return this->isSelected;
}

bool Player::isStill(){
  return this->still;
}

bool Player::isRunningFast() {
    return this->runningFast;
}

bool Player::isKicking() {
    return this->kicking;
}

bool Player::isSliding() {
    return this->sliding;
}

Player::~Player() {
    delete(this->position);
}
