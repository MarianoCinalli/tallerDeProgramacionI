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

void openLogin(SDL_Renderer* gRenderer) {
  log("Entra al openLogin", LOG_INFO);

  bool quit = false;
  SDL_Event e;
  SDL_Color textColor = { 0, 0, 0, 0xFF };

  TTF_Font *gFont = NULL;
  gFont = TTF_OpenFont( "lazy.ttf", 30 );
  if( gFont == NULL ) {
    log( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError(), LOG_INFO);
  }

  Texture servidorPromptTexture = Texture( "Servidor:", gRenderer, textColor, gFont );
  Texture usuarioPromptTexture = Texture( "Usuario:", gRenderer, textColor, gFont );
  Texture clavePromptTexture = Texture( "Clave:", gRenderer, textColor, gFont );

  log("P1", LOG_INFO);
  std::string inputText = "127.0.0.1:8080";
  log("P2", LOG_INFO);
  Texture servidorInputTexture = Texture( inputText.c_str(), gRenderer, textColor, gFont );
  log("P3", LOG_INFO);

  //Enable text input
  SDL_StartTextInput();

  //While application is running
  while( !quit ) {
    bool renderText = false;
    while( SDL_PollEvent( &e ) != 0 ) {
      if ( e.type == SDL_QUIT || e.key.keysym.sym == SDLK_ESCAPE ) {
        quit = true;
      } else if( e.type == SDL_KEYDOWN ) {
        //Handle backspace
        if( e.key.keysym.sym == SDLK_BACKSPACE && inputText.length() > 0 ) {
          //lop off character
          inputText.pop_back();
          renderText = true;
        }
      } else if( e.type == SDL_TEXTINPUT ) {
        //Append character
        inputText += e.text.text;
        renderText = true;
      }
    }

    // if( renderText ) {
    //   if( inputText != "" ) {
    //     gInputTextTexture.loadFromRenderedText( inputText.c_str(), textColor );
    //   }	else {
    //     gInputTextTexture.loadFromRenderedText( " ", textColor );
    //   }
    // }

    //Clear screen
    SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
    SDL_RenderClear( gRenderer );

    //Render text textures
    SDL_Rect renderQuad1 = { ( SCREEN_WIDTH - servidorPromptTexture.getWidth() ) / 2, 50, servidorPromptTexture.getWidth(), servidorPromptTexture.getHeight() };
    SDL_RenderCopyEx( gRenderer, servidorPromptTexture.getSpriteSheetTexture(), NULL, &renderQuad1, 0.0, NULL, SDL_FLIP_NONE );

    SDL_Rect renderQuad11 = { ( SCREEN_WIDTH - servidorInputTexture.getWidth() ) / 2, 100, servidorInputTexture.getWidth(), servidorInputTexture.getHeight() };
    SDL_RenderCopyEx( gRenderer, servidorInputTexture.getSpriteSheetTexture(), NULL, &renderQuad11, 0.0, NULL, SDL_FLIP_NONE );

    SDL_Rect renderQuad2 = { ( SCREEN_WIDTH - usuarioPromptTexture.getWidth() ) / 2, 150, usuarioPromptTexture.getWidth(), usuarioPromptTexture.getHeight() };
    SDL_RenderCopyEx( gRenderer, usuarioPromptTexture.getSpriteSheetTexture(), NULL, &renderQuad2, 0.0, NULL, SDL_FLIP_NONE );

    SDL_Rect renderQuad3 = { ( SCREEN_WIDTH - clavePromptTexture.getWidth() ) / 2, 250, clavePromptTexture.getWidth(), clavePromptTexture.getHeight() };
    SDL_RenderCopyEx( gRenderer, clavePromptTexture.getSpriteSheetTexture(), NULL, &renderQuad3, 0.0, NULL, SDL_FLIP_NONE );

    //gPromptTextTexture.render( ( SCREEN_WIDTH - gPromptTextTexture.getWidth() ) / 2, 0 );
    //SDL_Rect renderQuad2 = { ( SCREEN_WIDTH - gInputTextTexture.getWidth() ) / 2, 0, 100, 100 };
    //SDL_RenderCopyEx( gRenderer, gInputTextTexture.getSpriteSheetTexture(), NULL, &renderQuad2, 0.0, NULL, SDL_FLIP_NONE );
    //gInputTextTexture.render( ( SCREEN_WIDTH - gInputTextTexture.getWidth() ) / 2, gPromptTextTexture.getHeight() );

    //Update screen
    SDL_RenderPresent( gRenderer );
  }
  //Disable text input
  SDL_StopTextInput();
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
    bool quit = false;
    SDL_Event e;
    int frameRate = configuration->getFramerate();
    float sleepTime = (float)200000/(float)frameRate;
    log("Main: Frame rate: ", frameRate, LOG_INFO);
    delete(configuration);

    // Login - determinar IP, Port, Usuario y Clave
    SDL_Renderer* renderer = initializer->getRenderer();
    openLogin(renderer);
    log("SALIO DEL LOGIN", LOG_INFO);

    ConnectionManager* connectionManager = new ConnectionManager("127.0.0.1", 8080);
    if(!connectionManager->connectToServer()) {
        log("Main: No se pudo abrir la conexion.", LOG_ERROR);
        endProgram(1, connectionManager);
    }
    // Iniciar sesion. Elegir equipo y casaca. Usar el connectionManager para recibir
    // y mandar estos mensajes al server, y initializer tiene la pantalla de sdl para dibujar.
    // Esperar a que el server mande el mensaje de que todos los jugadores estan listos?
    // Abajo se Lanza thread que recibe mensajes de estado de juego.
    ThreadSpawner* threadSpawner = new ThreadSpawner();
    threadSpawner->spawn(
        read_server,
        connectionManager
    );
    log("Main: Juego inicializado correctamente.", LOG_INFO);
    // Lanzar thread que dibuja el juego.
    // threadSpawner->spawn(drawer, NULL);
    // Main loop ------------------------------------------
    log("Main: Entrando en el main loop...", LOG_INFO);
    // Este thread escucha los eventos de teclado y se los manda al server.
    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (actionsManager->shouldQuit(e)) {
                quit = true;
            } else {
                // Devuelve acciones que modifican modelos.
                // Se puede optimizar para que deje de hacer actions todo el tiempo.
                Action* action = actionsManager->getAction(e);
                if (action != NULL) {
                    connectionManager->sendToServer(action->toString());
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
