#include <mutex>
#include "util/Constants.h"
#include "util/logger.h"
#include "controller/GameController.h"
#include "view/Camera.h"


#ifndef GAMECONTROLLERPROXY_H
#define GAMECONTROLLERPROXY_H

class GameControllerProxy {
    private:

        bool hasStarted;
    public:
      GameController* gameController; //despues hacerlo privado
        GameControllerProxy(GameController* gameController);
        void execute(Action* action, std::string team);
        void updateModel(Camera* camera);
        bool shouldGameEnd();
        void end();
        void startGame();
        void addUser(std::string user, int teamNum);
        void removeUser(std::string user);
        bool hasGameStarted();
        std::string getTeamStats(int numberTeam);
        bool joinTeam(std::string playerName, int team, int maxPlayers, std::string &errorMessage);
        ~GameControllerProxy();
};
#endif // GAMECONTROLLERPROXY_H
