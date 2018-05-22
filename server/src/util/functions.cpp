#include "util/functions.h"

extern GameInitializer* initializer;
extern pthread_barrier_t players_ready_barrier;
extern bool quit;
extern std::mutex quit_mutex;

// Lee los mensajes de los clientes y actualiza el modelo.
void* read_client(void* argument) {
    log("read_client: Creado.", LOG_INFO);
    int socket;
    int readBytes;
    // std::string message;
    bool continueReading = true;
    bool firstBroadcastRead = true;
    socket = *((int*) argument);
    GameControllerProxy* gameControllerProxy = initializer->getGameControllerProxy();
    ConnectionManager* connectionManager = initializer->getConnectionManager();
    User* user = new User(initializer, socket);
    log("read_client: Socket: ", socket, LOG_DEBUG);
    while (continueReading && !quit) {
        log("read_client: Reading...", LOG_DEBUG);
        if (firstBroadcastRead && user->hasLogedIn() && user->hasPickedTeamAndFormation()) {
            log("read_client: Esperando para sincronizar...", LOG_INFO);
            pthread_barrier_wait(&players_ready_barrier);
            firstBroadcastRead = false;
            log("read_client: Sincronizacion terminada.", LOG_INFO);
        }
        std::string message = "";
        readBytes = connectionManager->getMessage(socket, message);
        if (readBytes < 0) {
            log("read_client: Error en la lectura del mensaje. ", LOG_ERROR);
            continueReading = false;
            gameControllerProxy->end();
        } else if (readBytes == 0) {
            // Cuando ser cierra la coneccion del cliente lee 0 bytes sin control.
            // Si puede pasar que la coneccion siga viva y haya un mensaje de 0 bytes hay que buscar otra vuelta.
            log("read_client: Se desconecto el usuario?. Saliendo...", LOG_INFO);
            continueReading = false;
            gameControllerProxy->end();
        } else {
            log("read_client: Bytes Recibidos " + std::to_string(readBytes) + " - Mensaje: ", message, LOG_DEBUG);
            if (!user->hasLogedIn()) {
                // No se logeo.
                user->processLogInMessage(message);
                if(!user->hasLogedIn()){
                  connectionManager->sendMessage(socket,"logged:false");
                } else {
                  connectionManager->sendMessage(socket,"logged:true");
                }
            } else if (!user->hasPickedTeamAndFormation()) {
                // Se logeo, pero no eligio equipo y formacion.
                log("ELIGE EL EQUIPO ", LOG_INFO);
                std::string key = getMessageKey(message);
                std::string value = getMessageValue(message);
                if (key == "get" && value == "max") {
                  // Mostrarle cuantos jugadores maximos tiene la partida
                  connectionManager->sendMessage(socket,"max:"+std::to_string(connectionManager->getMaxClients()));
                }
                // y mostrar cuantos jugadores hay en cada equipo
                if (key == "get" && value == "equipo1") {
                  connectionManager->sendMessage(socket,"Argentina:0");
                }
                if (key == "get" && value == "equipo2") {
                  connectionManager->sendMessage(socket,"Brasil:0");
                }
                if (key == "use") {
                  user->processTeamAndFormationMessage(message);
                  gameControllerProxy->addUser(user->getName(), user->getTeam());
                }
                //connectionManager->sendMessage(socket,"Argentina:Brasil");

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
    //gameControllerProxy->processDisconection();
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
    broadcaster->broadcastGameBegins();
    sleep(3); //HACK time to get your shit together
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


// Separan key:value en key value
std::string getMessageKey(std::string message) {
    return message.substr(0, message.find(":"));
}

std::string getMessageValue(std::string message) {
    return message.substr(message.find(":")+1, message.length());   //iba un +1 LPM
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
