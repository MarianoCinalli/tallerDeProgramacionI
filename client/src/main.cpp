#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string.h>
#include <fstream>
#include <stdio.h>
#include "util/Constants.h"
#include "util/logger.h"

// Para el test.
#include "view/Player.h"
#include "util/Coordinates.h"
#include "controller/Action.h"
#include "controller/ActionsManager.h"
// Fin Para el test.

// Global variables ---------------------------------------
int LOG_MIN_LEVEL = LOG_ERROR; // Cambiar por parametro parseado.
std::ofstream LOG_FILE_POINTER;
const std::string logFileName = "trabajoPractico.log";
// Global variables ---------------------------------------


// --------------------------------------------------------
//Screen dimension constants
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//Loads individual image as texture
SDL_Texture* loadTexture(std::string path);

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Current displayed texture
SDL_Texture* background = NULL;
SDL_Texture* sheet = NULL;

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
        gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (gWindow == NULL) {
            printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
            success = false;
        } else {
            //Create renderer for window
            gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
            if (gRenderer == NULL) {
                printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
                success = false;
            } else {
                //Initialize renderer color
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
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

bool loadMedia() {
    //Loading success flag
    bool success = true;

    //Load PNG texture
    background = loadTexture("img/background.png");
    if (background == NULL) {
        printf("Failed to load background image!\n");
        success = false;
    }

    sheet = loadTexture("img/sheet.png");
    if (sheet == NULL) {
        printf("Failed to load sheet image!\n");
        success = false;
    }

    return success;
}

void close() {
    //Free loaded image
    SDL_DestroyTexture(background);
    background = NULL;

    SDL_DestroyTexture(sheet);
    sheet = NULL;

    //Destroy window
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
    gRenderer = NULL;

    //Quit SDL subsystems
    IMG_Quit();
    SDL_Quit();
}

SDL_Texture* loadTexture(std::string path) {
    //The final texture
    SDL_Texture* newTexture = NULL;

    //Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == NULL) {
        printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
    } else {
        //Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
        if (newTexture == NULL) {
            printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
        }

        //Get rid of old loaded surface
        SDL_FreeSurface(loadedSurface);
    }
    return newTexture;
}
// --------------------------------------------------------



int main(int argc, char* argv[]) {
    // Log initialization ---------------------------------
    LOG_FILE_POINTER.open(logFileName, std::ofstream::app);
    logSessionStarted();

    // TEST (seguramente lo cambie) -----------------------

    // load player sprite sheet.
    //      addTransparency
    // Create Player texture
    // Dummy player. Parado en el medio de la pantalla.
    Coordinates* coordinates = new Coordinates(200, 200);
    Player* player = new Player(PLAYER_RIGHT, coordinates);

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
                Action* action = actionsManager->getAction(&e);
                action->execute(player);
                delete(action);
            }
        }
        SDL_RenderClear(screen);
        player->render(screen, coordinates);
        SDL_RenderPresent(screen);
        sleep(1/15); // Frame rate.
    }
    delete(actionsManager);
    delete(player);
    // Fin TEST (seguramente lo cambie) -----------------------
    logSessionFinished();
    LOG_FILE_POINTER.close();
    return 0;
}
