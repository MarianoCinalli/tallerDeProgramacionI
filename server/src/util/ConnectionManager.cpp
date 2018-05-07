#include "util/ConnectionManager.h"

ConnectionManager::ConnectionManager(int port, int maxConnections) {
    this->port = port;
    this->maxConnections = maxConnections;
    this->openedSockets = {};
    this->clientsThreadIds = {};
}

bool ConnectionManager::openConnections() {
    int opt = 1;

    log("ConnectionManager: Creando el socket...", LOG_INFO);
    this->my_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (this->my_socket == 0) {
        log("ConnectionManager: Creacion del socket fallida: ", strerror(errno), LOG_ERROR);
        return false;
    }

    log("ConnectionManager: Configurando el socket...", LOG_INFO);
    if (setsockopt(this->my_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        log("ConnectionManager: Configuracion del socket fallida: ", strerror(errno), LOG_ERROR);
        return false;
    }
    this->address.sin_family = AF_INET;
    this->address.sin_addr.s_addr = INADDR_ANY;
    this->address.sin_port = htons(this->port);

    log("ConnectionManager: Binding al puerto: ", this->port, LOG_INFO);
    if (bind(this->my_socket, (struct sockaddr*)&this->address, sizeof(this->address)) < 0) {
        log("ConnectionManager: Binding fallido: ", strerror(errno), LOG_ERROR);
        return false;
    }
    log("ConnectionManager: Escuchando al socket...", LOG_INFO);
    if (listen(this->my_socket, 3) < 0) {
        log("ConnectionManager: Escucha fallida: ", strerror(errno), LOG_ERROR);
        return false;
    }
    log("ConnectionManager: Escuchando conexiones. ", LOG_INFO);
    return true;
}

// Este es para la reconexion.
// Se va a quedar aceptando las connecciones entrantes. Cuando las actuales,
// size del vector, son menores que el maximo aceptar.
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
            log("ConnectionManager: Creando thread para nueva conexion.", LOG_INFO);
            // Validar el resultado de la creacion del thread.
            pthread_t threadId;
            pthread_create(
                &threadId,
                NULL,
                read_client,
                &(this->openedSockets.at(acceptedConnections))
            );
            this->clientsThreadIds.push_back(threadId);
            ++acceptedConnections;
        } else {
            log("ConnectionManager: Conexion rechazada.", LOG_ERROR);
        }
    }
}

void ConnectionManager::waitForAllConnectionsToFinish() {
    log("ConnectionManager: Esperando a que los threads terminen...", LOG_INFO);
    for(pthread_t threadId : this->clientsThreadIds) {
        log("ConnectionManager: Esperando al thread de ID: ", threadId, LOG_INFO);
        pthread_join(threadId, NULL);
        log("ConnectionManager: Termino el thread de ID: ", threadId, LOG_INFO);
    }
    log("ConnectionManager: Los threads terminaron.", LOG_INFO);
}

void ConnectionManager::closeOpenedSockets() {
    log("ConnectionManager: Cerrando sockets abiertos...", LOG_INFO);
    for(int openedSocket : this->openedSockets) {
        close(openedSocket);
        log("ConnectionManager: Se cerro el socket ", openedSocket, LOG_INFO);
    }
    log("ConnectionManager: Socket cerrados.", LOG_INFO);
}

/*
    TODO:
    Falta manejar la desconexion y reconexion.
    Sacar el thread ID de la lista luego de cerrar el socket de la connexion que finaliza.
*/

ConnectionManager::~ConnectionManager() {

}
