#include "controller/actions/Action.h"

Action::Action() {
}

void Action::execute(Player* player) {
    log("Action: Llamando al execute de Action!", LOG_ERROR);
}

void Action::execute(Pitch* pitch) {
    log("Action: Llamando al execute de Action!", LOG_ERROR);
}

bool Action::valid(Player* player) {
    return false;
}

bool Action::valid(Pitch* pitch) {
    return false;
}

std::string Action::getCommand() {
    log("Action: Llamando al getCommand de Action!", LOG_ERROR);
    return "NotImplemented";
}

std::string Action::getParameters() {
    log("Action: Llamando al getCommand de Action!", LOG_ERROR);
    return "";
}

std::string Action::toString() {
    return this->getCommand() + ":" + this->getParameters();
}

Action::~Action() {
}
