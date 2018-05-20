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
    public:
        Broadcaster(Pitch* pitch);
        void broadcast(std::vector<int> sockets);
        std::string getMessage();
        ~Broadcaster();
};
#endif // BROADCASTER_H
