#include <arpa/inet.h>
#include <string.h>
#include <errno.h>
#include "view/Camera.h"
#include "util/Constants.h"
#include "util/logger.h"

#ifndef BROADCASTER_H
#define BROADCASTER_H

class Broadcaster {
    private:
        Camera* camera;
        std::vector<int> sockets;
    public:
        Broadcaster(Camera* camera, std::vector<int>* sockets);
        void broadcast();
        ~Broadcaster();
};
#endif // BROADCASTER_H
