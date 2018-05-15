#include "util/functions.h"

extern GameInitializer* initializer;

// Recibe los mensajes del servidor.
// Actualiza el "modelo".
void* read_server(void* argument) {
    log("read_server: Creado.", LOG_INFO);
    std::string readMessage;
    ConnectionManager* connectionManager = (ConnectionManager*) argument;
    int count = 0; // esto esta provisorio.
    log("read_server: Empezando a recibir mensajes. ", LOG_INFO);
    while (count < 1) {
        readMessage = connectionManager->getMessage();
        try {
            YAML::Node node = YAML::Load(readMessage);
            for (YAML::const_iterator it = node.begin(); it != node.end(); ++it) {
                YAML::Node key = it->first;
                YAML::Node value = it->second;
                // Aca se sabe que tipo es.
                if (key.Type() == YAML::NodeType::Scalar) {
                    if (key.as<std::string>() == "ba") {
                        log("read_server: pelota", LOG_INFO);
                    } else {
                        log("read_server: jugador", LOG_INFO);
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
