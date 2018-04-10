#include "model/Pitch.h"
#include "util/Constants.h"


Pitch::Pitch() {
}

void Pitch::setLocalTeam(Team* team) {
    this->localTeam = team;
    changeActivePlayer();
}

Team* Pitch::getLocalTeam() {
    return this->localTeam;
}

Pitch::~Pitch() {
    delete(this->localTeam);
}

Coordinates* getCenter() {
    //harcodeado TODO
    return new Coordinates(800, 500);
}

void Pitch::changeActivePlayer(){
  Coordinates* center = getCenter();
  std::list <Player*>players = localTeam->getPlayers();
  int nearestDistance = LEVEL_WIDTH; //max distance harcodeadeo TODO
  Player* nearestPlayer = players.back();
  for (Player* p : players) {
    int distance = p->getPosition()->distanceTo(center);
    log("Distancia: ",distance, LOG_DEBUG);
    if (distance < nearestDistance){
      nearestDistance = distance;
      nearestPlayer = p;
    }
  }
    delete(center);
    activePlayer = nearestPlayer;
    //delete(nearestPlayer);
  }
