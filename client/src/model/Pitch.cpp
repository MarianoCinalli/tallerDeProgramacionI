#include "model/Pitch.h"
#include "util/Constants.h"


Pitch::Pitch(Camera* camera) {
  this->camera = camera;
}

void Pitch::setLocalTeam(Team* team) {
    this->localTeam = team;
    this->activePlayer = this->localTeam->getPlayers().back();
    this->activePlayer->toggleIsSelected();
    changeActivePlayer();
}

Team* Pitch::getLocalTeam() {
    return this->localTeam;
}

Pitch::~Pitch() {
    delete(this->localTeam);
}

Coordinates* getCenter() {
    return new Coordinates(LEVEL_WIDTH/2, SCREEN_HEIGHT/2);
}

void Pitch::changeActivePlayer(){
  Coordinates* center = activePlayer->getPosition();

  //std::list<Player*> players = localTeam->getPlayers();
  // Solo puede seleccionar de los jugadores dentro de los margenes
  std::list<Player*> players = this->camera->getPlayersInsideMargin(localTeam->getPlayers());
  if (!players.empty()){
    int nearestDistance = LEVEL_WIDTH; //max distance harcodeadeo TODO
    Player* nearestPlayer = players.back();
    for (Player* p : players) {
      int distance = p->getPosition()->distanceTo(center);
      log("Distancia: ",distance, LOG_DEBUG);
      if (distance < nearestDistance && distance > 0){
        nearestDistance = distance;
        nearestPlayer = p;
      }
    }
    // delete(center);
    this->activePlayer->toggleIsSelected();
    this->activePlayer = nearestPlayer;
    this->activePlayer->toggleIsSelected();
  }
    //delete(nearestPlayer);
  }
