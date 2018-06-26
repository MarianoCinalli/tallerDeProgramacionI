#include "util/User.h"

User::User(GameInitializer* initializer, int userSocket) {
    log("User: Creando User...", LOG_INFO);
    // Falta que conf le pase la lista de users y pass.
    // Para hacerlo creo que es mejor que el initializer tenga una ref conf.
    // Y se lo pasen por constructor a User.
    this->hasLoged = false;
    this->hasPicked = false;
    this->teamNumber = 0;
    this->connectionManager = initializer->getConnectionManager();
    this->userSocket = userSocket;
    this->manager = initializer->getUsersManager();
    this->gameControllerProxy = initializer->getGameControllerProxy();
}

bool User::hasLogedIn() {
    return this->hasLoged;
}

void User::processLogInMessage(std::string message) {
    // Parsea el mensaje para obtener el user y pass.
    // Guardar el nombre del usuario, en una variable
    // de clase, para agregarlo a los mensajes?
    // Lo valida contra la lista.
    // Le manda el resultado al cliente.
    std::string parsedUser = this->getMessageAction(message);
    std::string parsedPassword = this->getMessageValue(message);
    log("User: Intentando logear a: ", parsedUser, LOG_INFO);
    std::string errorMessage = "";
    this->hasLoged = this->manager->logIn(
        parsedUser,
        parsedPassword,
        errorMessage
    );
    if (this->hasLoged) {
        log("User: Usuario logeado: ", parsedUser, LOG_INFO);
        this->user = parsedUser;
        connectionManager->sendMessage(this->userSocket, "true:");
    } else {
        log("User: No se pudo logear el usuario.", LOG_INFO);
        connectionManager->sendMessage(this->userSocket, "false:" + errorMessage);
    }
}

bool User::hasPickedTeamAndFormation() {
    return this->hasPicked;
}

void User::processTeamAndFormationMessage(std::string message) {
    // Parsea el mensaje para obtener el equipo y formacion.
    std::string action = this->getMessageAction(message);
    std::string value = this->getMessageValue(message);
    int team = 0;
    int formation = 33;
    if (action == "use") {
        team = stoi(value);
        int usersInTeam = this->gameControllerProxy->getUsersInTeam(team);
        //formation = stoi(value.substr(value.find("-")+1,value.length()));
        // log("User: equipo", team, LOG_DEBUG);
        // log("User: formacion", formation, LOG_DEBUG);

        // Si puede unirse a ese equipo, setea la formacion al equipo luego se ordena.
        std::string errorMessage = "";
        bool couldJoin = this->gameControllerProxy->joinTeam(
            this->getName(),
            team,
            formation,
            this->connectionManager->getMaxClients(),
            errorMessage
        );
        if (couldJoin) {
            log("User: El usuario se unio al equipo: ", team, LOG_INFO);
            this->teamNumber = team;
            log("User: El usuario termino de elegir.", LOG_INFO);
            if (usersInTeam > 0) {
                // Si no es el primero del equipo no tiene que elegir.
                this->hasPicked = true;
            }
            this->connectionManager->sendMessage(this->userSocket, "true:" + std::to_string(usersInTeam));
        } else {
            log("User: El usuario no se pudo unir al equipo: ", team, LOG_INFO);
            this->connectionManager->sendMessage(this->userSocket, "false:" + errorMessage);
        }
    } else if (action == "formation") {
        formation = stoi(value);
        this->gameControllerProxy->setTeamFormation(
            this->getTeam(),
            formation
        );
        this->hasPicked = true;
        this->connectionManager->sendMessage(this->userSocket, "true:");
    } else {
        log("User: Accion no entendida: ", action, LOG_ERROR);
    }
}


Action* User::getAsAction(std::string message) {
    Action* action = NULL;
    std::string messageAction = this->getMessageAction(message);
    std::string messageValue = this->getMessageValue(message);
    if (messageAction == "StopRunningFast") {
        action = new StopRunningFastAction();
    } else if (messageAction == "RunningFast") {
        action = new RunningFastAction();
    } else if (messageAction == "Sliding") {
        action = new SlidingAction();
    } else if (messageAction == "Kicking") {
        action = new KickingAction(std::stoi(messageValue, nullptr));
    } else if (messageAction == "HighKicking") {
        action = new HighKickingAction(std::stoi(messageValue, nullptr));
    } else if (messageAction == "ChangeActivePlayer") {
        action = new ChangeActivePlayer();
    } else if (messageAction == "Stop") {
        action = new Stop();
    } else if (messageAction == "Accelerate") {
        action = new Accelerate(std::stoi(messageValue, nullptr));
    } else {
        log(
            "User: No se pudo transformar el mensaje a accion: ",
            message,
            LOG_INFO
        );
    }
    return action;
}

// Separan key:value en key value
std::string User::getMessageAction(std::string message) {
    return message.substr(0, message.find(":"));
}

std::string User::getMessageValue(std::string message) {
    return message.substr(message.find(":") + 1, message.length());
}

int User::getTeam() {
    return this->teamNumber;
}

std::string User::getName() {
    return this->user;
}

void User::logOff() {
    if (this->hasLogedIn()) {
        log("User: Deslogeando: ", this->user, LOG_INFO);
        this->manager->logOff(this->user);
    } else {
        log("User: No se logeo, no lo deslogeo.", LOG_INFO);
    }
}

User::~User() {
    log("User: Eliminando User...", LOG_INFO);
}
