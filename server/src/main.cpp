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
// #include "controller/ActionsManager.h"
#include "controller/GameController.h"
#include "util/ConnectionManager.h"
#include <mutex>
#include <pthread.h>

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
std::mutex quit_mutex;
std::mutex users_mutex;
pthread_barrier_t players_ready_barrier;
GameInitializer* initializer;
ConnectionManager* connectionManager;
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
    cout << "Version = tp2\n";
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

    // El + 2 es por la cantidad extra de threads.
    int status = pthread_barrier_init(&players_ready_barrier, NULL, configuration->getMaxClients() + 2);
    if (status != 0) {
        log("Main: No se pudo inicializar la barrera.", strerror(status), LOG_ERROR);
        delete(initializer);
        exit(1);
    }

    if(!connectionManager->openConnections()) {
        log("Main: No se pudo abrir la conexion.", LOG_ERROR);
        pthread_barrier_destroy(&players_ready_barrier);
        delete(initializer);
        exit(1);
    }
    ThreadSpawner* threads = new ThreadSpawner();
    log("Main: Juego inicializado correctamente.", LOG_INFO);

    // Se queda esperando por conexiones.
    threads->spawn(
        connection_listener,
        NULL
    );
    // Cuando todos se conecten, actualiza el modelo.
    threads->spawn(
        game_updater,
        NULL
    );
    // Cuando todos se conecten, envia el modelo a los players.
    threads->spawn(
        broadcast_to_clients,
        NULL
    );
    // End ------------------------------------------------
    // Espero a que terminen los threads de arriba.
    threads->joinSpawnedThreads();
    // Espero a que terminen todos los threads de clientes.
    connectionManager->waitForAllConnectionsToFinish();
    connectionManager->closeOpenedSockets();
    pthread_barrier_destroy(&players_ready_barrier);
    delete(configuration);
    delete(threads);
    delete(initializer);
    logSessionFinished();
    LOG_FILE_POINTER.close();
    return 0;
}
