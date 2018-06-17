#include "controller/Areas.h"

Areas::Areas() {
    log("Areas: Inicializando...", LOG_INFO);
    this->areasForPlayers = std::map<int, Rectangle*>();
    this->loadAreas();
}

void Areas::loadAreas() {
    log("Areas: Cargando areas...", LOG_DEBUG);
    this->loadGoalkeeperArea();
    this->loadDefendersAreas();
    this->loadMidFildersAreas();
    this->loadAtackersAreas();
}

void Areas::loadGoalkeeperArea() {
    log("Areas: Cargando areas para el arquero...", LOG_DEBUG);
    this->insertUnrestricted(1);
}

void Areas::loadDefendersAreas() {
    log("Areas: Cargando areas para los defensores...", LOG_DEBUG);
    this->insertUnrestricted(2);
    this->insertUnrestricted(3);
    this->insertUnrestricted(4);
}

void Areas::loadMidFildersAreas() {
    log("Areas: Cargando areas para los mediocampistas...", LOG_DEBUG);
    this->insertUnrestricted(5);
    this->insertUnrestricted(6);
}

void Areas::loadAtackersAreas() {
    log("Areas: Cargando areas para los atacantes...", LOG_DEBUG);
    this->insertUnrestricted(7);  
}

void Areas::insertUnrestricted(int position) {
    this->areasForPlayers[position] = new Rectangle(0, 0, LEVEL_WIDTH, LEVEL_HEIGHT);
}

Rectangle* Areas::get(int playerNumber) {
    return this->areasForPlayers[playerNumber];
}

Areas::~Areas() {
    log("Areas: Liberando memoria...", LOG_INFO);
    for (auto const& areaForPlayer : this->areasForPlayers) {
        log("Areas: Borrando area para jugador: ", areaForPlayer.first, LOG_DEBUG);
        delete(areaForPlayer.second);
    }
    log("Areas: Memoria liberada.", LOG_INFO);
}
