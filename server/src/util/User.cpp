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
    this->configuration = initializer->getConfiguration();
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
    log("VALIDANDO USUARIO ", LOG_INFO);
    std::string usuario = this->getMessageAction(message);
    std::string clave = this->getMessageValue(message);
    this->hasLoged = false;
     log("VALIDANDO USUARIO: ",usuario, LOG_INFO);
     log("VALIDANDO CLAVE: ",clave, LOG_INFO);
    map<string, string>::iterator it;
       for (it = configuration->getUsuarios().begin(); it != configuration->getUsuarios().end(); it++)
       {
         if ( it->first == usuario && it->second  == clave ){
           this->hasLoged = true;
           log("USUARIO LOGGEADO ", LOG_INFO);
         }
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
    return message.substr(message.find(":")+1, message.length());   //iba un +1 LPM
}

int User::getTeam() {
    return this->teamNumber;
}

User::~User() {
    log("User: Eliminando User...", LOG_INFO);
}
