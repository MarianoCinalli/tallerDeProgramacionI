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
    log("User: El usuario se logeo.", LOG_INFO);
    this->hasLoged = true;
}

bool User::hasPickedTeamAndFormation() {
    return this->hasPicked;
}

void User::processTeamAndFormationMessage(std::string message) {
    // Parsea el mensaje para obtener el equipo y formacion.
    int team = 0; // Guardar el que eligio el user.
    // Si puede unirse a ese equipo, setea la formacion al equipo luego se ordena.
    log("User: El usuario se unio al equipo: ", team, LOG_INFO);
    this->teamNumber = team;
    // Le manda el resultado al cliente.
    log("User: El usuario termino de elegir.", LOG_INFO);
    this->hasPicked = true;
}

        
Action* User::getAsAction(std::string message) {
    return NULL;
}

int User::getTeam() {
    return this->teamNumber;
}

User::~User() {
    log("User: Eliminando User...", LOG_INFO);
}
