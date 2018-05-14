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

std::vector<std::string> Action::getParameters() {
    log("Action: Llamando al getCommand de Action!", LOG_ERROR);
    std::string str = "";
    std::vector<std::string> vec = {str};
    return vec;
}

std::string Action::toString() {
    YAML::Emitter out;
    out << YAML::BeginMap;
    out << YAML::Key << "command";
    out << YAML::Value << this->getCommand();
    if (this->getParameters()[0] != "") {
        out << YAML::Key << "parameters";
        out << YAML::Value << this->getParameters();
    }
    out << YAML::EndMap;
    return out.c_str();
}



Action::~Action() {
}
