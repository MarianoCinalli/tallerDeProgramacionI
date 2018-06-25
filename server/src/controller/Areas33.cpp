#include "controller/Areas33.h"

Areas33::Areas33() {
    log("Areas33: Inicializando...", LOG_INFO);
    this->areasForPlayersRightSide = std::map<int, Rectangle*>();
    this->areasForPlayersLeftSide = std::map<int, Rectangle*>();
    this->loadAreas();
}

void Areas33::loadAreas() {
    log("Areas33: Cargando areas...", LOG_DEBUG);
    this->loadGoalkeeperArea();
    this->loadDefendersAreas();
    this->loadMidFildersAreas();
    this->loadAtackersAreas();
}

void Areas33::loadDefendersAreas() {
    log("Areas33: Cargando areas, del lado izquierdo, para los defensores...", LOG_DEBUG);
    this->areasForPlayersLeftSide[2] = new Rectangle(85, 21, 715, 321);
    this->areasForPlayersLeftSide[3] = new Rectangle(111, 342, 689, 320);
    this->areasForPlayersLeftSide[4] = new Rectangle(85, 661, 715, 319);

    log("Areas33: Cargando areas, del lado derecho, para los defensores...", LOG_DEBUG);
    this->areasForPlayersRightSide[2] = new Rectangle(800, 21, 715, 321);
    this->areasForPlayersRightSide[3] = new Rectangle(800, 342, 689, 320);
    this->areasForPlayersRightSide[4] = new Rectangle(800, 661, 715, 321);
}

void Areas33::loadMidFildersAreas() {
    log("Areas33: Esta formacion no tiene areas para mediocampistas, continuando...", LOG_DEBUG);
}

void Areas33::loadAtackersAreas() {
    log("Areas33: Cargando areas, del lado izquierdo, para los atacantes...", LOG_DEBUG);
    this->areasForPlayersLeftSide[5] = new Rectangle(800, 21, 715, 321);
    this->areasForPlayersLeftSide[6] = new Rectangle(800, 661, 715, 321);
    this->areasForPlayersLeftSide[7] = new Rectangle(800, 342, 689, 320);

    log("Areas33: Cargando areas, del lado derecho, para los atacantes...", LOG_DEBUG);
    this->areasForPlayersRightSide[5] = new Rectangle(85, 21, 715, 321);
    this->areasForPlayersRightSide[6] = new Rectangle(85, 661, 715, 319);
    this->areasForPlayersRightSide[7] = new Rectangle(111, 342, 689, 320);
}

Areas33::~Areas33() {
    log("Areas33: Liberando memoria...", LOG_INFO);
}
