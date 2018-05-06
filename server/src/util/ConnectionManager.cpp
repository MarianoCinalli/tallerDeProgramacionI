#include "util/ConnectionManager.h"

ConnectionManager::ConnectionManager(int port, int maxConnections) {
    this->port = port;
    this->openedSockets = {};
    // Inicializacion en 0 para que luego el .in() no falle.
    for (int i = 0; i < maxConnections; ++i) {
        this->clientsThreadIds.push_back(0);
    }
}

bool ConnectionManager::openConnections() {
    int opt = 1;

    log("ConnectionManager: Creando el socket...", LOG_INFO);
    this->my_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (this->my_socket == 0) {
        log("ConnectionManager: Creacion del socket fallida.", LOG_ERROR);
        return false;
    }

    log("ConnectionManager: Configurando el socket...", LOG_INFO);
    if (setsockopt(this->my_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        log("ConnectionManager: Configuracion del socket fallida.", LOG_ERROR);
        return false;
    }
    this->address.sin_family = AF_INET;
    this->address.sin_addr.s_addr = INADDR_ANY;
    this->address.sin_port = htons(this->port);

    log("ConnectionManager: Binding al puerto: ", this->port, LOG_INFO);
    if (bind(this->my_socket, (struct sockaddr*)&this->address, sizeof(this->address)) < 0) {
        log("ConnectionManager: Binding fallido.", LOG_ERROR);
        return false;
    }
    log("ConnectionManager: Escuchando al socket...", LOG_INFO);
    if (listen(this->my_socket, 3) < 0) {
        log("ConnectionManager: Escucha fallida.", LOG_ERROR);
        return false;
    }
    log("ConnectionManager: Escuchando conexiones. ", LOG_INFO);
    return true;
}

void ConnectionManager::acceptConnections() {
    // TODO.
}

void ConnectionManager::acceptConnectionsUntilMax() {
    int new_socket;
    int addrlen = sizeof(this->address);
    int acceptedConnections = 0;
    log("ConnectionManager: Aceptando conexiones...", LOG_INFO);
    while (acceptedConnections < this->maxConnections) {
        // Esto se bloquea hasta que haya una conexion entrante.
        new_socket = accept(this->my_socket, (struct sockaddr*)&address, (socklen_t*)&addrlen);
        if (new_socket > 0) {
            log("ConnectionManager: Conexion aceptada.", LOG_INFO);
            this->openedSockets.push_back(new_socket);
            log("ConnectionManager: Creando thread para nueva conexion", LOG_INFO);
            // Validar el resultado de la creacion del thread.
            pthread_create(
                &(this->clientsThreadIds.at(acceptedConnections)),
                NULL,
                read_client,
                &(this->openedSockets.at(acceptedConnections))
            );
            ++acceptedConnections;
        } else {
            log("ConnectionManager: Conexion rechazada.", LOG_ERROR);
        }
    }
}

ConnectionManager::~ConnectionManager() {

}
