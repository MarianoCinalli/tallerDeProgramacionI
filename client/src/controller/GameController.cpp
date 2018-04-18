#include "controller/GameController.h"

GameController::GameController(Pitch* pitch) {
    log("ActionsManager: Creando gameController...", LOG_INFO);
    this->pitch = pitch;
    this->activePlayer = this->pitch->activePlayer;
    log("ActionsManager: GameController creado.", LOG_INFO);
}

void GameController::execute(Action* action) {
    if(action->valid(this->activePlayer)){
      action->execute(this->activePlayer);
    } else if (action->valid(this->pitch)){
      action->execute(this->pitch);
    }

}

void GameController::updatePlayers() {
    // Por ahora es lo unico que necesitamos
    // porque solo se mueve un jugador.
    this->activePlayer = this->pitch->activePlayer;
    // Actualizar la posicion de todos los jugadores
    std::list<Player*> players = this->pitch->getLocalTeam()->getPlayers();
    if (!players.empty()){
        for (Player* p : players) {
            p->updatePosition();
        }
    }
}

// Cuando el jugador pise el borde mueve la camara.
// En este punto las coordenadas de el jugador son validas.
void GameController::updateCameraPosition(Camera* camera) {
    Coordinates* playerPosition = this->activePlayer->getPosition();
    int playerSpeed = this->activePlayer->getCurrentSpeed();
    camera->calculateNewPostion(playerPosition, playerSpeed);
}

GameController::~GameController() {
    log("ActionsManager: Liberando memoria. Borrando cancha...", LOG_INFO);
    delete(this->pitch);
    log("ActionsManager: Cancha borrada.", LOG_INFO);
}
