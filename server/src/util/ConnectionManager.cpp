#include "util/ConnectionManager.h"

ConnectionManager::ConnectionManager(int port, int maxConnections) {
    this->port = port;
    this->maxConnections = maxConnections;
    this->acceptedConnections = 0;
    this->clients = new ThreadSpawner();
    this->socketCache = {};
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

void ConnectionManager::acceptConnection() {
    int new_socket;
    int addrlen = sizeof(this->address);
    // Esto se bloquea hasta que haya una conexion entrante.
    new_socket = accept(this->my_socket, (struct sockaddr*)&this->address, (socklen_t*)&addrlen);
    log("ConnectionManager: Aceptando conexion...", LOG_INFO);
    if (new_socket > 0) {
        if (this->hasRoom()) {
            log("ConnectionManager: Conexion aceptada.", LOG_INFO);
            log("ConnectionManager: Creando thread para nueva conexion.", new_socket, LOG_INFO);
            this->socketCache.push_back(new_socket);
            pthread_t threadId = this->clients->spawn(
                read_client,
                &(this->socketCache.back())
            );
            if (threadId != 0) {
                this->threadIdsAndSockets[threadId] = new_socket;
                ++this->acceptedConnections;
            } else {
                log("ConnectionManager: Error creando lector de cliente.", LOG_ERROR);
                close(new_socket);
            }
        } else {
            log("ConnectionManager: Conexion rechazada por falta de espacio, numero de clientes: ", this->acceptedConnections, LOG_INFO);
            std::string message = "noRoom:" + std::to_string(this->acceptedConnections);
            this->sendMessage(new_socket, message);
            close(new_socket);
        }
    } else {
        log("ConnectionManager: Conexion rechazada.", LOG_ERROR);
    }
}

bool ConnectionManager::hasRoom() {
    return this->acceptedConnections < this->maxConnections;
}

std::vector<int> ConnectionManager::getSockets() {
    std::vector<int> sockets;
    for (auto const& threadAndSocket : this->threadIdsAndSockets) {
        sockets.push_back(threadAndSocket.second);
    }
    return sockets;
}

void ConnectionManager::processDisconection(pthread_t connectionHandlerId) {
    log("ConnectionManager: Liberando recursos de: ", connectionHandlerId, LOG_INFO);
    close(this->threadIdsAndSockets[connectionHandlerId]);
    this->threadIdsAndSockets.erase(connectionHandlerId);
    --this->acceptedConnections;
    log("ConnectionManager: Recursos liberados.", LOG_INFO);
}

void ConnectionManager::waitForAllConnectionsToFinish() {
    log("ConnectionManager: Esperando a que los clientes terminen...", LOG_INFO);
    this->clients->joinSpawnedThreads();
    log("ConnectionManager: Los clientes terminaron.", LOG_INFO);
}

void ConnectionManager::closeOpenedSockets() {
    log("ConnectionManager: Cerrando sockets abiertos...", LOG_INFO);
    for (auto const& threadAndSocket : this->threadIdsAndSockets) {
        log("ConnectionManager: Cerrando socket de: ", threadAndSocket.first, LOG_INFO);
        close(threadAndSocket.second);
    }
    log("ConnectionManager: Socket cerrados.", LOG_INFO);
}

int ConnectionManager::getMessage(int socket, std::string & readMessage) {
    int readBytes;
    int bufferLength = 1024;
    int bufferSize = sizeof(char) * bufferLength;
    char buffer[bufferLength] = {0};
    memset(buffer, 0x00, bufferSize);
    readBytes = read(socket, buffer, bufferSize);
    // Cuidado aca con strerror que no es thread safe:
    // Otro thread puede setear el errno, y este escribirlo.
    // Ver de usar strerror_r que es thread safe.
    if (readBytes < 0) {
        log("ConnectionManager: Lectura fallida: ", strerror(errno), LOG_ERROR);
        readMessage = "";
    } else if (readBytes == 0) {
        log("ConnectionManager: Lectura igual a 0. ", LOG_ERROR);
        readMessage = "";
    } else {
        log("ConnectionManager: Recibidos ", readBytes, LOG_DEBUG);
        readMessage = buffer;
    }
    return readBytes;
}

void ConnectionManager::sendMessage(int socket, std::string message) {
    const char* constantMessage = (message).c_str();
    send(socket, constantMessage, strlen(constantMessage), 0);
}

void ConnectionManager::sendToAll(std::string message) {
    for (auto const& threadAndSocket : this->threadIdsAndSockets) {
        this->sendMessage(threadAndSocket.second, message);
    }
}

int ConnectionManager::getMaxClients(){
    return this->maxConnections;
}

ConnectionManager::~ConnectionManager() {
    delete(this->clients);
}
