#include <string.h>
#include "model/Pitch.h"
#include "util/Constants.h"
#include "util/logger.h"
#include "util/ConnectionManager.h"

#ifndef BROADCASTER_H
#define BROADCASTER_H

class Broadcaster {
    private:
        Pitch* pitch;
        ConnectionManager* connectionManager;
    public:
        Broadcaster(Pitch* pitch, ConnectionManager* connectionManager);
        void broadcast(bool);
        std::string getFirstMessage();
        std::string getMessage();
        void broadcastGameEnded();
        ~Broadcaster();
};
#endif // BROADCASTER_H
