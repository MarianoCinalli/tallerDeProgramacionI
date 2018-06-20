#include "model/Team.h"
#include "util/Formaciones.h"

Team::Team(int local, std::string name) {
    log("Team: Creando equipo.", LOG_INFO);
    this->players = {};
    this->local = local;
    this->name = name;
    this->score = 0;
    log("Team: Equipo creado.", LOG_INFO);
}

void Team::addPlayer(Player* player) {
    log("Team: Agregando jugador.", LOG_INFO);
    this->players.push_back(player);
    log("Team: Jugador agregado.", LOG_INFO);
}

std::list<Player*> Team::getPlayers() {
    return this->players;
}

Player* Team::getPlayer(int num){
  if (this->local==1){
    num +=7;
  }
  for (Player* player : this->players){
    if (player->getId() == num){
      return player;
    }
  }
    return NULL;
}

int Team::getAmountPlayers() {
    return this->players.size();
}

int Team::getTeamNum(){
  return this->local;
}

std::string Team::getName() {
    return name;
}

Team::~Team() {
    log("Team: Liberando memoria. Borrando jugadores...", LOG_INFO);
    // Deleting all players.
    // std::list<Player*>::iterator it;
    // for (it = this->players.begin(); it != this->players.end(); it++) {
    //     delete(*it);

    for (Player* p : players) {
        delete(p);
    }
    // Como la lista sigue llena, de punteros borrados, la vacio.
    this->players.clear();
    log("Team: Jugadores borrados. Memoria liberada.", LOG_INFO);
}

void Team::setFormacion(int formacion) {
    this->formacion = formacion;
    for (Player* p: players){
        p->setFieldPosition(formacion);
    }
}

void Team::order() {
    log("Team: Ordenando equipo...", LOG_DEBUG);
    int i = 0;
    for (Player* p : players) {
        p->setPosition(getFormation(this->local, this->formacion)[i]);
        // Coloca la posicion base de regreso
        p->setBasePosition(getFormation(this->local, this->formacion)[i]);
        i++;
    }
    log("Team: Equipo ordenado.", LOG_DEBUG);
}

void Team::increaseScore() {
    ++this->score;
}

int Team::getScore() {
    return this->score;
}

void Team::addScoreInfo(std::string message) {
    log("Team: GOOOOOL ", message, LOG_INFO);
    this->scoreInfo.push_back(message);
}
