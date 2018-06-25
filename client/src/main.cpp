#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <string>
#include <fstream>
#include <unistd.h>
#include <stdio.h>
#include <yaml-cpp/yaml.h>
#include <getopt.h>
#include "GameInitializer.h"
#include "util/Constants.h"
#include "util/logger.h"
#include "util/conf.h"
#include "controller/ActionsManager.h"
#include "controller/GameController.h"
#include "view/Camera.h"
#include "util/ConnectionManager.h"
#include "util/ThreadSpawner.h"
#include "util/functions.h"

#include <iostream>
#include <mutex>

// Global variables ---------------------------------------
std::ofstream LOG_FILE_POINTER;
const std::string logFileName = "trabajoPractico.log";
const std::string defaultConfFileName = "config/default.yaml";
const std::string defaultSpritesFileName = "images/spritesDefaults.png";
std::string confFileName = "config/conf.yaml";
int LOG_MIN_LEVEL = LOG_DEBUG; // Dejarlo asi para que cuando empieze loggee todo.
std::string CLI_LOG_LEVEL = "";
std::mutex log_mutex;
GameInitializer* initializer;
std::mutex quit_mutex;
bool quit = false;
bool lostConnectionQuit = false;
std::string CLI_PORT = "";
std::string CLI_IP = "";
// Music
extern Mix_Music *gMusic = NULL;
// Sound effects
extern Mix_Chunk *gKickSound = NULL; //kick
extern Mix_Chunk *gGoalSound = NULL; //goal
Mix_Chunk *gWhistleSound = NULL; //whistle
Mix_Chunk *gStartSound = NULL; //start
Mix_Chunk *gCountdownSound = NULL; //countdown
// Global variables ---------------------------------------

void imprimir_ayuda() {
    cout << "Usage:\n";
    cout << "main -h\n";
    cout << "main -V\n";
    cout << "main [options]\n";
    cout << "-V, --version      Imprimir version y salir.\n";
    cout << "-h, --help     Imprimir esta ayuda.\n";
    cout << "-i, --input        Path del archivo de configuracion YAML.\n";
    cout << "-l, --logLevel        El nivel de log para la aplicacion. info;debug;error\n";
    cout << "-d, --serverip        La ip del servidor a conectar.\n";
    cout << "-p, --serverport        El puerto por el cual se conecta al servidor\n";
    cout << "Ejemplo:\n";
    cout << "main -i ~/conf.yaml -l info\n";
    cout << "\nREFERENCIA\nCon las flechas: se corre, \nCon W sprint, \nCon A barrida, \nCon S: patear, \nCon BARRA: cambiar de jugador";
}

void imprimir_version() {
    cout << "Version = tp2\n";
}

int chequearOpciones(int argc, char* argv[]) {
    int ch;
    std::string tempConf = "";
    while (1) {
        static struct option long_options[] = {
            //Flags posibles
            {"version", no_argument, 0, 'v'},
            {"help", no_argument, 0, 'h'},
            {"input", required_argument, 0, 'i'},
            {"logLevel", required_argument, 0, 'l'},
            {"serverip", required_argument, 0, 'd'},
            {"serverport", required_argument, 0, 'p'},
            {0, 0, 0, 0}
        };

        int option_index = 0;

        ch = getopt_long(argc, argv, "vhi:l:d:p:",
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
                tempConf = optarg;
                break;

            case 'l':
                CLI_LOG_LEVEL = optarg;
                break;

            case 'd':
                CLI_IP = optarg;
                break;

            case 'p':
                CLI_PORT = optarg;
                break;

            default:
                printf("Opcion invalida\n\n");
                return 1;
        }
    }
    if (tempConf != "") {
        confFileName = tempConf;
    } else {
        confFileName = "config/conf.yaml";
    }
    return 0;
}

void endProgram(int statusToExit, ConnectionManager* connectionManager) {
    //Free the music
    Mix_FreeMusic( gMusic );
    gMusic = NULL;
    //Free the sound effects
    Mix_FreeChunk( gKickSound );
    Mix_FreeChunk( gGoalSound );
    Mix_FreeChunk( gWhistleSound );
    Mix_FreeChunk( gStartSound );
    Mix_FreeChunk( gCountdownSound );
    gKickSound = NULL;
    gGoalSound = NULL;
    gWhistleSound = NULL;
    gStartSound = NULL;
    gCountdownSound = NULL;
    // Cerrar la conexion
    connectionManager->closeConnection();
    delete(connectionManager);
    delete(initializer);
    logSessionFinished();
    LOG_FILE_POINTER.close();
    exit(statusToExit);
}

// Colores
SDL_Color SDL_BLACK = { 0, 0, 0, 0xFF };
SDL_Color SDL_RED = { 0xFF, 0, 0, 0xFF };
SDL_Color SDL_GREEN = { 0, 0xFF, 0, 0xFF };
SDL_Color SDL_BLUE = { 0, 0, 0xFF, 0xFF };
SDL_Color SDL_WHITE = { 0xFF, 0xFF, 0xFF, 0xFF };

void openLoginServer(SDL_Renderer* gRenderer, std::string& servidor, std::string& puerto, std::string mensaje, ConnectionManager* connectionManager) {
    log("openLoginServer: Entra al openLogin Server", LOG_INFO);
    bool quit = false;
    SDL_Event e;
    TTF_Font* gFont = NULL;
    gFont = TTF_OpenFont("lazy.ttf", 30);
    if (gFont == NULL) {
        log("openLoginServer: Error al cargar la fuente! SDL_ttf Error: ", TTF_GetError(), LOG_INFO);
    }
    Texture opc1Texture;
    opc1Texture.loadFromRenderedText("->", gRenderer, SDL_RED, gFont);
    Texture mensajeTexture;
    mensajeTexture.loadFromRenderedText(mensaje, gRenderer, SDL_RED, gFont);
    Texture servidorPromptTexture;
    servidorPromptTexture.loadFromRenderedText("Servidor:", gRenderer, SDL_BLACK, gFont);
    Texture puertoPromptTexture;
    puertoPromptTexture.loadFromRenderedText("Puerto:", gRenderer, SDL_BLACK, gFont);
    std::string inputs [2] = { servidor, puerto};
    Texture servidorInputTexture;
    servidorInputTexture.loadFromRenderedText(inputs[0], gRenderer, SDL_BLUE, gFont);
    Texture puertoInputTexture;
    puertoInputTexture.loadFromRenderedText(inputs[1], gRenderer, SDL_BLUE, gFont);

    int inputsIndex = 0;
    SDL_StartTextInput();
    while (!quit) {
        bool renderText = false;
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT || e.key.keysym.sym == SDLK_ESCAPE) {
                endProgram(1, connectionManager);
            } else if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_BACKSPACE && inputs[inputsIndex].length() > 0) {
                    inputs[inputsIndex].pop_back();
                    renderText = true;
                }
                if (e.key.keysym.sym == SDLK_TAB) {
                    inputsIndex++;
                    if (inputsIndex >= 2) { inputsIndex = 0; }
                }
                if (e.key.keysym.sym == SDLK_RETURN) {
                    // enviar la configuracion al servidor
                    quit = true;
                }
            } else if (e.type == SDL_TEXTINPUT) {
                inputs[inputsIndex] += e.text.text;
                renderText = true;
            }
        }
        if (renderText) {
            if (inputs[inputsIndex] == "") { inputs[inputsIndex] = " "; }
            servidorInputTexture.loadFromRenderedText(inputs[0].c_str(), gRenderer, SDL_BLUE, gFont);
            puertoInputTexture.loadFromRenderedText(inputs[1].c_str(), gRenderer, SDL_BLUE, gFont);
            if (inputs[inputsIndex] == " ") { inputs[inputsIndex] = ""; }
        }
        //Clear screen
        SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(gRenderer);
        //Render text textures
        SDL_Rect renderQuadOpc1 = { 250, 150 + (inputsIndex * 100), opc1Texture.getWidth(), opc1Texture.getHeight() };
        SDL_RenderCopyEx(gRenderer, opc1Texture.getSpriteSheetTexture(), NULL, &renderQuadOpc1, 0.0, NULL, SDL_FLIP_NONE);
        SDL_Rect renderQuad0 = { (SCREEN_WIDTH - mensajeTexture.getWidth()) / 2, 50, mensajeTexture.getWidth(), mensajeTexture.getHeight() };
        SDL_RenderCopyEx(gRenderer, mensajeTexture.getSpriteSheetTexture(), NULL, &renderQuad0, 0.0, NULL, SDL_FLIP_NONE);
        SDL_Rect renderQuad1 = { (SCREEN_WIDTH - servidorPromptTexture.getWidth()) / 2, 100, servidorPromptTexture.getWidth(), servidorPromptTexture.getHeight() };
        SDL_RenderCopyEx(gRenderer, servidorPromptTexture.getSpriteSheetTexture(), NULL, &renderQuad1, 0.0, NULL, SDL_FLIP_NONE);
        SDL_Rect renderQuad11 = { (SCREEN_WIDTH - servidorInputTexture.getWidth()) / 2, 150, servidorInputTexture.getWidth(), servidorInputTexture.getHeight() };
        SDL_RenderCopyEx(gRenderer, servidorInputTexture.getSpriteSheetTexture(), NULL, &renderQuad11, 0.0, NULL, SDL_FLIP_NONE);
        SDL_Rect renderQuad2 = { (SCREEN_WIDTH - puertoPromptTexture.getWidth()) / 2, 200, puertoPromptTexture.getWidth(), puertoPromptTexture.getHeight() };
        SDL_RenderCopyEx(gRenderer, puertoPromptTexture.getSpriteSheetTexture(), NULL, &renderQuad2, 0.0, NULL, SDL_FLIP_NONE);
        SDL_Rect renderQuad22 = { (SCREEN_WIDTH - puertoInputTexture.getWidth()) / 2, 250, puertoInputTexture.getWidth(), puertoInputTexture.getHeight() };
        SDL_RenderCopyEx(gRenderer, puertoInputTexture.getSpriteSheetTexture(), NULL, &renderQuad22, 0.0, NULL, SDL_FLIP_NONE);
        SDL_RenderPresent(gRenderer);
    }
    SDL_StopTextInput();
    servidor = inputs[0];
    puerto = inputs[1];
}

void openLoginUsuario(SDL_Renderer* gRenderer, std::string& servidor, std::string& puerto, std::string& usuario, std::string& clave, std::string mensaje, ConnectionManager* connectionManager) {
    log("openLoginUsuario: Entra al openLogin Usuario", LOG_INFO);
    bool quit = false;
    SDL_Event e;
    TTF_Font* gFont = NULL;
    gFont = TTF_OpenFont("lazy.ttf", 30);
    if (gFont == NULL) {
        log("openLoginUsuario: Error al cargar la fuente! SDL_ttf Error: ", TTF_GetError(), LOG_INFO);
    }
    Texture opc1Texture;
    opc1Texture.loadFromRenderedText("->", gRenderer, SDL_RED, gFont);
    Texture mensajeTexture;
    mensajeTexture.loadFromRenderedText(mensaje, gRenderer, SDL_RED, gFont);
    Texture servidorPromptTexture;
    servidorPromptTexture.loadFromRenderedText("Servidor:", gRenderer, SDL_BLACK, gFont);
    Texture puertoPromptTexture;
    puertoPromptTexture.loadFromRenderedText("Puerto:", gRenderer, SDL_BLACK, gFont);
    Texture usuarioPromptTexture;
    usuarioPromptTexture.loadFromRenderedText("Usuario:", gRenderer, SDL_BLACK, gFont);
    Texture clavePromptTexture;
    clavePromptTexture.loadFromRenderedText("Clave:", gRenderer, SDL_BLACK, gFont);
    std::string inputs [2] = { usuario, clave };
    Texture servidorInputTexture;
    servidorInputTexture.loadFromRenderedText(servidor, gRenderer, SDL_GREEN, gFont);
    Texture puertoInputTexture;
    puertoInputTexture.loadFromRenderedText(puerto, gRenderer, SDL_GREEN, gFont);
    std::string hidden = "";
    for (int i = 0; (unsigned)i < clave.size(); i++) {
        hidden += "*";
    }
    Texture usuarioInputTexture;
    usuarioInputTexture.loadFromRenderedText(inputs[0], gRenderer, SDL_BLUE, gFont);
    Texture claveInputTexture;
    claveInputTexture.loadFromRenderedText(hidden, gRenderer, SDL_BLUE, gFont);

    int inputsIndex = 0;
    SDL_StartTextInput();
    while (!quit) {
        bool renderText = false;
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT || e.key.keysym.sym == SDLK_ESCAPE) {
                endProgram(1, connectionManager);
            } else if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_BACKSPACE && inputs[inputsIndex].length() > 0) {
                    inputs[inputsIndex].pop_back();
                    renderText = true;
                }
                if (e.key.keysym.sym == SDLK_TAB) {
                    inputsIndex++;
                    if (inputsIndex >= 2) { inputsIndex = 0; }
                }
                if (e.key.keysym.sym == SDLK_RETURN) {
                    // enviar la configuracion al servidor
                    quit = true;
                }
            } else if (e.type == SDL_TEXTINPUT) {
                inputs[inputsIndex] += e.text.text;
                renderText = true;
            }
        }
        if (renderText) {
            hidden = "";
            for (int i = 0; (unsigned)i < inputs[1].size(); i++) {
                hidden += "*";
            }
            if (hidden == "") { hidden = " "; }
            if (inputs[inputsIndex] == "") { inputs[inputsIndex] = " "; }
            usuarioInputTexture.loadFromRenderedText(inputs[0].c_str(), gRenderer, SDL_BLUE, gFont);
            claveInputTexture.loadFromRenderedText(hidden.c_str(), gRenderer, SDL_BLUE, gFont);
            if (hidden == " ") { hidden = ""; }
            if (inputs[inputsIndex] == " ") { inputs[inputsIndex] = ""; }
        }
        //Clear screen
        SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(gRenderer);
        //Render text textures
        SDL_Rect renderQuadOpc1 = { 250, 350 + (inputsIndex * 100), opc1Texture.getWidth(), opc1Texture.getHeight() };
        SDL_RenderCopyEx(gRenderer, opc1Texture.getSpriteSheetTexture(), NULL, &renderQuadOpc1, 0.0, NULL, SDL_FLIP_NONE);
        SDL_Rect renderQuad0 = { (SCREEN_WIDTH - mensajeTexture.getWidth()) / 2, 50, mensajeTexture.getWidth(), mensajeTexture.getHeight() };
        SDL_RenderCopyEx(gRenderer, mensajeTexture.getSpriteSheetTexture(), NULL, &renderQuad0, 0.0, NULL, SDL_FLIP_NONE);
        SDL_Rect renderQuad1 = { (SCREEN_WIDTH - servidorPromptTexture.getWidth()) / 2, 100, servidorPromptTexture.getWidth(), servidorPromptTexture.getHeight() };
        SDL_RenderCopyEx(gRenderer, servidorPromptTexture.getSpriteSheetTexture(), NULL, &renderQuad1, 0.0, NULL, SDL_FLIP_NONE);
        SDL_Rect renderQuad11 = { (SCREEN_WIDTH - servidorInputTexture.getWidth()) / 2, 150, servidorInputTexture.getWidth(), servidorInputTexture.getHeight() };
        SDL_RenderCopyEx(gRenderer, servidorInputTexture.getSpriteSheetTexture(), NULL, &renderQuad11, 0.0, NULL, SDL_FLIP_NONE);
        SDL_Rect renderQuad2 = { (SCREEN_WIDTH - puertoPromptTexture.getWidth()) / 2, 200, puertoPromptTexture.getWidth(), puertoPromptTexture.getHeight() };
        SDL_RenderCopyEx(gRenderer, puertoPromptTexture.getSpriteSheetTexture(), NULL, &renderQuad2, 0.0, NULL, SDL_FLIP_NONE);
        SDL_Rect renderQuad22 = { (SCREEN_WIDTH - puertoInputTexture.getWidth()) / 2, 250, puertoInputTexture.getWidth(), puertoInputTexture.getHeight() };
        SDL_RenderCopyEx(gRenderer, puertoInputTexture.getSpriteSheetTexture(), NULL, &renderQuad22, 0.0, NULL, SDL_FLIP_NONE);
        SDL_Rect renderQuad3 = { (SCREEN_WIDTH - usuarioPromptTexture.getWidth()) / 2, 300, usuarioPromptTexture.getWidth(), usuarioPromptTexture.getHeight() };
        SDL_RenderCopyEx(gRenderer, usuarioPromptTexture.getSpriteSheetTexture(), NULL, &renderQuad3, 0.0, NULL, SDL_FLIP_NONE);
        SDL_Rect renderQuad33 = { (SCREEN_WIDTH - usuarioInputTexture.getWidth()) / 2, 350, usuarioInputTexture.getWidth(), usuarioInputTexture.getHeight() };
        SDL_RenderCopyEx(gRenderer, usuarioInputTexture.getSpriteSheetTexture(), NULL, &renderQuad33, 0.0, NULL, SDL_FLIP_NONE);
        SDL_Rect renderQuad4 = { (SCREEN_WIDTH - clavePromptTexture.getWidth()) / 2, 400, clavePromptTexture.getWidth(), clavePromptTexture.getHeight() };
        SDL_RenderCopyEx(gRenderer, clavePromptTexture.getSpriteSheetTexture(), NULL, &renderQuad4, 0.0, NULL, SDL_FLIP_NONE);
        SDL_Rect renderQuad44 = { (SCREEN_WIDTH - claveInputTexture.getWidth()) / 2, 450, claveInputTexture.getWidth(), claveInputTexture.getHeight() };
        SDL_RenderCopyEx(gRenderer, claveInputTexture.getSpriteSheetTexture(), NULL, &renderQuad44, 0.0, NULL, SDL_FLIP_NONE);
        SDL_RenderPresent(gRenderer);
    }
    SDL_StopTextInput();
    usuario = inputs[0];
    clave = inputs[1];
}

void showLostConnectionMessage(SDL_Renderer* gRenderer, ConnectionManager* connectionManager, bool reconnect) {
    log("showLostConnectionMessage: Se registro la salida por perdida de conexion. Mostrando mensaje.", LOG_INFO);
    SDL_Event e;
    TTF_Font* gFont = NULL;
    gFont = TTF_OpenFont("lazy.ttf", 30);
    if (gFont == NULL) {
        log("showLostConnectionMessage: Error al cargar la fuente! SDL_ttf Error: ", TTF_GetError(), LOG_INFO);
    }
    Texture line1Texture;
    if (reconnect){
    line1Texture.loadFromRenderedText("Entablando conexion con el servidor!", gRenderer, SDL_BLUE, gFont);
  }else{
    line1Texture.loadFromRenderedText("Conexion perdida con el servidor!", gRenderer, SDL_BLUE, gFont);
  }
    Texture line2Texture;
    line2Texture.loadFromRenderedText("Para cancelar presionar 'escape'...", gRenderer, SDL_BLUE, gFont);
    bool continueShowingMessage = true;
    while (continueShowingMessage && lostConnectionQuit) {
        if (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT || e.key.keysym.sym == SDLK_ESCAPE) {
                continueShowingMessage = false;
                endProgram(1, connectionManager);
            }
        }
        //Clear screen
        SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(gRenderer);
        //Render text textures
        SDL_Rect renderQuad0 = { (SCREEN_WIDTH - line1Texture.getWidth()) / 2, 50, line1Texture.getWidth(), line1Texture.getHeight() };
        SDL_RenderCopyEx(gRenderer, line1Texture.getSpriteSheetTexture(), NULL, &renderQuad0, 0.0, NULL, SDL_FLIP_NONE);
        SDL_Rect renderQuad1 = { (SCREEN_WIDTH - line2Texture.getWidth()) / 2, 150, line2Texture.getWidth(), line2Texture.getHeight() };
        SDL_RenderCopyEx(gRenderer, line2Texture.getSpriteSheetTexture(), NULL, &renderQuad1, 0.0, NULL, SDL_FLIP_NONE);
        SDL_RenderPresent(gRenderer);
        usleep(1000);
    }
}

void openLoginEquipo(SDL_Renderer* gRenderer, int& seleccion, std::string mensaje, ConnectionManager* connectionManager) {
    log("openLoginEquipo: Entra al openLoginEquipo", LOG_INFO);
    bool quit = false;
    SDL_Event e;
    TTF_Font* gFont = NULL;
    gFont = TTF_OpenFont("lazy.ttf", 30);
    if (gFont == NULL) {
        log("openLoginEquipo: Error al cargar la fuente! SDL_ttf Error: ", TTF_GetError(), LOG_INFO);
    }
    Texture opc1Texture;
    opc1Texture.loadFromRenderedText("->", gRenderer, SDL_RED, gFont);
    Texture mensajeTexture;
    mensajeTexture.loadFromRenderedText(mensaje, gRenderer, SDL_RED, gFont);
    Texture equipo1PromptTexture;
    equipo1PromptTexture.loadFromRenderedText("Local:", gRenderer, SDL_BLACK, gFont);
    Texture equipo2PromptTexture;
    equipo2PromptTexture.loadFromRenderedText("Visitante:", gRenderer, SDL_BLACK, gFont);
    Texture equipo1InputTexture;
    equipo1InputTexture.loadFromRenderedText("Argentina", gRenderer, SDL_BLUE, gFont);
    Texture equipo2InputTexture;
    equipo2InputTexture.loadFromRenderedText("Brasil", gRenderer, SDL_BLUE, gFont);

    int inputsIndex = 0;
    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT || e.key.keysym.sym == SDLK_ESCAPE) {
                endProgram(1, connectionManager);
            } else if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_TAB) {
                    inputsIndex++;
                    if (inputsIndex >= 2) { inputsIndex = 0; }
                }
                if (e.key.keysym.sym == SDLK_RETURN) {
                    // enviar la configuracion al servidor
                    quit = true;
                }
            }
        }
        //Clear screen
        SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(gRenderer);
        //Render text textures
        SDL_Rect renderQuadOpc1 = { 250, 150 + (inputsIndex * 100), opc1Texture.getWidth(), opc1Texture.getHeight() };
        SDL_RenderCopyEx(gRenderer, opc1Texture.getSpriteSheetTexture(), NULL, &renderQuadOpc1, 0.0, NULL, SDL_FLIP_NONE);
        SDL_Rect renderQuad0 = { (SCREEN_WIDTH - mensajeTexture.getWidth()) / 2, 50, mensajeTexture.getWidth(), mensajeTexture.getHeight() };
        SDL_RenderCopyEx(gRenderer, mensajeTexture.getSpriteSheetTexture(), NULL, &renderQuad0, 0.0, NULL, SDL_FLIP_NONE);
        SDL_Rect renderQuad1 = { (SCREEN_WIDTH - equipo1PromptTexture.getWidth()) / 2, 100, equipo1PromptTexture.getWidth(), equipo1PromptTexture.getHeight() };
        SDL_RenderCopyEx(gRenderer, equipo1PromptTexture.getSpriteSheetTexture(), NULL, &renderQuad1, 0.0, NULL, SDL_FLIP_NONE);
        SDL_Rect renderQuad11 = { (SCREEN_WIDTH - equipo1InputTexture.getWidth()) / 2, 150, equipo1InputTexture.getWidth(), equipo1InputTexture.getHeight() };
        SDL_RenderCopyEx(gRenderer, equipo1InputTexture.getSpriteSheetTexture(), NULL, &renderQuad11, 0.0, NULL, SDL_FLIP_NONE);
        SDL_Rect renderQuad2 = { (SCREEN_WIDTH - equipo2PromptTexture.getWidth()) / 2, 200, equipo2PromptTexture.getWidth(), equipo2PromptTexture.getHeight() };
        SDL_RenderCopyEx(gRenderer, equipo2PromptTexture.getSpriteSheetTexture(), NULL, &renderQuad2, 0.0, NULL, SDL_FLIP_NONE);
        SDL_Rect renderQuad22 = { (SCREEN_WIDTH - equipo2InputTexture.getWidth()) / 2, 250, equipo2InputTexture.getWidth(), equipo2InputTexture.getHeight() };
        SDL_RenderCopyEx(gRenderer, equipo2InputTexture.getSpriteSheetTexture(), NULL, &renderQuad22, 0.0, NULL, SDL_FLIP_NONE);
        SDL_RenderPresent(gRenderer);
    }
    seleccion = inputsIndex;
    log("openLoginEquipo: Sale de openLoginEquipo", LOG_INFO);
}

void openLoginFormacion(SDL_Renderer* gRenderer, int& seleccion, std::string mensaje, ConnectionManager* connectionManager) {
    log("openLoginFormacion: Entra al openLoginFormacion", LOG_INFO);
    bool quit = false;
    SDL_Event e;
    TTF_Font* gFont = NULL;
    gFont = TTF_OpenFont("lazy.ttf", 30);
    if (gFont == NULL) {
        log("openLoginFormacion: Error al cargar la fuente! SDL_ttf Error: ", TTF_GetError(), LOG_INFO);
    }
    Texture opc1Texture;
    opc1Texture.loadFromRenderedText("->", gRenderer, SDL_RED, gFont);
    Texture mensajeTexture;
    mensajeTexture.loadFromRenderedText(mensaje, gRenderer, SDL_RED, gFont);
    Texture formacion1InputTexture;
    formacion1InputTexture.loadFromRenderedText("3-3", gRenderer, SDL_BLUE, gFont);
    Texture formacion2InputTexture;
    formacion2InputTexture.loadFromRenderedText("3-1-2", gRenderer, SDL_BLUE, gFont);
    Texture formacion3InputTexture;
    formacion3InputTexture.loadFromRenderedText("3-2-1", gRenderer, SDL_BLUE, gFont);

    int inputsIndex = 0;
    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT || e.key.keysym.sym == SDLK_ESCAPE) {
                endProgram(1, connectionManager);
            } else if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_TAB) {
                    inputsIndex++;
                    if (inputsIndex >= 3) { inputsIndex = 0; }
                }
                if (e.key.keysym.sym == SDLK_RETURN) {
                    // enviar la configuracion al servidor
                    quit = true;
                }
            }
        }
        //Clear screen
        SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(gRenderer);
        //Render text textures
        SDL_Rect renderQuadOpc1 = { 250, 150 + (inputsIndex * 50), opc1Texture.getWidth(), opc1Texture.getHeight() };
        SDL_RenderCopyEx(gRenderer, opc1Texture.getSpriteSheetTexture(), NULL, &renderQuadOpc1, 0.0, NULL, SDL_FLIP_NONE);
        SDL_Rect renderQuad0 = { (SCREEN_WIDTH - mensajeTexture.getWidth()) / 2, 50, mensajeTexture.getWidth(), mensajeTexture.getHeight() };
        SDL_RenderCopyEx(gRenderer, mensajeTexture.getSpriteSheetTexture(), NULL, &renderQuad0, 0.0, NULL, SDL_FLIP_NONE);
        SDL_Rect renderQuad1 = { (SCREEN_WIDTH - formacion1InputTexture.getWidth()) / 2, 150, formacion1InputTexture.getWidth(), formacion1InputTexture.getHeight() };
        SDL_RenderCopyEx(gRenderer, formacion1InputTexture.getSpriteSheetTexture(), NULL, &renderQuad1, 0.0, NULL, SDL_FLIP_NONE);
        SDL_Rect renderQuad2 = { (SCREEN_WIDTH - formacion2InputTexture.getWidth()) / 2, 200, formacion2InputTexture.getWidth(), formacion2InputTexture.getHeight() };
        SDL_RenderCopyEx(gRenderer, formacion2InputTexture.getSpriteSheetTexture(), NULL, &renderQuad2, 0.0, NULL, SDL_FLIP_NONE);
        SDL_Rect renderQuad3 = { (SCREEN_WIDTH - formacion3InputTexture.getWidth()) / 2, 250, formacion3InputTexture.getWidth(), formacion3InputTexture.getHeight() };
        SDL_RenderCopyEx(gRenderer, formacion3InputTexture.getSpriteSheetTexture(), NULL, &renderQuad3, 0.0, NULL, SDL_FLIP_NONE);
        SDL_RenderPresent(gRenderer);
    }
    seleccion = inputsIndex;
    log("openLoginEquipo: Sale de openLoginFormacion", LOG_INFO);
}


void showLostConnectionMessage(SDL_Renderer* gRenderer) {
    log("showLostConnectionMessage: Se registro la salida por perdida de conexion. Mostrando mensaje.", LOG_INFO);
    SDL_Event e;
    TTF_Font* gFont = NULL;
    gFont = TTF_OpenFont("lazy.ttf", 30);
    if (gFont == NULL) {
        log("showLostConnectionMessage: Error al cargar la fuente! SDL_ttf Error: ", TTF_GetError(), LOG_INFO);
    }
    Texture line1Texture;
    line1Texture.loadFromRenderedText("Se perdio la conexion con el servidor!", gRenderer, SDL_BLUE, gFont);
    Texture line2Texture;
    line2Texture.loadFromRenderedText("Presionar 'escape' para salir...", gRenderer, SDL_BLUE, gFont);
    bool continueShowingMessage = true;
    while (continueShowingMessage) {
        if (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT || e.key.keysym.sym == SDLK_ESCAPE) {
                continueShowingMessage = false;
            }
        }
        //Clear screen
        SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(gRenderer);
        //Render text textures
        SDL_Rect renderQuad0 = { (SCREEN_WIDTH - line1Texture.getWidth()) / 2, 50, line1Texture.getWidth(), line1Texture.getHeight() };
        SDL_RenderCopyEx(gRenderer, line1Texture.getSpriteSheetTexture(), NULL, &renderQuad0, 0.0, NULL, SDL_FLIP_NONE);
        SDL_Rect renderQuad1 = { (SCREEN_WIDTH - line2Texture.getWidth()) / 2, 150, line2Texture.getWidth(), line2Texture.getHeight() };
        SDL_RenderCopyEx(gRenderer, line2Texture.getSpriteSheetTexture(), NULL, &renderQuad1, 0.0, NULL, SDL_FLIP_NONE);
        SDL_RenderPresent(gRenderer);
        usleep(1000);
    }
}

void openLoginEsperar(SDL_Renderer* gRenderer, std::string mensaje, std::string servidor, std::string puerto, std::string usuario, std::string equipo, ConnectionManager* connectionManager) {
    log("openLoginEsperar: Entra al openLogin Esperar", LOG_INFO);
    SDL_Event e;
    TTF_Font* gFont = NULL;
    gFont = TTF_OpenFont("lazy.ttf", 30);
    if (gFont == NULL) {
        log("openLoginEsperar: Error al cargar la fuente! SDL_ttf Error: ", TTF_GetError(), LOG_INFO);
    }
    Texture mensajeTexture;
    mensajeTexture.loadFromRenderedText(mensaje, gRenderer, SDL_RED, gFont);
    Texture line1Texture;
    line1Texture.loadFromRenderedText("Esperando al resto de los jugadores", gRenderer, SDL_BLUE, gFont);
    Texture line2Texture;
    line2Texture.loadFromRenderedText("para comenzar la partida...", gRenderer, SDL_BLUE, gFont);
    Texture line3Texture;
    line3Texture.loadFromRenderedText("Servidor: " + servidor + ":" + puerto, gRenderer, SDL_GREEN, gFont);
    Texture line4Texture;
    line4Texture.loadFromRenderedText("Usuario: " + usuario, gRenderer, SDL_GREEN, gFont);
    Texture line5Texture;
    line5Texture.loadFromRenderedText("Equipo: " + equipo, gRenderer, SDL_GREEN, gFont);
    // Espera a que el server mande el mensaje de que todos los jugadores estan listos.
    std::string beginMessage;
    int readBytes;
    bool gameBegins = false;
    while (!gameBegins) {
        if (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT || e.key.keysym.sym == SDLK_ESCAPE) {
                endProgram(1, connectionManager);
            }
        }
        Texture mensajeTexture;
        mensajeTexture.loadFromRenderedText(mensaje, gRenderer, SDL_RED, gFont);
        //Clear screen
        SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(gRenderer);
        //Render text textures
        SDL_Rect renderQuad0 = { (SCREEN_WIDTH - mensajeTexture.getWidth()) / 2, 50, mensajeTexture.getWidth(), mensajeTexture.getHeight() };
        SDL_RenderCopyEx(gRenderer, mensajeTexture.getSpriteSheetTexture(), NULL, &renderQuad0, 0.0, NULL, SDL_FLIP_NONE);
        SDL_Rect renderQuad1 = { (SCREEN_WIDTH - line1Texture.getWidth()) / 2, 150, line1Texture.getWidth(), line1Texture.getHeight() };
        SDL_RenderCopyEx(gRenderer, line1Texture.getSpriteSheetTexture(), NULL, &renderQuad1, 0.0, NULL, SDL_FLIP_NONE);
        SDL_Rect renderQuad2 = { (SCREEN_WIDTH - line2Texture.getWidth()) / 2, 200, line2Texture.getWidth(), line2Texture.getHeight() };
        SDL_RenderCopyEx(gRenderer, line2Texture.getSpriteSheetTexture(), NULL, &renderQuad2, 0.0, NULL, SDL_FLIP_NONE);
        SDL_Rect renderQuad3 = { (SCREEN_WIDTH - line3Texture.getWidth()) / 2, 350, line3Texture.getWidth(), line3Texture.getHeight() };
        SDL_RenderCopyEx(gRenderer, line3Texture.getSpriteSheetTexture(), NULL, &renderQuad3, 0.0, NULL, SDL_FLIP_NONE);
        SDL_Rect renderQuad4 = { (SCREEN_WIDTH - line4Texture.getWidth()) / 2, 400, line4Texture.getWidth(), line4Texture.getHeight() };
        SDL_RenderCopyEx(gRenderer, line4Texture.getSpriteSheetTexture(), NULL, &renderQuad4, 0.0, NULL, SDL_FLIP_NONE);
        SDL_Rect renderQuad5 = { (SCREEN_WIDTH - line5Texture.getWidth()) / 2, 450, line5Texture.getWidth(), line5Texture.getHeight() };
        SDL_RenderCopyEx(gRenderer, line5Texture.getSpriteSheetTexture(), NULL, &renderQuad5, 0.0, NULL, SDL_FLIP_NONE);
        SDL_RenderPresent(gRenderer);

        log("Main: Esperando el mensaje de comienzo del partido...", LOG_INFO);
        readBytes = connectionManager->getMessage(beginMessage);
        if (readBytes < 0) {
            log("Main: Error esperando el mensaje de comienzo del partido. Saliendo...", LOG_ERROR);
            showLostConnectionMessage(gRenderer);
            endProgram(1, connectionManager);
        } else if (readBytes == 0) {
            showLostConnectionMessage(gRenderer, connectionManager, true);
            log("Main: No se pudo establecer coneccion con el server. Esta el server andando?. Saliendo...", LOG_INFO);
            showLostConnectionMessage(gRenderer);
            endProgram(1, connectionManager);
        } else {
            if (beginMessage == "gameBegins:") {
                log("Main: Se recibio el mensaje de comienzo del partido.", LOG_INFO);
                gameBegins = true;
                // Silbato
                Mix_PlayChannel( -1, gWhistleSound, 0 );
            } else {
                log("Main: Esperando el mensaje de comienzo de partido, se recibio otra cosa: ", beginMessage, LOG_INFO);
                gameBegins = false;
            }
        }
    }
}

void showFullParty(SDL_Renderer* gRenderer, std::string message) {
    log("openDirtyExitMessage: Mostrando mensaje de partida llena.", LOG_INFO);
    SDL_Event e;
    TTF_Font* gFont = NULL;
    gFont = TTF_OpenFont("lazy.ttf", 30);
    if (gFont == NULL) {
        log("openDirtyExitMessage: Error al cargar la fuente! SDL_ttf Error: ", TTF_GetError(), LOG_INFO);
    }
    Texture line1Texture;
    line1Texture.loadFromRenderedText(message, gRenderer, SDL_BLUE, gFont);
    bool continueShowingMessage = true;
    while (continueShowingMessage) {
        if (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT || e.key.keysym.sym == SDLK_ESCAPE) {
                continueShowingMessage = false;
            }
        }
        //Clear screen
        SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(gRenderer);
        //Render text textures
        SDL_Rect renderQuad0 = { (SCREEN_WIDTH - line1Texture.getWidth()) / 2, 50, line1Texture.getWidth(), line1Texture.getHeight() };
        SDL_RenderCopyEx(gRenderer, line1Texture.getSpriteSheetTexture(), NULL, &renderQuad0, 0.0, NULL, SDL_FLIP_NONE);
        SDL_RenderPresent(gRenderer);
        usleep(1000);
    }
}

int main(int argc, char* argv[]) {
    if (chequearOpciones(argc, argv)) {
        //Si da 1 es o la version o el help o un flag inexistente.
        printf("Salida del programa por flags o argumento invalido\n");
        return SALIDA_LINEA_COMANDOS;
    }
    // Inicializacion -------------------------------------
    LOG_FILE_POINTER.open(logFileName, std::ofstream::app);
    logSessionStarted();
    // Inicializacion -------------------------------------
    log("Main: Cargando configuracion...", LOG_INFO);
    Conf* configuration = new Conf(defaultConfFileName, defaultSpritesFileName);
    configuration->loadConf(confFileName);
    int formation = configuration->getFormacion();
    log("Main: Configuracion cargada: ", configuration, LOG_INFO);
    int cliLogLevelInt = LOG_WRONGLEVEL;
    LOG_MIN_LEVEL = configuration->getDebugLevel();
    if (CLI_LOG_LEVEL != "") {
        cliLogLevelInt = getLogLevelFromString(CLI_LOG_LEVEL);
        if (cliLogLevelInt == LOG_WRONGLEVEL) {
            log("Main: Nivel de logeo especificado erroneo " + CLI_LOG_LEVEL + " usando default valor de config.", LOG_ERROR);
        } else {
            LOG_MIN_LEVEL = cliLogLevelInt;
        }
    }
    log("Main: Nivel de log cambiado a: ", getMessageLevelString(LOG_MIN_LEVEL), LOG_ERROR);
    initializer = new GameInitializer(configuration);
    ActionsManager* actionsManager = initializer->getActionsManager();
    SDL_Event e;
    int frameRate = configuration->getFramerate();
    float sleepTime = (float)200000 / (float)frameRate;
    log("Main: Frame rate: ", frameRate, LOG_INFO);
    delete(configuration);

    // Cargar archivos de audio
    gMusic = Mix_LoadMUS( "sounds/beat.wav" );
    if( gMusic == NULL ){
        printf( "Failed to load music! SDL_mixer Error: %s\n", Mix_GetError() );
        log( "Failed to load music! SDL_mixer Error: %s\n", Mix_GetError(), LOG_ERROR );
    }
    gKickSound = Mix_LoadWAV( "sounds/kick.wav" );
    if( gKickSound == NULL ){
        printf( "Failed to load sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
        log( "Failed to load sound effect! SDL_mixer Error: %s\n", Mix_GetError(), LOG_ERROR );
    }
    gGoalSound = Mix_LoadWAV( "sounds/goal.wav" );
    if( gGoalSound == NULL ){
        printf( "Failed to load sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
        log( "Failed to load sound effect! SDL_mixer Error: %s\n", Mix_GetError(), LOG_ERROR );
    }
    gWhistleSound = Mix_LoadWAV( "sounds/whistle.wav" );
    if( gWhistleSound == NULL ){
        printf( "Failed to load sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
        log( "Failed to load sound effect! SDL_mixer Error: %s\n", Mix_GetError(), LOG_ERROR );
    }
    gStartSound = Mix_LoadWAV( "sounds/start.wav" );
    if( gStartSound == NULL ){
        printf( "Failed to load sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
        log( "Failed to load sound effect! SDL_mixer Error: %s\n", Mix_GetError(), LOG_ERROR );
    }
    gCountdownSound = Mix_LoadWAV( "sounds/countdown.wav" );
    if( gCountdownSound == NULL ){
        printf( "Failed to load sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
        log( "Failed to load sound effect! SDL_mixer Error: %s\n", Mix_GetError(), LOG_ERROR );
    }
    // Comenzar a reproducir la musica
    Mix_PlayMusic( gMusic, -1 );

    ConnectionManager* connectionManager = new ConnectionManager();

    // Iniciar sesion. Elegir equipo y casaca. Usar el connectionManager para recibir
    // y mandar estos mensajes al server, y initializer tiene la pantalla de sdl para dibujar.
    // Esperar a que el server mande el mensaje de que todos los jugadores estan listos?

    // Login - determinar IP, Port, Usuario y Clave
    SDL_Renderer* renderer = initializer->getRenderer();

    std::string servidor = "127.0.0.1";
    if (CLI_IP != "") {
        log("Main: Usando la ip definida por linea de comando:", CLI_IP, LOG_INFO);
        servidor = CLI_IP;
    }

    std::string puerto = "8080";
    if (CLI_PORT != "") {
        log("Main: Usando el puerto definida por linea de comando:", CLI_PORT, LOG_INFO);
        puerto = CLI_PORT;
    }

    std::string usuario = "zidane";
    std::string clave = "zidane";
    std::string equipo = " ";
    int formacion = 0;
    std::string mensaje = " ";
    // Respuesta del server
    std::string message = "";
    int result;
    int seleccion = 0;

    bool connected = false;
    bool hasLoggedIn = false;
    bool hasPickedTeam = false;
    bool hasPickedFormation = false;
    bool fullParty = false;
    std::string mensajeError = "";

    while (!connected || !hasLoggedIn || !hasPickedTeam || !hasPickedFormation) {
        // Si no esta conectado, intenta:
        if (!connected) {
            openLoginServer(renderer, servidor, puerto, mensaje, connectionManager);
            log("SALIO DEL LOGIN SERVER", LOG_INFO);
            log("Main: Servidor: ", servidor, LOG_INFO);
            log("Main: Puerto: ", puerto, LOG_INFO);
            connectionManager->setIp(servidor);
            connectionManager->setPort(stoi(puerto));
            log("Main: Intentando conectar con el server...", LOG_INFO);
            connected = connectionManager->connectToServer();
            if (connected) {
                log("Main: Conectado, viendo si puedo entrar al juego... ", LOG_INFO);
                std:: string resultMessage = "";
                connectionManager->getMessage(resultMessage);
                std::string resultKey = resultMessage.substr(0, resultMessage.find(":"));
                std::string resultValue = resultMessage.substr(resultMessage.find(":") + 1, resultMessage.length());
                if (resultKey == "true") {
                    log("Main: Conexion aceptada.", LOG_INFO);
                } else {
                    log("Main: Conexion rechazada por falta de espacio.", LOG_INFO);
                    fullParty = true;
                }
            } else {
                log("Main: No se pudo conectar con el server.", LOG_INFO);
            }
        }
        // Estoy conectado?
        if (connected && !hasLoggedIn && !fullParty) {
            openLoginUsuario(renderer, servidor, puerto, usuario, clave, mensaje, connectionManager);
            log("Main: Intentando logear con credenciales: usuario = " + usuario + " y clave = " + clave , LOG_INFO);
            log("Main: Conectado con el servidor.", LOG_INFO);
            if (!hasLoggedIn) { // Este if esta de mas?
                // Esta bien mi clave?
                connectionManager->sendMessage(usuario + ":" + clave);
                message = "";
                result = connectionManager->getMessage(message);
                if (result < 0) {
                    log("Main: Error en la lectura del mensaje. ", LOG_ERROR);
                    // Reintenta
                } else if (result == 0) {
                    // Cuando ser cierra la coneccion del cliente lee 0 bytes sin control.
                    // Si puede pasar que la coneccion siga viva y haya un mensaje de 0 bytes hay que buscar otra vuelta.
                    log("Main: El server contesta cualquiera!", LOG_INFO);
                } else {
                    std::string resultKey = message.substr(0, message.find(":"));
                    std::string resultValue = message.substr(message.find(":") + 1, message.length());
                    if (resultKey == "true") {
                        hasLoggedIn = true;
                        log("Main: Credenciales validas. Logeo aceptado.", LOG_INFO);
                    } else if (resultKey == "false") {
                        log("Main: No se pudo logear con usuario = " + usuario + " y clave = " + clave + ". Server error: " + resultValue, LOG_INFO);
                        mensaje = resultValue;
                        hasLoggedIn = false;
                    }
                }
            }
        }
        if (connected && hasLoggedIn && !hasPickedTeam && !fullParty) {
            // Aca, esta conectado al server, y esta bien logueado
            // Tiene que elegir el equipo con el cual va a jugar
            if (mensajeError != "") {
                mensaje = mensajeError;
            } else {
                mensaje = "Elegir el equipo:";
            }
            openLoginEquipo(renderer, seleccion, mensaje, connectionManager);
            log("Main: El cliente eligio: ", seleccion, LOG_INFO);
            if (seleccion == 0) {
                equipo = "Argentina";
            } else {
                equipo = "Brasil";
            }
            // Le aviso al servidor cual fue el equipo elegido
            log("Main: Mandandole al server: ", LOG_INFO);
            // int formation = conf->getFormacion();
            connectionManager->sendMessage("use:" + std::to_string(seleccion));
            std::string resultMessage;
            connectionManager->getMessage(resultMessage);
            std::string resultKey = resultMessage.substr(0, resultMessage.find(":"));
            std::string resultValue = resultMessage.substr(resultMessage.find(":") + 1, resultMessage.length());
            if (resultKey == "true") {
                hasPickedTeam = true;
            } else if (resultKey == "withFormation") {
                hasPickedTeam = true;
                hasPickedFormation = true;
            } else if (resultKey == "false") {
                if (resultValue == "noRoom") {
                    log("Main: El servidor respondio que el equipo estaba lleno. Elegir otro.", LOG_INFO);
                    mensajeError = "Equipo lleno. Por favor, elegir otro.";
                } else {
                    log("Main: Finalizando por valor no entendido: ", resultValue, LOG_INFO);
                    endProgram(1, connectionManager);
                }
            } else {
                log("Main: Finalizando por clave no entendida. ", resultKey, LOG_INFO);
                endProgram(1, connectionManager);
            }
        }

        if (connected && hasLoggedIn && hasPickedTeam && !hasPickedFormation && !fullParty) {
            // Aca, esta conectado al server, y esta bien logueado
            // Tiene que elegir el equipo con el cual va a jugar
            if (mensajeError != "") {
                mensaje = mensajeError;
            } else {
                mensaje = "Elegir Formacion:";
            }
            openLoginFormacion(renderer, seleccion, mensaje, connectionManager);
            log("Main: El cliente eligio: ", seleccion, LOG_INFO);
            if (seleccion == 0) {
                formacion = 33;
            } else if (seleccion == 1) {
                formacion = 312;
            } else if (seleccion == 2) {
                formacion = 321;
            }
            // Le aviso al servidor cual fue el equipo elegido
            log("Main: Mandandole al server: ", LOG_INFO);
            connectionManager->sendMessage("formation:" + std::to_string(formacion));
            std::string resultMessage;
            connectionManager->getMessage(resultMessage);
            std::string resultKey = resultMessage.substr(0, resultMessage.find(":"));
            std::string resultValue = resultMessage.substr(resultMessage.find(":") + 1, resultMessage.length());
            if (resultKey == "true") {
                hasPickedFormation = true;
            } else if (resultKey == "false") {
                if (resultValue == "noRoom") {
                    log("Main: El servidor respondio que el equipo estaba lleno. Elegir otro.", LOG_INFO);
                    mensajeError = "Equipo lleno. Por favor, elegir otro.";
                } else {
                    log("Main: Finalizando por valor no entendido: ", resultValue, LOG_INFO);
                    endProgram(1, connectionManager);
                }
            } else {
                log("Main: Finalizando por clave no entendida. ", resultKey, LOG_INFO);
                endProgram(1, connectionManager);
            }
        }


        if (connected && hasLoggedIn && hasPickedTeam && hasPickedFormation) {
            // optimus! esta conectado, logueado y con equipo ya seleccionado
            mensaje = "...";
            openLoginEsperar(renderer, mensaje, servidor, puerto, usuario, equipo, connectionManager);
        }
        if (fullParty) {
            showFullParty(renderer, "La partida ya esta llena.");
            endProgram(1, connectionManager);
        }
    }

    // Abajo se Lanza thread que recibe mensajes de estado de juego.
    ThreadSpawner* threadSpawner = new ThreadSpawner();
    threadSpawner->spawn(
        read_server,
        connectionManager
    );
    log("Main: Juego inicializado correctamente.", LOG_INFO);
    // Lanzar thread que dibuja el juego.
    threadSpawner->spawn(drawer, NULL);
    // Main loop ------------------------------------------
    log("Main: Entrando en el main loop...", LOG_INFO);
    // Este thread escucha los eventos de teclado y se los manda al server.
    bool sendSuccess = false;
    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (actionsManager->shouldQuit(e)) {
                setQuit(true);
            } else {
                // Devuelve acciones que modifican modelos.
                // Se puede optimizar para que deje de hacer actions todo el tiempo.
                Action* action = actionsManager->getAction(e);
                if ((action != NULL) && (initializer->getGameController()->state != GAME_START_STATE) ) {
                    sendSuccess = connectionManager->sendMessage(action->toString());
                    if (!sendSuccess) {
                        log("Main: Se detecto error en el envio del mensaje. Saliendo...", LOG_ERROR);
                        lostConnectionQuit = true;
                        setQuit(true);
                    }

                    delete(action);
                }
            }
        }
        if (lostConnectionQuit) {
            showLostConnectionMessage(renderer, connectionManager, true);
        }
        usleep(sleepTime/8);
    }

    // Si se sale por pedida de conexion mostrar mensaje.
    if (quit) {
        showLostConnectionMessage(renderer, connectionManager, false);
    }

    // threads->terminateSpawnedThreads(); // signalea a los threads para que terminen.
    threadSpawner->joinSpawnedThreads();
    log("Main: Main loop finalizado.", LOG_INFO);
    delete(threadSpawner);
    // Liberacion de memoria -------------------------------
    endProgram(0, connectionManager);
}
