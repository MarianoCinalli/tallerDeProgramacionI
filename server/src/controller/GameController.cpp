#include "controller/GameController.h"

GameController::GameController(Pitch* pitch, Camera* camera, Timer* timer) {
    log("GameController: Creando gameController...", LOG_INFO);
    this->camera = camera;
    this->pitch = pitch;
    this->ball = this->pitch->getBall();
    this->end = false;
    this->timer = timer;
    this->state = HALF_START_STATE;
    this->stateOption = CENTER_LEFT_START;
    this->isFistHalf = true;
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

void GameController::update() {
    Time* elapsedTime = this->timer->getTime();
    this->checkState();
    this->updatePlayers();
    this->updateBall();
    this->updateCameraPosition();
    this->checkTime(elapsedTime);
    delete(elapsedTime);
}

void GameController::checkState() {
    switch (this->state) {
        case NORMAL_STATE: {
            this->stateOption = this->pitch->goalkick();
            if (this->stateOption >= 0) {
                this->state = GOALKICK_STATE;
            }
            break;
        }
        case GOALKICK_STATE: {
            this->checkGoal();
            this->pitch->setStart(this->stateOption);
            this->state = NORMAL_STATE;
            break;
        }
        case HALF_START_STATE: {
          this->pitch->setStart(this->stateOption);
          this->state = NORMAL_STATE;
          break;
        }
    }
}

void GameController::checkGoal() {
    int x = this->ball->getPosition()->getX();
    if (x < 200 || x > 1400) {
        if ((this->stateOption == CENTER_LEFT_START) || (this->stateOption == CENTER_RIGHT_START)) {
            Team* team;
            if (this->stateOption == CENTER_LEFT_START) {
                team = this->pitch->getTeam(TEAM_RIGHT);
            }
            else if (this->stateOption == CENTER_RIGHT_START) {
                team = this->pitch->getTeam(TEAM_LEFT);
            }
            // Incrementa contador de goles del equipo
            team->increaseScore();
            // Para la estadistica, quien hizo el gol?
            std::string message = "";
            Time* time = this->timer->getTime();
            if (time != NULL) {
                message += time->toString();
                delete(time);
            } else {
                log("GameController: El tiempo es null.", LOG_ERROR);
                message += "00-00";
            }
            Player* player = this->ball->getPlayer();
            if (player != NULL) {
                message += " " + player->getUsername();
            } else {
                log("GameController: El player es null.", LOG_ERROR);
                message += " Alguien";
            }
            std::replace(message.begin(), message.end(), '-', ':');
            team->addScoreInfo(message);
        }
    }
}

void GameController::updatePlayers() {
    // Por ahora es lo unico que necesitamos
    // porque solo se mueve un jugador.
    // Actualizar la posicion de todos los jugadores
    std::list<Player*> teamPlayers = this->pitch->getTeam(0)->getPlayers();
    std::list<Player*> awayPlayers = this->pitch->getTeam(1)->getPlayers();
    teamPlayers.insert(teamPlayers.end(), awayPlayers.begin(), awayPlayers.end());
    if (!teamPlayers.empty()) {
        for (Player* p : teamPlayers) {
            p->updateState();
        }
    }
    log("GameController: se actualizaron los jugadores.", LOG_SPAM);
}


void GameController::updateBall() {
    if (this->ball->isDominated() && this->ball->getPlayer()->isKicking() && !this->ball->getPlayer()->hasKicked()) {
        log("GameController: La pelota fue pateada.", LOG_DEBUG);
        Player* player = this->ball->getPlayer();
        bool highPass = player->isAHighPass();
        Velocity* velocity = new Velocity (0,0);
        if(!highPass) {
            log("GameController(joako): Entro a calculatePassVelocity: ", LOG_DEBUG); //SACAR
            velocity->set(this->calculatePassVelocity(player));
        }
        else {
            log("GameController(joako): pase alto sin ayuda ", LOG_DEBUG);
            velocity->set(player->getVelocity());
        }
        log("GameController: calculo de vel hecho: ", velocity->getComponentY(), LOG_DEBUG);
        this->ball->isPassed(velocity, player->getKickPower()*PASS_SPEED, highPass); //TODO valor de pase?
        player->setKicked(true);
    }
    this->pitch->changeBallOwnership();
    this->ball->updatePosition();
}

Velocity* GameController::calculatePassVelocity(Player* player){
    int orientation = player->getOrientation();
    int teamNumber = player->getTeam();
    int distance = 30000;
    Coordinates* passPosition = player->getPosition();
    int x = passPosition->getX();
    int y = passPosition->getY();
    Coordinates* newPos = new Coordinates(0,0);
    Team* team = this->pitch->getTeam(teamNumber);
    std::list<Player*> players = team->getPlayers();
    switch(orientation) {
        case PLAYER_ORIENTATION_DOWN:
            for(Player* p: players){
                int newX = (p->getPosition()->getX());
                int newY = (p->getPosition()->getY());
                int failingFor = abs(x-newX);
                if ((failingFor < PASS_HELPING_CONST) && (!p->isThisPlayer(player)) && (y < newY)){
                    log("GameController: Pase con ayuda, JUGADOR CERCA ", LOG_DEBUG); //SACAR
                    int newDistance = passPosition->distanceTo(p->getPosition());
                    if (distance > newDistance || distance == 0){
                        int xPass = (newX - x);
                        int yPass = (newY - y);
                        newPos->setX(xPass);
                        newPos->setY(yPass);
                        distance = newDistance;
                    }
                }
            }
            break;
        case PLAYER_ORIENTATION_UP:
            for(Player* p: players){
                int newX = (p->getPosition()->getX());
                int newY = (p->getPosition()->getY());
                int failingFor = abs(x-newX);
                if ((failingFor < PASS_HELPING_CONST) && (!p->isThisPlayer(player)) && (y > newY)){
                    log("GameController: Pase con ayuda, JUGADOR CERCA ", LOG_DEBUG); //SACAR
                    int newDistance = passPosition->distanceTo(p->getPosition());
                    if (distance > newDistance || distance == 0){
                        int xPass = (newX - x);
                        int yPass = (newY - y);
                        newPos->setX(xPass);
                        newPos->setY(yPass);
                        distance = newDistance;
                    }
                }
            }
            break;
        case PLAYER_ORIENTATION_RIGHT:
            log("pase a derecha con ayuda", LOG_DEBUG);
            for(Player* p: players){
                int newY = (p->getPosition()->getY());
                int newX = (p->getPosition()->getY());
                int failingFor = abs(y-newY);
                if ((failingFor < PASS_HELPING_CONST) && (!p->isThisPlayer(player)) && (x < newX)){
                    log("GameController: Pase con ayuda, JUGADOR CERCA, a: ", abs(y-newY), LOG_DEBUG); //SACAR
                    int newDistance = passPosition->distanceTo(p->getPosition());
                    log("distance: ", distance, LOG_DEBUG);
                    log("new distance: ", newDistance, LOG_DEBUG);
                    if (distance > newDistance){
                        log("jugador mas cerca que otro", newDistance, LOG_DEBUG);
                        int xPass = (newX - x);
                        int yPass = (newY - y);
                        newPos->setX(xPass);
                        log("newX: ", xPass, LOG_DEBUG);
                        newPos->setY(yPass);
                        log("newY: ", yPass, LOG_DEBUG);
                        distance = newDistance;
                        log("dist: ", newDistance, LOG_DEBUG);
                    }
                }
            }
            break;
        case PLAYER_ORIENTATION_LEFT:
            for(Player* p: players){
                int newY = (p->getPosition()->getY());
                int newX = (p->getPosition()->getX());
                int failingFor = abs(y-newY);
                if ((failingFor < PASS_HELPING_CONST) && (!p->isThisPlayer(player)) && (x > newX)){
                    log("GameController: Pase con ayuda, JUGADOR CERCA ", LOG_DEBUG); //SACAR
                    int newDistance = passPosition->distanceTo(p->getPosition());
                    if (distance > newDistance || distance == 0){
                        int xPass = (newX - x);
                        int yPass = (newY - y);
                        newPos->setX(xPass);
                        newPos->setY(yPass);
                        distance = newDistance;
                    }
                }
            }
            break;
        default:
            break;
    }
    Velocity* velocity = new Velocity(newPos->getX(), newPos->getY());
    log("quedo: x ", velocity->getFloatX(), LOG_DEBUG);
    log("quedo: y ", velocity->getFloatY(), LOG_DEBUG);
    velocity->normalize();
    log("dps de normalizar: x ", velocity->getFloatX(), LOG_DEBUG);
    log("dps de normalizar: y ", velocity->getFloatY(), LOG_DEBUG);
    if (velocity->isZero()){
        log("tomo como vel cero", LOG_DEBUG);
        velocity->accelerate(player->getOrientation());
    }
    return velocity;
}

// Cuando el jugador pise el borde mueve la camara.
// En este punto las coordenadas de el jugador son validas.
void GameController::updateCameraPosition() {
    Coordinates* position = this->ball->getPosition();
    // Coordinates* position = this->pitch->getActivePlayer(0)->getPosition();
    this->camera->calculateNewPosition(position);
}

void GameController::checkTime(Time* elapsedTime) {
    if (this->isFistHalf && this->hasHalfEnded(elapsedTime, 1)) {
        this->isFistHalf = false;
        log("GameController: Termino el primer tiempo.", LOG_INFO);
        // Aca voy a invertir a las formaciones. Cuando termine con el refator.
    } else if (!this->isFistHalf && this->hasHalfEnded(elapsedTime, 2)) {
        log("GameController: Termino el segundo tiempo.", LOG_INFO);
        this->setEnd();
    }
}

bool GameController::hasHalfEnded(Time* elapsedTime, int halfNumber) {
    return elapsedTime->getMinutes() >= (MINUTES_PER_HALF * halfNumber);
}

// Aca deberia haber una nocion del tiempo.
// Un reloj que termine el juego luego del tiempo. Proximo tp?
// Tenemos que poner algo que nos permita controlar cuando teminar.
bool GameController::shouldGameEnd() {
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

bool GameController::setTeamFormation(int team, int formation) {
    return this->pitch->setTeamFormation(team,formation);
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
        log("GameController: La pelota es null.", LOG_ERROR);
        return "";
    }
    for (Player* player : players) {
        message += player->getAsYaml();
    }
    message += ball->getAsYaml();
    message += camera->getAsYaml();
    message += this->getGameStatsMessage();
    return message + ";;";
}

std::string GameController::getGameStatsMessage() {
    std::string message = "";
    message += "clock: ";
    Time* time = this->timer->getTime();
    if (time != NULL) {
        message += "'" + time->toString() + "'\n";
        delete(time);
    } else {
        log("GameController: El tiempo es null.", LOG_ERROR);
        message += "'er-er'\n";
    }
    message += "score:\n";
    message += this->pitch->getScoresYAML();
    return message;
}

GameController::~GameController() {
    log("GameController: Liberando memoria. Borrando cancha...", LOG_INFO);
    delete(this->pitch);
    log("GameController: Cancha borrada. Memoria liberada.", LOG_INFO);
}
