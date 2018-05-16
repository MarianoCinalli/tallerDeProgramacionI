#include "controller/GameController.h"

GameController::GameController(Pitch* pitch) {
    log("ActionsManager: Creando gameController...", LOG_INFO);
    this->pitch = pitch;
    this->ball = this->pitch->getBall();
    log("ActionsManager: GameController creado.", LOG_INFO);
}


Player* GameController::getActivePlayer(int user) {
    return this->pitch->getActivePlayer(user);
}

void GameController::execute(Action* action, int user) {
    Player* player = this->getActivePlayer(user);
    if (action->valid(player)) {
        action->execute(player);
    } else if (action->valid(this->pitch)) {
        action->execute(this->pitch, user);
    }
}

void GameController::updatePlayers() {
    // Por ahora es lo unico que necesitamos
    // porque solo se mueve un jugador.
    // this->activePlayer = this->pitch->activePlayer; Este parece no se necesario.
    // Actualizar la posicion de todos los jugadores
    std::list<Player*> players = this->pitch->getTeam(0)->getPlayers(); //TODO usuario 0
    if (!players.empty()) {
        for (Player* p : players) {
            p->updatePosition();
        }
    }
    log("ActionsManager: se actualizaron los jugadores.", LOG_INFO);
}


void GameController::updateBall() {

    if (this->ball->isDominated() && this->ball->getPlayer()->isKicking()) {
        log("La pelota fue pateada", LOG_INFO);
        this->ball->isPassed(this->ball->getPlayer()->getOrientation(), 15); //TODO valor de pase?
    }
    this->pitch->changeBallOwnership();
    this->ball->updatePosition();
}


// Cuando el jugador pise el borde mueve la camara.
// En este punto las coordenadas de el jugador son validas.
void GameController::updateCameraPosition(Camera* camera) {
    Coordinates* position = this->ball->getPosition();
    int speed = this->ball->getCurrentSpeed();
    camera->calculateNewPostion(position, speed);
}

// Aca deberia haber una nocion del tiempo.
// Un reloj que termine el juego luego del tiempo. Proximo tp?
// Tenemos que poner algo que nos permita controlar cuando teminar.
bool GameController::shouldGameEnd() {
    return true;
}

GameController::~GameController() {
    log("ActionsManager: Liberando memoria. Borrando cancha...", LOG_INFO);
    delete(this->pitch);
    log("ActionsManager: Cancha borrada.", LOG_INFO);
}
