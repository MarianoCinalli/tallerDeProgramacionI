#include "GameInitializer.h"
#include "util/Constants.h"
#include "util/logger.h"
#include "util/Broadcaster.h"
#include "util/ConnectionManager.h"
#include "controller/actions/Action.h"

#ifndef USER_H
#define USER_H

class User {
    private:
        bool hasLoged;
        bool hasPicked;
        ConnectionManager* connectionManager;
        int userSocket;
        int teamNumber;
    public:
        User(GameInitializer* initializer, int socket);
        bool hasLogedIn();
        void processLogInMessage(std::string message);
        bool hasPickedTeamAndFormation();
        void processTeamAndFormationMessage(std::string message);
        Action* getAsAction(std::string message);
        int getTeam();
        ~User();
};
#endif // USER_H
