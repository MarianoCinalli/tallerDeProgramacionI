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
    this->areasForPlayersLeftSide[2] = new Rectangle(225, 239, 525, 530);
    this->areasForPlayersLeftSide[3] = new Rectangle(190, 22, 590, 150);
    this->areasForPlayersLeftSide[4] = new Rectangle(190, 820, 580, 147);

    log("Areas33: Cargando areas, del lado derecho, para los defensores...", LOG_DEBUG);
    this->areasForPlayersRightSide[2] = new Rectangle(822, 239, 545, 510);
    this->areasForPlayersRightSide[3] = new Rectangle(870, 830, 490, 140);
    this->areasForPlayersRightSide[4] = new Rectangle(855, 24, 500, 144);
}

void Areas33::loadMidFildersAreas() {
    log("Areas33: Esta formacion no tiene areas para mediocampistas, continuando...", LOG_DEBUG);
}

void Areas33::loadAtackersAreas() {
    log("Areas33: Cargando areas, del lado izquierdo, para los atacantes...", LOG_DEBUG);
    this->areasForPlayersLeftSide[5] = new Rectangle(840, 240, 540, 528);
    this->areasForPlayersLeftSide[6] = new Rectangle(840, 834, 520, 148);
    this->areasForPlayersLeftSide[7] = new Rectangle(822, 24, 530, 164);

    log("Areas33: Cargando areas, del lado derecho, para los atacantes...", LOG_DEBUG);
    this->areasForPlayersRightSide[5] = new Rectangle(230, 233, 530, 530);
    this->areasForPlayersRightSide[6] = new Rectangle(190, 23, 600, 145);
    this->areasForPlayersRightSide[7] = new Rectangle(180, 820, 610, 148);
}

Areas33::~Areas33() {
    log("Areas33: Liberando memoria...", LOG_INFO);
}
