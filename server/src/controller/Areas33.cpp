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
    this->areasForPlayersLeftSide[2] = new Rectangle(215, 239, 535, 530);
    this->areasForPlayersLeftSide[3] = new Rectangle(84, 22, 689, 155);
    this->areasForPlayersLeftSide[4] = new Rectangle(84, 820, 600, 153);

    log("Areas33: Cargando areas, del lado derecho, para los defensores...", LOG_DEBUG);
    this->areasForPlayersRightSide[2] = new Rectangle(822, 239, 567, 510);
    this->areasForPlayersRightSide[3] = new Rectangle(870, 830, 650, 140);
    this->areasForPlayersRightSide[4] = new Rectangle(855, 24, 606, 144);
}

void Areas33::loadMidFildersAreas() {
    log("Areas33: Esta formacion no tiene areas para mediocampistas, continuando...", LOG_DEBUG);
}

void Areas33::loadAtackersAreas() {
    log("Areas33: Cargando areas, del lado izquierdo, para los atacantes...", LOG_DEBUG);
    this->areasForPlayersLeftSide[5] = new Rectangle(832, 240, 570, 528);
    this->areasForPlayersLeftSide[6] = new Rectangle(840, 834, 684, 148);
    this->areasForPlayersLeftSide[7] = new Rectangle(822, 24, 692, 164);

    log("Areas33: Cargando areas, del lado derecho, para los atacantes...", LOG_DEBUG);
    this->areasForPlayersRightSide[5] = new Rectangle(220, 233, 550, 530);
    this->areasForPlayersRightSide[6] = new Rectangle(84, 23, 710, 145);
    this->areasForPlayersRightSide[7] = new Rectangle(84, 820, 688, 148);
}

Areas33::~Areas33() {
    log("Areas33: Liberando memoria...", LOG_INFO);
}
