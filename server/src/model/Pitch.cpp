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

Team* Pitch::getTeam(int teamNumber) {
  if (teamNumber == 0) {
      return this->localTeam;
    }
  else {
      return this->awayTeam;
    }
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

void Pitch::changeActivePlayer(int user) {

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


void Pitch::checkIntercepts(){
	int value = STEAL_VALUE;
	std::list<Player*> players = this->localTeam->getPlayers();
	std::list<Player*> awayPlayers = this->awayTeam->getPlayers();
	players.insert(players.end(), awayPlayers.begin(), awayPlayers.end());
	if (!players.empty()) {
		int nearestDistance = 500; //max distance harcodeadeo TODO
		Player* nearestPlayer = NULL;
		for (Player* p : players) {
			if(p->isSliding()){
				int distance = p->getPosition()->distanceTo(this->ball->getPosition());
				log("Distancia a pelota: ", distance, LOG_DEBUG);
				if (distance < nearestDistance && distance > 0 && distance < value) {
					nearestDistance = distance;
					nearestPlayer = p;
				}
			}
		}
		if (nearestPlayer != NULL) {
			this->ball->isIntercepted(nearestPlayer);
			log("Cambiado a jugador!", LOG_INFO);
		}
	}
}

void Pitch::changeBallOwnership()
{
	if (this->ball->isInterceptable()) {
		int value = INTERCEPT_VALUE;
		std::list<Player*> players = this->localTeam->getPlayers();
		std::list<Player*> awayPlayers = this->awayTeam->getPlayers();
		players.insert(players.end(), awayPlayers.begin(), awayPlayers.end());
		if (!players.empty()) {
			int nearestDistance = 300; //max distance harcodeadeo TODO
			Player* nearestPlayer = NULL;
			for (Player* p : players) {
				int distance = p->getPosition()->distanceTo(this->ball->getPosition());
				log("Distancia a pelota: ", distance, LOG_DEBUG);
				if (distance < nearestDistance && distance > 0 && distance < value) {
					nearestDistance = distance;
					nearestPlayer = p;
				}
			}
			if (nearestPlayer != NULL) {
				this->ball->isIntercepted(nearestPlayer);
				log("Cambiado a jugador!", LOG_INFO);
			}
		}
	}
}
