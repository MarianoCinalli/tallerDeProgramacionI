#include "util/functions.h"

extern GameInitializer* initializer;
extern pthread_barrier_t players_ready_barrier;

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
    while (continueReading) {
        log("read_client: Reading...", LOG_INFO);
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
                  connectionManager->sendMessage(socket,"max:4");
                }
                // y mostrar cuantos jugadores hay en cada equipo
                if (key == "get" && value == "equipo1") {
                  connectionManager->sendMessage(socket,"Argentina:2");
                }
                if (key == "get" && value == "equipo2") {
                  connectionManager->sendMessage(socket,"Brasil:0");
                }
                if (key == "use") {
                  user->processTeamAndFormationMessage(message);
                }
                //connectionManager->sendMessage(socket,"Argentina:Brasil");

            } else {
                // El usuario se inicializo. Vienen acciones sobre los jugadores.
                Action* action = user->getAsAction(message);
                if (action != NULL) {
                    gameControllerProxy->execute(action, user->getTeam());
                    delete(action);
                }
            }
        }
    }
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
    std::vector<int> sockets = *((std::vector<int>*) argument);
    Broadcaster* broadcaster = new Broadcaster(initializer->getPitch(), &sockets);
    // broadcaster->broadcastGameBegins();
    // int count = 0; // esto esta provisorio.
    log("broadcast_to_clients: Se comienza a broadcastear...", LOG_INFO);
    GameControllerProxy* gameControllerProxy = initializer->getGameControllerProxy();
    while (!gameControllerProxy->shouldGameEnd()) {
        broadcaster->broadcast();
        // count++;
        usleep(MICROSECONDS_BETWEEEN_BROADCAST*2);
    }
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
    while (!gameControllerProxy->shouldGameEnd()) {
        gameControllerProxy->updateModel(camera);
        usleep(MICROSECONDS_BETWEEEN_BROADCAST*2);
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
