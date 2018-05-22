#include <mutex>
#include "util/Constants.h"
#include "util/logger.h"
#include "controller/GameController.h"
#include "view/Camera.h"

#ifndef GAMECONTROLLERPROXY_H
#define GAMECONTROLLERPROXY_H

class GameControllerProxy {
    private:
        GameController* gameController;
        bool hasStarted;
    public:
        GameControllerProxy(GameController* gameController);
        void execute(Action* action, int team);
        void updateModel(Camera* camera);
        bool shouldGameEnd();
        void end();
        void startGame();
        bool hasGameStarted();
        ~GameControllerProxy();
};
#endif // GAMECONTROLLERPROXY_H
