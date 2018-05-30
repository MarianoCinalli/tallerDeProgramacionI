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
                ++this->acceptedConnections;
            } else {
                log("ConnectionManager: Error creando lector de cliente.", LOG_ERROR);
                close(new_socket);
            }
        } else {
            log("ConnectionManager: Conexion rechazada por falta de espacio, numero de clientes: ", this->acceptedConnections, LOG_INFO);
            std::string message = "noRoom:" + std::to_string(this->acceptedConnections);
            this->sendMessage(new_socket, message);
            // Esto no cierra el socket.
            // Solo lo inhabilita para send y recieve.
            // Cuando se llame a closeOpenedSockets() se cierra los colgados.
            shutdown(new_socket, SHUT_RDWR);
            this->socketsRejected.push_back(new_socket);
        }
    } else {
        log("ConnectionManager: Conexion rechazada.", LOG_ERROR);
    }
}

void ConnectionManager::ready(pthread_t threadId, int socket) {
    log("ConnectionManager: Socket is ready for broadcast: ", socket, LOG_INFO);
    this->threadIdsAndSockets[threadId] = socket;
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
    auto search = this->threadIdsAndSockets.find(connectionHandlerId);
    if(search != this->threadIdsAndSockets.end()) {
        int socketToClose = search->second;
        log("ConnectionManager: Procesando la desconexion de: ", socketToClose, LOG_INFO);
        this->threadIdsAndSockets.erase(connectionHandlerId);
        close(socketToClose);
        --this->acceptedConnections;
    } else {
        log("ConnectionManager: No se encontro el cliente a desconectar, correspondiente a: ", connectionHandlerId, LOG_ERROR);
    }
}

void ConnectionManager::waitForAllConnectionsToFinish() {
    log("ConnectionManager: Esperando a que los clientes terminen...", LOG_INFO);
    this->clients->joinSpawnedThreads();
    log("ConnectionManager: Los clientes terminaron.", LOG_INFO);
}

void ConnectionManager::closeOpenedSockets() {
    log("ConnectionManager: Cerrando sockets aceptados que quedaron abiertos...", LOG_INFO);
    for (auto const& threadAndSocket : this->threadIdsAndSockets) {
        log("ConnectionManager: Cerrando socket de: ", threadAndSocket.first, LOG_INFO);
        close(threadAndSocket.second);
    }
    log("ConnectionManager: Cerrando sockets rechazados que quedaron abiertos...", LOG_INFO);
    for(int socket : this->socketsRejected) {
        log("ConnectionManager: Cerrando socket de: ", socket, LOG_INFO);
        close(socket);
    }
    log("ConnectionManager: Socket cerrados.", LOG_INFO);
}

int ConnectionManager::getMessage(int socket, std::string & readMessage) {
    int readBytes;
    int bufferLength = 1024;
    int bufferSize = sizeof(char) * bufferLength;
    char buffer[bufferLength] = {0};
    memset(buffer, 0x00, bufferSize);

    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 10000; //HACK, timeout en microsegundos

    fd_set rfds;
    FD_ZERO(&rfds);
    FD_SET(socket, &rfds);
    int rv = select(socket + 1,  &rfds,NULL, NULL, &timeout);
    if (rv<0){
      log("Connection Manager: socket no disponible, error:", strerror(errno), LOG_ERROR);
      return rv;
    } else if (rv == 0){
      log("Connection Manager: timeout, 0 bytes recieved. ",strerror(errno), LOG_DEBUG); //no es propiamente un error si es timeout
      // log("Connection Manager: segundos", timeout.tv_sec, LOG_ERROR);
      // log("Connection Manager: microsegundos", timeout.tv_usec, LOG_ERROR);
    } else{
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
        log("ConnectionManager: Recibidos ", readBytes, LOG_SPAM);
        readMessage = buffer;

    }
    return readBytes;
  }
    return rv;

}

void ConnectionManager::sendMessage(int socket, std::string message) {
    log("ConnectionManager: Enviando " + message + " a ", socket, LOG_SPAM);
    const char* constantMessage = (message).c_str();
    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 100000/12; //HACK, timeout en microsegundos

    fd_set wfds;
    FD_ZERO(&wfds);
    FD_SET(socket, &wfds);
    if(select(socket + 1, NULL, &wfds, NULL, &timeout) < 0) {
        log("Connection Manager: error en socket al escribir, posible timeout ", strerror(errno), LOG_DEBUG);
    }else if(FD_ISSET(socket, &wfds)){
        send(socket, constantMessage, strlen(constantMessage), 0);
    }

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
    log("ConnectionManager: Liberando recursos...", LOG_INFO);
    this->waitForAllConnectionsToFinish();
    this->closeOpenedSockets();
    delete(this->clients);
    log("ConnectionManager: Recursos liberados.", LOG_INFO);
}
