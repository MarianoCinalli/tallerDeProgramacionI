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
    log("VALIDANDO USUARIO ", LOG_INFO);
    this->hasLoged = this->manager->logIn(
        parsedUser,
        parsedPassword
    );
    if (this->hasLoged) {
        log("User: Usuario logeado: ", parsedUser, LOG_INFO);
        this->user = parsedUser;
    } else {
        log("User: No se pudo logear el usuario.", LOG_INFO);
    }
    log("FIN VALIDANDO USUARIO ", LOG_INFO);
}

bool User::hasPickedTeamAndFormation() {
    return this->hasPicked;
}

void User::processTeamAndFormationMessage(std::string message) {
    // Parsea el mensaje para obtener el equipo y formacion.
    std::string action = this->getMessageAction(message);
    std::string value = this->getMessageValue(message);
    int team = 0;
    if (action == "use") {
      team = stoi(value); // Guardar el que eligio el user.
    }
    // Si puede unirse a ese equipo, setea la formacion al equipo luego se ordena.

    // Dependiendo de la cantidad de jugadores
    // Hay que llevar la cuenta de cuantos usuarios hay en cada equipo
    // maxClients == 1
    // Puede elegir cualquiera de los dos
    // maxClients == 2
    // ninguno conectado-> elige cualquiera
    // uno ya conectado-> tiene que usar el otro equipo
    // maxClients == 3
    // ninguno conectado-> elige cualquiera
    // uno ya conectado-> elige cualquiera
    // dos ya conectado-> tiene que usar el otro equipo
    // maxClients == 4
    // ninguno conectado-> elige cualquiera
    // uno ya conectado-> elige cualquiera
    // dos ya conectado-> elige cualquiera
    // tres ya conectado-> tiene que usar el otro equipo

    log("User: El usuario se unio al equipo: ", team, LOG_INFO);
    this->teamNumber = team;
    // Le manda el resultado al cliente.
    log("User: El usuario termino de elegir.", LOG_INFO);
    this->hasPicked = true;
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
        action = new KickingAction();
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
    return message.substr(message.find(":")+1, message.length());
}

int User::getTeam() {
    return this->teamNumber;
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
