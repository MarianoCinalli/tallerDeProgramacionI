#include "model/Pitch.h"
#include "util/Constants.h"


Pitch::Pitch(Camera* camera) {
    this->localTeam = NULL;
    this->awayTeam = NULL;
    this->camera = camera;
}

void Pitch::setTeam(Team* team, int teamNumber) {
    if (teamNumber == 0) {
        this->localTeam = team;
    } else {
        this->awayTeam = team;
    }
}

void Pitch::setBall(Ball* ball) {
    this->ball = ball;
}

Ball* Pitch::getBall() {
    return this->ball;
}

Team* Pitch::getLocalTeam() {
    return this->localTeam;
}

Pitch::~Pitch() {
    if (this->localTeam != NULL) {
        delete(this->localTeam);
    }
    if (this->awayTeam != NULL) {
        delete(this->awayTeam);
    }
}

Coordinates* getCenter() {
    return new Coordinates(LEVEL_WIDTH / 2, SCREEN_HEIGHT / 2);
}

void Pitch::changeActivePlayer() {
    Coordinates* center = activePlayer->getPosition();
    // Solo puede seleccionar de los jugadores dentro de los margenes
    std::list<Player*> players = this->camera->getPlayersInsideMargin(localTeam->getPlayers());
    if (!players.empty()) {
        int nearestDistance = LEVEL_WIDTH; //max distance harcodeadeo TODO
        Player* nearestPlayer = players.back();
        for (Player* p : players) {
            int distance = p->getPosition()->distanceTo(center);
            log("Distancia: ", distance, LOG_DEBUG);
            if (distance < nearestDistance && distance > 0) {
                nearestDistance = distance;
                nearestPlayer = p;
            }
        }
        this->activePlayer->copyStats(nearestPlayer);
        this->activePlayer->toggleIsSelected();
        this->activePlayer = nearestPlayer;
        this->activePlayer->toggleIsSelected();
        log("Pitch: Se cambio el jugador activo.", LOG_INFO);
    }
}

std::list<Player*> Pitch::getPlayersInsideCamera() {
    std::list<Player*> localPlayers = this->camera->getPlayersInsideMargin(localTeam->getPlayers());
    std::list<Player*> awayPlayers = this->camera->getPlayersInsideMargin(awayTeam->getPlayers());
    std::list<Player*>::iterator it;
    it = localPlayers.begin();
    ++it;
    localPlayers.splice(it, awayPlayers);
    // Luego de esto, los jugadores de awayPlayers se pasan a localPlayers.
    return localPlayers;
}
