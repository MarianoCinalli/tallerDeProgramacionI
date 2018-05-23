#include "GameInitializer.h"

GameInitializer::GameInitializer(Conf* configuration) {
    log("GameInitializer: Inicializando juego...", LOG_INFO);
    this->initializePitch(configuration);
    this->initializeTeam(configuration, 0);
    this->initializeTeam(configuration, 1);
    // this->pitch->setUserTeam(0,0);  //inicializacion de usuarios
    // this->pitch->setUserTeam(1,1);
    this->initializeBall();
    this->initializeGameController();
    this->initializeGameControllerProxy(); // abajo de initializeGameController
    this->initializeConnectionManager(configuration);
    this->initializeUsersManager(configuration); // abajo de initializeGameControllerProxy
    this->configuration = configuration;
    log("GameInitializer: Juego inicializado...", LOG_INFO);
}

Pitch* GameInitializer::getPitch() {
    return this->pitch;
}


GameController* GameInitializer::getGameController() {
    return this->gameController;
}

Camera* GameInitializer::getCamera() {
    return this->camera;
}

ConnectionManager* GameInitializer::getConnectionManager() {
    return this->connectionManager;
}

GameControllerProxy* GameInitializer::getGameControllerProxy() {
    return this->gameControllerProxy;
}

UsersManager* GameInitializer::getUsersManager() {
    return this->usersManager;
}

GameInitializer::~GameInitializer() {
    log("GameInitializer: Liberando memoria...", LOG_INFO);
    log("GameInitializer: Liberando gameControllerProxy.", LOG_INFO);
    delete(this->gameControllerProxy);
    log("GameInitializer: Liberando gameController.", LOG_INFO);
    delete(this->gameController);
    log("GameInitializer: Liberando ConnectionManager.", LOG_INFO);
    delete(this->connectionManager);
    log("GameInitializer: Liberando vista de la cancha.", LOG_INFO);
    delete(this->usersManager);
    log("GameInitializer: Liberando usersManager.", LOG_INFO);
}

// Metodos privados. ---------------------------------------------




void GameInitializer::initializeTeam(Conf* conf, int teamNumber) {
    std::map<int,std::string> nombresEquipos;
    nombresEquipos[0] = "Argentina";
    nombresEquipos[1] = "Brasil";
    log("GameInitializer: Creando equipo.", LOG_INFO);
    Team* team = new Team(teamNumber, nombresEquipos[teamNumber]);
    log("GameInitializer: Seteando formacion.", LOG_INFO);
    team->setFormacion(33); //TODO formacion default, despues le pregunta al usuario
    for (int i = 0; i < PLAYERS_PER_TEAM; ++i) {
        log("GameInitializer: Creando jugador numero: ", i, LOG_INFO);
        Coordinates* coordinates = new Coordinates(800, 500);
        int orientation = this->getOrientation(teamNumber);
        Player* player = new Player(orientation, coordinates, teamNumber);
        team->addPlayer(player);
    }
    log("GameInitializer: Ordenando equipo local.", LOG_INFO);
    team->order();
    log("GameInitializer: Agregando el equipo a la cancha: ", this->getTeamString(teamNumber), LOG_INFO);
    this->setTeam(team, teamNumber); // aca se agrega cada equipo a la cancha.
    this->pitch->setTeam(team, teamNumber); // Se usa para algo esto? Si, para despues elegir jugador activo
}

void GameInitializer::initializeBall() {
    log("GameInitializer: Inicializando pelota...", LOG_INFO);
    // Player* player = this->pitch->getActivePlayer(0); //TODO user 0 es el dueño del balon al ppio
    Coordinates* coords = new Coordinates(800, 600);
    Ball* ball = new Ball(coords);  //TODO: pasarle el jugador del medio
    this->pitch->setBall(ball);
    log("GameInitializer: Pelota inicializada", LOG_INFO);
}

std::string GameInitializer::getTeamString(int teamNumber) {
    if (teamNumber == 0) {
        return "local";
    } else {
        return "visitante";
    }
}

void GameInitializer::setTeam(Team* team, int teamNumber) {
    if (teamNumber == 0) {
        this->localTeam = team;
    } else if (teamNumber == 1) {
        this->awayTeam = team;
    }
}

int GameInitializer::getOrientation(int teamNumber) {
    if (teamNumber == 0) {
        return PLAYER_ORIENTATION_RIGHT;
    } else {
        return PLAYER_ORIENTATION_LEFT;
    }
}

Conf*  GameInitializer::getConfiguration() {
  return configuration;
}

void GameInitializer::initializePitch(Conf* conf) {
    log("GameInitializer: Creando la cancha...", LOG_INFO);
    Coordinates* cameraPosition = new Coordinates(400, 200);
    this->camera = new Camera(cameraPosition, SCREEN_WIDTH, SCREEN_HEIGHT, conf->getMargen());
    this->pitch = new Pitch(this->camera);
}

void GameInitializer::initializeGameController() {
    log("GameInitializer: Creando GameController...", LOG_INFO);
    this->gameController = new GameController(this->pitch);
}

void GameInitializer::initializeConnectionManager(Conf* configuration) {
    log("GameInitializer: Creando ConnectionManager...", LOG_INFO);
    this->connectionManager = new ConnectionManager(configuration->getPort(), configuration->getMaxClients());
}

void GameInitializer::initializeGameControllerProxy() {
    log("GameInitializer: Creando GameControllerProxy...", LOG_INFO);
    this->gameControllerProxy = new GameControllerProxy(this->gameController);
}

void GameInitializer::initializeUsersManager(Conf* configuration) {
    log("GameInitializer: Creando UsersManager...", LOG_INFO);
    this->usersManager = new UsersManager(configuration->getUsuarios(), this->getGameControllerProxy());
}
