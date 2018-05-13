#include "util/ConnectionManager.h"

ConnectionManager::ConnectionManager(std::string ip, int port) {
    this->ip = ip;
    this->port = port;
    this->my_socket = 0;
}

bool ConnectionManager::connectToServer() {
    struct sockaddr_in serv_addr;
    log("ConnectionManager: Creando el socket...", LOG_INFO);
    this->my_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (this->my_socket == 0) {
        log("ConnectionManager: Creacion del socket fallida: ", strerror(errno), LOG_ERROR);
        return false;
    }

    memset(&serv_addr, '0', sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(this->port);
    // Convert IPv4 and IPv6 addresses from text to binary form
    log("ConnectionManager: Convirtiendo direccion a binario " + this->ip + ":" , this->port, LOG_INFO);
    if (inet_pton(AF_INET, this->ip.c_str(), &serv_addr.sin_addr) <= 0) {
        log("ConnectionManager: Invalid address/Address not supported: ", strerror(errno), LOG_ERROR);
        return false;
    }

    log("ConnectionManager: Conectando...", LOG_INFO);
    if (connect(this->my_socket, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        log("ConnectionManager: Conexion fallida: ", strerror(errno), LOG_ERROR);
        return false;
    }

    log("ConnectionManager: Conexion exitosa.", LOG_INFO);
    return true;
}

void ConnectionManager::sendToServer(std::string message) {
    log("ConnectionManager: Enviando: \n", message, LOG_DEBUG);
    const char * constantMessage = (message).c_str();
    send(this->my_socket, constantMessage, strlen(constantMessage)+4, 0);
    log("ConnectionManager: Mensaje enviado.", LOG_DEBUG);
}

int ConnectionManager::getSocket() {
    return this->my_socket;
}

void ConnectionManager::closeConnection() {
    close(this->my_socket);
}

ConnectionManager::~ConnectionManager() {

}
