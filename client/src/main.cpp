#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <fstream>
#include <stdio.h>
#include "util/Constants.h"
#include "util/logger.h"

// Para el test.
#include "model/Player.h"
#include "util/Coordinates.h"
#include "controller/actions/Action.h"
#include "controller/ActionsManager.h"
#include "view/PlayerSpriteManager.h"
#include "view/SpriteSheet.h"
// Fin Para el test.

// Global variables ---------------------------------------
int LOG_MIN_LEVEL = LOG_DEBUG; // Cambiar por parametro parseado.
std::ofstream LOG_FILE_POINTER;
const std::string logFileName = "trabajoPractico.log";
// Global variables ---------------------------------------


// --------------------------------------------------------
//Screen dimension constants
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

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



int main(int argc, char* argv[]) {
    // Log initialization ---------------------------------
    LOG_FILE_POINTER.open(logFileName, std::ofstream::app);
    logSessionStarted();

    // TEST -----------------------------------------------
    init();
    // Toda esta inicializacion se va a mover cuando se cree el equipo.

    // Carga los sprites como una textura.
    // Tiene hardcodeado la transparencia del color verde.
    // Puede que en el merge se refactoree, para que tenga
    // una clase textura como atributo.
    SpriteSheet* spriteSheet = new SpriteSheet("images/sprites.png", renderer);

    // Este es el encargado de dibujar el sprite correspondiente
    // en cada frame. Se lo invoca cuando el jugador hace render.
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
            } else if (e.type == SDL_KEYDOWN) {
                // Devuelve acciones que modifican vistas/modelos.
                // Seguramente deje crear actions todo el tiempo. Por ahora es así.
                Action* action = actionsManager->getAction(e);
                if (action != NULL) {
                    action->execute(player);    
                    delete(action);
                }
            }
        }
        player->updatePosition();
        SDL_RenderClear(renderer);
        playerSpriteManager->render(player, renderer);
        SDL_RenderPresent(renderer);
        sleep(1); // Frame rate.
    }
    delete(actionsManager);
    delete(player);
    delete(playerSpriteManager);
    delete(spriteSheet);
    close();
    // Fin TEST -------------------------------------------
    logSessionFinished();
    LOG_FILE_POINTER.close();
    return 0;
}
