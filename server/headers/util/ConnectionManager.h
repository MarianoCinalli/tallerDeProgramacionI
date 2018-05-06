#ifndef CONNECTIONMANAGER_H
#define CONNECTIONMANAGER_H

class ConnectionManager {
    private:
        int port;
        int maxConnections;
        int socket;
        struct sockaddr_in address;
        std::vector<pthread_t> clientsThreadIds;
        std::vector<int> openedSockets;
    public:
        ConnectionManager(int port, int maxConnections);
        // Acepta un numero infinito de conecciones.
        void acceptConnections();
        // Acepta un numero finito de conecciones.
        void acceptConnectionsUntilMax();
        ~ConnectionManager();
};
#endif // CONNECTIONMANAGER_H


