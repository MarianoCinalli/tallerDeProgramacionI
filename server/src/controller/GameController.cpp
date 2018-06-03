#include "controller/GameController.h"

GameController::GameController(Pitch* pitch) {
    log("GameController: Creando gameController...", LOG_INFO);
    this->pitch = pitch;
    this->ball = this->pitch->getBall();
    this->end = false;
    this->time = 0;    //ventana de tiempo de 1024 frames
    // std::list users1;
    this->timer = new Timer();
    this->users[0] = std::set<std::string>();
    this->users[1] = std::set<std::string>();
    log("GameController: GameController creado.", LOG_INFO);
}

void GameController::addUser(std::string user, int teamNum, int formation) {
    log("GameController: Agregando usuario...", LOG_INFO);
    this->users[teamNum].insert(user);
    log("Pitch: Agregando usuario al equipo...", user, LOG_DEBUG);
    this->pitch->setUserTeam(user, teamNum, formation);
    log("Pitch: Cambiando jugador...", user, LOG_DEBUG);
    this->pitch->changeActivePlayer(user);
    log("GameController: Usuario agregado.", LOG_INFO);
}


void GameController::removeUser(std::string user) {
    log("GameController: Removiendo usuario...", LOG_INFO);
    this->removeUserFromTeam(user);
    this->pitch->removeActivePlayer(user);
    log("GameController: Usuario removido.", LOG_INFO);
}

// Recorre el map, para cada team busca al jugador.
// Si lo encuentra lo borra.
void GameController::removeUserFromTeam(std::string user) {
    log("GameController: Original: ", this->getUsersWithTeamAsString(), LOG_DEBUG);
    log("GameController: Buscando al jugador:", user, LOG_DEBUG);
    bool errased = false;
    log("GameController: En el equipo local...", LOG_DEBUG);
    for (auto it = this->users[0].begin(); it != this->users[0].end();) {
        if (*it == user) {
            it = this->users[0].erase(it);
            errased = true;
        } else {
            ++it;
        }
    }
    log("GameController: En el equipo visitante...", LOG_DEBUG);
    for (auto it = this->users[1].begin(); it != this->users[1].end();) {
        if (*it == user) {
            it = this->users[1].erase(it);
            errased = true;
        } else {
            ++it;
        }
    }
    if (errased) {
        log("GameController: Jugador removido: ", user, LOG_DEBUG);
        log("GameController: Despues: ", this->getUsersWithTeamAsString(), LOG_DEBUG);
    } else {
        log("GameController: No se encontro el jugador, dentro de los equipos", user, LOG_ERROR);
        log("GameController: Despues: ", this->getUsersWithTeamAsString(), LOG_DEBUG);
    }
}

// Metodo para debugear.
std::string GameController::getUsersWithTeamAsString() {
    std::string usersWithActivePlayers = "";
    for (auto it = this->users[0].begin(); it != this->users[0].end(); ++it) {
        usersWithActivePlayers += "E:0 J:" + *it + " - ";
    }
    for (auto it = this->users[1].begin(); it != this->users[1].end(); ++it) {
        usersWithActivePlayers += "E:1 J:" + *it + " - ";
    }
    return usersWithActivePlayers;
}

Player* GameController::getActivePlayer(std::string user) {
    return this->pitch->getActivePlayer(user);
}

void GameController::execute(Action* action, std::string user) {
    Player* player = this->getActivePlayer(user);
    if (action->valid(player)) {
        action->execute(player);
    }
    if (action->valid(this->pitch)) {
        action->execute(this->pitch, user);
    }
}

void GameController::update(Camera* camera) {
    // Dejo el tiempo pasado, por si se quiere usar en los updates.
    Time* elapsedTime = this->timer->getTime();
    this->updatePlayers();
    this->updateBall();
    this->updateCameraPosition(camera);
    this->count();
    delete(elapsedTime);
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
    log("GameController: se actualizaron los jugadores.", LOG_SPAM);
}


void GameController::updateBall() {
    if (this->ball->isDominated() && this->ball->getPlayer()->isKicking()) {
        log("GameController: La pelota fue pateada.", LOG_DEBUG);
        Player* player = this->ball->getPlayer();
        this->ball->isPassed(player->getOrientation(), player->getKickPower()*PASS_SPEED); //TODO valor de pase?
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
    camera->calculateNewPosition(position);
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
    return name + ":" + amnt;
}

void GameController::setEnd() {
    log("GameController: Seteando que el juego termine...", LOG_INFO);
    this->end = true;
    log("GameController: Terminacion de juego seteada.", LOG_INFO);
}


bool GameController::joinTeam(std::string playerName, int team, int formation, int maxPlayers, std::string& errorMessage) {
    log("GameController: Viendo si el usuario " + playerName + " puede unirse al equipo: ", team, LOG_INFO);
    int usersInTeam = this->users[team].size();
    if (maxPlayers == 1) {
        if (usersInTeam != 0) {
            log("GameController: Equipo lleno. El usuario " + playerName + " no puede unirse al equipo: ", team, LOG_INFO);
            errorMessage = "noRoom";
            return false;
        } else {
            log("GameController: El usuario " + playerName + " puede unirse al equipo: ", team, LOG_INFO);
            this->addUser(playerName, team, formation);
            return true;
        }
    } else {
        if (usersInTeam == (maxPlayers - 1)) {
            log("GameController: Equipo lleno. El usuario " + playerName + " no puede unirse al equipo: ", team, LOG_INFO);
            errorMessage = "noRoom";
            return false;
        } else {
            log("GameController: El usuario " + playerName + " puede unirse al equipo: ", team, LOG_INFO);
            this->addUser(playerName, team, formation);
            return true;
        }
    }
}

void GameController::startGame() {
    this->timer->start();
}

bool GameController::hasGameStarted() {
    return this->timer->hasStarted();
}

std::string GameController::getMessageToBroadcast(bool allPlayers) {
    std::string message = "";
    std::list<Player*> players = {};
    if (allPlayers) {
        players = this->pitch->getPlayers();
    } else {
        players = this->pitch->getPlayersInsideCamera();
    }
    Ball* ball = this->pitch->getBall();
    Camera* camera = this->pitch->getCamera();
    if (ball == NULL) {
        log("Broadcaster: La pelota es null!", LOG_ERROR);
        return "";
    }
    for (Player* player : players) {
        message += player->getAsYaml();
    }
    message += ball->getAsYaml();
    message += camera->getAsYaml();
    return message + ";;";
}


GameController::~GameController() {
    log("GameController: Liberando memoria.", LOG_INFO);
    log("GameController: Borrando cancha...", LOG_INFO);
    delete(this->pitch);
    log("GameController: Borrando timer...", LOG_INFO);
    delete(this->timer);
    log("GameController: Cancha borrada.", LOG_INFO);
}
