#include "util/functions.h"

extern GameInitializer* initializer;
extern bool quit;
extern bool lostConnectionQuit;
extern std::mutex quit_mutex;

// Recibe los mensajes del servidor.
// Actualiza el "modelo".
// Se tiene un modelo minimo solo con las propiedades que se usan para dibujar.
// Las vistas ven todo el modelo.
// Se inicializan ambos equipos, en cualquier parte.
// Despues en los mensajes que se reciben, ir actualizando los modelos con la informacion.
// Para los mensajes que llegan del server:
// Los jugadores tienen numeros (this->id en la clase jugador del server) del 1 al 7
// para el equipo local y del 8 al 14 para el visitante. El cliente lo parsea en * (ver abajo).
// Las propiedades son un diccionario que esta luego del tipo.
// Cada propiedad esta como:
// this->id:
//  te: this->team
//  cx: this->position->getX()
//  cy: this->position->getY()
//  se: this->isSelected
//  ki: this->kicking <- Cuidado: Al querer pisar el valor del player con esto,
//      creo que hay que guardar el valor a pisar en wasKicking porque se usa para dibujar.
//  sl: this->sliding <- idem kicking
//  ru: this->runningFast


// Setea el quit de manera segura.
void setQuit(bool newQuit) {
    log("setQuit: Seteando salida...", LOG_INFO);
    quit_mutex.lock();
    quit = quit || newQuit;
    quit_mutex.unlock();
    log("setQuit: Salida seteada.", LOG_INFO);
}

// Setea el lost Connection de manera segura.
void setLostConnectionQuit(bool newLostConnection){
  log("setLostConnection: Seteando salida...", LOG_INFO);
  quit_mutex.lock();
  lostConnectionQuit =  newLostConnection;
  quit_mutex.unlock();
  log("setLostConnection: Salida seteada.", LOG_INFO);
}

void* read_server(void* argument) {
    log("read_server: Creado.", LOG_INFO);
    std::string readMessage;
    int readBytes;
    ConnectionManager* connectionManager = (ConnectionManager*) argument;
    log("read_server: Empezando a recibir mensajes. ", LOG_INFO);
    int nroDeMensajes = 0;
    while (!quit) {
        readMessage = "";
        readBytes = connectionManager->getMessage(readMessage, 1); //TODO 1 sec of timeout
        if (readBytes < 0) {
            log("read_client: Error en la lectura del mensaje. Saliendo...", LOG_ERROR);
            setLostConnectionQuit(true);
            setQuit(true);
        } else if (readBytes == 0) {
            // Cuando ser cierra la coneccion del cliente lee 0 bytes sin control.
            // Si puede pasar que la coneccion siga viva y haya un mensaje de 0 bytes hay que buscar otra vuelta.
            log("read_server: No se pudo establecer coneccion con el server. Saliendo...", LOG_INFO);
            setLostConnectionQuit(true);
            // setQuit(true);
        } else {
            if (readMessage == "gameEnds:") {
                log("read_server: Se recibio el mensaje de fin de juego. Saliendo...", LOG_INFO);
                setQuit(true);
            } else {
              if(lostConnectionQuit == true){
                setLostConnectionQuit(false);
              }
                try {
                    std::string delimiter = ";;";

                    size_t pos = 0;
                    std::string token;
                    while ((pos = readMessage.find(delimiter)) != std::string::npos) {
                        token = readMessage.substr(0, pos);
                        //log("msg", LOG_SPAM);
                        Player* player;
                        GameController* gameController = initializer->getGameController();
                        Ball* ball = initializer->getGameController()->getBall();
                        Camera* camera = initializer->getGameController()->getCamera();
                        Clock* clock = initializer->getGameController()->getClock();
                        Score* score = initializer->getGameController()->getScore();
                        YAML::Node node = YAML::Load(token);
                        for (YAML::const_iterator it = node.begin(); it != node.end(); ++it) {
                            YAML::Node key = it->first;
                            YAML::Node value = it->second;
                            // * Aca se sabe que tipo es.
                            if (key.Type() == YAML::NodeType::Scalar) {
                                if (key.as<std::string>() == "ba") {
                                    //log("read_server: pelota", key.as<std::string>(), LOG_SPAM);
                                    ball->parseYaml(value);
                                } else if ((key.as<std::string>() == "cam")) {
                                    //log("read_server: camara", key.as<std::string>(), LOG_SPAM);
                                    camera->parseYaml(value);
                                } else if ((key.as<std::string>() == "clock")) {
                                    //log("read_server: clock ", value.as<std::string>(), LOG_DEBUG);
                                    //clock->parseYaml(value);
                                    clock->value = value.as<std::string>(); // No es un YAML
                                } else if ((key.as<std::string>() == "score")) {
                                    //log("read_server: score ", value.as<std::string>(), LOG_DEBUG);
                                    score->parseYaml(value);
                                } else if ((key.as<std::string>() == "st")) {
                                    //log("read_server: state ", value.as<std::string>(), LOG_DEBUG);
                                    gameController->parseYaml(value);
                                } else {
                                    //log("read_server: jugador", key.as<std::string>(), LOG_SPAM);
                                    player = initializer->getGameController()->getPlayer(key.as<int>());
                                    player->parseYaml(value);
                                }
                            }

                            // Valores para objeto del tipo parseado arriba.
                            if (value.Type() == YAML::NodeType::Map) {
                                // Este for iria en el constructor para el objeto del tipo de arriba.
                                for (YAML::const_iterator secondNode = value.begin(); secondNode != value.end(); ++secondNode) {
                                    std::string secondkey = secondNode->first.as<std::string>();
                                    std::string secondvalue = secondNode->second.as<std::string>();
                                    //log("read_server: " + secondkey + " = " + secondvalue, LOG_SPAM);
                                }
                            }
                        }
                        readMessage.erase(0, pos + delimiter.length());
                    }
                } catch (const std::exception& e) {
                    log("read_client: yaml error .what() = ", e.what(), LOG_ERROR);
                    log("mensaje leido en error: ", readMessage, LOG_SPAM);
                }
                nroDeMensajes++;
            }
        }
    }
    log("read_server: Finalizado.", LOG_INFO);
    return NULL;
}

// Dibuja las vistas.
// Este metodo esta dentro de todo completo.
// Hay que ver si le falta algo.
void* drawer(void* argument) {
    log("drawer: Creado.", LOG_INFO);
    SDL_Renderer* renderer = initializer->getRenderer();
    PitchView* pitchView = initializer->getPitchView();
    GameController* gameController = initializer->getGameController();
    Clock* clock = gameController->getClock();
    Score* score = gameController->getScore();
    const int MILISECONDS_TIMEOUT = 20;
    int timeout = SDL_GetTicks() + MILISECONDS_TIMEOUT;
    while (!quit) {
      if (!lostConnectionQuit){
        if(SDL_TICKS_PASSED(SDL_GetTicks(), timeout)){
          timeout = SDL_GetTicks() + MILISECONDS_TIMEOUT;
          clock->render(renderer);
          score->render(renderer);
          pitchView->renderMinimap(renderer);

          pitchView->render(renderer);
          if (gameController->state == GAME_START_STATE){
            //renderea el countdown para que inicie el partido
            // pitchView->render(renderer);
            pitchView->renderCountdown(renderer, gameController->stateOption);
          }
          if(gameController->state == HALF_START_STATE){
            std::string msg = "Cambio de Lado";
            pitchView->renderMessage(renderer, msg);
          }

        //renderea la cancha de cualquier modo
        SDL_RenderPresent(renderer);
        }
      }
    }
    log("drawer: Finalizado.", LOG_INFO);
    return NULL;
}
