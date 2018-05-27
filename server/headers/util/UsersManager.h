#include "util/Constants.h"
#include "util/logger.h"
#include <map>
#include <mutex>
#include <string>
#include <set>
#include "controller/GameControllerProxy.h"

#ifndef USERSMANAGER_H
#define USERSMANAGER_H

class UsersManager {
    private:
        std::map<std::string, std::string> usersAndPasswords;
        std::set<std::string> loggedInUsers;
        std::set<std::string> loggedOffUsers;
        GameControllerProxy* gameControllerProxy;
    public:
        UsersManager(std::map<std::string, std::string> usersAndPasswords, GameControllerProxy* gameControllerProxy);
        bool logIn(std::string user, std::string password, std::string &errorMessage);
        void processLogIn(std::string user);
        bool canLogIn(std::string user, std::string &errorMessage);
        bool isLoggedIn(std::string user);
        bool isLoggedOff(std::string user);
        void logOff(std::string user);
        ~UsersManager();
};
#endif // USERSMANAGER_H
