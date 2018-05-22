#include "controller/GameController.h"

GameController::GameController(Pitch* pitch) {
    log("ActionsManager: Creando gameController...", LOG_INFO);
    this->pitch = pitch;
    this->ball = this->pitch->getBall();
    this->end = false;
    this->time = 0;    //ventana de tiempo de 1024 frames
    log("ActionsManager: GameController creado.", LOG_INFO);
}


Player* GameController::getActivePlayer(int user) {
    return this->pitch->getActivePlayer(user);
}

void GameController::execute(Action* action, int user) {
    Player* player = this->getActivePlayer(user);
    log("user: ", user, LOG_INFO);
    if (action->valid(player)) {
        action->execute(player);
    }
    if (action->valid(this->pitch)) {
        action->execute(this->pitch, user);
    }
}

void GameController::update(Camera* camera) {
    this->updatePlayers();
    this->updateBall();
    this->updateCameraPosition(camera);
    this->count();
}

void GameController::count() {
    this->time++;
    if (this->time == 1024) {
        this->time = 0;
    }
}

void GameController::updatePlayers() {
    // Por ahora es lo unico que necesitamos
    // porque solo se mueve un jugador.
    // this->activePlayer = this->pitch->activePlayer; Este parece no se necesario.
    // Actualizar la posicion de todos los jugadores
    std::list<Player*> teamPlayers = this->pitch->getTeam(0)->getPlayers();
    std::list<Player*> awayPlayers = this->pitch->getTeam(1)->getPlayers();
    teamPlayers.insert(teamPlayers.end(), awayPlayers.begin(), awayPlayers.end());
    // std::list<Player*> players = this->pitch->getTeam(0)->getPlayers(); //TODO usuario 0
    if (!teamPlayers.empty()) {
        for (Player* p : teamPlayers) {
            p->updateState();
        }
    }
    log("ActionsManager: se actualizaron los jugadores.", LOG_INFO);
}


void GameController::updateBall() {

    if (this->ball->isDominated() && this->ball->getPlayer()->isKicking()) {
        log("La pelota fue pateada", LOG_INFO);
        this->ball->isPassed(this->ball->getPlayer()->getOrientation(), PASS_SPEED); //TODO valor de pase?
    }
    this->pitch->changeBallOwnership();
    this->ball->updatePosition();
}


// Cuando el jugador pise el borde mueve la camara.
// En este punto las coordenadas de el jugador son validas.
void GameController::updateCameraPosition(Camera* camera) {
    Coordinates* position = this->ball->getPosition();
    // Coordinates* position = this->pitch->getActivePlayer(0)->getPosition();
    // int speed = this->pitch->getActivePlayer(0)->getCurrentSpeed();
    camera->calculateNewPostion(position);
}

// Aca deberia haber una nocion del tiempo.
// Un reloj que termine el juego luego del tiempo. Proximo tp?
// Tenemos que poner algo que nos permita controlar cuando teminar.
bool GameController::shouldGameEnd() {
    // this->end++;
    // if (this->end>100000){
    // return true;
    // }
    return this->end;
}

void GameController::setEnd() {
    this->end = true;
}

GameController::~GameController() {
    log("ActionsManager: Liberando memoria. Borrando cancha...", LOG_INFO);
    delete(this->pitch);
    log("ActionsManager: Cancha borrada.", LOG_INFO);
}
