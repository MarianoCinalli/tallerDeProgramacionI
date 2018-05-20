#include "util/Broadcaster.h"


Broadcaster::Broadcaster(Pitch* pitch) {
    log("Broadcaster: Creando broadcaster...", LOG_INFO);
    this->pitch = pitch;
}

void Broadcaster::broadcast(std::vector<int> sockets) {
    std::string message = this->getMessage();
    if (message != "") {
        const char* constantMessage = (message).c_str();
        for(int client_socket : sockets) {
            log("Broadcaster: Broadcasteando a: ", client_socket, LOG_DEBUG);
            // Convertirlas a string y enviarlas.
            send(client_socket, constantMessage, strlen(constantMessage), 0);
        }
    } else {
        log("Broadcaster: Mensaje vacio, no se broadcastea.", LOG_ERROR);
    }
}

// Pide la lista de cosas vistas (jugadores y pelota).
std::string Broadcaster::getMessage() {
    std::string message = "";
    std::list<Player*> players = this->pitch->getPlayersInsideCamera();
    Ball* ball = this->pitch->getBall();
    if (ball == NULL) {
        log("Broadcaster: La pelota es null!", LOG_ERROR);
        return "";
    }
    for (Player* player : players) {
        message += player->getAsYaml();
    }
    message += ball->getAsYaml();
    return message;
}

Broadcaster::~Broadcaster() {
    log("Broadcaster: Eliminando broadcaster...", LOG_INFO);
}
