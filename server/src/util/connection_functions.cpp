#include "util/connection_functions.h"
#include <yaml-cpp/yaml.h>

void* read_client(void* argument) {
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
        memset( buffer, 0x00, sizeof(char)*1024);
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
            YAML::Node node = YAML::Load(buffer);
            std::string name;
            std::string parameter1;
            if (node["command"]) {
              name =  node["command"].as<std::string>();
              log("leido mensaje con nombre: ", name, LOG_INFO);
            }
            if (node["parameters"]) {
              parameter1 = node["parameters"][0].as<std::string>();
              log("leido primer parametro con valor: ", parameter1, LOG_INFO);
            }
        }
    }
    log("read_client: Termino el Thread.", LOG_DEBUG);
    return NULL;
}
