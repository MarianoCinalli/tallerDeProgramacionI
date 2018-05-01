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
#include <stdio.h>

#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/types.h>


// Global variables ---------------------------------------
std::ofstream LOG_FILE_POINTER;
const std::string logFileName = "trabajoPractico.log";
const std::string defaultConfFileName = "src/default.yaml";
const std::string defaultSpritesFileName = "images/spritesDefaults.png";
std::string confFileName = "conf.yaml";
int LOG_MIN_LEVEL = LOG_DEBUG; // Dejarlo asi para que cuando empieze loggee todo.
std::string CLI_LOG_LEVEL = "";
// Global variables ---------------------------------------


void imprimir_ayuda()
{
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

void imprimir_version()
{
	cout << "Version = tp1\n";
}

int chequearOpciones(int argc, char* argv[])
{
	int ch;

	std::string tempConf = "";
	while (1) {
		static struct option long_options[] = {
			//Flags posibles
			{ "version",  no_argument,	 0, 'v' },
			{ "help",     no_argument,	 0, 'h' },
			{ "input",    required_argument, 0, 'i' },
			{ "logLevel", required_argument, 0, 'l' },
			{ 0,	      0,		 0, 0	}
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


int main(int argc, char* argv[])
{
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

	// GameInitializer* initializer = new GameInitializer(configuration);
	// GameController* gameController = initializer->getGameController();
	// ActionsManager* actionsManager = initializer->getActionsManager();
	// Camera* camera = initializer->getCamera();
	// SDL_Renderer* renderer = initializer->getRenderer();
	// PitchView* pitchView = initializer->getPitchView();
	bool quit = false;
	// SDL_Event e;
	// int frameRate = configuration->getFramerate();
	// float sleepTime = (float)200000/(float)frameRate;
	// log("Main: Frame rate: ", frameRate, LOG_INFO);
	delete(configuration);
	log("Main: Juego inicializado correctamente.", LOG_INFO);
	// Main loop ------------------------------------------
	log("Main: Entrando en el main loop...", LOG_INFO);
	// memset(&hints, 0, sizeof(struct addrinfo))

//
// 	unsigned int sockfd, newsockfd, clilen;
// 	struct sockaddr_in cli_addr, serv_addr;
// 	printf("Creando socket...n");
// // Creo el socket
// 	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
// 		perror("creando socket");
// 		printf("%sn", strerror(errno));
// 		exit(1);
// 	}
// // Lleno de ceros la estructura
// 	memset((char*)&serv_addr, 0, sizeof(serv_addr));
// 	serv_addr.sin_family = AF_INET; // Tipo internet
// 	serv_addr.sin_port = htons(SERVER_PORT);
// 	serv_addr.sin_addr.s_addr = INADDR_ANY;
// // Enlazo la dirección al socket
// 	printf("Bindeando...n");
//
// 	if ( bind( sockfd,
// 		   (struct sockaddr *)&serv_addr,
// 		   (socklen_t)sizeof(struct sockaddr)) < 0 ) {
// 		perror("bind");
// 		printf("%sn", strerror(errno));
// 		exit(1);
// 	}
// 	printf("Escuchando...n");
// 	listen(sockfd, 5); // Escucho por conexiones
// 	printf("Aceptando...n");
// 	clilen = (socklen_t)sizeof(cli_addr);
// // Acepto la conexión entrante
// 	newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
// 	if (newsockfd < 0) {
// 		perror("acept");
// 		printf("%sn", strerror(errno));
// 		exit(1);
// 	}
//
// 	printf("Conexión establecida.n");
// 	write(newsockfd, "Recibí el mensajen", 19);
// 	close(sockfd);
	// while (!quit) {
		//
		//     while (SDL_PollEvent(&e) != 0) {
		//         if (actionsManager->shouldQuit(e)) {
		//             quit = true;
		//         } else {
		//             // Devuelve acciones que modifican modelos.
		//             // Se puede optimizar para que deje de hacer actions todo el tiempo.
		//             Action* action = actionsManager->getAction(e);
		//             if (action != NULL) {
		//                 gameController->execute(action);
		//                 delete(action);
		//             }
		//         }
		//     }
		//     gameController->updatePlayers();
		//     gameController->updateCameraPosition(camera);
		//     pitchView->render(renderer);
		//     usleep(sleepTime); // Frame rate.
	// }
	log("Main: Main loop finalizado.", LOG_INFO);
	// Liberacion de memoria -------------------------------
	// delete(initializer);
	logSessionFinished();
	LOG_FILE_POINTER.close();
	return 0;
}
