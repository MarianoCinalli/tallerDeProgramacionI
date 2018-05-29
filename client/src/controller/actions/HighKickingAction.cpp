#include "controller/actions/HighKickingAction.h"

HighKickingAction::HighKickingAction(int power) {
    this->power = power;
}


std::string HighKickingAction::getCommand() {
    return "HighKicking";
}

std::string HighKickingAction::getParameters() {
    return std::to_string(this->power);
}

HighKickingAction::~HighKickingAction() {
}
