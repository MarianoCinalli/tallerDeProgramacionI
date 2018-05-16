#include "controller/actions/StopRunningFastAction.h"

StopRunningFastAction::StopRunningFastAction() {
    //TODO ver clase
}

void StopRunningFastAction::execute(Player* player) {
    log("StopRunningFastAction: Tecla de dejar de correr rapido detectada.", LOG_DEBUG);
    player->stopsRunningFast();
}

bool StopRunningFastAction::valid(Player* player) {
    return true;
}

std::string StopRunningFastAction::getCommand() {
    return "StopRunningFast";
}

std::string StopRunningFastAction::getParameters() {
    return "";
}

StopRunningFastAction::~StopRunningFastAction() {
}
