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
        void updateModel();
        bool shouldGameEnd();
        void end();
        void startGame();
        void removeUser(std::string user);
        bool hasGameStarted();
        int getUsersInTeam(int teamNumber);
        bool joinTeam(std::string playerName, int team, int formation, int maxPlayers, std::string &errorMessage);
        bool setTeamFormation(int team,int formation);
        std::string getMessageToBroadcast(bool allPlayers);
        ~GameControllerProxy();
};
#endif // GAMECONTROLLERPROXY_H
