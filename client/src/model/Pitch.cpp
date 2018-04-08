#include "model/Pitch.h"

Pitch::Pitch() {
}

void Pitch::setLocalTeam(Team* team) {
    this->localTeam = team;
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

Player* Pitch::activePlayer() {
    Coordinates* center = getCenter();
    std::list <Player*>players = localTeam->getPlayers();
    int nearestDistance = 1000; //max distance harcodeadeo TODO
    Player* nearestPlayer = players.front();
    for (Player* p : players) {
        int distance = p->getPosition()->distanceTo(center);
        if (distance < nearestDistance) {
            nearestDistance = distance;
            nearestPlayer = p;
        }
    }
    delete(center);
    return nearestPlayer;
}
