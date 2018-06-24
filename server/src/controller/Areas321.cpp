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
    this->areasForPlayersLeftSide[2] = new Rectangle(84, 22, 1430, 214);
    this->areasForPlayersLeftSide[3] = new Rectangle(84, 766, 1430, 214);
    this->areasForPlayersLeftSide[4] = new Rectangle(124, 236, 344, 531);

    log("Areas321: Cargando areas, del lado derecho, para los defensores...", LOG_DEBUG);
    this->areasForPlayersRightSide[2] = new Rectangle(84, 22, 1430, 214);
    this->areasForPlayersRightSide[3] = new Rectangle(84, 766, 1430, 214);
    this->areasForPlayersRightSide[4] = new Rectangle(1132, 236, 344, 531);
}

void Areas321::loadMidFildersAreas() {
    log("Areas321: Cargando areas, del lado izquierdo, para los mediocampistas...", LOG_DEBUG);
    this->areasForPlayersLeftSide[5] = new Rectangle(468, 236, 682, 265);
    this->areasForPlayersLeftSide[6] = new Rectangle(468, 500, 682, 265);

    log("Areas321: Cargando areas, del lado derecho, para los mediocampistas...", LOG_DEBUG);
    this->areasForPlayersRightSide[5] = new Rectangle(450, 236, 682, 265);
    this->areasForPlayersRightSide[6] = new Rectangle(450, 500, 682, 265);
}

void Areas321::loadAtackersAreas() {
    log("Areas321: Cargando areas, del lado izquierdo, para los atacantes...", LOG_DEBUG);
    this->areasForPlayersLeftSide[7] = new Rectangle(1150, 236, 344, 530);

    log("Areas321: Cargando areas, del lado derecho, para los atacantes...", LOG_DEBUG);
    this->areasForPlayersRightSide[7] = new Rectangle(106, 236, 344, 530);
}

Areas321::~Areas321() {
    log("Areas321: Liberando memoria...", LOG_INFO);
}
