#include "GameInitializer.h"

// Screen dimension constants
const int SCREEN_WIDTH = (800) * 0.8;
const int SCREEN_HEIGHT = (600) * 0.8;


GameInitializer::GameInitializer(Conf* configuration) {
    log("GameInitializer: Inicializando juego...", LOG_INFO);
    this->initializePitch(configuration);
    this->initializeTeam(configuration, 0);
    this->initializeTeam(configuration, 1);
    // this->pitch->setUserTeam(0,0);  //inicializacion de usuarios
    // this->pitch->setUserTeam(1,1);
    this->initializeBall();
    this->initializeTimer(configuration); // arriba de initializeGameController
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

Timer* GameInitializer::getTimer() {
    return this->timer;
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
    log("GameController: Borrando timer...", LOG_INFO);
    delete(this->timer);
    log("GameInitializer: Liberando usersManager.", LOG_INFO);
}

// Metodos privados. ---------------------------------------------




void GameInitializer::initializeTeam(Conf* conf, int teamNumber) {
    std::map<int, std::string> nombresEquipos;
    nombresEquipos[0] = "Argentina";
    nombresEquipos[1] = "Brasil";
    log("GameInitializer: Creando equipo.", LOG_INFO);
    Team* team = new Team(teamNumber, nombresEquipos[teamNumber]);
    log("GameInitializer: Seteando formacion.", LOG_INFO);
    for (int i = 0; i < PLAYERS_PER_TEAM; ++i) {
        log("GameInitializer: Creando jugador numero: ", i, LOG_INFO);
        Coordinates* coordinates = new Coordinates(800, 500);
        Player* player = new Player(coordinates, teamNumber);
        team->addPlayer(player);
    }
    team->setFormacion(33); //TODO formacion default, despues le pregunta al usuario
    log("GameInitializer: Ordenando equipo local.", LOG_INFO);
    team->order();
    log("GameInitializer: Agregando el equipo a la cancha: ", this->getTeamString(teamNumber), LOG_INFO);
    this->setTeam(team, teamNumber); // aca se agrega cada equipo a la cancha.
    this->pitch->setTeam(team, teamNumber); // Se usa para algo esto? Si, para despues elegir jugador activo
}

void GameInitializer::initializeBall() {
    log("GameInitializer: Inicializando pelota...", LOG_INFO);
    Coordinates* coords = new Coordinates(800, 600);
    Ball* ball = new Ball(coords);  //TODO: pasarle el jugador del medio
    this->pitch->setBall(ball);
    Player* jugador = this->pitch->getTeam(0)->getPlayers().back();
    ball->isIntercepted(jugador); //darle la pelota al jugador mas cerca
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

Conf*  GameInitializer::getConfiguration() {
    return configuration;
}

void GameInitializer::initializePitch(Conf* conf) {
    log("GameInitializer: Creando la cancha...", LOG_INFO);
    Coordinates* cameraPosition = new Coordinates(400, 300);
    this->camera = new Camera(cameraPosition, SCREEN_WIDTH, SCREEN_HEIGHT, conf->getMargen());
    this->pitch = new Pitch(this->camera);
}

void GameInitializer::initializeGameController() {
    log("GameInitializer: Creando GameController...", LOG_INFO);
    this->gameController = new GameController(this->pitch, this->camera, this->timer);
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

void GameInitializer::initializeTimer(Conf* configuration) {
    log("GameInitializer: Creando timer...", LOG_INFO);
    this->timer = new Timer(configuration->getTimePerHalf());
}
