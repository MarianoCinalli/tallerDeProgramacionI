#include "controller/actions/KickingAction.h"

KickingAction::KickingAction(int power, bool highPass) {
    this->power = power;
    this->highPass = highPass;
}


std::string KickingAction::getCommand() {
    return "Kicking";
}

std::string KickingAction::getParameters() {
    return std::to_string(this->power);
}

KickingAction::~KickingAction() {
}
