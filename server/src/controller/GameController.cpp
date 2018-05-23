#include "controller/GameController.h"

GameController::GameController(Pitch* pitch) {
    log("GameController: Creando gameController...", LOG_INFO);
    this->pitch = pitch;
    this->ball = this->pitch->getBall();
    this->end = false;
    this->time = 0;    //ventana de tiempo de 1024 frames
    // std::list users1;
    this->users[0] = std::set<std::string>();
    this->users[1] = std::set<std::string>();
    log("GameController: GameController creado.", LOG_INFO);
}

void GameController::addUser(std::string user, int teamNum){
  this->users[teamNum].insert(user);
  this->pitch->setUserTeam(user,teamNum);
  this->pitch->changeActivePlayer(user);
}

void GameController::removeUser(std::string user){
  this->pitch->removeActivePlayer(user);
}

Player* GameController::getActivePlayer(std::string user) {
    return this->pitch->getActivePlayer(user);
}

void GameController::execute(Action* action, std::string user) {
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
    log("GameController: se actualizaron los jugadores.", LOG_DEBUG);
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

std::string GameController::getTeamStats(int numberTeam) {
  Team* team = this->pitch->getTeam(numberTeam);
  std::string name = team->getName();
  std::string amnt = std::to_string(users[numberTeam].size());
  return name+":"+amnt;
}

void GameController::setEnd() {
    this->end = true;
}


bool GameController::joinTeam(std::string playerName, int team, int maxPlayers) {
    log("GameController: Viendo si el usuario " + playerName + "puede unirse al equipo:", team, LOG_INFO);
    int usersInTeam = this->users[team].size();
    if (maxPlayers == 1) {
        if (usersInTeam != 0) {
            log("GameController: El usuario " + playerName + "no puede unirse al equipo:", team, LOG_INFO);
            return false;
        } else {
            log("GameController: El usuario " + playerName + "puede unirse al equipo:", team, LOG_INFO);
            this->addUser(playerName, team);
            return true;
        }
    } else {
        if (usersInTeam == (maxPlayers - 1)) {
            log("GameController: El usuario " + playerName + "no puede unirse al equipo:", team, LOG_INFO);
            return false;
        } else {
            log("GameController: El usuario " + playerName + "puede unirse al equipo:", team, LOG_INFO);
            this->addUser(playerName, team);
            return true;
        }
    }
}

GameController::~GameController() {
    log("GameController: Liberando memoria. Borrando cancha...", LOG_INFO);
    delete(this->pitch);
    log("GameController: Cancha borrada.", LOG_INFO);
}
