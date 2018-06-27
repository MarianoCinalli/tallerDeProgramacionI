#include "util/Broadcaster.h"

Broadcaster::Broadcaster(GameControllerProxy* gameControllerProxy, ConnectionManager* connectionManager) {
    log("Broadcaster: Creando broadcaster...", LOG_INFO);
    this->gameControllerProxy = gameControllerProxy;
    this->connectionManager = connectionManager;
}

void Broadcaster::broadcast(bool first = false) {
    std::string message = this->gameControllerProxy->getMessageToBroadcast(first);
    if (message != "") {
        connectionManager->sendToAll(message);
    } else {
        log("Broadcaster: Mensaje vacio, no se broadcastea.", LOG_ERROR);
    }
}

Broadcaster::~Broadcaster() {
    log("Broadcaster: Eliminando broadcaster...", LOG_INFO);
}

void Broadcaster::broadcastGameEnded() {
    log("broadcast_to_clients: Broadcasteando fin de partido...", LOG_INFO);
    std::string message = "gameEnds:";
    connectionManager->sendToAll(message);
}

void Broadcaster::broadcastGameStats() {
    log("broadcast_to_clients: Broadcasteando estadisticas del partido...", LOG_INFO);
    std::string message = "stats:";
    message += this->gameControllerProxy->getStatsToBroadcast();
    log("broadcast_to_clients: Broadcasteando: ", message, LOG_INFO);

    connectionManager->sendToAll(message);
}
