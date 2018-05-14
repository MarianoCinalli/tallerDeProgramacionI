#include "util/Broadcaster.h"


Broadcaster::Broadcaster(Camera* camera, std::vector<int>* sockets) {
    log("Broadcaster: Creando broadcaster...", LOG_INFO);
    this->camera = camera;
    this->sockets = *sockets;
}

void Broadcaster::broadcast() {
    for(int client_socket : sockets) {
        log("Broadcaster: Broadcasteando a: ", client_socket, LOG_DEBUG);
        // Pedirle a this->camera la lista de cosas vistas (jugadores y pelota).
        // Convertirlas a string y enviarlas.
        std::string message = "Mensaje al cliente!";
        const char* constantMessage = (message).c_str();
        send(client_socket, constantMessage, strlen(constantMessage), 0);
    }
}

Broadcaster::~Broadcaster() {
    log("Broadcaster: Eliminando broadcaster...", LOG_INFO);
}
