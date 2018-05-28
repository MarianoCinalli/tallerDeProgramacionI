#include "util/UsersManager.h"

extern std::mutex users_mutex;

UsersManager::UsersManager(std::map<std::string, std::string> usersAndPasswords, GameControllerProxy* gameControllerProxy) {
    log("UsersManager: Creando UsersManager...", LOG_INFO);
    for (auto const& userAndPassword : usersAndPasswords) {
        log("UsersManager: Cargando usuario: " + userAndPassword.first + " con password: " + userAndPassword.second, LOG_DEBUG);
        this->usersAndPasswords[userAndPassword.first] = userAndPassword.second;
    }
    this->gameControllerProxy = gameControllerProxy;
    log("UsersManager: UsersManager creado.", LOG_INFO);
}

bool UsersManager::logIn(std::string user, std::string password, std::string &errorMessage) {
    users_mutex.lock();
    bool result = false;
    for (auto const& userAndPassword : this->usersAndPasswords) {
        if ((userAndPassword.first == user) && (userAndPassword.second == password)) {
            if (this->canLogIn(user, errorMessage)) {
                this->processLogIn(user);
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
    errorMessage = "Credenciales invalidas.";
    users_mutex.unlock();
    return result;
}

void UsersManager::processLogIn(std::string user) {
    loggedInUsers.insert(user);
    if (this->gameControllerProxy->hasGameStarted()) {
        auto loggedOffSearch = loggedOffUsers.find(user);
        if(loggedOffSearch != loggedOffUsers.end()) {
            log("UsersManager: Removiendo al usuario de la lista de desloggeados.", LOG_INFO);
            loggedOffUsers.erase(loggedOffSearch);
        } else {
            log("UsersManager: No estaba deslogeado. Que paso?", LOG_ERROR);
        }
    }
}

bool UsersManager::canLogIn(std::string user, std::string &errorMessage) {
    bool result = false;
    if (this->gameControllerProxy->hasGameStarted()) {
        // Si el juego ya empezo solo se pueden logear los que se habian deslogeado alguna vez.
        log("UsersManager: El juego comenzo, entonces veo que no este logeado pero que se haya logeado en algun momento.", LOG_DEBUG);
        if (this->isLoggedIn(user)) {
            errorMessage = "El usuario ya esta jugando.";
        } else {
            if (this->isLoggedOff(user)) {
                result = true;
            } else {
                errorMessage = "El usuario no se logeo antes del comienzo del partido.";
            }
        }
    } else {
        // Sino solo basta con que no esten logeados.
        log("UsersManager: El juego no comenzo, entonces veo que no este logeado.", LOG_DEBUG);
        if (!this->isLoggedIn(user)) {
            result = true;
        } else {
            errorMessage = "El usuario esta logeado.";
        }
    }
    return result;
}

bool UsersManager::isLoggedIn(std::string user) {
    log("UsersManager: Buscando " + user + " en la lista de loggeados...", LOG_DEBUG);
    auto loggedInSearch = this->loggedInUsers.find(user);
    bool encontrado = loggedInSearch != this->loggedInUsers.end();
    if (encontrado) {
        log("UsersManager: Loggeado encontrado: ", user, LOG_DEBUG);
    } else {
        log("UsersManager: Loggeado no encontrado: ", user, LOG_DEBUG);
    }
    return encontrado;
}

bool UsersManager::isLoggedOff(std::string user) {
    log("UsersManager: Buscando " + user + " en la lista de desloggeados...", LOG_ERROR);
    auto loggedOffSearch = this->loggedOffUsers.find(user);
    bool encontrado = loggedOffSearch != this->loggedOffUsers.end();
    if (encontrado) {
        log("UsersManager: Desloggeado encontrado: ", user, LOG_DEBUG);
    } else {
        log("UsersManager: Desloggeado no encontrado: ", user, LOG_DEBUG);
    }
    return encontrado;
}

void UsersManager::logOff(std::string user) {
    users_mutex.lock();
    // Necesito el iterator por eso no uso la func de arriba.
    auto loggedInSearch = this->loggedInUsers.find(user);
    if(loggedInSearch != this->loggedInUsers.end()) {
        log("UsersManager: Estaba logeado removiendo al usuario " + user + " de la lista.", LOG_DEBUG);
        this->loggedInUsers.erase(loggedInSearch);
        log("UsersManager: Agregando a la lista de deslogeados a: " + user, LOG_DEBUG);
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
