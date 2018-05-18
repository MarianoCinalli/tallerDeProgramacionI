#include "util/functions.h"

extern GameInitializer* initializer;
extern bool quit;
extern std::mutex quit_mutex;

// Recibe los mensajes del servidor.
// Actualiza el "modelo".
// Se tiene un modelo minimo solo con las propiedades que se usan para dibujar.
// Las vistas ven todo el modelo.
// Se inicializan ambos equipos, en cualquier parte.
// Despues en los mensajes que se reciben, ir actualizando los modelos con la informacion.
// Para los mensajes que llegan del server:
// Los jugadores tienen numeros (this->id en la clase jugador del server) del 1 al 7
// para el equipo local y del 8 al 14 para el visitante. El cliente lo parsea en * (ver abajo).
// Las propiedades son un diccionario que esta luego del tipo.
// Cada propiedad esta como:
// this->id:
//  te: this->team
//  cx: this->position->getX()
//  cy: this->position->getY()
//  se: this->isSelected
//  ki: this->kicking <- Cuidado: Al querer pisar el valor del player con esto,
//      creo que hay que guardar el valor a pisar en wasKicking porque se usa para dibujar.
//  sl: this->sliding <- idem kicking
//  ru: this->runningFast
void* read_server(void* argument) {
    log("read_server: Creado.", LOG_INFO);
    std::string readMessage;
    int readBytes;
    ConnectionManager* connectionManager = (ConnectionManager*) argument;
    int count = 0; // esto esta provisorio.
    log("read_server: Empezando a recibir mensajes. ", LOG_INFO);
    while (count < 10000 && !quit) {
        readMessage = "";
        readBytes = connectionManager->getMessage(readMessage);
        if (readBytes < 0) {
            log("read_client: Error en la lectura del mensaje. Saliendo...", LOG_ERROR);
            setQuit(true);
        } else if (readBytes == 0) {
            // Cuando ser cierra la coneccion del cliente lee 0 bytes sin control.
            // Si puede pasar que la coneccion siga viva y haya un mensaje de 0 bytes hay que buscar otra vuelta.
            log("read_server: No se pudo establecer coneccion con el server. Saliendo...", LOG_INFO);
            setQuit(true);
        } else {
            try {
                Player* player;
                YAML::Node node = YAML::Load(readMessage);
                for (YAML::const_iterator it = node.begin(); it != node.end(); ++it) {
                    YAML::Node key = it->first;
                    YAML::Node value = it->second;
                    // * Aca se sabe que tipo es.
                    if (key.Type() == YAML::NodeType::Scalar) {
                        if (key.as<std::string>() == "ba") {
                            log("read_server: pelota", key.as<std::string>(), LOG_INFO);
                        } else {
                            log("read_server: jugador", key.as<std::string>(), LOG_INFO);
                            player =  initializer->getGameController()->getPlayer(key.as<int>());
                            player->parseYaml(value);
                        }
                    }

                    // Valores para objeto del tipo parseado arriba.
                    if (value.Type() == YAML::NodeType::Map) {
                        // Este for iria en el constructor para el objeto del tipo de arriba.
                        for (YAML::const_iterator secondNode = value.begin(); secondNode != value.end(); ++secondNode) {
                            std::string secondkey = secondNode->first.as<std::string>();
                            std::string secondvalue = secondNode->second.as<std::string>();
                            log("read_server: " + secondkey + " = " + secondvalue, LOG_INFO);
                        }
                    }
                }
            } catch (const std::exception& e) {
                log("read_client: yaml error .what() = ", e.what(), LOG_ERROR);
            }
        }
        count++;
    }
    log("read_server: Finalizado.", LOG_INFO);
    return NULL;
}

// Dibuja las vistas.
// Este metodo esta dentro de todo completo.
// Hay que ver si le falta algo.
void* drawer(void* argument) {
    log("drawer: Creado.", LOG_INFO);
    SDL_Renderer* renderer = initializer->getRenderer();
    PitchView* pitchView = initializer->getPitchView();
    while (!quit) {
        pitchView->render(renderer);
        usleep(50000); // Frame rate.
    }
    log("drawer: Finalizado.", LOG_INFO);
    return NULL;
}

// Setea el quit de manera segura.
void setQuit(bool newQuit) {
    quit_mutex.lock();
    quit = quit || newQuit;
    quit_mutex.unlock();
}
