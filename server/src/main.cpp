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

#include <stdlib.h>
// For server
#include <sys/socket.h>
#include <netinet/in.h>
// For threads
#include <pthread.h>

#define NUM_THREADS 5
#define PORT 8080


// Global variables ---------------------------------------
std::ofstream LOG_FILE_POINTER;
const std::string logFileName = "trabajoPractico.log";
const std::string defaultConfFileName = "src/default.yaml";
const std::string defaultSpritesFileName = "images/spritesDefaults.png";
std::string confFileName = "conf.yaml";
int LOG_MIN_LEVEL = LOG_DEBUG; // Dejarlo asi para que cuando empieze loggee todo.
std::string CLI_LOG_LEVEL = "";
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

extern int errno;
#define SERVER_PORT 4321


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
        if (cliLogLevelInt == LOG_WRONGLEVEL)
            log("Main: Nivel de logeo especificado erroneo " + CLI_LOG_LEVEL + " usando default valor de config.", LOG_ERROR);
        else
            LOG_MIN_LEVEL = cliLogLevelInt;
    }
    log("Main: Nivel de log cambiado a: ", getMessageLevelString(LOG_MIN_LEVEL), LOG_ERROR);
    delete(configuration);
    log("Main: Juego inicializado correctamente.", LOG_INFO);
    // Main loop ------------------------------------------
    // Crear game manager.
    // Crear broadcaster.
    // Crear server.

    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    LOG_FILE_POINTER.open(logFileName, std::ofstream::app);

    // Creating socket file descriptor
    log("socket\n", LOG_INFO);
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        log("socket failed\n", LOG_ERROR);
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080
    log("setsockopt\n", LOG_INFO);
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        log("setsockopt failed\n", LOG_ERROR);
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Forcefully attaching socket to the port 8080
    log("bind\n", LOG_INFO);
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        log("bind failed\n", LOG_ERROR);
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0) {
        log("listen error\n", LOG_ERROR);
        exit(EXIT_FAILURE);
    }
    pthread_t clients[4];
    int sockets[4];
    int result_code;
    int index = 0;
    // Launch thread for each connection.
    log("listening\n", LOG_INFO);
    while (index < 4) {
        log("accepting\n", LOG_INFO);
        if ((new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) > 0) {
            log("accepted\n", LOG_INFO);
            sockets[index] = new_socket;
            result_code = pthread_create(&clients[index], NULL, readClient, &sockets[index]);
            log("%d - %d\n", result_code, index, LOG_INFO);
            ++index;
        } else {
            log("rejected\n", LOG_ERROR);
        }
    }
    // wait for each thread to complete
    for (int index = 0; index < NUM_THREADS; ++index) {
        log("join\n", LOG_INFO);
        // block until thread 'index' completes
        result_code = pthread_join(clients[index], NULL);
        log("In main: thread %d has completed\n", index, LOG_INFO);
    }
    // Close open sockets.
    for (int index = 0; index < NUM_THREADS; ++index) {
        close(sockets[index]);
        log("In main: socket %d closed\n", sockets[index], LOG_INFO);
    }

    logSessionFinished();
    LOG_FILE_POINTER.close();
    return 0;
}
