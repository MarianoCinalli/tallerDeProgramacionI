#include "util/Broadcaster.h"


Broadcaster::Broadcaster(Pitch* pitch, ConnectionManager* connectionManager) {
    log("Broadcaster: Creando broadcaster...", LOG_INFO);
    this->pitch = pitch;
    this->connectionManager = connectionManager;
}

void Broadcaster::broadcast(bool first = false) {

    std::string message;
    if (first) {
        message = this->getFirstMessage();
    } else {
        message = this->getMessage();
    }
    if (message != "") {
        connectionManager->sendToAll(message);
    } else {
        log("Broadcaster: Mensaje vacio, no se broadcastea.", LOG_ERROR);
    }
}

std::string Broadcaster::getFirstMessage() {
    std::string message = "";
    std::list<Player*> players = this->pitch->getPlayers();
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
    return message + ";;";
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
    return message + ";;";
}

Broadcaster::~Broadcaster() {
    log("Broadcaster: Eliminando broadcaster...", LOG_INFO);
}

void Broadcaster::broadcastGameEnded() {
    log("broadcast_to_clients: Broadcasteando fin de partido...", LOG_INFO);
    std::string message = "gameEnds:";
    connectionManager->sendToAll(message);
}
