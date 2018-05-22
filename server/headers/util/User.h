#include "GameInitializer.h"

#include "controller/actions/Action.h"
#include "controller/actions/Accelerate.h"
#include "controller/actions/Stop.h"
#include "controller/actions/ChangeActivePlayer.h"
#include "controller/actions/SlidingAction.h"
#include "controller/actions/KickingAction.h"
#include "controller/actions/RunningFastAction.h"
#include "controller/actions/StopRunningFastAction.h"

#include "util/Constants.h"
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
        ConnectionManager* connectionManager;
        int userSocket;
        int teamNumber;
        UsersManager* manager;
    public:
        User(GameInitializer* initializer, int socket);
        bool hasLogedIn();
        void processLogInMessage(std::string message);
        bool hasPickedTeamAndFormation();
        void processTeamAndFormationMessage(std::string message);
        Action* getAsAction(std::string message);
        int getTeam();
        ~User();
    private:
        std::string getMessageAction(std::string message);
        std::string getMessageValue(std::string message);
};
#endif // USER_H
