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
    bool result = false;
    for (auto const& userAndPassword : this->usersAndPasswords) {
        if ((userAndPassword.first == user) && (userAndPassword.second == password)) {
            if (this->canLogIn(user)) {
                this->loggedInUsers.insert(user);
                log("UsersManager: Se loggeo el usuario: ", user, LOG_INFO);
                result = true;
            } else {
                log("UsersManager: No se pudo loggear el usuario: ", user, LOG_INFO);
            }
            users_mutex.unlock();
            return result;
        }
    }
    log("UsersManager: No se pudo loggear el usuario:" + user + " con password: " + password, LOG_INFO);
    users_mutex.unlock();
    return result;
}

bool UsersManager::canLogIn(std::string user) {
    return !this->isLoggedIn(user);
}

bool UsersManager::isLoggedIn(std::string user) {
    log("UsersManager: Buscando " + user + " en la lista de loggeados...", LOG_DEBUG);
    auto loggedInSearch = this->loggedInUsers.find(user);
    bool encontrado = loggedInSearch != this->loggedInUsers.end();
    if (encontrado) {
        log("UsersManager: Loggeado encontrado a ", user, LOG_DEBUG);
    } else {
        log("UsersManager: Loggeado no encontrado a ", user, LOG_DEBUG);
    }
    return encontrado;
}

bool UsersManager::isLoggedOff(std::string user) {
    log("UsersManager: Buscando " + user + "en la lista de desloggeados...", LOG_ERROR);
    auto loggedOffSearch = this->loggedOffUsers.find(user);
    bool encontrado = loggedOffSearch != this->loggedOffUsers.end();
    if (encontrado) {
        log("UsersManager: Desloggeado encontrado a ", user, LOG_DEBUG);
    } else {
        log("UsersManager: Desloggeado no encontrado a ", user, LOG_DEBUG);
    }
    return encontrado;
}

void UsersManager::logOff(std::string user) {
    users_mutex.lock();
    // Necesito el iterator por eso no uso la func de arriba.
    auto loggedInSearch = this->loggedInUsers.find(user);
    if(loggedInSearch != this->loggedInUsers.end()) {
        log("UsersManager: Estaba logeado removiendo al usuario " + user + " de la lista.", LOG_INFO);
        this->loggedInUsers.erase(loggedInSearch);
        log("UsersManager: Agregando a la lista de deslogeados a: " + user, LOG_INFO);
        this->loggedOffUsers.insert(user);
    } else {
        log("UsersManager: El usuario a deslogear no esta en la lista de loggeados: " + user, LOG_ERROR);
        if(this->isLoggedOff(user)) {
            log("UsersManager: No estaba en lista de desloggeados, agregando: ", user ,LOG_ERROR);
            this->loggedOffUsers.insert(user);
        } else {
            log("UsersManager: Estaba en la lista de deslogeados. Que paso?", LOG_ERROR);
        }
    }
    users_mutex.unlock();
}

UsersManager::~UsersManager() {
    log("UsersManager: Borrado UsersManager.", LOG_INFO);
}
