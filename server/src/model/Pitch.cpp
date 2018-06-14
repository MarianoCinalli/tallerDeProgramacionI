#include "model/Pitch.h"
#include "util/Constants.h"


using namespace std;

Pitch::Pitch(Camera* camera) {
    this->localTeam = NULL;
    this->awayTeam = NULL;
    this->camera = camera;
    // teams();
}

void Pitch::setTeam(Team* team, int teamNumber) {
    if (teamNumber == 0) {
        this->localTeam = team;
    } else if (teamNumber == 1) {
        this->awayTeam = team;
    }

}

const int GOAL_HEIGHT = 3;

int Pitch::goalkick(){
  Ball* ball = this->ball;
  int x = ball->getPosition()->getX();
  int y = ball->getPosition()->getY();
  //int height = ball->getHeight();
  if (((x < 30) || (x > 1510)) && ((y<600) && (y>400))){ //&& height<GOAL_HEIGHT){
    return CENTER_START;
  }
  else if (x < 30){
    return LEFT_START;
  }
  else if (x > 1510){
    return RIGHT_START;
  }
  return -1;
}

void Pitch::setStart(int position){
  this->localTeam->order();
  this->awayTeam->order();
  this->ball->restart(position);
}

void Pitch::setUserTeam(std::string user, int teamNum, int formation) {
    log("Pitch: Intentando agregar al usuario " + user + " al equipo: ", teamNum, LOG_DEBUG);
    Team* team;
    if (teamNum == 0) {
        if (this->localTeam != NULL) {
            log("Pitch: Usuario " + user + " agregado al equipo: ", teamNum, LOG_DEBUG);
            teams[this->localTeam].push_front(user);
            team = this->localTeam;
        } else {
            log("Pitch: El equpo local es nulo.", LOG_ERROR);
        }
    } else if (teamNum == 1) {
        if (this->awayTeam != NULL) {
            log("Pitch: Usuario " + user + " agregado al equipo: ", teamNum, LOG_DEBUG);
            teams[this->awayTeam].push_front(user);
            team = this->awayTeam;
        } else {
            log("Pitch: El equpo visitante es nulo.", LOG_ERROR);
        }
    } else {
        log("Pitch: Error numero de equipo desconocido: ", teamNum, LOG_ERROR);
    }
    log("Pitch: Se le asignaron jugador al usuario: ", user, LOG_DEBUG);
    team->setFormacion(formation);
    team->order();
    log("Pitch: formacion: ", formation, LOG_DEBUG);
    this->activePlayers[user] = team->getPlayers().back();
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

Player* Pitch::getActivePlayer(std::string user) {
    Player* activePlayer = NULL;
    auto search = this->activePlayers.find(user);
    if(search != this->activePlayers.end()) {
        activePlayer = this->activePlayers[user];
    } else {
        log("Pitch: No se encontro el jugador activo para el usuario: ", user, LOG_ERROR);
    }
    if (activePlayer== NULL) {
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
    // Coordinates* center = this->activePlayers[user]->getPosition();
    Coordinates* center = this->ball->getPosition();
    Player* player = this->activePlayers[user];
    Team* team = this->getTeam(player->getTeam());
    if (!player->isWithBall()){
    // Solo puede seleccionar de los jugadores dentro de los margenes,
    std::list<Player*> playersList = this->camera->getPlayersInsideMargin(team->getPlayers(), 1);
    if (!playersList.empty()) {
        int nearestDistance = LEVEL_WIDTH; //max distance harcodeadeo TODO
        Player* nearestPlayer = NULL;
        for (Player* p : playersList) {
            int distance = p->getPosition()->distanceTo(center);
            log("Pitch: Distancia ", distance, LOG_SPAM);
            // if (distance < nearestDistance && distance > 0 && !p->getIsSelected()) {
            if (distance < nearestDistance  && !p->getIsSelected()) {
                nearestDistance = distance;
                nearestPlayer = p;
            }
        }
        if (nearestPlayer == NULL){
          nearestPlayer = playersList.back();
        }
        // Player* player = this->activePlayers[user];
        this->setActivePlayer(user, nearestPlayer);
    } else {
        log("Pitch: La lista de jugadores esta vacia!.", LOG_ERROR);
    }
  }
}

void Pitch::setActivePlayer(std::string user, Player* player){

  Player* currentPlayer = this->activePlayers[user];
  if (currentPlayer != NULL){
  currentPlayer->copyStats(player);
  currentPlayer->toggleIsSelected(user);
  this->activePlayers[user] = player;
  player->toggleIsSelected(user);
  log("Pitch: Se cambio el jugador activo.", LOG_INFO);
}
}

std::list<Player*> Pitch::getPlayers() {
    list<Player*> teamPlayers = this->localTeam->getPlayers();
    list<Player*> awayPlayers = this->awayTeam->getPlayers();
    teamPlayers.insert(teamPlayers.end(), awayPlayers.begin(), awayPlayers.end());

    return teamPlayers;
}

std::list<Player*> Pitch::getPlayersInsideCamera() {
    list<Player*> teamPlayers = this->localTeam->getPlayers();
    list<Player*> awayPlayers = this->awayTeam->getPlayers();
    teamPlayers.insert(teamPlayers.end(), awayPlayers.begin(), awayPlayers.end());

    return this->camera->getPlayersInsideMargin(teamPlayers, 1);
}


void Pitch::checkSteals() {
    log("Pitch: Chequeando intercepciones...", LOG_DEBUG);

    std::list<Player*> players = this->localTeam->getPlayers();
    std::list<Player*> awayPlayers = this->awayTeam->getPlayers();
    players.insert(players.end(), awayPlayers.begin(), awayPlayers.end());
    if (!players.empty()) {
        int nearestDistance = 500; //max distance harcodeadeo TODO
        Player* nearestPlayer = NULL;
        for (Player* p : players) {
            if (p->isSliding()) {
                int value = p->getStealCoef();
                int prob = rand()%100;
                log("Pitch: probabilidad de sacar:",prob,LOG_SPAM);
                int distance = p->getPosition()->distanceTo(this->ball->getPosition());
                log("Pitch: Distancia a pelota: ", distance, LOG_SPAM);
                if (prob<value && distance < nearestDistance && distance > 0 && distance < STEAL_DISTANCE) {
                    nearestDistance = distance;
                    nearestPlayer = p;
                }
            }
        }
        if (nearestPlayer != NULL) {
            this->ball->isIntercepted(nearestPlayer);
            // Team* team = this->getTeam(nearestPlayer->getTeam());
            // std::string user = teams[team].front();
            // this->setActivePlayer(user, nearestPlayer); //which user?
            log("Pitch: Cambiando de jugador con la posesion de la pelota.", LOG_DEBUG);
        }
    }
}

void Pitch::changeBallOwnership() {
    if (this->ball->isInterceptable()) {
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
                Team* team = this->getTeam(nearestPlayer->getTeam());
                std::string user = teams[team].front();
                this->setActivePlayer(user, nearestPlayer); //which user?
                log("Pitch: Cambiando de jugador con la posesion de la pelota.", LOG_DEBUG);
            }
        }
    }
}

std::string Pitch::getScoresYAML() {
    std::string message = "";
    if (this->localTeam != NULL) {
        message += " gl: " + std::to_string(this->localTeam->getScore()) + "\n";
    } else {
        log("Pitch: El equipo local es null.", LOG_ERROR);
    }
    if (this->awayTeam != NULL) {
        message += " gv: " + std::to_string(this->awayTeam->getScore()) + "\n";
    } else {
        log("Pitch: El equipo visitante es null.", LOG_ERROR);
    }
    return message;
}
