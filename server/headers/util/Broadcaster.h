#include <string.h>
#include "model/Pitch.h"
#include "util/Constants.h"
#include "util/logger.h"
#include "util/ConnectionManager.h"

#ifndef BROADCASTER_H
#define BROADCASTER_H

class Broadcaster {
    private:
        GameControllerProxy* gameControllerProxy;
        ConnectionManager* connectionManager;
    public:
        Broadcaster(GameControllerProxy* gameControllerProxy, ConnectionManager* connectionManager);
        void broadcast(bool);
        void broadcastGameEnded();
        ~Broadcaster();
};
#endif // BROADCASTER_H
