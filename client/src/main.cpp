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

void openLogin(SDL_Renderer* gRenderer, std::string& servidor, std::string& puerto, std::string& usuario, std::string& clave, std::string mensaje) {
    log("Entra al openLogin", LOG_INFO);

    bool quit = false;
    SDL_Event e;

    // Colores
    SDL_Color SDL_BLACK = { 0, 0, 0, 0xFF };
    SDL_Color SDL_RED = { 0xFF, 0, 0, 0xFF };
    SDL_Color SDL_GREEN = { 0, 0xFF, 0, 0xFF };
    SDL_Color SDL_BLUE = { 0, 0, 0xFF, 0xFF };
    SDL_Color SDL_WHITE = { 0xFF, 0xFF, 0xFF, 0xFF };

    TTF_Font* gFont = NULL;
    gFont = TTF_OpenFont("lazy.ttf", 30);
    if (gFont == NULL) {
        log("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError(), LOG_INFO);
    }

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

    std::string inputs [4] = { servidor, puerto, usuario, clave};

    Texture servidorInputTexture;
    servidorInputTexture.loadFromRenderedText(inputs[0], gRenderer, SDL_BLUE, gFont);
    Texture puertoInputTexture;
    puertoInputTexture.loadFromRenderedText(inputs[1], gRenderer, SDL_BLUE, gFont);
    Texture usuarioInputTexture;
    usuarioInputTexture.loadFromRenderedText(inputs[2], gRenderer, SDL_BLUE, gFont);
    Texture claveInputTexture;
    claveInputTexture.loadFromRenderedText(inputs[3], gRenderer, SDL_BLUE, gFont);

    int inputsIndex = 0;
    std::string hidden = "*****";

    //Enable text input
    SDL_StartTextInput();

    //While application is running
    while (!quit) {
        bool renderText = false;
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT || e.key.keysym.sym == SDLK_ESCAPE) {
                quit = true;
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
                    if (inputsIndex >= 4) { inputsIndex = 0; }
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
            for (int i = 0; (unsigned)i < inputs[3].size(); i++) {
                hidden += "*";
            }
            // if (hidden == "") { hidden = " "; }
            // if (inputs[0] == "") { inputs[inputsIndex] = " "; }
            // if (inputs[1] == "") { inputs[inputsIndex] = " "; }
            // if (inputs[2] == "") { inputs[inputsIndex] = " "; }
            // if (inputs[3] == "") { inputs[inputsIndex] = " "; }
            servidorInputTexture.loadFromRenderedText(inputs[0].c_str(), gRenderer, SDL_BLUE, gFont);
            puertoInputTexture.loadFromRenderedText(inputs[1].c_str(), gRenderer, SDL_BLUE, gFont);
            usuarioInputTexture.loadFromRenderedText(inputs[2].c_str(), gRenderer, SDL_BLUE, gFont);
            claveInputTexture.loadFromRenderedText(hidden.c_str(), gRenderer, SDL_BLUE, gFont);
        }

        //Clear screen
        SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(gRenderer);

        //Render text textures
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

        //gPromptTextTexture.render( ( SCREEN_WIDTH - gPromptTextTexture.getWidth() ) / 2, 0 );
        //SDL_Rect renderQuad2 = { ( SCREEN_WIDTH - gInputTextTexture.getWidth() ) / 2, 0, 100, 100 };
        //SDL_RenderCopyEx( gRenderer, gInputTextTexture.getSpriteSheetTexture(), NULL, &renderQuad2, 0.0, NULL, SDL_FLIP_NONE );
        //gInputTextTexture.render( ( SCREEN_WIDTH - gInputTextTexture.getWidth() ) / 2, gPromptTextTexture.getHeight() );

        //Update screen
        SDL_RenderPresent(gRenderer);
    }
    //Disable text input
    SDL_StopTextInput();

    servidor = inputs[0];
    puerto = inputs[1];
    usuario = inputs[2];
    clave = inputs[3];
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

    std::string servidor="127.0.0.1";
    std::string puerto="8080";
    std::string usuario="zidane";
    std::string clave="****";
    std::string mensaje = " ";
    bool quitLogin = false;
    while (!quitLogin) {
        openLogin(renderer, servidor, puerto, usuario, clave, mensaje);
        log("SALIO DEL LOGIN", LOG_INFO);
        log("Login: Servidor: ", servidor, LOG_INFO);
        log("Login: Puerto: ", puerto, LOG_INFO);
        log("Login: Usuario: ", usuario, LOG_INFO);
        log("Login: Clave: ", clave, LOG_INFO);
        connectionManager->setIp(servidor);
        connectionManager->setPort(stoi(puerto));

        //validarServidor(config);
        if (!connectionManager->connectToServer()) {
            log("Main: No se pudo abrir la conexion.", LOG_INFO);
            mensaje = "No se pudo conectar con el servidor.";
            //endProgram(1, connectionManager);
        } else {
            log("Main: Conectado con el servidor.", LOG_INFO);
            connectionManager->sendMessage(usuario+":"+clave);
            std::string message;
            int result = connectionManager->getMessage(message);
            log("VARIABLE RESULT:",result, LOG_INFO);
            std::string logged = message.substr(message.find(":")+1, message.length());
            log("VARIABLE LOGGED:",logged, LOG_INFO);
            if ( logged == "true" ){
              mensaje ="usuario logueado";
              quitLogin = true;
            } else if ( logged == "false" ) {
              mensaje ="clave incorrecta";
              //connectionManager->closeConnection();
              quitLogin = false;
            }

            //log("Main: Validando credenciales.", LOG_INFO);
            //validarCredenciales();
            //mensaje = "Usuario y/o clave incorrectos.";

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
