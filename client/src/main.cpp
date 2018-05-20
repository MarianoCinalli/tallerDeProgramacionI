#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
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
const std::string defaultConfFileName = "src/default.yaml";
const std::string defaultSpritesFileName = "images/spritesDefaults.png";
std::string confFileName = "conf.yaml";
int LOG_MIN_LEVEL = LOG_DEBUG; // Dejarlo asi para que cuando empieze loggee todo.
std::string CLI_LOG_LEVEL = "";
std::mutex log_mutex;
GameInitializer* initializer;
std::mutex quit_mutex;
bool quit = false;
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
    cout << "Ejemplo:\n";
    cout << "main -i ~/conf.yaml -l info\n";
    cout << "\nREFERENCIA\nCon las flechas: se corre, \nCon W sprint, \nCon A barrida, \nCon S: patear, \nCon BARRA: cambiar de jugador";
}

void imprimir_version() {
    cout << "Version = tp1\n";
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
            {0, 0, 0, 0}
        };

        int option_index = 0;

        ch = getopt_long(argc, argv, "vhi:l:",
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

            default:
                printf("Opcion invalida\n\n");
                return 1;
        }
    }
    if (tempConf != "") {
        confFileName = tempConf;
    } else {
        confFileName = "conf.yaml";
    }
    return 0;
}

void endProgram(int statusToExit, ConnectionManager* connectionManager) {
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

void openLoginServer(SDL_Renderer* gRenderer, std::string& servidor, std::string& puerto, std::string mensaje) {
    log("Entra al openLogin Server", LOG_INFO);
    bool quit = false;
    SDL_Event e;
    TTF_Font* gFont = NULL;
    gFont = TTF_OpenFont("lazy.ttf", 30);
    if (gFont == NULL) {
        log("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError(), LOG_INFO);
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
    //Enable text input
    SDL_StartTextInput();
    //While application is running
    while (!quit) {
        bool renderText = false;
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT || e.key.keysym.sym == SDLK_ESCAPE) {
                exit(1);
            } else if (e.type == SDL_KEYDOWN) {
                //Handle backspace
                if (e.key.keysym.sym == SDLK_BACKSPACE && inputs[inputsIndex].length() > 0) {
                    //lop off character
                    inputs[inputsIndex].pop_back();
                    renderText = true;
                }
                if (e.key.keysym.sym == SDLK_TAB) {
                    //cambiar posicion
                    inputsIndex++;
                    if (inputsIndex >= 2) { inputsIndex = 0; }
                }
                if (e.key.keysym.sym == SDLK_RETURN) {
                    //TODO: enviar la configuracion al servidor
                    quit = true;
                }
            } else if (e.type == SDL_TEXTINPUT) {
                //Append character
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

void openLoginUsuario(SDL_Renderer* gRenderer, std::string& servidor, std::string& puerto, std::string& usuario, std::string& clave, std::string mensaje) {
    log("Entra al openLogin Usuario", LOG_INFO);
    bool quit = false;
    SDL_Event e;
    TTF_Font* gFont = NULL;
    gFont = TTF_OpenFont("lazy.ttf", 30);
    if (gFont == NULL) {
        log("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError(), LOG_INFO);
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
    //Enable text input
    SDL_StartTextInput();
    //While application is running
    while (!quit) {
        bool renderText = false;
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT || e.key.keysym.sym == SDLK_ESCAPE) {
                exit(1);
            } else if (e.type == SDL_KEYDOWN) {
                //Handle backspace
                if (e.key.keysym.sym == SDLK_BACKSPACE && inputs[inputsIndex].length() > 0) {
                    //lop off character
                    inputs[inputsIndex].pop_back();
                    renderText = true;
                }
                if (e.key.keysym.sym == SDLK_TAB) {
                    //cambiar posicion
                    inputsIndex++;
                    if (inputsIndex >= 2) { inputsIndex = 0; }
                }
                if (e.key.keysym.sym == SDLK_RETURN) {
                    //TODO: enviar la configuracion al servidor
                    quit = true;
                }
            } else if (e.type == SDL_TEXTINPUT) {
                //Append character
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

void openLoginEquipo(SDL_Renderer* gRenderer, int& seleccion, int max, std::string equipo1, std::string cantidad1, std::string equipo2, std::string cantidad2, std::string mensaje) {
    log("Entra al openLogin Equipo", LOG_INFO);
    bool quit = false;
    SDL_Event e;
    TTF_Font* gFont = NULL;
    gFont = TTF_OpenFont("lazy.ttf", 30);
    if (gFont == NULL) {
        log("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError(), LOG_INFO);
    }
    Texture opc1Texture;
    opc1Texture.loadFromRenderedText("->", gRenderer, SDL_RED, gFont);

    Texture mensajeTexture;
    mensajeTexture.loadFromRenderedText(mensaje, gRenderer, SDL_RED, gFont);
    Texture equipo1PromptTexture;
    equipo1PromptTexture.loadFromRenderedText("Equipo 1:", gRenderer, SDL_BLACK, gFont);
    Texture equipo2PromptTexture;
    equipo2PromptTexture.loadFromRenderedText("Equipo 2:", gRenderer, SDL_BLACK, gFont);

    Texture equipo1InputTexture;
    equipo1InputTexture.loadFromRenderedText(equipo1+" ("+cantidad1+")", gRenderer, SDL_BLUE, gFont);
    Texture equipo2InputTexture;
    equipo2InputTexture.loadFromRenderedText(equipo2+" ("+cantidad2+")", gRenderer, SDL_BLUE, gFont);

    int inputsIndex = 0;
    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT || e.key.keysym.sym == SDLK_ESCAPE) {
                exit(1);
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

    ConnectionManager* connectionManager = new ConnectionManager();

    // Iniciar sesion. Elegir equipo y casaca. Usar el connectionManager para recibir
    // y mandar estos mensajes al server, y initializer tiene la pantalla de sdl para dibujar.
    // Esperar a que el server mande el mensaje de que todos los jugadores estan listos?

    // Login - determinar IP, Port, Usuario y Clave
    SDL_Renderer* renderer = initializer->getRenderer();

    std::string servidor = "127.0.0.1";
    std::string puerto = "8080";
    std::string usuario = "zidane";
    std::string clave = "zidane";
    std::string player = " ";
    std::string equipo = " ";
    std::string mensaje = " ";
    // Respuesta del server
    std::string message = "";
    int result;
    int max = 0;
    std::string equipo1 = "";
    std::string equipo2 = "";
    std::string cantidad1str = "0";
    int cantidad1 = 0;
    std::string cantidad2str = "0";
    int cantidad2 = 0;

    bool connected = false;
    bool hasLoggedIn = false;
    bool hasPickedTeam = false;

    while (!connected || !hasLoggedIn || !hasPickedTeam) {
        // Si no esta conectado, intenta:
        if (!connected) {
            openLoginServer(renderer, servidor, puerto, mensaje);
            log("SALIO DEL LOGIN SERVER", LOG_INFO);
            log("Login: Servidor: ", servidor, LOG_INFO);
            log("Login: Puerto: ", puerto, LOG_INFO);
            connectionManager->setIp(servidor);
            connectionManager->setPort(stoi(puerto));
            connected = connectionManager->connectToServer();
        }
        // Estoy conectado?
        if (connected) {
          openLoginUsuario(renderer, servidor, puerto, usuario, clave, mensaje);
          log("SALIO DEL LOGIN USUARIO", LOG_INFO);
          log("Login: Usuario: ", usuario, LOG_INFO);
          log("Login: Clave: ", clave, LOG_INFO);
          log("Main: Conectado con el servidor.", LOG_INFO);
          if (!hasLoggedIn) {
            // Esta bien mi clave?
            connectionManager->sendMessage(usuario+":"+clave);
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
              std::string logged = message.substr(message.find(":")+1, message.length());
              log("Main: Resultado del login: ", logged, LOG_INFO);
              if ( logged == "true" ){
                hasLoggedIn = true;
                log("Main: Joya, esta logueado.", logged, LOG_INFO);
              } else if ( logged == "false" ) {
                log("Main: Mal, usuario o clave incorrecto.", logged, LOG_INFO);
                mensaje = "Credenciales incorrectas.";
                hasLoggedIn = false;
              }
            }
          }
          if (hasLoggedIn && !hasPickedTeam) {
            // Aca, esta conectado al server, y esta bien logueado
            // Tiene que elegir el equipo con el cual va a jugar
            // Cuantos jugadores acepta la partida?
            connectionManager->sendMessage("get:max");
            message = "";
            result = connectionManager->getMessage(message);
            if (result < 0) {
              log("Main: Error en la lectura del mensaje. ", LOG_ERROR);
            } else if (result == 0) {
              log("Main: El server contesta cualquiera!", LOG_INFO);
            } else {
              std::string key = message.substr(0, message.find(":"));
              std::string value = message.substr(message.find(":")+1, message.length());
              if (key=="max") {
                  max = stoi(value);
              }
              log("Main: Maximo: ", max, LOG_INFO);
            }
            // Cuales son los equipos posibles?
            // Estado del equipo 1
            connectionManager->sendMessage("get:equipo1");
            message = "";
            result = connectionManager->getMessage(message);
            if (result < 0) {
              log("Main: Error en la lectura del mensaje. ", LOG_ERROR);
            } else if (result == 0) {
              log("Main: El server contesta cualquiera!", LOG_INFO);
            } else {
              equipo1 = message.substr(0, message.find(":"));
              cantidad1str = message.substr(message.find(":")+1, message.length());
              cantidad1 = stoi(cantidad1str);
              log("Main: Equipo1: ", equipo1, LOG_INFO);
              log("Main: Cant usuarios: ", cantidad1str, LOG_INFO);
            }
            // Estado del equipo 2
            connectionManager->sendMessage("get:equipo2");
            message = "";
            result = connectionManager->getMessage(message);
            if (result < 0) {
              log("Main: Error en la lectura del mensaje. ", LOG_ERROR);
            } else if (result == 0) {
              log("Main: El server contesta cualquiera!", LOG_INFO);
            } else {
              equipo2 = message.substr(0, message.find(":"));
              cantidad2str = message.substr(message.find(":")+1, message.length());
              cantidad2 = stoi(cantidad2str);
              log("Main: Equipo2: ", equipo2, LOG_INFO);
              log("Main: Cant usuarios: ", cantidad2str, LOG_INFO);
            }
            if (cantidad1 + cantidad2 < max) {
              int seleccion = 0;
              mensaje = "Elegir el equipo:";
              openLoginEquipo(renderer, seleccion, max, equipo1, cantidad1str, equipo2, cantidad2str, mensaje);
              log("Main: Elige el equipo: ", seleccion, LOG_INFO);
              // Le aviso al servidor cual fue el equipo elegido
              connectionManager->sendMessage("use:"+std::to_string(seleccion));
              // Espero la validacion
              //if (result == "ok") {
                // Grabamos el player number
                hasPickedTeam = true;
              //} else {
              //  mensaje = "La partida ya esta llena.";
              //}
            } else {
              mensaje = "Ocurrio un error. Se llenó la partida?";
            }
            exit(1);

          }
        } else {
          mensaje = "No se pudo conectar con el servidor.";
          log("Main: No se pudo conectar con el servidor", LOG_INFO);
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
    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (actionsManager->shouldQuit(e)) {
                setQuit(true);
            } else {
                // Devuelve acciones que modifican modelos.
                // Se puede optimizar para que deje de hacer actions todo el tiempo.
                Action* action = actionsManager->getAction(e);
                if (action != NULL) {
                    connectionManager->sendMessage(action->toString());
                    delete(action);
                }
            }
        }
        usleep(sleepTime);
    }
    // threads->terminateSpawnedThreads(); // signalea a los threads para que terminen.
    threadSpawner->joinSpawnedThreads();
    log("Main: Main loop finalizado.", LOG_INFO);
    delete(threadSpawner);
    // Liberacion de memoria -------------------------------
    endProgram(0, connectionManager);
}
