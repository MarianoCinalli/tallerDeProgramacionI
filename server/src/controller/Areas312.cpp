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
    this->areasForPlayersLeftSide[2] = new Rectangle(84, 766, 1437, 214);
    this->areasForPlayersLeftSide[3] = new Rectangle(124, 236, 443, 531);
    this->areasForPlayersLeftSide[4] = new Rectangle(84, 22, 1437, 214);

    log("Areas312: Cargando areas, del lado derecho, para los defensores...", LOG_DEBUG);
    this->areasForPlayersRightSide[2] = new Rectangle(88, 766, 1424, 210);
    this->areasForPlayersRightSide[3] = new Rectangle(1032, 234, 444, 530);
    this->areasForPlayersRightSide[4] = new Rectangle(90, 26, 1424, 210);
}

void Areas312::loadMidFildersAreas() {
    log("Areas312: Cargando areas, del lado izquierdo, para los mediocampistas...", LOG_DEBUG);
    this->areasForPlayersLeftSide[5] = new Rectangle(568, 236, 469, 530);

    log("Areas312: Cargando areas, del lado derecho, para los mediocampistas...", LOG_DEBUG);
    this->areasForPlayersRightSide[5] = new Rectangle(563, 234, 469, 530);
}

void Areas312::loadAtackersAreas() {
    log("Areas312: Cargando areas, del lado izquierdo, para los atacantes...", LOG_DEBUG);
    this->areasForPlayersLeftSide[6] = new Rectangle(1037, 500, 443, 265);
    this->areasForPlayersLeftSide[7] = new Rectangle(1037, 236, 443, 265);

    log("Areas312: Cargando areas, del lado derecho, para los atacantes...", LOG_DEBUG);
    this->areasForPlayersRightSide[6] = new Rectangle(119, 499, 443, 265);
    this->areasForPlayersRightSide[7] = new Rectangle(119, 236, 443, 265);
}

Areas312::~Areas312() {
    log("Areas312: Liberando memoria...", LOG_INFO);
}
