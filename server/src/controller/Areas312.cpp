#include "controller/Areas312.h"

Areas312::Areas312() {
    log("Areas312: Inicializando...", LOG_INFO);
    this->areasForPlayers = std::map<int, Rectangle*>();
    this->loadAreas();
}

void Areas312::loadAreas() {
    log("Areas312: Cargando areas...", LOG_DEBUG);
    this->loadGoalkeeperArea();
    this->loadDefendersAreas();
    this->loadMidFildersAreas();
    this->loadAtackersAreas();
}

void Areas312::loadDefendersAreas() {
    log("Areas312: Cargando areas para los defensores...", LOG_DEBUG);
    this->areasForPlayers[2] = new Rectangle(84, 766, 1437, 214);
    this->areasForPlayers[3] = new Rectangle(124, 236, 443, 531);
    this->areasForPlayers[4] = new Rectangle(84, 22, 1437, 214);
}

void Areas312::loadMidFildersAreas() {
    log("Areas312: Cargando areas para los mediocampistas...", LOG_DEBUG);
    this->areasForPlayers[5] = new Rectangle(568, 236, 469, 530);
}

void Areas312::loadAtackersAreas() {
    log("Areas312: Cargando areas para los atacantes...", LOG_DEBUG);
    this->areasForPlayers[6] = new Rectangle(1037, 500, 443, 265);
    this->areasForPlayers[7] = new Rectangle(1037, 236, 443, 265);
}

Areas312::~Areas312() {
    log("Areas312: Liberando memoria...", LOG_INFO);
}
