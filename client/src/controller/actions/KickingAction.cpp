#include "controller/actions/KickingAction.h"

KickingAction::KickingAction(int power) {
    this->power = power;
}


std::string KickingAction::getCommand() {
    return "Kicking";
}

std::string KickingAction::getParameters() {
    return std::to_string(this->power);
}

KickingAction::~KickingAction() {
}
