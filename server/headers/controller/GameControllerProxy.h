#include "util/Constants.h"
#include "util/logger.h"
#include "controller/GameController.h"

#ifndef GAMECONTROLLERPROXY_H
#define GAMECONTROLLERPROXY_H

class GameControllerProxy {
    private:
        GameController* gameController;
    public:
        GameControllerProxy(GameController* gameController);
        void execute(Action* action, int team);
        ~GameControllerProxy();
};
#endif // GAMECONTROLLERPROXY_H


