#include "controller/GameControllerProxy.h"

extern std::mutex update_model_mutex;

GameControllerProxy::GameControllerProxy(GameController* gameController) {
    log("GameControllerProxy: Creando gameControllerProxy...", LOG_INFO);
    this->gameController = gameController;
}

void GameControllerProxy::execute(Action* action, int team) {
    update_model_mutex.lock();
    this->gameController->execute(action, team);
    update_model_mutex.unlock();
}

void GameControllerProxy::updateModel(Camera* camera) {
    update_model_mutex.lock();
    this->gameController->updatePlayers();
    this->gameController->updateBall();
    this->gameController->updateCameraPosition(camera);
    update_model_mutex.unlock();
}

bool GameControllerProxy::shouldGameEnd() {
    // Esto no es necesario bloquearlo.
    return this->gameController->shouldGameEnd();
}

GameControllerProxy::~GameControllerProxy() {
    log("GameControllerProxy: Eliminando gameControllerProxy...", LOG_INFO);
}
