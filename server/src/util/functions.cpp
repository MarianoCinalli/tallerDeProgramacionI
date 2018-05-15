#include "util/functions.h"

extern GameInitializer* initializer;

// Lee los mensajes de los clientes y actualiza el modelo.
void* read_client(void* argument) {
    log("read_client: Creado.", LOG_INFO);
    int socket;
    int readBytes;
    char buffer[1024] = {0};
    bool continueReading = true;
    socket = *((int*) argument);
    log("read_client: Socket: ", socket, LOG_DEBUG);
    log("read_client: Thread spawneado.", LOG_DEBUG);
    // Poner algo global para controlar el juego.
    // La F es solo para que un mensaje finalize el thread.
    // Hay que buscarle la vuelta con una var global.
    while (continueReading) {
        // importante resetear el buffer a cero sino al sobreescribirse queda
        // lo anterior. hacer esto o copiar en un buffer aparte la cantidad
        //  de bytes recibidos
        memset(buffer, 0x00, sizeof(char) * 1024);
        readBytes = read(socket, buffer, 1024);

        if (readBytes < 0) {
            // Cuidado aca con strerror que no es thread safe:
            // Otro thread puede setear el errno, y este escribirlo.
            // Ver de usar strerror_r que es thread safe.
            log("read_client: Error en la lectura del socket: ", strerror(errno), LOG_ERROR);
            continueReading = false;
        } else if (readBytes == 0) {
            // Cuando ser cierra la coneccion del cliente lee 0 bytes sin control.
            // Si puede pasar que la coneccion siga viva y haya un mensaje de 0 bytes hay que buscar otra vuelta.
            log("read_client: Leyo 0 bytes. Saliendo...", LOG_INFO);
            continueReading = false;
        } else {
            log("read_client: Bytes Recibidos " + std::to_string(readBytes) + " - Mensaje: ", buffer, LOG_DEBUG);
            continueReading = (buffer[0] != 'F');
            // Aca se parsea el buffer, problema con que se pisan cuando van muy rapid
            // quizas definir el tamaño de entrada y leer bien, o mandar un struct
            // try {
            //     YAML::Node node = YAML::Load(buffer);
            //     std::string name;
            //     std::string parameter1;
            //     if (node["command"]) {
            //         name =  node["command"].as<std::string>();
            //         log("read_client: Leido mensaje con nombre: ", name, LOG_INFO);
            //     }
            //     if (node["parameters"]) {
            //         parameter1 = node["parameters"].as<std::string>();
            //         log("read_client: Leido primer parametro con valor: ", parameter1, LOG_INFO);
            //     }
            // } catch (const std::exception& e) {
            //     log("read_client: yaml error .what() = ", e.what(), LOG_ERROR);
            // }
        }
    }
    log("read_client: Finalizado.", LOG_INFO);
    return NULL;
}

// Este es el que le envia el juego a los clientes.
void* broadcast_to_clients(void* argument) {
    log("broadcast_to_clients: Creado.", LOG_INFO);
    std::vector<int> sockets = *((std::vector<int>*) argument);
    Broadcaster* broadcaster = new Broadcaster(initializer->getPitch(), &sockets);
    // broadcaster->broadcastGameBegins();
    int count = 0; // esto esta provisorio.
    log("broadcast_to_clients: Se comienza a broadcastear...", LOG_INFO);
    while (count < 1) {
        broadcaster->broadcast();
        count++;
        usleep(MICROSECONDS_BETWEEEN_BROADCAST);
    }
    delete(broadcaster);
    log("broadcast_to_clients: Finalizado.", LOG_INFO);
    return NULL;
}

// Actualiza el modelo dependiendo de las propiedades.
void* game_updater(void* argument) {
    log("game_updater: Creado.", LOG_INFO);
    Camera* camera = initializer->getCamera();
    GameController* gameController = initializer->getGameController();
    while (gameController->shouldGameEnd()) {
        gameController->updatePlayers();
        // gameController->updateBall();
        gameController->updateCameraPosition(camera);
        usleep(MICROSECONDS_BETWEEEN_BROADCAST/10);
    }
    log("game_updater: Finalizado.", LOG_INFO);
    return NULL;
}
