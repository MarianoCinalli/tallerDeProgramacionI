#include "controller/Areas.h"


Areas::Areas() {
    log("Areas: Inicializando...", LOG_INFO);
    this->areasForPlayersLeftSide = std::map<int, Rectangle*>();
    this->areasForPlayersRightSide = std::map<int, Rectangle*>();
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
    this->areasForPlayersLeftSide[1] = new Rectangle(85, 434, 40, 134);
    this->areasForPlayersRightSide[1] = new Rectangle(1476, 434, 40, 134);
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
    this->areasForPlayersLeftSide[position] = new Rectangle(0, 0, LEVEL_WIDTH, LEVEL_HEIGHT);
    this->areasForPlayersRightSide[position] = new Rectangle(0, 0, LEVEL_WIDTH, LEVEL_HEIGHT);
}

Rectangle* Areas::get(int playerNumber, bool isLeftsideTeam) {
    // Povisorio. Por ahora soporto un solo equipo.
    int number = 0;
    if (playerNumber > 7) {
        number = playerNumber - 7;
    } else {
        number = playerNumber;
    }
    // --------------------------------------------
    Rectangle* area = NULL;
    if (isLeftsideTeam) {
        auto search = this->areasForPlayersLeftSide.find(number);
        if(search != this->areasForPlayersLeftSide.end()) {
            area = this->areasForPlayersLeftSide[number];
        } else {
            log("Areas: No se encontro un area para el jugador: ", number, LOG_ERROR);
            log("Areas: Jugadores con areas: ", this->getPlayerWithAreasAsString(isLeftsideTeam), LOG_DEBUG);
        }
    } else {
        auto search = this->areasForPlayersRightSide.find(number);
        if(search != this->areasForPlayersRightSide.end()) {
            area = this->areasForPlayersRightSide[number];
        } else {
            log("Areas: No se encontro un area para el jugador: ", number, LOG_ERROR);
            log("Areas: Jugadores con areas: ", this->getPlayerWithAreasAsString(isLeftsideTeam), LOG_DEBUG);
        }
    }
    return area;
}

// Metodo para debugear.
std::string Areas::getPlayerWithAreasAsString(bool isLeftsideTeam) {
    std::string areasForPlayersString = "";
    if (isLeftsideTeam) {
        for (auto const& areaForPlayer : this->areasForPlayersLeftSide) {
            areasForPlayersString += areaForPlayer.first + " - ";
        }
    } else {
        for (auto const& areaForPlayer : this->areasForPlayersRightSide) {
            areasForPlayersString += areaForPlayer.first + " - ";
        }
    }
    return areasForPlayersString;
}

std::string Areas::getDebugLines() {
    std::string message = "[";
    bool first = true;
    for (auto const& areaForPlayer : this->areasForPlayersLeftSide) {
        if (first) {
            first = false;
        } else {
            message += ", ";
        }
        message += "[" + (areaForPlayer.second)->getAsString() + "]";
    }
    message += "]";
    return message;
}

Areas::~Areas() {
    log("Areas: Liberando memoria...", LOG_INFO);
    log("Areas: Borrando areas para jugadores en lado izquierdo...", LOG_INFO);
    for (auto const& areaForPlayerLeftSide : this->areasForPlayersLeftSide) {
        log("Areas: Borrando area para jugador: ", areaForPlayerLeftSide.first, LOG_DEBUG);
        delete(areaForPlayerLeftSide.second);
    }
    log("Areas: Borrando areas para jugadores en lado derecho...", LOG_INFO);
    for (auto const& areaForPlayerRightSide : this->areasForPlayersRightSide) {
        log("Areas: Borrando area para jugador: ", areaForPlayerRightSide.first, LOG_DEBUG);
        delete(areaForPlayerRightSide.second);
    }
    log("Areas: Memoria liberada.", LOG_INFO);
}
