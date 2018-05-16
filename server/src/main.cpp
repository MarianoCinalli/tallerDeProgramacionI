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
#include "util/functions.h"
#include "controller/ActionsManager.h"
#include "controller/GameController.h"
#include "util/ConnectionManager.h"
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
std::mutex update_model_mutex;
GameInitializer* initializer;
ConnectionManager* connectionManager;
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
            { "version",  no_argument,   0, 'v' },
            { "help",     no_argument,   0, 'h' },
            { "input",    required_argument, 0, 'i' },
            { "logLevel", required_argument, 0, 'l' },
            { 0,          0,         0, 0   }
        };

        int option_index = 0;

        ch = getopt_long(argc, argv, "vhi:l:",
                         long_options, &option_index);

        /* Detecta fin de opciones. */
        if (ch == -1)
            break;

        switch (ch) {
            case 0:
                if (long_options[option_index].flag != 0)
                    break;
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
    if (tempConf != "")
        confFileName = tempConf;
    else
        confFileName = "conf.yaml";
    return 0;
}


int main(int argc, char* argv[]) {
    if (chequearOpciones(argc, argv)) {
        //Si da 1 es o la version o el help o un flag inexistente.
        printf("Salida del programa por flags o argumento invalido.\n");
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
    connectionManager = initializer->getConnectionManager();
    if(!connectionManager->openConnections()) {
        log("Main: No se pudo abrir la conexion.", LOG_ERROR);
        delete(initializer);
        exit(1);
    }
    ThreadSpawner* threads = new ThreadSpawner();
    log("Main: Juego inicializado correctamente.", LOG_INFO);

    connectionManager->acceptConnectionsUntilMax(); // Pasarle el gameControllerProxy.
    // Launch game_updater thread.
    threads->spawn(
        game_updater,
        NULL
    );
    connectionManager->createBroadcaster();
    // Wait gameManager thread.
    // Signal children threads.
    // Wait children threads.
    connectionManager->waitForAllConnectionsToFinish();
    threads->joinSpawnedThreads();
    connectionManager->closeOpenedSockets();
    // End ------------------------------------------------
    delete(configuration);
    delete(threads);
    delete(initializer);
    logSessionFinished();
    LOG_FILE_POINTER.close();
    return 0;
}
