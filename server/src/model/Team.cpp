#include "model/Team.h"

Team::Team(int local, std::string name) {
    log("Team: Creando equipo.", LOG_INFO);
    this->players = {};
    this->local = local; // 1 visitante - 0 local
    this->name = name;
    this->score = 0;
    this->formation = new Formation(312, local == 0);
    this->playerMovement = new PlayerMovement(); // Lo deja moverse sin restricciones.
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

Player* Team::getPlayer(int num) {
    if (this->local == 1) {
        num += 7;
    }
    for (Player* player : this->players) {
        if (player->getId() == num) {
            return player;
        }
    }
    return NULL;
}

int Team::getAmountPlayers() {
    return this->players.size();
}
int Team::getTeamNum() {
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
    log("Team: Borrando el controlador de jugadores...", LOG_INFO);
    delete(this->playerMovement);
    log("Team: Borrando la formacion...", LOG_INFO);
    delete(this->formation);
    log("Team: Jugadores borrados. Memoria liberada.", LOG_INFO);
}

void Team::setFormacion(int formacion) {
    this->formacion = formacion;
    this->formation->setFormation(formacion);
    this->playerMovement->setFormation(formacion);
    this->playerMovement->setSide(this->local == 0);
    for (Player* player : players) {
        player->setFieldPosition(formacion);
        player->setMovement(this->playerMovement);
    }
}

void Team::order() {
    log("Team: Ordenando equipo...", LOG_DEBUG);
    int i = 0;
    for (Player* p : players) {
        Coordinates* startingCoordinates = this->formation->getCoordinatesForPlayerBehindMiddle(
            p->getId(),
            this->local == 0
        );
        Coordinates* baseCoordinates = this->formation->getCoordinatesForPlayer(
            p->getId(),
            this->local == 0
        );
        p->setPosition(startingCoordinates);
        p->setBasePosition(baseCoordinates);
        delete(startingCoordinates);
        delete(baseCoordinates);
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

std::string Team::getDebugLines() {
    std::string message = "";
    if (this->playerMovement != NULL) {
        message = "debug: " + this->playerMovement->getDebugLines() + "\n";
    }
    return message;
}
