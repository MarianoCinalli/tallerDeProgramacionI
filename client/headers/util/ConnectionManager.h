#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <errno.h>
#include "util/Constants.h"
#include "util/logger.h"

#ifndef CONNECTIONMANAGER_H
#define CONNECTIONMANAGER_H

class ConnectionManager {
    private:
        std::string ip;
        int port;
        int my_socket;
    public:
        ConnectionManager();
        ConnectionManager(std::string ip, int port);
        bool connectToServer();
        void sendToServer(std::string message);
        std::string getMessage();
        void closeConnection();
        int getSocket();
        void setIp(std::string ip);
        void setPort(int port);
        ~ConnectionManager();
};
#endif // CONNECTIONMANAGER_H
