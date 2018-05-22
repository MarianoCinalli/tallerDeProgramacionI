#include "util/Broadcaster.h"


Broadcaster::Broadcaster(Pitch* pitch, ConnectionManager* connectionManager) {
    log("Broadcaster: Creando broadcaster...", LOG_INFO);
    this->pitch = pitch;
    this->connectionManager = connectionManager;
}

void Broadcaster::broadcast() {
    std::string message = this->getMessage();
    if (message != "") {
        connectionManager->sendToAll(message);
    } else {
        log("Broadcaster: Mensaje vacio, no se broadcastea.", LOG_ERROR);
    }
}

// Pide la lista de cosas vistas (jugadores y pelota).
std::string Broadcaster::getMessage() {
    std::string message = "";
    std::list<Player*> players = this->pitch->getPlayersInsideCamera();
    Ball* ball = this->pitch->getBall();
    Camera* camera = this->pitch->getCamera();
    if (ball == NULL) {
        log("Broadcaster: La pelota es null!", LOG_ERROR);
        return "";
    }
    for (Player* player : players) {
        message += player->getAsYaml();
    }
    message += ball->getAsYaml();
    message += camera->getAsYaml();
    return message+";;";
}

Broadcaster::~Broadcaster() {
    log("Broadcaster: Eliminando broadcaster...", LOG_INFO);
}

void Broadcaster::broadcastGameBegins() {
    log("broadcast_to_clients: Broadcasteando inicio de partido...", LOG_INFO);
    std::string message = "gameBegins:";
    sleep(3); //HACK time to get your shit together
    connectionManager->sendToAll(message);
    //sleep(2); //HACK time to get your shit together
}

void Broadcaster::broadcastGameEnded() {
    log("broadcast_to_clients: Broadcasteando fin de partido...", LOG_INFO);
    std::string message = "gameEnds:";
    connectionManager->sendToAll(message);
}
