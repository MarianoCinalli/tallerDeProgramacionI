#include "controller/Areas312.h"

Areas312::Areas312() {
    log("Areas312: Inicializando...", LOG_INFO);
    this->areasForPlayersLeftSide = std::map<int, Rectangle*>();
    this->areasForPlayersRightSide = std::map<int, Rectangle*>();
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
    log("Areas312: Cargando areas, del lado izquierdo, para los defensores...", LOG_DEBUG);
    this->areasForPlayersLeftSide[2] = new Rectangle(206, 202, 302, 608);
    this->areasForPlayersLeftSide[3] = new Rectangle(88, 24, 1428, 133);
    this->areasForPlayersLeftSide[4] = new Rectangle(88, 844, 1428, 131);

    log("Areas312: Cargando areas, del lado derecho, para los defensores...", LOG_DEBUG);
    this->areasForPlayersRightSide[2] = new Rectangle(1078, 202, 302, 608);
    this->areasForPlayersRightSide[3] = new Rectangle(88, 24, 1428, 132);
    this->areasForPlayersRightSide[4] = new Rectangle(88, 844, 1428, 132);
}

void Areas312::loadMidFildersAreas() {
    log("Areas312: Cargando areas, del lado izquierdo, para los mediocampistas...", LOG_DEBUG);
    this->areasForPlayersLeftSide[5] = new Rectangle(551, 202, 486, 608);

    log("Areas312: Cargando areas, del lado derecho, para los mediocampistas...", LOG_DEBUG);
    this->areasForPlayersRightSide[5] = new Rectangle(551, 202, 486, 608);
}

void Areas312::loadAtackersAreas() {
    log("Areas312: Cargando areas, del lado izquierdo, para los atacantes...", LOG_DEBUG);
    this->areasForPlayersLeftSide[6] = new Rectangle(1078, 203, 302, 288);
    this->areasForPlayersLeftSide[7] = new Rectangle(1078, 500, 302, 303);

    log("Areas312: Cargando areas, del lado derecho, para los atacantes...", LOG_DEBUG);
    this->areasForPlayersRightSide[6] = new Rectangle(206, 205, 302, 285);
    this->areasForPlayersRightSide[7] = new Rectangle(206, 297, 302, 306);
}

Areas312::~Areas312() {
    log("Areas312: Liberando memoria...", LOG_INFO);
}
