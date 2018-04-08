#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <fstream>
#include <stdio.h>
#include <yaml-cpp/yaml.h>
#include <getopt.h>
#include "util/Constants.h"
#include "util/logger.h"
#include "util/conf.h"
#include "controller/CanchaController.h"

// Para el test.
#include "model/Player.h"
#include "util/Coordinates.h"
#include "controller/actions/Action.h"
#include "controller/ActionsManager.h"
#include "view/PlayerSpriteManager.h"
#include "view/Texture.h"
#include "model/Team.h"
#include "view/PitchView.h"
#include "util/Colour.h"
#include "view/Camera.h"
// Fin Para el test.


void imprimir_ayuda() {
    cout << "Usage:\n";
    cout << "main -h\n";
    cout << "main -V\n";
    cout << "main [options]\n";
    cout << "-V, --version		Imprimir version y salir.\n";
    cout << "-h, --help		Imprimir esta ayuda.\n";
    cout << "-i, --input		Path del archivo de configuracion YAML.\n";
    cout << "Ejemplo:\n";
    cout << "main -i ~/conf.yaml \n";
}

void imprimir_version() {
    cout << "Version = tp1\n";
}

// Global variables ---------------------------------------
std::ofstream LOG_FILE_POINTER;

const std::string logFileName = "trabajoPractico.log";
const std::string defaultConfFileName = "src/default.yaml";
std::string confFileName;
int LOG_MIN_LEVEL = LOG_DEBUG;
Conf conf(defaultConfFileName);

void cargarConfiguracion(string confFile) {
    conf.loadConf(confFile);
    LOG_MIN_LEVEL = conf.getDebugLevel(); // Cambiar por parametro parseado.
}

int chequearOpciones(int argc, char* argv[]) {
    int ch;
    while (1) {
        static struct option long_options[] = {
            //Flags posibles
            {"version", no_argument, 0, 'v'},
            {"help", no_argument, 0, 'h'},
            {"input", required_argument, 0, 'i'},
            {0, 0, 0, 0}
        };

        int option_index = 0;

        ch = getopt_long(argc, argv, "vhi:",
                         long_options, &option_index);

        /* Detecta fin de opciones. */
        if (ch == -1) {
            break;
        }

        switch (ch) {
            case 0:
                if (long_options[option_index].flag != 0) {
                    break;
                }
                printf("Opcion %s", long_options[option_index].name);
                if (optarg) {
                    printf(" con argumentos %s", optarg);
                    printf("\n");
                    break;
                }

            case 'v':
                imprimir_version();
                return 1;
                break;

            case 'h':
                imprimir_ayuda();
                return 1;
                break;

            case 'i':
                confFileName = optarg;
                return 0;
                break;

            default:
                printf("Opcion invalida\n\n");
                return 1;
        }
    }
    confFileName = "conf.yaml";
    return 0;
}

bool init();
void close();

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL; // Para dibujar el sprite.

bool init() {
    //Initialization flag
    bool success = true;

    //Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        success = false;
    } else {
        //Set texture filtering to linear
        if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
            printf("Warning: Linear texture filtering not enabled!");
        }
        //Create window
        window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (window == NULL) {
            printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
            success = false;
        } else {
            //Create renderer for window
            renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
            if (renderer == NULL) {
                printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
                success = false;
            } else {
                //Initialize renderer color
                SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
                //Initialize PNG loading
                int imgFlags = IMG_INIT_PNG;
                if (!(IMG_Init(imgFlags) & imgFlags)) {
                    printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
                    success = false;
                }
            }
        }
    }
    return success;
}

void close() {
    //Destroy window
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    window = NULL;
    renderer = NULL;

    //Quit SDL subsystems
    IMG_Quit();
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    if (chequearOpciones(argc, argv)) {     //Si da 1 es o la version o el help o un flag inexistente
        printf("Salida del programa por flags o argumento invalido");
        return SALIDA_LINEA_COMANDOS;
    }
    // Log initialization ---------------------------------
    LOG_FILE_POINTER.open(logFileName, std::ofstream::app);
    logSessionStarted();
    // Main loop ------------------------------------------
    // Esquema de inicializacion.
    init();
    // Crear los jugadores.
    Coordinates* coordinates = new Coordinates(400, 300);
    Player* player = new Player(PLAYER_ORIENTATION_RIGHT, coordinates);
    // Agregarlos a su equipo.
    Team* team = new Team();
    team->addPlayer(player);
    // Crear las views.
    Colour* transparency = new Colour(0, 0xa0, 0, 0); // green
    Texture* spriteSheet = new Texture("images/sprites.png", renderer, transparency);
    delete(transparency);
    PlayerSpriteManager* playerSpriteManager = new PlayerSpriteManager(spriteSheet, player);
    // Crear la pitchView pasandole los jugadores.
    Texture* pitchImg = new Texture("images/bg.png", renderer, transparency);
    // punto arriba a la izquierda = (1600/2  - 800/2, 1000/2 - 600/2) = (400, 200)
    Coordinates* cameraPosition = new Coordinates(400, 200);
    Camera* camera = new Camera(cameraPosition, SCREEN_WIDTH, SCREEN_HEIGHT);
    PitchView* pitchView = new PitchView(pitchImg, camera);
    pitchView->addPlayerView(playerSpriteManager);
    // Crear el game manager.
    bool quit = false;
    SDL_Event e;
    // Va a manejar los eventos de teclado.
    ActionsManager* actionsManager = new ActionsManager();
    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            } else {
                // Devuelve acciones que modifican modelos.
                // Se puede optimizar para que deje de hacer actions todo el tiempo.
                Action* action = actionsManager->getAction(e);
                if (action != NULL) {
                    // gameManager->execute(Action);
                    delete(action);
                }
            }
        }
        // gameManager->updatePlayers();
        // pitchView->draw();
    }
    // Main loop ------------------------------------------
    close();
    LOG_FILE_POINTER.close();
    logSessionFinished();
    return 0;
}

/*
// --------------------------------------------------------

bool init();
void close();

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL; // Para dibujar el sprite.

bool init() {
    //Initialization flag
    bool success = true;

    //Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        success = false;
    } else {
        //Set texture filtering to linear
        if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
            printf("Warning: Linear texture filtering not enabled!");
        }
        //Create window
        window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (window == NULL) {
            printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
            success = false;
        } else {
            //Create renderer for window
            renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
            if (renderer == NULL) {
                printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
                success = false;
            } else {
                //Initialize renderer color
                SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
                //Initialize PNG loading
                int imgFlags = IMG_INIT_PNG;
                if (!(IMG_Init(imgFlags) & imgFlags)) {
                    printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
                    success = false;
                }
            }
        }
    }
    return success;
}

void close() {
    //Destroy window
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    window = NULL;
    renderer = NULL;

    //Quit SDL subsystems
    IMG_Quit();
    SDL_Quit();
}
// --------------------------------------------------------
    // Toda esta inicializacion se va a mover cuando se cree el equipo.

    // Carga los sprites como una textura.
    // Tiene hardcodeado la transparencia del color verde.
    // Puede que en el merge se refactoree, para que tenga
    // una clase textura como atributo.
    Texture* spriteSheet = new Texture("images/sprites.png", renderer);
    init();
    // Este es el encargado de dibujar el sprite correspondiente en cada frame.
    // Cada jugador tiene su manager, porque es el que sabe que
    // sprite tuvo el jugador y cual sigue.
    // Seria como una PlayerView, pero este nombre es mas descriptivo.
    PlayerSpriteManager* playerSpriteManager = new PlayerSpriteManager(spriteSheet);


    // Dummy player. Parado en el medio de la pantalla.
    Coordinates* coordinates = new Coordinates(400, 300);
    // Puede que se mueva la orientacion al playerSpriteManager.
    Player* player = new Player(PLAYER_ORIENTATION_RIGHT, coordinates);

    bool quit = false;
    SDL_Event e;
    // Va a manejar los eventos de teclado.
    ActionsManager* actionsManager = new ActionsManager();
    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            } else {
                // Devuelve acciones que modifican modelos.
                // Se puede optimizar para que deje de hacer actions todo el tiempo.
                Action* action = actionsManager->getAction(e);
                if (action != NULL) {
                    action->execute(player);
                    // Aca estaria bueno tener una clase que valide el estado del
                    // jugador luego de la accion. Por ej.: que no salga afuera de la cancha.
                    // Si no es valido: la accion puede tener un deshacer()
                    delete(action);
                }
            }
        }
        // Este metodo es para que en el ciclo actual se actualize la posicion del jugador.
        // Cuando se presiona alguna tecla de movimiento se le asigna una velocidad en una componente.
        player->updatePosition();
        SDL_RenderClear(renderer);
        // Aca se deberian transformar las coordenadas absolutas del
        // player a las relativas del display.
        Coordinates* coordenadas = player->getPosition();
        // La "vista" es un obsevador del modelo que se fija los
        // valores del jugador y dibuja lo que corresponde.
        playerSpriteManager->render(player, renderer, coordenadas);
        SDL_RenderPresent(renderer);
        sleep(1 / 15); // Frame rate.
    }
    delete(actionsManager);
    delete(player);
    delete(playerSpriteManager);
    delete(spriteSheet);
    close();
    // Fin TEST -------------------------------------------
    =======
        //Configuracion
        cargarConfiguracion(confFileName);
        log(&conf, LOG_INFO);

        // Program
        CanchaController canchaController;
        canchaController.startView();

    >>>>>>> origin/tp1
*/
