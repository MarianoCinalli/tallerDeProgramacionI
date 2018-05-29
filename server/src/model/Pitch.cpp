#include "model/Pitch.h"
#include "util/Constants.h"


using namespace std;

Pitch::Pitch(Camera* camera) {
    this->localTeam = NULL;
    this->awayTeam = NULL;
    this->camera = camera;
    // teams[0] = this->localTeam; //TODO definir que jugadores tienen que equipo. por ejempl,
    // teams[1] = this->awayTeam;
}

void Pitch::setTeam(Team* team, int teamNumber) {
    if (teamNumber == 0) {
        this->localTeam = team;
    } else if (teamNumber == 1) {
        this->awayTeam = team;
    }
}

void Pitch::setUserTeam(std::string user, int team) {
    log("Pitch: Intentando agregar al usuario " + user + " al equipo: ", team, LOG_DEBUG);
    if (team == 0) {
        if (this->localTeam != NULL) {
            log("Pitch: Usuario " + user + " agregado al equipo: ", team, LOG_DEBUG);
            teams[user] = this->localTeam;
        } else {
            log("Pitch: El equpo local es nulo.", LOG_ERROR);
        }
    } else if (team == 1) {
        if (this->awayTeam != NULL) {
            log("Pitch: Usuario " + user + " agregado al equipo: ", team, LOG_DEBUG);
            teams[user] = this->awayTeam;
        } else {
            log("Pitch: El equpo visitante es nulo.", LOG_ERROR);
        }
    } else {
        log("Pitch: Error numero de equipo desconocido: ", team, LOG_ERROR);
    }
    log("Pitch: Se le asignaron jugador al usuario: ", user, LOG_DEBUG);
    this->activePlayers[user] = teams[user]->getPlayers().back();
    this->activePlayers[user]->toggleIsSelected(user);
    log("Pitch: Se le asignaron equipo y jugador al usuario: ", user, LOG_DEBUG);
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

Player* Pitch::getActivePlayer(std::string user) {
    Player* activePlayer = NULL;
    auto search = this->activePlayers.find(user);
    if(search != this->activePlayers.end()) {
        activePlayer = this->activePlayers[user];
    } else {
        log("Pitch: No se encontro el jugador activo para el usuario: ", user, LOG_ERROR);
    }
    if (activePlayer) {
        log("Pitch: El jugador activo es nulo para el usuario: ", user, LOG_ERROR);
    }
    return activePlayer;
}

void Pitch::removeActivePlayer(std::string user) {
    log("Pitch: Para remover el jugador activo, buscandolo para usuario: ", user, LOG_DEBUG);
    auto search = this->activePlayers.find(user);
    if(search != this->activePlayers.end()) {
        log("Pitch: Removiendo jugador activo para usuario ", user, LOG_DEBUG);
        this->activePlayers[user]->toggleIsSelected(user);
        this->activePlayers[user] = NULL;
        this->activePlayers.erase(search);
    } else {
        log("Pitch: No se encontro el jugador activo para el usuario: ", user, LOG_ERROR);
        log("Pitch: ", this->getUsersWithActivePlayersAsString(), LOG_DEBUG);
    }
}

// Metodo para debugear.
std::string Pitch::getUsersWithActivePlayersAsString() {
    std::string usersWithActivePlayers = "";
    for (auto const& activePlayer : this->activePlayers) {
        usersWithActivePlayers += activePlayer.first + "";
    }
    return usersWithActivePlayers;
}

void Pitch::changeActivePlayer(std::string user) {
    log("Pitch: Cambiando jugador activo", LOG_INFO);
    Team* team = teams[user];
    // Coordinates* center = this->activePlayers[user]->getPosition();
    Coordinates* center = this->ball->getPosition();

    // Solo puede seleccionar de los jugadores dentro de los margenes,
    std::list<Player*> playersList = this->camera->getPlayersInsideMargin(team->getPlayers(), 1);
    if (!playersList.empty()) {
        int nearestDistance = LEVEL_WIDTH; //max distance harcodeadeo TODO
        Player* nearestPlayer = playersList.back();
        for (Player* p : playersList) {
            int distance = p->getPosition()->distanceTo(center);
            log("Pitch: Distancia ", distance, LOG_SPAM);
            // if (distance < nearestDistance && distance > 0 && !p->getIsSelected()) {
            if (distance < nearestDistance  && !p->getIsSelected()) {
                nearestDistance = distance;
                nearestPlayer = p;
            }
        }
        Player* player = this->activePlayers[user];
        player->copyStats(nearestPlayer);
        player->toggleIsSelected(user);
        this->activePlayers[user] = nearestPlayer;
        nearestPlayer->toggleIsSelected(user);
        log("Pitch: Se cambio el jugador activo.", LOG_INFO);
    } else {
        log("Pitch: La lista de jugadores esta vacia!.", LOG_ERROR);
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


void Pitch::checkSteals() {
    log("Pitch: Chequeando interceptaciones...", LOG_DEBUG);
    if (this->ball->getHeight() < 1) {  //TODO mejorar, esta feo
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
                    log("Pitch: Distancia a pelota: ", distance, LOG_SPAM);
                    if (distance < nearestDistance && distance > 0 && distance < value) {
                        nearestDistance = distance;
                        nearestPlayer = p;
                    }
                }
            }
            if (nearestPlayer != NULL) {
                this->ball->isIntercepted(nearestPlayer);
                log("Pitch: Cambiando de jugador con la posesion de la pelota.", LOG_DEBUG);
            }
        }
    }
}

void Pitch::changeBallOwnership() {
    if (this->ball->isInterceptable() && (this->ball->getHeight() < 1)) {
        int value = CHANGE_OWNERSHIP_VALUE;
        std::list<Player*> players = this->localTeam->getPlayers();
        std::list<Player*> awayPlayers = this->awayTeam->getPlayers();
        players.insert(players.end(), awayPlayers.begin(), awayPlayers.end());
        if (!players.empty()) {
            int nearestDistance = 300; //max distance harcodeadeo TODO
            Player* nearestPlayer = NULL;
            for (Player* p : players) {
                int distance = p->getPosition()->distanceTo(this->ball->getPosition());
                log("Pitch: Distancia a pelota: ", distance, LOG_SPAM);
                if (distance < nearestDistance && distance > 0 && distance < value) {
                    nearestDistance = distance;
                    nearestPlayer = p;
                }
            }
            if (nearestPlayer != NULL) {
                this->ball->isIntercepted(nearestPlayer);
                log("Pitch: Cambiando de jugador con la posesion de la pelota.", LOG_DEBUG);
            }
        }
    }
}
