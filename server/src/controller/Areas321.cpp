#include "controller/Areas321.h"

Areas321::Areas321(): Areas()  {
    log("Areas321: Inicializando...", LOG_INFO);
}

void Areas321::loadAreas() {
    log("Areas321: Cargando areas...", LOG_DEBUG);
    this->loadGoalkeeperArea();
    this->loadDefendersAreas();
    this->loadMidFildersAreas();
    this->loadAtackersAreas();
}

void Areas321::loadDefendersAreas() {
    log("Areas321: Cargando areas para los defensores...", LOG_DEBUG);
    this->areasForPlayers[2] = new Rectangle(84, 22, 1430, 214);
    this->areasForPlayers[3] = new Rectangle(84, 766, 1430, 214);
    this->areasForPlayers[4] = new Rectangle(124, 236, 344, 531);
}

void Areas321::loadMidFildersAreas() {
    log("Areas321: Cargando areas para los mediocampistas...", LOG_DEBUG);
    this->areasForPlayers[5] = new Rectangle(468, 236, 682, 265);
    this->areasForPlayers[6] = new Rectangle(468, 500, 682, 265);
}

void Areas321::loadAtackersAreas() {
    log("Areas321: Cargando areas para los atacantes...", LOG_DEBUG);
    this->areasForPlayers[7] = new Rectangle(1150, 236, 344, 530);
}

Areas321::~Areas321() {
    log("Areas321: Liberando memoria...", LOG_INFO);
}
