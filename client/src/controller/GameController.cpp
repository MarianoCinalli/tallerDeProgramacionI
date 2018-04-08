#include "controller/GameController.h"

GameController::GameController(Pitch* pitch) {
}

void GameController::execute(Action* action) {
    action->execute(this->activePlayer);
}

void GameController::updatePlayers() {
    // Por ahora es lo unico que necesitamos
    // porque solo se mueve un jugador.
    activePlayer->updatePosition();
}

GameController::~GameController() {
    delete(this->pitch);
}
