#include "util/functions.h"

extern GameInitializer* initializer;

// Lee los mensajes de los clientes y actualiza el modelo.
void* read_client(void* argument) {
    log("read_client: Creado.", LOG_INFO);
    int socket;
    int readBytes;
    std::string message;
    bool continueReading = true;
    socket = *((int*) argument);
    GameControllerProxy* gameControllerProxy = initializer->getGameControllerProxy();
    ConnectionManager* connectionManager = initializer->getConnectionManager();
    User* user = new User(initializer, socket);
    log("read_client: Socket: ", socket, LOG_DEBUG);
    log("read_client: Thread spawneado.", LOG_DEBUG);
    while (continueReading) {
        readBytes = connectionManager->getMessage(socket, message);
        if (readBytes < 0) {
            log("read_client: Error en la lectura del mensaje. ", LOG_ERROR);
            continueReading = false;
        } else if (readBytes == 0) {
            // Cuando ser cierra la coneccion del cliente lee 0 bytes sin control.
            // Si puede pasar que la coneccion siga viva y haya un mensaje de 0 bytes hay que buscar otra vuelta.
            log("read_client: Se desconecto el usuario?. Saliendo...", LOG_INFO);
            continueReading = false;
        } else {
            log("read_client: Bytes Recibidos " + std::to_string(readBytes) + " - Mensaje: ", message, LOG_DEBUG);
            if (!user->hasLogedIn()) {
                // No se logeo.
                user->processLogInMessage(message);
            } else if (!user->hasPickedTeamAndFormation()) {
                // Se logeo, pero no eligio equipo y formacion.
                user->processTeamAndFormationMessage(message);
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
    std::vector<int> sockets = *((std::vector<int>*) argument);
    Broadcaster* broadcaster = new Broadcaster(initializer->getPitch(), &sockets);
    // broadcaster->broadcastGameBegins();
    int count = 0; // esto esta provisorio.
    log("broadcast_to_clients: Se comienza a broadcastear...", LOG_INFO);
    while (count < 1) {
        broadcaster->broadcast();
        count++;
        usleep(MICROSECONDS_BETWEEEN_BROADCAST);
    }
    delete(broadcaster);
    log("broadcast_to_clients: Finalizado.", LOG_INFO);
    return NULL;
}

// Actualiza el modelo dependiendo de las propiedades.
void* game_updater(void* argument) {
    log("game_updater: Creado.", LOG_INFO);
    Camera* camera = initializer->getCamera();
    GameControllerProxy* gameControllerProxy = initializer->getGameControllerProxy();
    while (gameControllerProxy->shouldGameEnd()) {
        gameControllerProxy->updateModel(camera);
        usleep(MICROSECONDS_BETWEEEN_BROADCAST/10);
    }
    log("game_updater: Finalizado.", LOG_INFO);
    return NULL;
}
