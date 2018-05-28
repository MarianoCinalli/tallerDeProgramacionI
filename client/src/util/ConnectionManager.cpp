#include "util/ConnectionManager.h"

ConnectionManager::ConnectionManager() {
    this->my_socket = 0;
}

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

int ConnectionManager::getSocket() {
    return this->my_socket;
}

void ConnectionManager::setIp(std::string ip) {
    this->ip = ip;
}

void ConnectionManager::setPort(int port) {
    this->port = port;
}

int ConnectionManager::getMessage(std::string& readMessage) {
    int readBytes;
    int bufferLength = 10000;
    int bufferSize = sizeof(char) * bufferLength;
    char buffer[bufferLength] = {0};
    memset(buffer, 0x00, bufferSize);
    readBytes = read(this->my_socket, buffer, bufferSize);
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
        // readMessage = buffer;
        // std::string s(buffer, readBytes/sizeof(char));
        readMessage = buffer;
    }
    return readBytes;
}

bool ConnectionManager::sendMessage(std::string message) {
    log("ConnectionManager: Enviando: ", message, LOG_SPAM);
    const char* constantMessage = message.c_str();
    int sendBytes = send(this->my_socket, constantMessage, strlen(constantMessage), 0);
    bool success = false;
    if (sendBytes < 0) {
        log("ConnectionManager: Escritura fallida: ", strerror(errno), LOG_ERROR);
    } else if (sendBytes == 0) {
        log("ConnectionManager: Escritura igual a 0. ", LOG_ERROR);
    } else {
        success = true;
    }
    log("ConnectionManager: Mensaje enviado. Bytes enviados: ", sendBytes, LOG_SPAM);
    return success;
}

void ConnectionManager::closeConnection() {
    close(this->my_socket);
}

ConnectionManager::~ConnectionManager() {

}
