#include <SDL2/SDL.h>
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

#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define PORT 8080

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
    GameInitializer* initializer = new GameInitializer(configuration);
    GameController* gameController = initializer->getGameController();
    ActionsManager* actionsManager = initializer->getActionsManager();
    Camera* camera = initializer->getCamera();
    SDL_Renderer* renderer = initializer->getRenderer();
    PitchView* pitchView = initializer->getPitchView();
    bool quit = false;
    SDL_Event e;
    int frameRate = configuration->getFramerate();
    float sleepTime = (float)200000/(float)frameRate;
    log("Main: Frame rate: ", frameRate, LOG_INFO);
    delete(configuration);
    log("Main: Juego inicializado correctamente.", LOG_INFO);

    // Client code. Will be moved.
    int sock = 0;
    struct sockaddr_in serv_addr;
    log("sock", LOG_INFO);
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        log("\n Socket creation error ", LOG_ERROR);
        return -1;
    }
    log("memset", LOG_INFO);
    memset(&serv_addr, '0', sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    // Convert IPv4 and IPv6 addresses from text to binary form
    log("inet_pton", LOG_INFO);
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        log("Invalid address or Address not supported ", LOG_ERROR);
        return -1;
    }

    log("connect", LOG_INFO);
    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        log("\nConnection Failed ", LOG_ERROR);
        return -1;
    }
    int shouldEnd = 0;
    log("while", LOG_INFO);
    while (shouldEnd != 1) {
        send(sock, "Hello from client", strlen("Hello from client") , 0);
        std::cout << "Continue? ";
        std::cin >> shouldEnd;
        std::cout << std::endl;
    }

    // Liberacion de memoria -------------------------------
    delete(initializer);
    logSessionFinished();
    LOG_FILE_POINTER.close();
    return 0;
}

/*
    // Main loop ------------------------------------------
    log("Main: Entrando en el main loop...", LOG_INFO);
    while (!quit) {

        while (SDL_PollEvent(&e) != 0) {
            if (actionsManager->shouldQuit(e)) {
                quit = true;
            } else {
                // Devuelve acciones que modifican modelos.
                // Se puede optimizar para que deje de hacer actions todo el tiempo.
                Action* action = actionsManager->getAction(e);
                if (action != NULL) {
                    gameController->execute(action);
                    delete(action);
                }
            }
        }
        gameController->updatePlayers();
        gameController->updateCameraPosition(camera);
        pitchView->render(renderer);
        usleep(sleepTime); // Frame rate.
    }
    log("Main: Main loop finalizado.", LOG_INFO);
*/
