#include "util/functions.h"

extern GameInitializer* initializer;

// Recibe los mensajes del servidor.
// Actualiza el "modelo".
void* read_server(void* argument) {
    log("read_server: Creado.", LOG_INFO);
    int socket;
    int readBytes;
    char buffer[1024] = {0};
    socket = *((int*) argument);
    int count = 0; // esto esta provisorio.
    log("read_server: Empezando a recibir mensajes. ", LOG_INFO);
    while (count < 11) {
        memset(buffer, 0x00, sizeof(char) * 1024);
        readBytes = read(socket, buffer, 1024);
        log("read_server: Recibido ", readBytes, LOG_DEBUG);
        log("read_server: Recibido ", buffer, LOG_DEBUG);
        count++;
    }
    log("read_server: Finalizado.", LOG_INFO);
    return NULL;
}

// Dibuja las vistas.
void* drawer(void* argument) {
    log("drawer: Creado.", LOG_INFO);
/*
    SDL_Renderer* renderer = initializer->getRenderer();
    PitchView* pitchView = initializer->getPitchView();
    bool quit = false;
    while (!quit) {
        pitchView->render(renderer);
        usleep(sleepTime); // Frame rate.
    }
*/
    log("drawer: Finalizado.", LOG_INFO);
    return NULL;
}
