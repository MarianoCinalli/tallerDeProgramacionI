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
    log("ActionsManager: GameController creado.", LOG_INFO);
}


void GameController::execute(Action* action) {
    if(action->valid(this->activePlayer)){
      action->execute(this->activePlayer);
    }
    if (action->valid(this->pitch)){
      action->execute(this->pitch);
    }
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
    log("ActionsManager: se actualizaron los jugadores.", LOG_INFO);
}

void GameController::updateBall() {

  if(this->ball->isDominated() && this->ball->getPlayer()->isKicking()){
    log("La pelota fue pateada", LOG_INFO);
    this->ball->isPassed(this->ball->getPlayer()->getOrientation(), 15); //TODO valor de pase?
  }
  this->pitch->changeBallOwnership();
  this->ball->updatePosition();
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
