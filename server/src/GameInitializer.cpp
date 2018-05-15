#include "GameInitializer.h"

GameInitializer::GameInitializer(Conf* configuration) {
    log("GameInitializer: Inicializando juego...", LOG_INFO);
    this->initializePitch(configuration);
    this->initializeTeam(configuration, 0);
    this->initializeTeam(configuration, 1);
    this->initializeBall();
    this->initializeGameController();
    //this->initializeActionsManager(); // Por ahora no
    log("GameInitializer: Juego inicializado...", LOG_INFO);
}

Pitch* GameInitializer::getPitch() {
    return this->pitch;
}

GameController* GameInitializer::getGameController() {
    return this->gameController;
}

ActionsManager* GameInitializer::getActionsManager() {
    return this->actionsManager;
}

Camera* GameInitializer::getCamera() {
    return this->camera;
}

GameInitializer::~GameInitializer() {
    log("GameInitializer: Liberando memoria...", LOG_INFO);
    log("GameInitializer: Liberando gameController.", LOG_INFO);
    delete(this->gameController);
    log("GameInitializer: Liberando actionsManager.", LOG_INFO);
    delete(this->actionsManager);
    log("GameInitializer: Liberando vista de la cancha.", LOG_INFO);
}

// Metodos privados. ---------------------------------------------

void GameInitializer::initializeTeam(Conf* conf, int teamNumber) {
    log("GameInitializer: Creando equipo.", LOG_INFO);
    Team* team = new Team();
    for (int i = 0; i < PLAYERS_PER_TEAM; ++i) {
        log("GameInitializer: Creando jugador numero: ", i, LOG_INFO);
        Coordinates* coordinates = new Coordinates(800, 500);
        int orientation = this->getOrientation(teamNumber);
        Player* player = new Player(orientation, coordinates, teamNumber);
        team->addPlayer(player);
    }
    log("GameInitializer: Agregando el equipo a la cancha: ", this->getTeamString(teamNumber), LOG_INFO);
    this->setTeam(team, teamNumber); // aca se agrega cada equipo a la cancha.
    this->pitch->setTeam(team, teamNumber); // Se usa para algo esto?
}

void GameInitializer::initializeBall() {
    log("GameInitializer: Inicializando pelota...", LOG_INFO);
    Player* player = this->pitch->activePlayer;
    Coordinates* coords = new Coordinates(800, 600);
    Ball* ball = new Ball(coords, player);  //TODO: pasarle el jugador del medio
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

void GameInitializer::initializeActionsManager() {
    log("GameInitializer: Creando ActionsManager...", LOG_INFO);
    this->actionsManager = new ActionsManager();
}
