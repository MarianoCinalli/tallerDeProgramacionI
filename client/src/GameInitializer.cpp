#include "GameInitializer.h"

GameInitializer::GameInitializer(Conf* configuration) {
    log("GameInitializer: Inicializando juego...", LOG_INFO);
    this->initializeCommonSdlResources();
    this->initializePitch(configuration);
    this->initializePitchView(configuration);
    this->initializeTeam(configuration, 0);
    this->initializeTeam(configuration, 1);
    this->initializeBall();
    this->initializeGameController();
    this->initializeActionsManager();
    log("GameInitializer: Juego inicializado...", LOG_INFO);
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

SDL_Renderer* GameInitializer::getRenderer() {
    return this->renderer;
}

PitchView* GameInitializer::getPitchView() {
    return this->pitchView;
}

Team* GameInitializer::getTeam(int teamNumber) {
  if (teamNumber == 0) {
      return this->localTeam;
    }
  else {
      return this->awayTeam;
    }
}


GameInitializer::~GameInitializer() {
    log("GameInitializer: Liberando memoria...", LOG_INFO);
    log("GameInitializer: Liberando gameController.", LOG_INFO);
    delete(this->gameController);
    log("GameInitializer: Liberando actionsManager.", LOG_INFO);
    delete(this->actionsManager);
    log("GameInitializer: Liberando vista de la cancha.", LOG_INFO);
    delete(this->pitchView);
    log("GameInitializer: Liberando sprites del equipo local.", LOG_INFO);
    delete(this->localTeamSprites);
    log("GameInitializer: Memoria liberada.", LOG_INFO);
    log("GameInitializer: Liberado recursos de SDL.", LOG_INFO);
    this->cleanCommonSdlResources();
    log("GameInitializer: Recursos de SDL liberados.", LOG_INFO);
}

// Metodos privados. ---------------------------------------------

void GameInitializer::initializePitchView(Conf* conf) {
    log("GameInitializer: Creando vista de la cancha...", LOG_INFO);
    Texture* pitchImg = new Texture(conf->getAssetsCancha(), this->renderer);
    // punto arriba a la izquierda = (1600/2  - 800/2, 1000/2 - 600/2) = (400, 200)
    this->pitchView = new PitchView(pitchImg, this->camera);
    log("GameInitializer: Vista de la cancha creada.", LOG_INFO);
}

void GameInitializer::initializeBall(){
    log("GameInitializer: Inicializando pelota...", LOG_INFO);
    Player* player = this->pitch->activePlayer;
    Coordinates* coords = new Coordinates(800,600);
    Ball* ball = new Ball(coords, player);  //TODO: pasarle el jugador del medio
    this->pitch->setBall(ball);
    BallSpriteManager* ballSpriteManager = new BallSpriteManager(this->localTeamSprites, ball);
    this->pitchView->addBallView(ballSpriteManager);
    log("GameInitializer: Pelota inicializada", LOG_INFO);
}

void GameInitializer::initializeTeam(Conf* conf, int teamNumber) {
    log("GameInitializer: Creando equipo local...", LOG_INFO);
    int shirtColour = 0;
    if (teamNumber == CASACA_PRINCIPAL) {
        shirtColour = 255;
    }
    Colour* shirt = new Colour(shirtColour, 0, 0, 0);
    log("GameInitializer: Creando sprites para el equipo local.", LOG_INFO);
    this->initializeTeamSprites(conf->getSpritesPath(),shirt, teamNumber);
    delete(shirt);
    // Crear los jugadores.
    log("GameInitializer: Creando equipo.", LOG_INFO);
    Team* team = new Team(teamNumber);
    log("GameInitializer: Seteando formacion.", LOG_INFO);
    team->setFormacion(conf->getFormacion());
    for (int i = 0; i < PLAYERS_PER_TEAM; ++i) {
        log("GameInitializer: Creando jugador numero: ", i, LOG_INFO);
        Coordinates* coordinates = new Coordinates(800, 500);
        Player* player = new Player(PLAYER_ORIENTATION_RIGHT, coordinates, teamNumber);
        team->addPlayer(player);
        log("GameInitializer: Creando vista de jugador numero: ", i, LOG_INFO);
        PlayerSpriteManager* playerSpriteManager;
        if(teamNumber == 0){
          playerSpriteManager = new PlayerSpriteManager(this->localTeamSprites, player);
        }
        else {
          playerSpriteManager = new PlayerSpriteManager(this->awayTeamSprites, player);
        }
        this->pitchView->addPlayerView(playerSpriteManager);
    }
    log("GameInitializer: Ordenando equipo local.", LOG_INFO);
    team->order();
    log("GameInitializer: Agregando el equipo local a la cancha.", LOG_INFO);
    if (teamNumber==0){
      this->localTeam = team;
    } else if (teamNumber == 1){
      this->awayTeam = team;
    }
    this->pitch->setTeam(team, teamNumber);
}

void GameInitializer::initializeTeamSprites(std::string shirtsPath, Colour* shirt, int teamNumber) {
    Colour* transparency = new Colour(0, 0xa0, 0, 0);
    // Supongo que el path de los sprites lo deberia sacar de conf, no?

    Texture* texture = new Texture(
        shirtsPath,
        "images/spritesShirts.png",
        this->renderer,
        transparency,
        shirt
    );
    if (teamNumber == 0){
      this->localTeamSprites = texture;
    } else if (teamNumber == 1){
      this->awayTeamSprites = texture;
    }
    delete(transparency);
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

void GameInitializer::initializeCommonSdlResources() {
    //Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
    } else {
        if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
            printf("Warning: Linear texture filtering not enabled!");
        }
        this->window = SDL_CreateWindow("ZIDANE", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (this->window == NULL) {
            printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
        } else {
            this->renderer = SDL_CreateRenderer(this->window, -1, SDL_RENDERER_ACCELERATED);
            if (this->renderer == NULL) {
                printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
            } else {
                SDL_SetRenderDrawColor(this->renderer, 0xFF, 0xFF, 0xFF, 0xFF);
                int imgFlags = IMG_INIT_PNG;
                if (!(IMG_Init(imgFlags) & imgFlags)) {
                    printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
                }
            }
        }
    }
}

void GameInitializer::cleanCommonSdlResources() {
    //Destroy window
    log("GameInitializer: Destruyendo renderer.", LOG_INFO);
    SDL_DestroyRenderer(this->renderer);
    this->renderer = NULL;
    log("GameInitializer: Destruyendo ventana.", LOG_INFO);
    SDL_DestroyWindow(this->window);
    this->window = NULL;
    //Quit SDL subsystems
    log("GameInitializer: Saliendo de SDL_IMG.", LOG_INFO);
    IMG_Quit();
    log("GameInitializer: Saliendo de SDL.", LOG_INFO);
    SDL_Quit();
}
