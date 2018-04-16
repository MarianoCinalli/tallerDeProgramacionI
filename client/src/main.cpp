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

// Global variables ---------------------------------------
std::ofstream LOG_FILE_POINTER;
const std::string logFileName = "trabajoPractico.log";
const std::string defaultConfFileName = "src/default.yaml";
std::string confFileName = "conf.yaml";
int LOG_MIN_LEVEL = LOG_DEBUG; // Dejarlo asi para que cuando empieze loggee todo.
// Global variables ---------------------------------------


void imprimir_ayuda() {
    cout << "Usage:\n";
    cout << "main -h\n";
    cout << "main -V\n";
    cout << "main [options]\n";
    cout << "-V, --version      Imprimir version y salir.\n";
    cout << "-h, --help     Imprimir esta ayuda.\n";
    cout << "-i, --input        Path del archivo de configuracion YAML.\n";
    cout << "Ejemplo:\n";
    cout << "main -i ~/conf.yaml \n";
}

void imprimir_version() {
    cout << "Version = tp1\n";
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

int main(int argc, char* argv[]) {
    if (chequearOpciones(argc, argv)) {
        //Si da 1 es o la version o el help o un flag inexistente.
        printf("Salida del programa por flags o argumento invalido");
        return SALIDA_LINEA_COMANDOS;
    }
    // Inicializacion -------------------------------------
    LOG_FILE_POINTER.open(logFileName, std::ofstream::app);
    logSessionStarted();
    // Inicializacion -------------------------------------
    log("Main: Cargando configuracion...", LOG_INFO);
    Conf* configuration = new Conf(defaultConfFileName);
    configuration->loadConf(confFileName);
    log("Main: Configuracion cargada: ", configuration, LOG_INFO);
    LOG_MIN_LEVEL = configuration->getDebugLevel();
    log("Main: Nivel de log cambiado a: ", getMessageLevelString(LOG_MIN_LEVEL), LOG_INFO);
    GameInitializer* initializer = new GameInitializer(configuration);
    GameController* gameController = initializer->getGameController();
    ActionsManager* actionsManager = initializer->getActionsManager();
    Camera* camera = initializer->getCamera();
    SDL_Renderer* renderer = initializer->getRenderer();
    PitchView* pitchView = initializer->getPitchView();
    bool quit = false;
    SDL_Event e;
    int frameRate = configuration->getFramerate();
    float sleepTime = (float)20000/(float)frameRate;
    log("Main: Frame rate: ", frameRate, LOG_INFO);
    delete(configuration);
    log("Main: Juego inicializado correctamente.", LOG_INFO);
    // Main loop ------------------------------------------
    log("Main: Entrando en el main loop...", LOG_INFO);
    while (!quit) {

        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
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
        usleep(7 * sleepTime); // Frame rate.
    }
    log("Main: Main loop finalizado.", LOG_INFO);
    // Liberacion de memoria -------------------------------
    delete(initializer);
    logSessionFinished();
    LOG_FILE_POINTER.close();
    return 0;
}
