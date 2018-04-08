#include "model/Pitch.h"

Pitch::Pitch() {
}

Pitch::~Pitch() {
}

Coordinates* getCenter(){
  //harcodeado TODO
  return new Coordinates(0,0);
}

Player* Pitch::activePlayer(){
  Coordinates* center = getCenter();
  std::list <Player*>players = localTeam->getPlayers();
  int nearestDistance = 1000; //max distance harcodeadeo TODO
  Player* nearestPlayer = players.front();
  for (Player* p : players) {
    int distance = p->getPosition()->distanceTo(center);
    if (distance < nearestDistance){
      nearestDistance = distance;
      nearestPlayer = p;
    }
  }
  free(center);
  return nearestPlayer;
}
