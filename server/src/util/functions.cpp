#include "util/functions.h"
#include <ctime>

extern GameInitializer* initializer;
extern pthread_barrier_t players_ready_barrier;
extern bool quit;
extern std::mutex quit_mutex;

// Lee los mensajes de los clientes y actualiza el modelo.
void* read_client(void* argument) {
    log("read_client: Creado.", LOG_INFO);
    int socket;
    int readBytes;
    bool continueReading = true;
    bool firstBroadcastRead = true;
    bool firstError = false;
    double timeDifference = 0;
    socket = *((int*) argument);
    time_t firstTimeError;
    time_t currentErrorTime;
    GameControllerProxy* gameControllerProxy = initializer->getGameControllerProxy();
    ConnectionManager* connectionManager = initializer->getConnectionManager();
    User* user = new User(initializer, socket);
    log("read_client: Se recibio el socket: ", socket, LOG_DEBUG);
    log("read_client: Eviando mensaje de conexion aceptada...", LOG_SPAM);
    connectionManager->sendMessage(socket, "true:");
    while (continueReading && !quit) {
        log("read_client: Reading...", LOG_SPAM);
        if (firstBroadcastRead && user->hasLogedIn() && user->hasPickedTeamAndFormation()) {
            log("read_client: Esperando para sincronizar...", LOG_INFO);
            if (!gameControllerProxy->hasGameStarted()) {
                pthread_barrier_wait(&players_ready_barrier);
            }
            log("read_client: Sincronizacion terminada.", LOG_INFO);
            connectionManager->sendMessage(socket, "gameBegins:");
            sleep(3);
            connectionManager->ready(pthread_self(), socket);
            firstBroadcastRead = false;
        }
        std::string message = "";
        readBytes = connectionManager->getMessage(socket, message);
        if (readBytes < 0) {
            log("read_client: Error en la lectura del mensaje. ", LOG_ERROR);
            continueReading = false;
        } else if (readBytes == 0) {
            // Cuando ser cierra la coneccion del cliente lee 0 bytes sin control.
            // Si puede pasar que la coneccion siga viva y haya un mensaje de 0 bytes hay que buscar otra vuelta.
            // log("read_client: Se desconecto el usuario?. Saliendo...", LOG_INFO);
            if (!firstError){
              firstError = true;
              time(&firstTimeError);
            }else {
              time(&currentErrorTime);
              timeDifference = difftime(currentErrorTime, firstTimeError);
              // log("read_client: diferencia de tiempo:",timeDifference, LOG_DEBUG);
              if (timeDifference > 25){
                log("read_client: superados tiempo maximo de socket: esperando ", socket, LOG_INFO);
                continueReading = false;
              }
            }

        } else {
          if (firstError){
            firstError = false;
            timeDifference = 0;
          }
            log("read_client: Mensaje recibido: ", message, LOG_SPAM);
            if (!user->hasLogedIn()) {
                // No se logeo.
                user->processLogInMessage(message);
            } else if (!user->hasPickedTeamAndFormation()) {
                user->processTeamAndFormationMessage(message);
            } else {
                // El usuario se inicializo. Vienen acciones sobre los jugadores.
                Action* action = user->getAsAction(message);
                if (action != NULL) {
                    gameControllerProxy->execute(action, user->getName());
                    delete(action);
                }
            }
        }
    }
    // Para que el active player del jugador que se va pase a ser seleccionable.
    // gameControllerProxy->processDisconection();
    gameControllerProxy->removeUser(user->getName());
    connectionManager->processDisconection(pthread_self());
    user->logOff();
    delete(user);
    log("read_client: Finalizado.", LOG_INFO);
    return NULL;
}

// Este es el que le envia el juego a los clientes.
void* broadcast_to_clients(void* argument) {
    log("broadcast_to_clients: Creado.", LOG_INFO);
    log("broadcast_to_clients: Esperando para sincronizar...", LOG_INFO);
    pthread_barrier_wait(&players_ready_barrier);
    log("broadcast_to_clients: Sincronizacion terminada.", LOG_INFO);
    Broadcaster* broadcaster = new Broadcaster(
        initializer->getPitch(),
        initializer->getConnectionManager()
    );
    GameControllerProxy* gameControllerProxy = initializer->getGameControllerProxy();
    // Termino la espera
    sleep(2); //HACK time to get your shit together
    while (!gameControllerProxy->shouldGameEnd() && !quit) {
        broadcaster->broadcast();
        usleep(MICROSECONDS_BETWEEEN_BROADCAST);
    }
    // Termino el juego
    broadcaster->broadcastGameEnded();
    delete(broadcaster);
    log("broadcast_to_clients: Finalizado.", LOG_INFO);
    return NULL;
}

// Actualiza el modelo dependiendo de las propiedades.
void* game_updater(void* argument) {
    log("game_updater: Creado.", LOG_INFO);
    log("game_updater: Esperando para sincronizar...", LOG_INFO);
    pthread_barrier_wait(&players_ready_barrier);
    log("game_updater: Sincronizacion terminada.", LOG_INFO);
    Camera* camera = initializer->getCamera();
    GameControllerProxy* gameControllerProxy = initializer->getGameControllerProxy();
    gameControllerProxy->startGame();
    while (!gameControllerProxy->shouldGameEnd() && !quit) {
        gameControllerProxy->updateModel(camera);
        usleep(MICROSECONDS_BETWEEEN_BROADCAST);
    }
    log("game_updater: Finalizado.", LOG_INFO);
    return NULL;
}

// Espera y acepta conecciones.
void* connection_listener(void* argument) {
    log("connection_listener: Creado.", LOG_INFO);
    ConnectionManager* connectionManager = initializer->getConnectionManager();
    while (!quit) {
        log("connection_listener: Esperando por una conexion...", LOG_INFO);
        connectionManager->acceptConnection();
    }
    log("connection_listener: Finalizado.", LOG_INFO);
    return NULL;
}

// Signal handling ---------------------------------------------------------------------------
void handleSignal(int sig) {
    log("handleSignal: Seteando salida. Signal recibida.", LOG_INFO);
    setQuit();
    log("handleSignal: Fin procesar signal.", LOG_INFO);
}

void registerSignalHandler(int signal) {
    log("registerSignalHandler: Seteando un handler para la signal.", LOG_INFO);
    struct sigaction new_action;

    new_action.sa_handler = handleSignal;
    sigemptyset(&new_action.sa_mask);
    new_action.sa_flags = 0;

    if (sigaction(signal, &new_action, NULL) < 0) {
        log("registerSignalHandler: Error seteando  el handler.", strerror(errno), LOG_INFO);
    } else {
        log("registerSignalHandler: Handler seteado.", LOG_INFO);
    }
}

void setQuit() {
    quit_mutex.lock();
    quit = true;
    quit_mutex.unlock();
}
