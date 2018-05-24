#include "controller/actions/Action.h"

Action::Action() {
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
