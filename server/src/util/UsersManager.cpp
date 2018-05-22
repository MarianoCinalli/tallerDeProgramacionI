#include "util/UsersManager.h"

extern std::mutex users_mutex;

UsersManager::UsersManager(std::map<std::string, std::string> usersAndPasswords) {
    log("UsersManager: Creando UsersManager...", LOG_INFO);
    for (auto const& userAndPassword : usersAndPasswords) {
        log("UsersManager: Cargando usuario: " + userAndPassword.first + " con password: " + userAndPassword.second, LOG_DEBUG);
        this->usersAndPasswords[userAndPassword.first] = userAndPassword.second;
    }
    log("UsersManager: UsersManager creado.", LOG_INFO);
}

bool UsersManager::logIn(std::string user, std::string password) {
    users_mutex.lock();
    for (auto const& userAndPassword : this->usersAndPasswords) {
        if ((userAndPassword.first == user) && (userAndPassword.second == password)) {
            log("UsersManager: Se loggeo el usuario: ", user, LOG_INFO);
            users_mutex.unlock();
            return true;
        }
    }
    log("UsersManager: No se pudo loggear el usuario:" + user + " con password: " + password, LOG_INFO);
    users_mutex.unlock();
    return false;
}

UsersManager::~UsersManager() {
    log("UsersManager: Borrado UsersManager.", LOG_INFO);
}
