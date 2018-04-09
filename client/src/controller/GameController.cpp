#include "controller/GameController.h"

GameController::GameController(Pitch* pitch) {
    log("ActionsManager: Creando gameController...", LOG_INFO);
    this->pitch = pitch;
    this->activePlayer = this->pitch->activePlayer();
    log("ActionsManager: GameController creado.", LOG_INFO);
}

void GameController::execute(Action* action) {
    action->execute(this->activePlayer);
}

void GameController::updatePlayers() {
    // Por ahora es lo unico que necesitamos
    // porque solo se mueve un jugador.
    activePlayer->updatePosition();
    // TODO: Validar posiciones de los jugadores.
}

// Cuando el jugador pise el borde mueve la camara.
// En este punto las coordenadas de el jugador son validas.
void GameController::updateCameraPosition(Camera* camera) {
    Coordinates* playerPosition = this->activePlayer->getPosition();
    camera->calculateNewPostion(playerPosition);
}

GameController::~GameController() {
    log("ActionsManager: Liberando memoria. Borrando cancha...", LOG_INFO);
    delete(this->pitch);
    log("ActionsManager: Cancha borrada.", LOG_INFO);
}
