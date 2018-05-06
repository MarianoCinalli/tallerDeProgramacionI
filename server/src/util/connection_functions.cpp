#include "util/connection_functions.h"

void* read_client(void* argument) {
    int socket;
    int valread;
    char buffer[1024] = {0};
    socket = *((int*) argument);
    // Poner algo global para controlar el juego.
    while (1) {
        valread = read(socket, buffer, 1024);
        printf("%d - %s\n", valread, buffer);
    }
    return NULL;
}
