#include "util/Constants.h"
#include "util/logger.h"
#include <map>
#include <mutex>
#include <string>
#include <set>

#ifndef USERSMANAGER_H
#define USERSMANAGER_H

class UsersManager {
    private:
        std::map<std::string, std::string> usersAndPasswords;
        std::set<std::string> loggedInUsers;
        std::set<std::string> loggedOffUsers;
    public:
        UsersManager(std::map<std::string, std::string> usersAndPasswords);
        bool logIn(std::string user, std::string password);
        bool canLogIn(std::string user);
        bool isLoggedIn(std::string user);
        bool isLoggedOff(std::string user);
        void logOff(std::string user);
        ~UsersManager();
};
#endif // USERSMANAGER_H
