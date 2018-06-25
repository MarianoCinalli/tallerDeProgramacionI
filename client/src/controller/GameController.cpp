#include "controller/GameController.h"

GameController::GameController(Pitch* pitch) {
    log("ActionsManager: Creando gameController...", LOG_INFO);
    this->pitch = pitch;
    this->activePlayer = this->pitch->activePlayer;
    this->ball = this->pitch->ball;
    list<Player*> teamPlayers = this->pitch->getTeam(0)->getPlayers();
    list<Player*> awayPlayers = this->pitch->getTeam(1)->getPlayers();
    teamPlayers.insert(teamPlayers.end(), awayPlayers.begin(), awayPlayers.end());
    int i = 1;
    for (Player* p : teamPlayers) {
      players[i] = p;
      i++;
    }
    this->clock = new Clock();
    this->score = new Score();
    this->state = GAME_START_STATE;
    this->stateOption = 0;
    log("ActionsManager: GameController creado.", LOG_INFO);
}

Player* GameController::getPlayer(int num){
  return this->players[num];
}

Ball* GameController::getBall(){
  return this->ball;
}

Camera* GameController::getCamera() {
  return this->pitch->camera;
}

Clock* GameController::getClock() {
  return this->clock;
}

Score* GameController::getScore() {
  return this->score;
}

void GameController::parseYaml(YAML::Node node){
  if (node["t"]){
    this->state = node["t"].as<int>();
  }
  if (node["op"]){
    this->stateOption = node["op"].as<int>();
}
}


GameController::~GameController() {
    log("ActionsManager: Liberando memoria. Borrando cancha...", LOG_INFO);
    delete(this->pitch);
    delete(this->clock);
    delete(this->score);
    log("ActionsManager: Cancha borrada.", LOG_INFO);
}
