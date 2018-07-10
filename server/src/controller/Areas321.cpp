#include "controller/Areas321.h"

Areas321::Areas321() {
    log("Areas321: Inicializando...", LOG_INFO);
    this->areasForPlayersRightSide = std::map<int, Rectangle*>();
    this->areasForPlayersLeftSide = std::map<int, Rectangle*>();
    this->loadAreas();
}

void Areas321::loadAreas() {
    log("Areas321: Cargando areas...", LOG_DEBUG);
    this->loadGoalkeeperArea();
    this->loadDefendersAreas();
    this->loadMidFildersAreas();
    this->loadAtackersAreas();
}

void Areas321::loadDefendersAreas() {
    log("Areas321: Cargando areas, del lado izquierdo, para los defensores...", LOG_DEBUG);
    this->areasForPlayersLeftSide[2] = new Rectangle(206, 202, 302, 608);
    this->areasForPlayersLeftSide[3] = new Rectangle(88, 24, 1428, 128);
    this->areasForPlayersLeftSide[4] = new Rectangle(88, 844, 1428, 132);

    log("Areas321: Cargando areas, del lado derecho, para los defensores...", LOG_DEBUG);
    this->areasForPlayersRightSide[2] = new Rectangle(1078, 202, 302, 608);
    this->areasForPlayersRightSide[3] = new Rectangle(88, 24, 1428, 132);
    this->areasForPlayersRightSide[4] = new Rectangle(88, 844, 1428, 132);
}

void Areas321::loadMidFildersAreas() {
    log("Areas321: Cargando areas, del lado izquierdo, para los mediocampistas...", LOG_DEBUG);
    this->areasForPlayersLeftSide[5] = new Rectangle(551, 204, 486, 288);
    this->areasForPlayersLeftSide[6] = new Rectangle(551, 520, 486, 280);

    log("Areas321: Cargando areas, del lado derecho, para los mediocampistas...", LOG_DEBUG);
    this->areasForPlayersRightSide[5] = new Rectangle(561, 214, 462, 264);
    this->areasForPlayersRightSide[6] = new Rectangle(560, 535, 460, 260);
}

void Areas321::loadAtackersAreas() {
    log("Areas321: Cargando areas, del lado izquierdo, para los atacantes...", LOG_DEBUG);
    this->areasForPlayersLeftSide[7] = new Rectangle(1078, 202, 302, 608);

    log("Areas321: Cargando areas, del lado derecho, para los atacantes...", LOG_DEBUG);
    this->areasForPlayersRightSide[7] = new Rectangle(206, 202, 302, 608);
}

Areas321::~Areas321() {
    log("Areas321: Liberando memoria...", LOG_INFO);
}
