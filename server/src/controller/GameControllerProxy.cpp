#include "controller/GameControllerProxy.h"

GameControllerProxy::GameControllerProxy(GameController* gameController) {
    log("GameControllerProxy: Creando gameControllerProxy...", LOG_INFO);
    this->gameController = gameController;
}

void GameControllerProxy::execute(Action* action, int team) {

}

GameControllerProxy::~GameControllerProxy() {
    log("GameControllerProxy: Eliminando gameControllerProxy...", LOG_INFO);
}
