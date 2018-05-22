#include "util/Constants.h"
#include "util/logger.h"
#include <map>
#include <mutex>
#include <string>

#ifndef USERSMANAGER_H
#define USERSMANAGER_H

class UsersManager {
    private:
        std::map<std::string, std::string> usersAndPasswords;
    public:
        UsersManager(std::map<std::string, std::string> usersAndPasswords);
        bool logIn(std::string user, std::string password);
        ~UsersManager();
};
#endif // USERSMANAGER_H


