#include "util/connection_functions.h"

void* read_client(void* argument) {
    int socket;
    int valread;
    char buffer[1024] = {0};
    char firstChar = 'A';
    socket = *((int*) argument);
    log("read_client: Socket: ", socket, LOG_DEBUG);
    log("read_client: Thread spawneado.", LOG_DEBUG);
    // Poner algo global para controlar el juego.
    // La F es solo para que un mensaje finalize el thread.
    // Hay que buscarle la vuelta con una var global.
    while (firstChar != 'F') {
        valread = read(socket, buffer, 1024);
        firstChar = buffer[0];
        log("read_client: Bytes Recibidos " + std::to_string(valread) + " - Mensaje: ", buffer, LOG_DEBUG);
    }
    log("read_client: Termino el Thread.", valread, LOG_DEBUG);
    return NULL;
}
