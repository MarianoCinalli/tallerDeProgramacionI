#include "controller/GameControllerProxy.h"

extern std::mutex update_model_mutex;

GameControllerProxy::GameControllerProxy(GameController* gameController) {
    log("GameControllerProxy: Creando gameControllerProxy...", LOG_INFO);
    this->gameController = gameController;
    // ESTO ES PROVISORIO!
    // Cuando manejemos tiempo borrar.
    this->hasStarted = false;
}

void GameControllerProxy::addUser(std::string user, int teamNum){
  this->gameController->addUser(user, teamNum);
}

void GameControllerProxy::execute(Action* action, std::string team) {
    update_model_mutex.lock();
    this->gameController->execute(action, team);
    update_model_mutex.unlock();
}

void GameControllerProxy::updateModel(Camera* camera) {
    update_model_mutex.lock();
    this->gameController->update(camera);
    update_model_mutex.unlock();
}

bool GameControllerProxy::shouldGameEnd() {
    // Esto no es necesario bloquearlo.
    return this->gameController->shouldGameEnd();
}

void GameControllerProxy::end(){
  this->gameController->setEnd();
}

void GameControllerProxy::startGame() {
    // ESTO ES PROVISORIO!
    // Cuando manejemos tiempo borrar, y decirle al controller que comienze a contar el reloj.
    this->hasStarted = true;
}

bool GameControllerProxy::hasGameStarted() {
    // ESTO ES PROVISORIO!
    // Cuando manejemos tiempo borrar, y preguntarle al controller.
    return this->hasStarted;
}

GameControllerProxy::~GameControllerProxy() {
    log("GameControllerProxy: Eliminando gameControllerProxy...", LOG_INFO);
}
