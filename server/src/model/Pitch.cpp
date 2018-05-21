#include "model/Pitch.h"
#include "util/Constants.h"


using namespace std;

Pitch::Pitch(Camera* camera) {
    this->localTeam = NULL;
    this->awayTeam = NULL;
    this->camera = camera;
    teams[0] = this->localTeam; //TODO definir que jugadores tienen que equipo. por ejempl,
    teams[1] = this->awayTeam;
}

void Pitch::setTeam(Team* team, int teamNumber) {
    if (teamNumber == 0) {
        this->localTeam = team;
    } else if (teamNumber == 1) {
        this->awayTeam = team;
    }
}

void Pitch::setUserTeam(int user, int team) {
    if (team == 0) {
        if (this->localTeam != NULL) {
            teams[user] = this->localTeam;
        }
    } else if (team == 1) {
        if (this->awayTeam != NULL) {
            teams[user] = this->awayTeam;
        }
    }
    this->activePlayers[user] = teams[user]->getPlayers().back();
    this->activePlayers[user]->toggleIsSelected();
}

void Pitch::setBall(Ball* ball) {
    this->ball = ball;
}

Ball* Pitch::getBall() {
    return this->ball;
}

Camera* Pitch::getCamera() {
    return this->camera;
}


Team* Pitch::getTeam(int teamNumber) {
    if (teamNumber == 0) {
        return this->localTeam;
    } else {
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

Player* Pitch::getActivePlayer(int user) {
    return this->activePlayers[user];
}

void Pitch::changeActivePlayer(int user) {
    log("cambiando jugador activo", LOG_INFO);
    Team* team = teams[user];
    // Coordinates* center = this->activePlayers[user]->getPosition();
    Coordinates* center = this->ball->getPosition();

    // Solo puede seleccionar de los jugadores dentro de los margenes
    std::list<Player*> playersList = this->camera->getPlayersInsideMargin(team->getPlayers(), 0);
    if (!playersList.empty()) {
        int nearestDistance = LEVEL_WIDTH; //max distance harcodeadeo TODO
        Player* nearestPlayer = playersList.back();
        for (Player* p : playersList) {
            int distance = p->getPosition()->distanceTo(center);
            log("Distancia: ", distance, LOG_DEBUG);
            // if (distance < nearestDistance && distance > 0 && !p->getIsSelected()) {
            if (distance < nearestDistance  && !p->getIsSelected()) {
                nearestDistance = distance;
                nearestPlayer = p;
            }
        }
        Player* player = this->activePlayers[user];
        player->copyStats(nearestPlayer);
        player->toggleIsSelected();
        this->activePlayers[user] = nearestPlayer;
        nearestPlayer->toggleIsSelected();
        log("Pitch: Se cambio el jugador activo.", LOG_INFO);
    }
}

std::list<Player*> Pitch::getPlayersInsideCamera() {
    // std::list<Player*> localPlayers = this->camera->getPlayersInsideMargin(localTeam->getPlayers());
    // std::list<Player*> awayPlayers = this->camera->getPlayersInsideMargin(awayTeam->getPlayers());
    list<Player*> teamPlayers = this->localTeam->getPlayers();
    list<Player*> awayPlayers = this->awayTeam->getPlayers();
    teamPlayers.insert(teamPlayers.end(), awayPlayers.begin(), awayPlayers.end());

    // std::list<Player*>::iterator it;
    // it = localPlayers.begin();
    // ++it;
    // localPlayers.splice(it, awayPlayers);
    // Luego de esto, los jugadores de awayPlayers se pasan a localPlayers.

    return this->camera->getPlayersInsideMargin(teamPlayers, 1);
}


void Pitch::checkIntercepts() {
    log("chequeando intercept", LOG_INFO);
    int value = STEAL_VALUE;
    std::list<Player*> players = this->localTeam->getPlayers();
    std::list<Player*> awayPlayers = this->awayTeam->getPlayers();
    players.insert(players.end(), awayPlayers.begin(), awayPlayers.end());
    if (!players.empty()) {
        int nearestDistance = 500; //max distance harcodeadeo TODO
        Player* nearestPlayer = NULL;
        for (Player* p : players) {
            if (p->isSliding()) {
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

void Pitch::changeBallOwnership() {
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
