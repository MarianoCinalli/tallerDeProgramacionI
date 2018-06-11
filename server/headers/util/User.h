#include "GameInitializer.h"

#include "controller/actions/Action.h"
#include "controller/actions/Accelerate.h"
#include "controller/actions/Stop.h"
#include "controller/actions/ChangeActivePlayer.h"
#include "controller/actions/SlidingAction.h"
#include "controller/actions/KickingAction.h"
#include "controller/actions/HighKickingAction.h"
#include "controller/actions/RunningFastAction.h"
#include "controller/actions/StopRunningFastAction.h"

// #include "util/Constants.h"
#include "controller/GameControllerProxy.h"
#include "util/logger.h"
#include "util/Broadcaster.h"
#include "util/ConnectionManager.h"
#include "util/UsersManager.h"

#ifndef USER_H
#define USER_H

class User {
    private:
        bool hasLoged;
        bool hasPicked;
        int userSocket;
        int teamNumber;
        std::string user;
        ConnectionManager* connectionManager;
        UsersManager* manager;
        GameControllerProxy* gameControllerProxy;
    public:
        User(GameInitializer* initializer, int socket);
        bool hasLogedIn();
        void processLogInMessage(std::string message);
        bool hasPickedTeamAndFormation();
        void processTeamAndFormationMessage(std::string message);
        Action* getAsAction(std::string message);
        void logOff();
        int getTeam();
        std::string getName();
        ~User();
    private:
        std::string getMessageAction(std::string message);
        std::string getMessageValue(std::string message);
};
#endif // USER_H
