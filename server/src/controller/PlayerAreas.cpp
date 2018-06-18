#include "controller/PlayerAreas.h"

PlayerAreas::PlayerAreas() {
    log("PlayerAreas: Inicializando las areas...", LOG_INFO);
    log("PlayerAreas: Formacion no especificada. Creando areas default.", LOG_DEBUG);
    this->areas = new Areas();
}

PlayerAreas::PlayerAreas(int formation) {
    log("PlayerAreas: Inicializando las areas...", LOG_INFO);
    if (formation == 312) {
        log("PlayerAreas: Formacion 312. Creando areas...", LOG_DEBUG);
        this->areas = new Areas312();
    } else if (formation == 321) {
        log("PlayerAreas: Formacion 321. Creando areas...", LOG_DEBUG);
        this->areas = new Areas321();
    } else if (formation == 33) {
        log("PlayerAreas: Formacion 33. Creando areas...", LOG_DEBUG);
        this->areas = new Areas33();
    } else {
        log("PlayerAreas: No se pudo crear areas. Formacion desconocida: ", formation, LOG_ERROR);
        this->areas = new Areas();
    }
}

Rectangle* PlayerAreas::getForPlayer(int playerNumber) {
    if (this->areas == NULL) {
        log("PlayerAreas: Las areas son nulas. ", LOG_ERROR);
        return NULL;
    }
    return this->areas->get(playerNumber);
}

PlayerAreas::~PlayerAreas() {
    log("PlayerAreas: Liberando memoria. Borrando areas...", LOG_INFO);
    delete(this->areas);
}
