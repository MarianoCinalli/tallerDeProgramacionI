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
        int timeoutSeconds;
    public:
        ConnectionManager();
        ConnectionManager(std::string ip, int port);
        bool connectToServer();
        int getMessage(std::string& readMessage);
        bool sendMessage(std::string message);
        void closeConnection();
        int getSocket();
        void setIp(std::string ip);
        void setPort(int port);
        ~ConnectionManager();
};
#endif // CONNECTIONMANAGER_H
