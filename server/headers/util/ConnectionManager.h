#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include <vector>
#include "util/Constants.h"
#include "util/logger.h"
#include "util/connection_functions.h"
#include "util/ThreadSpawner.h"

#ifndef CONNECTIONMANAGER_H
#define CONNECTIONMANAGER_H

class ConnectionManager {
    private:
        int port;
        int maxConnections;
        int my_socket;
        struct sockaddr_in address;
        ThreadSpawner* clients;
        std::vector<int> openedSockets;
    public:
        ConnectionManager(int port, int maxConnections);
        bool openConnections();
        // Acepta un numero infinito de conecciones.
        void acceptConnections();
        // Acepta un numero finito de conecciones.
        void acceptConnectionsUntilMax();
        void waitForAllConnectionsToFinish();
        void closeOpenedSockets();
        ~ConnectionManager();
};
#endif // CONNECTIONMANAGER_H
