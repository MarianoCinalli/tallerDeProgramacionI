#include "controller/Areas.h"

Areas::Areas() {
    log("Areas: Inicializando...", LOG_INFO);
    this->areasForPlayers = std::map<int, Rectangle*>();
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
    this->areasForPlayers[1] = new Rectangle(85, 434, 40, 134);
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
    this->areasForPlayers[position] = new Rectangle(0, 0, LEVEL_WIDTH, LEVEL_HEIGHT);
}

Rectangle* Areas::get(int playerNumber) {
    // Povisorio. Por ahora soporto un solo equipo.
    int number = 0;
    if (playerNumber > 7) {
        number = playerNumber - 7;
    } else {
        number = playerNumber;
    }
    // --------------------------------------------
    Rectangle* area = NULL;
    auto search = this->areasForPlayers.find(number);
    if(search != this->areasForPlayers.end()) {
        area = this->areasForPlayers[number];
    } else {
        log("Areas: No se encontro un area para el jugador: ", number, LOG_ERROR);
        log("Areas: Jugadores con areas: ", this->getPlayerWithAreasAsString(), LOG_DEBUG);
    }
    return area;
}

// Metodo para debugear.
std::string Areas::getPlayerWithAreasAsString() {
    std::string areasForPlayersString = "";
    for (auto const& areaForPlayer : this->areasForPlayers) {
        areasForPlayersString += areaForPlayer.first + " - ";
    }
    return areasForPlayersString;
}

std::string Areas::getDebugLines() {
    std::string message = "[";
    bool first = true;
    for (auto const& areaForPlayer : this->areasForPlayers) {
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
    for (auto const& areaForPlayer : this->areasForPlayers) {
        log("Areas: Borrando area para jugador: ", areaForPlayer.first, LOG_DEBUG);
        delete(areaForPlayer.second);
    }
    log("Areas: Memoria liberada.", LOG_INFO);
}
