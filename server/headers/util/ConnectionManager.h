#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <string>
#include <errno.h>

#include <vector>
#include <map>
#include <pthread.h>
#include "util/Constants.h"
#include "util/logger.h"
#include "util/functions.h"
#include "util/ThreadSpawner.h"
#include "view/Camera.h"

#ifndef CONNECTIONMANAGER_H
#define CONNECTIONMANAGER_H

class ConnectionManager {
    private:
        int port;
        int maxConnections;
        int my_socket;
        int acceptedConnections;
        struct sockaddr_in address;
        ThreadSpawner* clients;
        std::map<pthread_t, int> threadIdsAndSockets;
        std::vector<int> socketCache;
    public:
        ConnectionManager(int port, int maxConnections);
        // Prepara el socket para escucha.
        bool openConnections();
        // Acepta una conexion si hay lugar disponible.
        void acceptConnection();
        // Espera a que todos los threads de los clientes terminen.
        void waitForAllConnectionsToFinish();
        // Cierra los file descriptors para cada socket.
        void closeOpenedSockets();
        // Devuelve el mensaje, en readMessage, leido a un socket y los bytes leidos (para validar)
        int getMessage(int socket, std::string& readMessage);
        // Envia un mensaje a un socket.
        void sendMessage(int socket, std::string message);
        bool hasRoom();
        std::vector<int> getSockets();
        void processDisconection(pthread_t connectionHandlerId);
        ~ConnectionManager();
};
#endif // CONNECTIONMANAGER_H
