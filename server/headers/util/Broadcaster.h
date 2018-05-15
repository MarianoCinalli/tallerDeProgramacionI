#include <arpa/inet.h>
#include <string.h>
#include <vector>
#include <errno.h>
#include "model/Pitch.h"
#include "util/Constants.h"
#include "util/logger.h"

#ifndef BROADCASTER_H
#define BROADCASTER_H

class Broadcaster {
    private:
        Pitch* pitch;
        std::vector<int> sockets;
    public:
        Broadcaster(Pitch* pitch, std::vector<int>* sockets);
        void broadcast();
        std::string getMessage();
        ~Broadcaster();
};
#endif // BROADCASTER_H
