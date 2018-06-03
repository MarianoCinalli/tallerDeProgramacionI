#include "controller/GameControllerProxy.h"

extern std::mutex update_model_mutex;

GameControllerProxy::GameControllerProxy(GameController* gameController) {
    log("GameControllerProxy: Creando gameControllerProxy...", LOG_INFO);
    this->gameController = gameController;
}
// 
// void GameControllerProxy::addUser(std::string user, int teamNum){
//   this->gameController->addUser(user, teamNum);
// }

void GameControllerProxy::removeUser(std::string user){
  update_model_mutex.lock();
  this->gameController->removeUser(user);
  update_model_mutex.unlock();
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
    this->gameController->startGame();
}

std::string GameControllerProxy::getTeamStats(int numberTeam) {
  return this->gameController->getTeamStats(numberTeam);
}

bool GameControllerProxy::hasGameStarted() {
    update_model_mutex.lock();
    return this->gameController->hasGameStarted();
    update_model_mutex.unlock();
}

bool GameControllerProxy::joinTeam(std::string playerName, int team, int formation, int maxPlayers, std::string &errorMessage) {
    update_model_mutex.lock();
    bool result = this->gameController->joinTeam(playerName, team, formation,maxPlayers, errorMessage);
    update_model_mutex.unlock();
    return result;
}

std::string GameControllerProxy::getMessageToBroadcast(bool allPlayers) {
    // No hace falta bloquear.
    return this->gameController->getMessageToBroadcast(allPlayers);
}


GameControllerProxy::~GameControllerProxy() {
    log("GameControllerProxy: Eliminando gameControllerProxy...", LOG_INFO);
}
